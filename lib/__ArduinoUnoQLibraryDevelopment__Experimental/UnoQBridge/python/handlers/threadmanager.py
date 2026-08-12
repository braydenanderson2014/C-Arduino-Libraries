#!/usr/bin/env python3
"""
Thread manager handler for UnoQBridgeService.

Provides a named-task job queue backed by Python daemon threads.
The sketch submits jobs by registered task name, polls status, then reads
the result in chunks — cleanly avoiding the Bridge RPC timeout for any
long-running work (HTTP fetches, file scans, heavy computation, etc.).

Supported ops (called as "threadmanager.<op>" from the bridge):
  ping, submit, status, result_size, result_chunk, result,
  cancel, jobs, clear_done

How to add a custom task:
  In main.py, call:
      _tmgr.register("my_task", my_task_function)
  where my_task_function(arg: str, cancel_event: threading.Event) -> str

The cancel_event is set when the sketch calls thread_cancel(). Your function
should check cancel_event.is_set() periodically and return early if true.
"""

from __future__ import annotations

import threading
from typing import Any, Callable, Dict, Optional


# ---------------------------------------------------------------------------
# Job record
# ---------------------------------------------------------------------------

class _Job:
    def __init__(self, task: str, arg: str) -> None:
        self.task   = task
        self.arg    = arg
        self.status = "pending"   # pending | running | done | error | cancelled
        self.result = ""
        self.error  = ""
        self._cancel = threading.Event()
        self._thread: Optional[threading.Thread] = None

    def request_cancel(self) -> None:
        self._cancel.set()

    @property
    def cancel_event(self) -> threading.Event:
        return self._cancel


# ---------------------------------------------------------------------------
# Backend
# ---------------------------------------------------------------------------

