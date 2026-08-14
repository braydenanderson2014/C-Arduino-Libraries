#!/usr/bin/env python3
"""
Thread-memory handler for UnoQBridgeService.

Supported ops (called as "threadmemory.<op>" from the bridge):
  ping, record, read, threads, clear

Records are stored in-process memory; they do not survive a service restart.
"""

from __future__ import annotations

from typing import Any, Dict, List, Optional


# ---------------------------------------------------------------------------
# Errors
# ---------------------------------------------------------------------------

class ThreadMemoryError(Exception):
	def __init__(self, code: str, message: str) -> None:
		super().__init__(message)
		self.code = code
		self.message = message


# ---------------------------------------------------------------------------
# Backend
# ---------------------------------------------------------------------------

class ThreadMemoryBackend:
	def __init__(self) -> None:
		self._records: Dict[str, List[Dict[str, Any]]] = {}

	def ping(self) -> Dict[str, Any]:
		return {"service": "threadmemory", "status": "ok"}

	def record(
		self,
		thread: str,
		message: str,
		kind: str = "note",
		tags: Optional[List[str]] = None,
	) -> Dict[str, Any]:
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


# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------

def _err(request_id: Any, code: str, message: str) -> Dict[str, Any]:
	return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok(request_id: Any, result: Any) -> Dict[str, Any]:
	return {"id": request_id, "ok": True, "result": result}


def dispatch(backend: ThreadMemoryBackend, request: Dict[str, Any]) -> Dict[str, Any]:
	request_id = request.get("id")
	op = request.get("op")
	if not isinstance(op, str) or not op:
		return _err(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

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
			result = backend.clear(thread=None if thread is None else str(thread))
		else:
			return _err(request_id, "UNKNOWN_OP", f"Unsupported threadmemory op: {op}")

		return _ok(request_id, result)

	except KeyError as exc:
		return _err(request_id, "MISSING_ARG", f"Missing required field: {exc}")
	except ThreadMemoryError as exc:
		return _err(request_id, exc.code, exc.message)
	except Exception as exc:  # pragma: no cover
		return _err(request_id, "INTERNAL", str(exc))
