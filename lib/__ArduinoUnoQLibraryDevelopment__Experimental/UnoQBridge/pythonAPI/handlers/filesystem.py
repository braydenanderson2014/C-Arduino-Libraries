#!/usr/bin/env python3
"""
Filesystem handler for UnoQBridgeService.

Supported ops (called as "file.<op>" from the bridge):
  ping, exists, stat, mkdir, listdir,
  read_text, write_text, read_bytes, write_bytes,
  delete, rename, copy

All paths are sandboxed inside the root supplied to FileBackend.
"""

from __future__ import annotations

import base64
import os
import shutil
from pathlib import Path
from typing import Any, Dict, Iterable, Optional


# ---------------------------------------------------------------------------
# Errors
# ---------------------------------------------------------------------------

class FileBackendError(Exception):
	def __init__(self, code: str, message: str) -> None:
		super().__init__(message)
		self.code = code
		self.message = message


# ---------------------------------------------------------------------------
# Backend
# ---------------------------------------------------------------------------

class FileBackend:
	def __init__(self, root: Path) -> None:
		self.root = root.resolve()
		self.root.mkdir(parents=True, exist_ok=True)

	def _resolve(self, relative_path: str) -> Path:
		p = (self.root / relative_path).resolve()
		try:
			p.relative_to(self.root)
		except ValueError as exc:
			raise FileBackendError("PATH_ESCAPE", "Path escapes sandbox root") from exc
		return p

	def _stat_payload(self, p: Path) -> Dict[str, Any]:
		st = p.stat()
		return {
			"path": str(p.relative_to(self.root)).replace("\\", "/"),
			"exists": True,
			"is_file": p.is_file(),
			"is_dir": p.is_dir(),
			"size": st.st_size,
			"mtime": st.st_mtime,
			"ctime": st.st_ctime,
		}

	def ping(self) -> Dict[str, Any]:
		return {"service": "filesystem", "status": "ok"}

	def exists(self, path: str) -> Dict[str, Any]:
		p = self._resolve(path)
		return {"path": path, "exists": p.exists()}

	def stat(self, path: str) -> Dict[str, Any]:
		p = self._resolve(path)
		if not p.exists():
			return {"path": path, "exists": False}
		return self._stat_payload(p)

	def mkdir(self, path: str, parents: bool = True, exist_ok: bool = True) -> Dict[str, Any]:
		p = self._resolve(path)
		p.mkdir(parents=parents, exist_ok=exist_ok)
		return {"path": path, "created": True}

	def listdir(self, path: str = ".", recursive: bool = False) -> Dict[str, Any]:
		base = self._resolve(path)
		if not base.exists():
			raise FileBackendError("NOT_FOUND", f"Path not found: {path}")
		if not base.is_dir():
			raise FileBackendError("NOT_DIR", f"Path is not a directory: {path}")
		iterator: Iterable[Path] = base.rglob("*") if recursive else base.iterdir()
		entries = []
		for item in iterator:
			rel = item.relative_to(self.root)
			entries.append({
				"path": str(rel).replace("\\", "/"),
				"is_file": item.is_file(),
				"is_dir": item.is_dir(),
				"size": item.stat().st_size if item.is_file() else None,
			})
		entries.sort(key=lambda e: e["path"])
		return {"path": path, "entries": entries}

	def read_text(self, path: str, encoding: str = "utf-8") -> Dict[str, Any]:
		p = self._resolve(path)
		if not p.exists():
			raise FileBackendError("NOT_FOUND", f"Path not found: {path}")
		if not p.is_file():
			raise FileBackendError("NOT_FILE", f"Path is not a file: {path}")
		return {"path": path, "content": p.read_text(encoding=encoding)}

	def write_text(
		self,
		path: str,
		content: str,
		encoding: str = "utf-8",
		append: bool = False,
		create_dirs: bool = True,
	) -> Dict[str, Any]:
		p = self._resolve(path)
		if create_dirs:
			p.parent.mkdir(parents=True, exist_ok=True)
		with p.open("a" if append else "w", encoding=encoding) as f:
			written = f.write(content)
		return {"path": path, "bytes_written": written}

	def read_bytes(self, path: str, offset: int = 0, size: Optional[int] = None) -> Dict[str, Any]:
		if offset < 0:
			raise FileBackendError("INVALID_ARG", "offset must be >= 0")
		p = self._resolve(path)
		if not p.exists():
			raise FileBackendError("NOT_FOUND", f"Path not found: {path}")
		if not p.is_file():
			raise FileBackendError("NOT_FILE", f"Path is not a file: {path}")
		with p.open("rb") as f:
			f.seek(offset)
			data = f.read() if size is None else f.read(max(0, int(size)))
		return {
			"path": path,
			"offset": offset,
			"size": len(data),
			"data": base64.b64encode(data).decode("ascii"),
		}

	def write_bytes(
		self,
		path: str,
		data: str,
		append: bool = False,
		create_dirs: bool = True,
	) -> Dict[str, Any]:
		p = self._resolve(path)
		if create_dirs:
			p.parent.mkdir(parents=True, exist_ok=True)
		raw = base64.b64decode(data.encode("ascii"), validate=True)
		with p.open("ab" if append else "wb") as f:
			f.write(raw)
		return {"path": path, "bytes_written": len(raw)}

	def delete(self, path: str, recursive: bool = False) -> Dict[str, Any]:
		p = self._resolve(path)
		if not p.exists():
			return {"path": path, "deleted": False, "reason": "not_found"}
		if p.is_dir():
			shutil.rmtree(p) if recursive else p.rmdir()
		else:
			p.unlink()
		return {"path": path, "deleted": True}

	def rename(self, src: str, dst: str, replace: bool = False) -> Dict[str, Any]:
		src_p = self._resolve(src)
		dst_p = self._resolve(dst)
		if not src_p.exists():
			raise FileBackendError("NOT_FOUND", f"Source not found: {src}")
		if dst_p.exists() and not replace:
			raise FileBackendError("ALREADY_EXISTS", f"Destination exists: {dst}")
		dst_p.parent.mkdir(parents=True, exist_ok=True)
		os.replace(src_p, dst_p) if replace else src_p.rename(dst_p)
		return {"src": src, "dst": dst, "renamed": True}

	def copy(self, src: str, dst: str, replace: bool = False) -> Dict[str, Any]:
		src_p = self._resolve(src)
		dst_p = self._resolve(dst)
		if not src_p.exists():
			raise FileBackendError("NOT_FOUND", f"Source not found: {src}")
		if dst_p.exists() and not replace:
			raise FileBackendError("ALREADY_EXISTS", f"Destination exists: {dst}")
		dst_p.parent.mkdir(parents=True, exist_ok=True)
		if src_p.is_dir():
			if dst_p.exists() and replace:
				shutil.rmtree(dst_p)
			shutil.copytree(src_p, dst_p)
		else:
			shutil.copy2(src_p, dst_p)
		return {"src": src, "dst": dst, "copied": True}


# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------

def _err(request_id: Any, code: str, message: str) -> Dict[str, Any]:
	return {"id": request_id, "ok": False, "error": {"code": code, "message": message}}


def _ok(request_id: Any, result: Any) -> Dict[str, Any]:
	return {"id": request_id, "ok": True, "result": result}


def dispatch(backend: FileBackend, request: Dict[str, Any]) -> Dict[str, Any]:
	request_id = request.get("id")
	op = request.get("op")
	if not isinstance(op, str) or not op:
		return _err(request_id, "INVALID_REQUEST", "Missing or invalid 'op'")

	try:
		if op == "ping":
			result = backend.ping()
		elif op == "exists":
			result = backend.exists(path=str(request["path"]))
		elif op == "stat":
			result = backend.stat(path=str(request["path"]))
		elif op == "mkdir":
			result = backend.mkdir(
				path=str(request["path"]),
				parents=bool(request.get("parents", True)),
				exist_ok=bool(request.get("exist_ok", True)),
			)
		elif op == "listdir":
			result = backend.listdir(
				path=str(request.get("path", ".")),
				recursive=bool(request.get("recursive", False)),
			)
		elif op == "read_text":
			result = backend.read_text(
				path=str(request["path"]),
				encoding=str(request.get("encoding", "utf-8")),
			)
		elif op == "write_text":
			result = backend.write_text(
				path=str(request["path"]),
				content=str(request.get("content", "")),
				encoding=str(request.get("encoding", "utf-8")),
				append=bool(request.get("append", False)),
				create_dirs=bool(request.get("create_dirs", True)),
			)
		elif op == "read_bytes":
			size = request.get("size")
			result = backend.read_bytes(
				path=str(request["path"]),
				offset=int(request.get("offset", 0)),
				size=None if size is None else int(size),
			)
		elif op == "write_bytes":
			result = backend.write_bytes(
				path=str(request["path"]),
				data=str(request.get("data", "")),
				append=bool(request.get("append", False)),
				create_dirs=bool(request.get("create_dirs", True)),
			)
		elif op == "delete":
			result = backend.delete(
				path=str(request["path"]),
				recursive=bool(request.get("recursive", False)),
			)
		elif op == "rename":
			result = backend.rename(
				src=str(request["src"]),
				dst=str(request["dst"]),
				replace=bool(request.get("replace", False)),
			)
		elif op == "copy":
			result = backend.copy(
				src=str(request["src"]),
				dst=str(request["dst"]),
				replace=bool(request.get("replace", False)),
			)
		else:
			return _err(request_id, "UNKNOWN_OP", f"Unsupported filesystem op: {op}")

		return _ok(request_id, result)

	except KeyError as exc:
		return _err(request_id, "MISSING_ARG", f"Missing required field: {exc}")
	except FileBackendError as exc:
		return _err(request_id, exc.code, exc.message)
	except base64.binascii.Error:
		return _err(request_id, "INVALID_BASE64", "data is not valid base64")
	except Exception as exc:  # pragma: no cover
		return _err(request_id, "INTERNAL", str(exc))