class ThreadManagerBackend:
    def __init__(self) -> None:
        self._jobs:     Dict[str, _Job]      = {}
        self._registry: Dict[str, Callable]  = {}
        self._lock = threading.Lock()

    # --- Registration -------------------------------------------------------

    def register(self, name: str, fn: Callable[[str, threading.Event], str]) -> None:
        """Register a named callable that can be triggered by name from the sketch."""
        self._registry[name] = fn

    def registered_tasks(self) -> list:
        return sorted(self._registry.keys())

    # --- Ops ----------------------------------------------------------------

    def ping(self) -> Dict[str, Any]:
        return {"service": "threadmanager", "status": "ok",
                "registered_tasks": self.registered_tasks()}

    def submit(self, task: str, job: str, arg: str = "") -> Dict[str, Any]:
        """Start a registered task in a daemon thread under the given job name."""
        if task not in self._registry:
            return {"submitted": False, "error": f"Unknown task: {task!r}. "
                    f"Registered: {self.registered_tasks()}"}
        with self._lock:
            if job in self._jobs and self._jobs[job].status == "running":
                return {"submitted": False, "error": f"Job {job!r} is already running"}
            record = _Job(task, arg)
            self._jobs[job] = record

        fn = self._registry[task]

        def _run() -> None:
            with self._lock:
                record.status = "running"
            try:
                result = fn(arg, record.cancel_event)
                with self._lock:
                    if record.cancel_event.is_set():
                        record.status = "cancelled"
                        record.result = ""
                    else:
                        record.status = "done"
                        record.result = str(result) if result is not None else ""
            except Exception as exc:
                with self._lock:
                    record.status = "error"
                    record.error  = str(exc)

        t = threading.Thread(target=_run, name=f"unoq-{job}", daemon=True)
        record._thread = t
        t.start()
        return {"submitted": True, "job": job, "task": task}

    def status(self, job: str) -> Dict[str, Any]:
        with self._lock:
            if job not in self._jobs:
                return {"job": job, "status": "not_found"}
            r = self._jobs[job]
            return {"job": job, "status": r.status,
                    "task": r.task, "error": r.error}

    def result_size(self, job: str) -> Dict[str, Any]:
        """Return the character length of a job's result string."""
        with self._lock:
            if job not in self._jobs:
                return {"job": job, "size": -1, "status": "not_found"}
            r = self._jobs[job]
            return {"job": job, "size": len(r.result), "status": r.status}

    def result_chunk(self, job: str, offset: int = 0, length: int = 128) -> Dict[str, Any]:
        """Return a bounded slice of a job's result — safe for Bridge string returns."""
        with self._lock:
            if job not in self._jobs:
                return {"job": job, "chunk": "", "status": "not_found"}
            r = self._jobs[job]
            chunk = r.result[offset: offset + length]
            return {"job": job, "chunk": chunk, "status": r.status,
                    "offset": offset, "length": len(chunk)}

    def result(self, job: str, max_chars: int = 200) -> Dict[str, Any]:
        """Return up to max_chars of a job's result (convenience for small results)."""
        with self._lock:
            if job not in self._jobs:
                return {"job": job, "result": "", "status": "not_found"}
            r = self._jobs[job]
            return {"job": job, "result": r.result[:max_chars],
                    "truncated": len(r.result) > max_chars, "status": r.status}

    def cancel(self, job: str) -> Dict[str, Any]:
        """Request cooperative cancellation — the task function must honour cancel_event."""
        with self._lock:
            if job not in self._jobs:
                return {"job": job, "cancelled": False, "reason": "not_found"}
            r = self._jobs[job]
            if r.status not in ("pending", "running"):
                return {"job": job, "cancelled": False, "reason": r.status}
            r.request_cancel()
        return {"job": job, "cancelled": True}

    def jobs(self) -> Dict[str, Any]:
        """List all jobs and their current statuses."""
        with self._lock:
            return {"jobs": [
                {"job": name, "task": r.task, "status": r.status}
                for name, r in self._jobs.items()
            ]}

    def clear_done(self) -> Dict[str, Any]:
        """Remove completed/errored/cancelled jobs from the job table."""
        with self._lock:
            terminal = ("done", "error", "cancelled")
            removed = [n for n, r in self._jobs.items() if r.status in terminal]
            for name in removed:
                del self._jobs[name]
        return {"cleared": removed}


# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------

def _err(request_id: Any, code: str, message: str) -> Dict[str, Any]:
    return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok(request_id: Any, result: Any) -> Dict[str, Any]:
    return {"id": request_id, "ok": True, "result": result}


def dispatch(backend: ThreadManagerBackend, request: Dict[str, Any]) -> Dict[str, Any]:
    request_id = request.get("id")
    op = request.get("op")
    if not isinstance(op, str) or not op:
        return _err(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

    try:
        if op == "ping":
            result = backend.ping()
        elif op == "submit":
            result = backend.submit(
                task=str(request["task"]),
                job=str(request["job"]),
                arg=str(request.get("arg", "")),
            )
        elif op == "status":
            result = backend.status(job=str(request["job"]))
        elif op == "result_size":
            result = backend.result_size(job=str(request["job"]))
        elif op == "result_chunk":
            result = backend.result_chunk(
                job=str(request["job"]),
                offset=int(request.get("offset", 0)),
                length=int(request.get("length", 128)),
            )
        elif op == "result":
            result = backend.result(
                job=str(request["job"]),
                max_chars=int(request.get("max_chars", 200)),
            )
        elif op == "cancel":
            result = backend.cancel(job=str(request["job"]))
        elif op == "jobs":
            result = backend.jobs()
        elif op == "clear_done":
            result = backend.clear_done()
        else:
            return _err(request_id, "UNKNOWN_OP", f"Unsupported threadmanager op: {op}")

        return _ok(request_id, result)

    except KeyError as exc:
        return _err(request_id, "MISSING_ARG", f"Missing required field: {exc}")
    except Exception as exc:  # pragma: no cover
        return _err(request_id, "INTERNAL", str(exc))
