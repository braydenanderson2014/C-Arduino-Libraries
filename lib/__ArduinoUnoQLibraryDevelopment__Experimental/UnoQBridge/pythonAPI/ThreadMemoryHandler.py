#!/usr/bin/env python3
"""
ThreadMemoryHandler — standalone entry point and backward-compat re-export.

Business logic lives in handlers/threadmemory.py.
Run this file directly only for isolated testing; production use goes through
UnoQBridgeService.py where the threadmemory domain is registered automatically.

    python3 ThreadMemoryHandler.py --mode json
    python3 ThreadMemoryHandler.py --once '{"id":1,"op":"ping"}'
"""

from __future__ import annotations

import argparse
import json
import sys
from typing import Any, Dict

# Re-export for backward compatibility (other code may import from here)
from handlers.threadmemory import ThreadMemoryBackend, dispatch  # noqa: F401


def _err(request_id: Any, code: str, message: str) -> Dict[str, Any]:
	return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _run_json_loop(backend: ThreadMemoryBackend) -> int:
	for line in sys.stdin:
		line = line.strip()
		if not line:
			continue
		try:
			request = json.loads(line)
			response = dispatch(backend, request) if isinstance(request, dict) else _err(None, "INVALID_REQUEST", "Request must be an object")
		except json.JSONDecodeError as exc:
			response = _err(None, "INVALID_JSON", str(exc))
		sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
		sys.stdout.flush()
	return 0


def main() -> int:
	parser = argparse.ArgumentParser(description="UnoQ thread-memory handler (standalone)")
	parser.add_argument("--mode", choices=("json",), default="json", help="Transport format (json only in standalone mode)")
	parser.add_argument("--once", default="", help="Process one JSON request and exit")
	args = parser.parse_args()

	backend = ThreadMemoryBackend()

	if args.once:
		try:
			request = json.loads(args.once)
		except json.JSONDecodeError as exc:
			sys.stdout.write(json.dumps(_err(None, "INVALID_JSON", str(exc)), ensure_ascii=True) + "\n")
			return 1
		if not isinstance(request, dict):
			sys.stdout.write(json.dumps(_err(None, "INVALID_REQUEST", "Request must be an object"), ensure_ascii=True) + "\n")
			return 1
		sys.stdout.write(json.dumps(dispatch(backend, request), ensure_ascii=True) + "\n")
		return 0

	return _run_json_loop(backend)


if __name__ == "__main__":
	raise SystemExit(main())


from __future__ import annotations

import argparse
import json
import struct
import sys
from typing import Any, Dict, List, Optional

try:
	import msgpack  # type: ignore
except Exception:  # pragma: no cover
	msgpack = None


class ThreadMemoryError(Exception):
	def __init__(self, code: str, message: str) -> None:
		super().__init__(message)
		self.code = code
		self.message = message


class ThreadMemoryBackend:
	def __init__(self) -> None:
		self._records: Dict[str, List[Dict[str, Any]]] = {}

	def ping(self) -> Dict[str, Any]:
		return {"service": "ThreadMemoryHandler", "status": "ok"}

	def record(self, thread: str, message: str, kind: str = "note", tags: Optional[List[str]] = None) -> Dict[str, Any]:
		entry = {
			"thread": thread,
			"kind": kind,
			"message": message,
			"tags": list(tags or []),
		}
		self._records.setdefault(thread, []).append(entry)
		return {"thread": thread, "recorded": True, "count": len(self._records[thread])}

	def read(self, thread: str, limit: int = 50) -> Dict[str, Any]:
		records = list(self._records.get(thread, []))
		if limit >= 0:
			records = records[-limit:]
		return {"thread": thread, "records": records}

	def threads(self) -> Dict[str, Any]:
		return {"threads": sorted(self._records.keys())}

	def clear(self, thread: Optional[str] = None) -> Dict[str, Any]:
		if thread is None:
			self._records.clear()
			return {"cleared": True, "thread": None}
		removed = self._records.pop(thread, None)
		return {"cleared": removed is not None, "thread": thread}


_BINARY_MAGIC = 0x5155
_BINARY_VERSION = 1
_BINARY_FORMAT_JSON = 1
_BINARY_FORMAT_MSGPACK = 2
_BINARY_HEADER = struct.Struct("<HBBHI")


def _error_response(request_id: Any, code: str, message: str) -> Dict[str, Any]:
	return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok_response(request_id: Any, result: Any) -> Dict[str, Any]:
	return {"id": request_id, "ok": True, "result": result}


