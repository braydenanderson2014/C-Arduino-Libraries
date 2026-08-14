#!/usr/bin/env python3
"""
UnoQBridgeService — transport, framing, and top-level dispatch.

This service runs on the Linux side and routes requests to handler modules.
Adding a new capability means adding a handler module and one entry to
_HANDLERS / _BACKEND_FACTORIES below — no changes to transport code needed.

Protocol summary:
  Request:  {"id": <any>, "op": "<domain>.<sub_op>", ...args}
  Response: {"id": <same>, "ok": true,  "result": <object>}
            {"id": <same>, "ok": false, "error": {"code": "...", "message": "..."}}

Op routing:
  "file.read_text"        -> filesystem handler,  op="read_text"
  "threadmemory.record"   -> threadmemory handler, op="record"
  "ping"                  -> service-level, no domain needed

Transport modes (--mode):
  msgpack  (default) — back-to-back MessagePack objects over stdio
  json               — one JSON object per line over stdio
  binary             — binary-framed packets (Arduino BinaryStreamTransport)

--once "<json>"  Process one JSON request and exit (Yún/process bridge mode).
--root  <path>   Sandbox root for the filesystem handler.
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path
from typing import Any, Callable, Dict

from handlers.filesystem import FileBackend, dispatch as dispatch_filesystem
from handlers.threadmemory import ThreadMemoryBackend, dispatch as dispatch_threadmemory

try:
	import msgpack  # type: ignore
except Exception:  # pragma: no cover
	msgpack = None


_BINARY_MAGIC = 0x5155
_BINARY_VERSION = 1
_BINARY_FORMAT_JSON = 1
_BINARY_FORMAT_MSGPACK = 2
_BINARY_HEADER = struct.Struct("<HBBHI")


# ---------------------------------------------------------------------------
# Handler registry
# To add a new capability: create handlers/<name>.py with a Backend class and
# a dispatch(backend, request) function, then add entries below.
# ---------------------------------------------------------------------------

_HANDLERS: Dict[str, Callable[[Any, Dict[str, Any]], Dict[str, Any]]] = {
	"file":         dispatch_filesystem,
	"threadmemory": dispatch_threadmemory,
}


def _make_backends(args: argparse.Namespace) -> Dict[str, Any]:
	return {
		"file":         FileBackend(Path(args.root)),
		"threadmemory": ThreadMemoryBackend(),
	}


# ---------------------------------------------------------------------------
# Response helpers
# ---------------------------------------------------------------------------

def _error_response(request_id: Any, code: str, message: str) -> Dict[str, Any]:
	return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok_response(request_id: Any, result: Any) -> Dict[str, Any]:
	return {"id": request_id, "ok": True, "result": result}


# ---------------------------------------------------------------------------
# Top-level dispatch
# ---------------------------------------------------------------------------

def dispatch(backends: Dict[str, Any], request: Dict[str, Any]) -> Dict[str, Any]:
	"""Route a request to the correct handler by op domain prefix."""
	request_id = request.get("id")
	op = request.get("op", "")
	if not isinstance(op, str) or not op:
		return _error_response(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

	# Service-level ping
	if op == "ping":
		return _ok_response(request_id, {"service": "UnoQBridgeService", "status": "ok"})

	# "domain.sub_op" routing
	domain, sep, sub_op = op.partition(".")
	if sep and domain in _HANDLERS and sub_op:
		routed = dict(request)
		routed["op"] = sub_op
		return _HANDLERS[domain](backends[domain], routed)

	return _error_response(request_id, "UNKNOWN_OP", f"Unknown op: {op!r}. Use '<domain>.<op>' format.")


# ---------------------------------------------------------------------------
# Binary transport helpers
# ---------------------------------------------------------------------------

def _read_exact(stream, size: int) -> bytes:
	buf = bytearray()
	while len(buf) < size:
		chunk = stream.read(size - len(buf))
		if not chunk:
			raise EOFError("Unexpected end of stream")
		buf.extend(chunk)
	return bytes(buf)


def _encode_frame_payload(response: Dict[str, Any], payload_format: int) -> bytes:
	if payload_format == _BINARY_FORMAT_MSGPACK:
		if msgpack is None:
			raise RuntimeError("msgpack not installed — run: pip install msgpack")
		return msgpack.packb(response, use_bin_type=True)
	return json.dumps(response, ensure_ascii=True).encode("utf-8")


def _decode_frame_payload(payload: bytes, payload_format: int) -> Dict[str, Any]:
	if payload_format == _BINARY_FORMAT_MSGPACK:
		if msgpack is None:
			raise RuntimeError("msgpack not installed — run: pip install msgpack")
		obj = msgpack.unpackb(payload, raw=False)
	else:
		obj = json.loads(payload.decode("utf-8"))
	if not isinstance(obj, dict):
		raise ValueError("Request must be a JSON object")
	return obj


# ---------------------------------------------------------------------------
# Transport loops
# ---------------------------------------------------------------------------

def run_json_loop(backends: Dict[str, Any]) -> int:
	for line in sys.stdin:
		line = line.strip()
		if not line:
			continue
		try:
			request = json.loads(line)
			if not isinstance(request, dict):
				response = _error_response(None, "INVALID_REQUEST", "Request must be an object")
			else:
				response = dispatch(backends, request)
		except json.JSONDecodeError as exc:
			response = _error_response(None, "INVALID_JSON", str(exc))
		sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
		sys.stdout.flush()
	return 0


def run_msgpack_loop(backends: Dict[str, Any]) -> int:
	if msgpack is None:
		raise RuntimeError("msgpack not installed — run: pip install msgpack")
	unpacker = msgpack.Unpacker(sys.stdin.buffer, raw=False)
	for request in unpacker:
		if not isinstance(request, dict):
			response = _error_response(None, "INVALID_REQUEST", "Request must be an object")
		else:
			response = dispatch(backends, request)
		sys.stdout.buffer.write(msgpack.packb(response, use_bin_type=True))
		sys.stdout.buffer.flush()
	return 0


def run_binary_loop(backends: Dict[str, Any]) -> int:
	while True:
		header = sys.stdin.buffer.read(_BINARY_HEADER.size)
		if not header:
			return 0
		if len(header) != _BINARY_HEADER.size:
			raise EOFError("Incomplete binary header")

		magic, version, payload_format, request_id, payload_length = _BINARY_HEADER.unpack(header)
		payload = _read_exact(sys.stdin.buffer, payload_length) if payload_length else b""

		if magic != _BINARY_MAGIC or version != _BINARY_VERSION:
			response = _error_response(request_id, "INVALID_FRAME", "Unsupported binary frame")
		else:
			try:
				request = _decode_frame_payload(payload, payload_format)
				response = dispatch(backends, request)
			except Exception as exc:  # pragma: no cover
				response = _error_response(request_id, "INTERNAL", str(exc))

		response_payload = _encode_frame_payload(response, payload_format)
		response_header = _BINARY_HEADER.pack(
			_BINARY_MAGIC,
			_BINARY_VERSION,
			payload_format,
			int(response.get("id", request_id) or request_id) & 0xFFFF,
			len(response_payload),
		)
		sys.stdout.buffer.write(response_header)
		sys.stdout.buffer.write(response_payload)
		sys.stdout.buffer.flush()


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def parse_args() -> argparse.Namespace:
	import os
	parser = argparse.ArgumentParser(description="UnoQ bridge service")
	parser.add_argument(
		"--root",
		default=os.environ.get("UNOQ_FILE_ROOT", "./unoq_files"),
		help="Sandbox root directory for filesystem handler",
	)
	parser.add_argument(
		"--mode",
		choices=("msgpack", "json", "binary"),
		default="msgpack",
		help="RPC transport format over stdio",
	)
	parser.add_argument(
		"--once",
		default="",
		help="Process one JSON request string and exit (Yún/process bridge mode)",
	)
	return parser.parse_args()


def main() -> int:
	args = parse_args()
	backends = _make_backends(args)

	if args.once:
		try:
			request = json.loads(args.once)
		except json.JSONDecodeError as exc:
			sys.stdout.write(json.dumps(_error_response(None, "INVALID_JSON", str(exc)), ensure_ascii=True) + "\n")
			return 1
		if not isinstance(request, dict):
			sys.stdout.write(json.dumps(_error_response(None, "INVALID_REQUEST", "Request must be an object"), ensure_ascii=True) + "\n")
			return 1
		sys.stdout.write(json.dumps(dispatch(backends, request), ensure_ascii=True) + "\n")
		return 0

	if args.mode == "json":
		return run_json_loop(backends)
	if args.mode == "binary":
		return run_binary_loop(backends)
	return run_msgpack_loop(backends)


if __name__ == "__main__":
	raise SystemExit(main())