def dispatch(backend: ThreadMemoryBackend, request: Dict[str, Any]) -> Dict[str, Any]:
	request_id = request.get("id")
	op = request.get("op")
	if not isinstance(op, str) or not op:
		return _error_response(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

	try:
		if op == "ping":
			result = backend.ping()
		elif op == "record":
			result = backend.record(
				thread=str(request["thread"]),
				message=str(request.get("message", "")),
				kind=str(request.get("kind", "note")),
				tags=list(request.get("tags", [])),
			)
		elif op == "read":
			result = backend.read(
				thread=str(request["thread"]),
				limit=int(request.get("limit", 50)),
			)
		elif op == "threads":
			result = backend.threads()
		elif op == "clear":
			thread = request.get("thread")
			result = backend.clear(None if thread in (None, "") else str(thread))
		else:
			return _error_response(request_id, "UNKNOWN_OP", f"Unsupported op: {op}")

		return _ok_response(request_id, result)
	except KeyError as exc:
		return _error_response(request_id, "MISSING_ARG", f"Missing required field: {exc}")
	except ThreadMemoryError as exc:
		return _error_response(request_id, exc.code, exc.message)
	except Exception as exc:  # pragma: no cover
		return _error_response(request_id, "INTERNAL", str(exc))


def _read_exact(stream, size: int) -> bytes:
	buffer = bytearray()
	while len(buffer) < size:
		chunk = stream.read(size - len(buffer))
		if not chunk:
			raise EOFError("Unexpected end of stream")
		buffer.extend(chunk)
	return bytes(buffer)


def _encode_response(response: Dict[str, Any], payload_format: int) -> bytes:
	if payload_format == _BINARY_FORMAT_MSGPACK:
		if msgpack is None:
			raise RuntimeError("msgpack is not installed. Install with: pip install msgpack")
		return msgpack.packb(response, use_bin_type=True)
	return json.dumps(response, ensure_ascii=True).encode("utf-8")


def _decode_request(payload: bytes, payload_format: int) -> Dict[str, Any]:
	if payload_format == _BINARY_FORMAT_MSGPACK:
		if msgpack is None:
			raise RuntimeError("msgpack is not installed. Install with: pip install msgpack")
		request = msgpack.unpackb(payload, raw=False)
	else:
		request = json.loads(payload.decode("utf-8"))

	if not isinstance(request, dict):
		raise ThreadMemoryError("INVALID_REQUEST", "Request must be an object")
	return request


def run_json_loop(backend: ThreadMemoryBackend) -> int:
	for line in sys.stdin:
		line = line.strip()
		if not line:
			continue
		try:
			request = json.loads(line)
			if not isinstance(request, dict):
				response = _error_response(None, "INVALID_REQUEST", "Request must be an object")
			else:
				response = dispatch(backend, request)
		except json.JSONDecodeError as exc:
			response = _error_response(None, "INVALID_JSON", str(exc))

		sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
		sys.stdout.flush()
	return 0


def run_msgpack_loop(backend: ThreadMemoryBackend) -> int:
	if msgpack is None:
		raise RuntimeError("msgpack is not installed. Install with: pip install msgpack")

	unpacker = msgpack.Unpacker(sys.stdin.buffer, raw=False)
	for request in unpacker:
		if not isinstance(request, dict):
			response = _error_response(None, "INVALID_REQUEST", "Request must be an object")
		else:
			response = dispatch(backend, request)
		sys.stdout.buffer.write(msgpack.packb(response, use_bin_type=True))
		sys.stdout.buffer.flush()
	return 0


def run_binary_loop(backend: ThreadMemoryBackend) -> int:
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
				request = _decode_request(payload, payload_format)
				response = dispatch(backend, request)
			except Exception as exc:  # pragma: no cover
				response = _error_response(request_id, "INTERNAL", str(exc))

		response_payload = _encode_response(response, payload_format)
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


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(description="Experimental Arduino Uno Q thread-memory handler")
	parser.add_argument(
		"--mode",
		choices=("msgpack", "json", "binary"),
		default="msgpack",
		help="RPC transport format over stdio",
	)
	parser.add_argument(
		"--once",
		default="",
		help="Process one JSON request and exit (bridge mode)",
	)
	return parser.parse_args()


def main() -> int:
	args = parse_args()
	backend = ThreadMemoryBackend()
	if args.once:
		try:
			request = json.loads(args.once)
		except json.JSONDecodeError as exc:
			response = _error_response(None, "INVALID_JSON", str(exc))
			sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
			return 1

		if not isinstance(request, dict):
			response = _error_response(None, "INVALID_REQUEST", "Request must be an object")
			sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
			return 1

		response = dispatch(backend, request)
		sys.stdout.write(json.dumps(response, ensure_ascii=True) + "\n")
		return 0

	if args.mode == "json":
		return run_json_loop(backend)
	if args.mode == "binary":
		return run_binary_loop(backend)
	return run_msgpack_loop(backend)


if __name__ == "__main__":
	raise SystemExit(main())
