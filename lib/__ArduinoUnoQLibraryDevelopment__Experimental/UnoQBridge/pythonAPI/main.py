#!/usr/bin/env python3
"""
python/main.py — UnoQ Bridge API wrapper for Arduino App Lab.

═══════════════════════════════════════════════════════════════════════════════
OVERVIEW
═══════════════════════════════════════════════════════════════════════════════

This file is the Python-side entry point for an Arduino App Lab project running
on the Arduino Uno Q. It acts as an API wrapper — every public function
registered with Bridge.provide() becomes callable from the Arduino sketch via
Bridge.call("function_name", args...).

The transport layer (IPC between the STM32 MCU and the Linux container) is
handled entirely by Arduino_RouterBridge / Arduino_RPClite. This file only
contains business logic.

Business logic is split into handler modules under python/handlers/:
  handlers/filesystem.py   — sandboxed file I/O on the Linux side
  handlers/threadmemory.py — in-process per-thread record store
  handlers/networking.py   — TCP reachability, HTTP GET/POST

═══════════════════════════════════════════════════════════════════════════════
PROJECT STRUCTURE (inside App Lab's python/ folder)
═══════════════════════════════════════════════════════════════════════════════

  python/
    main.py               ← this file — API wrapper + App.run()
    requirements.txt      ← add third-party packages here (one per line)
    handlers/
      __init__.py
      filesystem.py       ← FileBackend, sandboxed to unoq_files/
      threadmemory.py     ← ThreadMemoryBackend, in-process records
      networking.py       ← NetworkingBackend, TCP + HTTP

═══════════════════════════════════════════════════════════════════════════════
BRIDGE API REFERENCE
═══════════════════════════════════════════════════════════════════════════════

All paths are relative to the sandboxed filesystem root (python/unoq_files/).
Return types shown are what the sketch receives from result().

─── Filesystem ────────────────────────────────────────────────────────────────

  fs_read_size(path)            → int    file length in chars; -1 on error
  fs_read_chunk(path, off, len) → String up to `len` chars from `off`
  fs_read(path)                 → String full contents (use chunked for large files)
  fs_write(path, content)       → bool   write/overwrite file
  fs_exists(path)               → bool   true if file or directory exists
  fs_delete(path, recursive)    → bool   delete file or directory tree
  fs_mkdir(path)                → bool   create directory (parents included)
  fs_move(src, dst)             → bool   move/rename; prints moved contents to Python tab
  fs_hello(message)             → String write timestamped helloworld.txt, return contents
  fs_list(path, recursive)      → String "Nf Md" summary; full tree printed to Python tab
  fs_search(pattern, path)      → String newline-joined matching paths (≤200 chars)

  LARGE FILE PATTERN — use chunked reads to avoid Bridge timeout:
    int   sz  = 0;  Bridge.call("fs_read_size",  path).result(sz);
    String ch = ""; Bridge.call("fs_read_chunk", path, offset, 128).result(ch);

─── Thread Memory ─────────────────────────────────────────────────────────────

  tm_record(thread, message)    → bool   append a record to a named thread
  tm_read(thread, limit)        → String last N records, newline-joined

─── Networking ────────────────────────────────────────────────────────────────

  net_ping(host)                → bool   TCP reachability (ports 53/80/443)
  net_check(url)                → bool   HTTP 2xx check (fast, bool only)
  net_get(url)                  → String HTTP GET body ≤2048 chars
                                          WARNING: large/slow responses time out;
                                          use net_check if you only need success/fail
  net_post(url, body)           → String HTTP POST body ≤2048 chars

═══════════════════════════════════════════════════════════════════════════════
SKETCH-SIDE USAGE (sketch.ino)
═══════════════════════════════════════════════════════════════════════════════

  #include <Arduino_RouterBridge.h>

  // Simple call — bool result
  bool ok = false;
  Bridge.call("fs_write", String("data/log.txt"), String("reading=22.5")).result(ok);

  // Chunked read helper (see BridgeConnectivityTest.ino for full implementation)
  String content = fs_read_full("data/log.txt");   // assembles chunks automatically

  // Directory listing — summary to sketch, full tree to Python tab
  String summary;
  Bridge.call("fs_list", String("data"), (int)1).result(summary);
  // summary = "3f 1d" (3 files, 1 dir)

  // File search
  String matches;
  Bridge.call("fs_search", String(".txt"), String("")).result(matches);
  // matches = "data/log.txt\ndata/config.txt" (newline-joined, ≤200 chars)

═══════════════════════════════════════════════════════════════════════════════
EXTENDING THE API
═══════════════════════════════════════════════════════════════════════════════

1. Add a handler module: python/handlers/myfeature.py
     class MyFeatureBackend: ...
     def dispatch(backend, request): ...

2. Import and instantiate in main.py:
     from handlers.myfeature import MyFeatureBackend, dispatch as mf_dispatch
     _mf = MyFeatureBackend()

3. Add to _call() dispatchers dict:
     "myfeature": (mf_dispatch, _mf),

4. Write a bridge function and register it:
     def my_action(arg): ...
     Bridge.provide("my_action", my_action)

═══════════════════════════════════════════════════════════════════════════════
RULES
═══════════════════════════════════════════════════════════════════════════════

• App.run() MUST be the last call — it blocks and keeps the container alive.
• Any code after App.run() is unreachable and ignored by the framework.
• Bridge functions that return large strings risk timeout; prefer bool + Python tab logging.
• All file paths are sandboxed — paths that escape the root are rejected.
"""

from __future__ import annotations

import os
import sys
import time
from pathlib import Path

# abspath guards against __file__ being a bare filename with no directory component
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from arduino.app_utils import App, Bridge   # provided by the App Lab runtime

from handlers.filesystem import FileBackend, dispatch as fs_dispatch
from handlers.threadmemory import ThreadMemoryBackend, dispatch as tm_dispatch
from handlers.networking import NetworkingBackend, dispatch as net_dispatch
from handlers.threadmanager import ThreadManagerBackend, dispatch as tmgr_dispatch

# ---------------------------------------------------------------------------
# Backend instances — one per handler, kept alive for the session
# ---------------------------------------------------------------------------

_fs   = FileBackend(Path(__file__).parent / "unoq_files")
_tm   = ThreadMemoryBackend()
_net  = NetworkingBackend()
_tmgr = ThreadManagerBackend()


def _call(domain: str, op: str, **kwargs):
    """Internal helper — call a handler and return the result dict."""
    dispatchers = {
        "file":         (fs_dispatch,   _fs),
        "threadmemory": (tm_dispatch,   _tm),
        "networking":   (net_dispatch,  _net),
        "threadmanager":(tmgr_dispatch, _tmgr),
    }
    fn, backend = dispatchers[domain]
    return fn(backend, {"id": None, "op": op, **kwargs})


def _ok(response: dict):
    """Return result value or raise on error."""
    if not response.get("ok"):
        err = response.get("error", {})
        raise RuntimeError(f"{err.get('code')}: {err.get('message')}")
    return response.get("result")


# ---------------------------------------------------------------------------
# Functions exposed to the sketch via Bridge.call("name", args...)
# Add or remove Bridge.provide() entries to match your sketch's Bridge.call()s
# ---------------------------------------------------------------------------

def fs_read_size(path: str) -> int:
    """Return the character count of a file, or -1 on error. Fast int response."""
    print(f"[bridge] fs_read_size({path!r})")
    try:
        content = _ok(_call("file", "read_text", path=path)).get("content", "")
        print(f"[bridge] fs_read_size -> {len(content)}")
        return len(content)
    except Exception as exc:
        print(f"[bridge] fs_read_size ERROR: {exc}")
        return -1


def fs_read_chunk(path: str, offset: int = 0, length: int = 128) -> str:
    """Return up to `length` chars of a file starting at `offset`."""
    print(f"[bridge] fs_read_chunk({path!r}, offset={offset}, length={length})")
    try:
        content = _ok(_call("file", "read_text", path=path)).get("content", "")
        chunk = content[offset: offset + length]
        print(f"[bridge] fs_read_chunk -> {len(chunk)} chars")
        return chunk
    except Exception as exc:
        print(f"[bridge] fs_read_chunk ERROR: {exc}")
        return ""


def fs_read(path: str) -> str:
    """Return file contents as a string, or "" on error."""
    print(f"[bridge] fs_read({path!r})")
    try:
        content = _ok(_call("file", "read_text", path=path)).get("content", "")
        print(f"[bridge] fs_read -> {len(content)} chars")
        return content
    except Exception as exc:
        print(f"[bridge] fs_read ERROR: {exc}")
        return ""


def fs_write(path: str, content: str) -> bool:
    """Write text to a file. Returns True on success."""
    print(f"[bridge] fs_write({path!r}, {len(content)} chars)")
    try:
        _ok(_call("file", "write_text", path=path, content=content))
        print(f"[bridge] fs_write -> ok")
        return True
    except Exception as exc:
        print(f"[bridge] fs_write ERROR: {exc}")
        return False


def fs_exists(path: str) -> bool:
    """Return True if the file or directory exists."""
    print(f"[bridge] fs_exists({path!r})")
    try:
        result = _ok(_call("file", "exists", path=path)).get("exists", False)
        print(f"[bridge] fs_exists -> {result}")
        return result
    except Exception as exc:
        print(f"[bridge] fs_exists ERROR: {exc}")
        return False


def tm_record(thread: str, message: str, kind: str = "note") -> bool:
    """Append a thread-memory record. Returns True on success."""
    print(f"[bridge] tm_record({thread!r}, {message!r})")
    try:
        _ok(_call("threadmemory", "record", thread=thread, message=message, kind=kind))
        return True
    except Exception as exc:
        print(f"[bridge] tm_record ERROR: {exc}")
        return False


def tm_read(thread: str, limit: int = 20) -> str:
    """Return the last `limit` records for a thread as a newline-joined string."""
    print(f"[bridge] tm_read({thread!r}, limit={limit})")
    try:
        records = _ok(_call("threadmemory", "read", thread=thread, limit=limit)).get("records", [])
        result = "\n".join(f"[{r.get('kind','note')}] {r.get('message','')}" for r in records)
        print(f"[bridge] tm_read -> {len(records)} records")
        return result
    except Exception as exc:
        print(f"[bridge] tm_read ERROR: {exc}")
        return ""


def net_ping(host: str) -> bool:
    """Return True if host is reachable via ICMP ping."""
    print(f"[bridge] net_ping({host!r})")
    try:
        result = _ok(_call("networking", "net_ping", host=host))
        reachable = result.get("reachable", False)
        print(f"[bridge] net_ping -> {reachable}")
        return reachable
    except Exception as exc:
        print(f"[bridge] net_ping ERROR: {exc}")
        return False


def net_check(url: str) -> bool:
    """Return True if url responds with HTTP 2xx — minimal Bridge round-trip."""
    print(f"[bridge] net_check({url!r})")
    try:
        result = _ok(_call("networking", "http_get", url=url, max_bytes=1))
        status = result.get("status", 0)
        ok = 200 <= status < 300
        print(f"[bridge] net_check -> status={status}, reachable={ok}")
        return ok
    except Exception as exc:
        print(f"[bridge] net_check ERROR: {exc}")
        return False


def net_get(url: str) -> str:
    """HTTP GET url, return up to 2048 chars of response body."""
    print(f"[bridge] net_get({url!r})")
    try:
        result = _ok(_call("networking", "http_get", url=url))
        body = result.get("body", "")
        status = result.get("status", 0)
        error = result.get("error", "")
        print(f"[bridge] net_get -> status={status}, {len(body)} chars" + (f", error={error}" if error else ""))
        return body
    except Exception as exc:
        print(f"[bridge] net_get ERROR: {exc}")
        return ""


def net_post(url: str, body: str) -> str:
    """HTTP POST body to url, return up to 2048 chars of response body."""
    print(f"[bridge] net_post({url!r}, {len(body)} chars)")
    try:
        result = _ok(_call("networking", "http_post", url=url, body=body))
        response = result.get("body", "")
        status = result.get("status", 0)
        print(f"[bridge] net_post -> status={status}, {len(response)} chars")
        return response
    except Exception as exc:
        print(f"[bridge] net_post ERROR: {exc}")
        return ""


def fs_delete(path: str, recursive: bool = False) -> bool:
    """Delete a file or directory. Set recursive=True to remove a non-empty directory."""
    print(f"[bridge] fs_delete({path!r}, recursive={recursive})")
    try:
        _ok(_call("file", "delete", path=path, recursive=bool(recursive)))
        print(f"[bridge] fs_delete -> ok")
        return True
    except Exception as exc:
        print(f"[bridge] fs_delete ERROR: {exc}")
        return False


def fs_search(pattern: str, path: str = "") -> str:
    """Return newline-joined paths whose names contain `pattern` (case-insensitive).

    Searches recursively from `path` (default: filesystem root).
    Returns up to 50 matches as a short newline-joined string safe for the Bridge.
    Full results are always printed to the Python tab.
    """
    start = path if path else "."
    print(f"[bridge] fs_search({pattern!r}, start={start!r})")
    try:
        result = _ok(_call("file", "listdir", path=start, recursive=True))
        entries = result.get("entries", [])
        pat = pattern.lower()
        matches = [
            e["path"] for e in entries
            if pat in e["path"].lower()
        ][:50]
        print(f"[bridge] fs_search -> {len(matches)} match(es):")
        for m in matches:
            print(f"  {m}")
        # Return newline-joined; caller should use fs_read_size/chunk if content is needed
        joined = "\n".join(matches)
        # Truncate to 200 chars so the Bridge return never times out
        return joined[:200]
    except Exception as exc:
        print(f"[bridge] fs_search ERROR: {exc}")
        return ""


def fs_list(path: str = "", recursive: bool = True) -> str:
    """List files under path (default: root). Prints tree to Python tab, returns summary."""
    start = path if path else "."
    print(f"[bridge] fs_list({start!r}, recursive={recursive})")
    try:
        result = _ok(_call("file", "listdir", path=start, recursive=bool(recursive)))
        entries = result.get("entries", [])
        files   = [e for e in entries if e.get("is_file")]
        dirs    = [e for e in entries if e.get("is_dir")]
        print(f"[bridge] fs_list -> {len(files)} file(s), {len(dirs)} dir(s)")
        for e in sorted(entries, key=lambda x: x["path"]):
            kind = "FILE" if e.get("is_file") else "DIR "
            size = f"  {e['size']}B" if e.get("size") is not None else ""
            print(f"  [{kind}] {e['path']}{size}")
        return f"{len(files)}f {len(dirs)}d"
    except Exception as exc:
        print(f"[bridge] fs_list ERROR: {exc}")
        return "error"


def fs_hello(message: str) -> str:
    """Write helloworld.txt with timestamp + message, return the file contents."""
    import datetime
    print(f"[bridge] fs_hello({message!r})")
    try:
        now = datetime.datetime.now().isoformat(timespec="seconds")
        content = (
            f"=== Hello from Arduino Uno Q ===\n"
            f"Timestamp : {now}\n"
            f"Message   : {message}\n"
            f"Handler   : handlers/filesystem.py\n"
            f"================================\n"
        )
        _ok(_call("file", "write_text", path="helloworld.txt", content=content))
        print(f"[bridge] fs_hello -> wrote helloworld.txt")
        print(content)
        return content
    except Exception as exc:
        print(f"[bridge] fs_hello ERROR: {exc}")
        return ""


def fs_mkdir(path: str) -> bool:
    """Create a directory (and parents). Returns True on success."""
    print(f"[bridge] fs_mkdir({path!r})")
    try:
        _ok(_call("file", "mkdir", path=path))
        print(f"[bridge] fs_mkdir -> ok")
        return True
    except Exception as exc:
        print(f"[bridge] fs_mkdir ERROR: {exc}")
        return False


def fs_move(src: str, dst: str) -> bool:
    """Move/rename a file or directory. Returns True on success."""
    print(f"[bridge] fs_move({src!r} -> {dst!r})")
    try:
        _ok(_call("file", "rename", src=src, dst=dst, replace=True))
        # Read back moved file and print so evidence is visible in Python tab
        result = _call("file", "read_text", path=dst)
        if result.get("ok"):
            print(f"[bridge] fs_move -> ok, contents of {dst!r}:")
            print(result["result"]["content"])
        return True
    except Exception as exc:
        print(f"[bridge] fs_move ERROR: {exc}")
        return False


def fs_copy(src: str, dst: str) -> bool:
    """Copy a file or directory tree. Returns True on success."""
    print(f"[bridge] fs_copy({src!r} -> {dst!r})")
    try:
        _ok(_call("file", "copy", src=src, dst=dst, replace=True))
        print(f"[bridge] fs_copy -> ok")
        return True
    except Exception as exc:
        print(f"[bridge] fs_copy ERROR: {exc}")
        return False


# ---------------------------------------------------------------------------
# Thread manager bridge functions
# The sketch fires a named task, polls status, then reads the result in chunks.
# This avoids every Bridge timeout problem — nothing blocks on the sketch side.
#
# Built-in tasks registered below:
#   net_fetch  — async HTTP GET; result is the response body
#   fs_scan    — recursive directory listing; result is newline-joined paths
#
# Add your own tasks by calling _tmgr.register("name", fn) where
# fn(arg: str, cancel: threading.Event) -> str
# ---------------------------------------------------------------------------

import threading as _threading


def _task_net_fetch(url: str, cancel: _threading.Event) -> str:
    """Built-in task: HTTP GET url, return full response body."""
    import urllib.request, ssl
    req = urllib.request.Request(url, headers={"User-Agent": "UnoQBridge/1.0"})
    for ctx in (ssl.create_default_context(), ssl._create_unverified_context()):
        try:
            with urllib.request.urlopen(req, timeout=15, context=ctx) as resp:
                return resp.read(65536).decode("utf-8", errors="replace")
        except ssl.SSLError:
            continue
        except Exception as exc:
            return f"ERROR: {exc}"
    return "ERROR: ssl_failed"


def _task_fs_scan(start: str, cancel: _threading.Event) -> str:
    """Built-in task: recursive file listing from start path (default root)."""
    try:
        result = _ok(_call("file", "listdir", path=start or ".", recursive=True))
        entries = result.get("entries", [])
        lines = []
        for e in sorted(entries, key=lambda x: x["path"]):
            kind = "F" if e.get("is_file") else "D"
            size = f" ({e['size']}B)" if e.get("size") is not None else ""
            lines.append(f"[{kind}] {e['path']}{size}")
            if cancel.is_set():
                break
        return "\n".join(lines)
    except Exception as exc:
        return f"ERROR: {exc}"


_tmgr.register("net_fetch", _task_net_fetch)
_tmgr.register("fs_scan",   _task_fs_scan)


def thread_submit(task: str, job: str, arg: str = "") -> str:
    """Start a registered task in a background thread. Returns 'ok' or 'error: ...'."""
    print(f"[bridge] thread_submit(task={task!r}, job={job!r}, arg={arg!r})")
    try:
        result = _ok(_call("threadmanager", "submit", task=task, job=job, arg=arg))
        print(f"[bridge] thread_submit -> {result}")
        return "ok" if result.get("submitted") else f"error: {result.get('error', '')}"
    except Exception as exc:
        print(f"[bridge] thread_submit ERROR: {exc}")
        return f"error: {exc}"


def thread_status(job: str) -> str:
    """Return job status: pending / running / done / error / cancelled / not_found."""
    try:
        result = _ok(_call("threadmanager", "status", job=job))
        status = result.get("status", "not_found")
        print(f"[bridge] thread_status({job!r}) -> {status}")
        return status
    except Exception as exc:
        print(f"[bridge] thread_status ERROR: {exc}")
        return "error"


def thread_result_size(job: str) -> int:
    """Return the char length of a job's result (-1 if not ready or not found)."""
    try:
        result = _ok(_call("threadmanager", "result_size", job=job))
        sz = result.get("size", -1)
        print(f"[bridge] thread_result_size({job!r}) -> {sz}")
        return sz
    except Exception as exc:
        print(f"[bridge] thread_result_size ERROR: {exc}")
        return -1


def thread_result_chunk(job: str, offset: int = 0, length: int = 128) -> str:
    """Return up to `length` chars of a job's result starting at `offset`."""
    try:
        result = _ok(_call("threadmanager", "result_chunk",
                           job=job, offset=offset, length=length))
        chunk = result.get("chunk", "")
        print(f"[bridge] thread_result_chunk({job!r}, {offset}) -> {len(chunk)} chars")
        return chunk
    except Exception as exc:
        print(f"[bridge] thread_result_chunk ERROR: {exc}")
        return ""


def thread_cancel(job: str) -> bool:
    """Request cooperative cancellation of a running job."""
    print(f"[bridge] thread_cancel({job!r})")
    try:
        result = _ok(_call("threadmanager", "cancel", job=job))
        return bool(result.get("cancelled", False))
    except Exception as exc:
        print(f"[bridge] thread_cancel ERROR: {exc}")
        return False


def thread_list() -> str:
    """Return newline-joined 'job: status' lines (≤200 chars); full list in Python tab."""
    try:
        result = _ok(_call("threadmanager", "jobs"))
        jobs = result.get("jobs", [])
        lines = [f"{j['job']}: {j['status']} ({j['task']})" for j in jobs]
        print(f"[bridge] thread_list -> {len(lines)} job(s):")
        for l in lines:
            print(f"  {l}")
        return "\n".join(lines)[:200]
    except Exception as exc:
        print(f"[bridge] thread_list ERROR: {exc}")
        return ""


def thread_clear(dummy: str = "") -> bool:
    """Remove completed/errored/cancelled jobs from the job table."""
    try:
        result = _ok(_call("threadmanager", "clear_done"))
        removed = result.get("cleared", [])
        print(f"[bridge] thread_clear -> removed {len(removed)} job(s)")
        return True
    except Exception as exc:
        print(f"[bridge] thread_clear ERROR: {exc}")
        return False


# ---------------------------------------------------------------------------
# MCU pin-control wrappers  (Python → Arduino)
#
# These call functions registered on the Arduino side with Bridge.provide_safe().
# Bridge.call() blocks until the MCU returns — do NOT call these from inside
# a Bridge.provide() callback (deadlock). Call them from loop() or a thread.
#
# The Arduino sketch must register the matching functions:
#   Bridge.provide_safe("pin_write",   pin_write);
#   Bridge.provide_safe("pin_read",    pin_read);
#   Bridge.provide_safe("pwm_write",   pwm_write);
#   Bridge.provide_safe("adc_read",    adc_read);
#   Bridge.provide_safe("pin_mode",    pin_mode_set);
#   Bridge.provide_safe("led_set",     led_set);
#   Bridge.provide_safe("multi_write", multi_write);
# ---------------------------------------------------------------------------

def mcu_pin_write(pin: int, value: bool) -> bool:
    """Python → Arduino: digitalWrite(pin, value)."""
    print(f"[mcu] pin_write(pin={pin}, value={value})")
    try:
        return bool(Bridge.call("pin_write", int(pin), bool(value)))
    except Exception as exc:
        print(f"[mcu] pin_write ERROR: {exc}")
        return False


def mcu_pin_read(pin: int) -> int:
    """Python → Arduino: digitalRead(pin) → 0 or 1; -1 on error."""
    print(f"[mcu] pin_read(pin={pin})")
    try:
        return int(Bridge.call("pin_read", int(pin)))
    except Exception as exc:
        print(f"[mcu] pin_read ERROR: {exc}")
        return -1


def mcu_pwm_write(pin: int, value: int) -> bool:
    """Python → Arduino: analogWrite(pin, value) where value is 0-255."""
    value = max(0, min(255, int(value)))
    print(f"[mcu] pwm_write(pin={pin}, value={value})")
    try:
        return bool(Bridge.call("pwm_write", int(pin), value))
    except Exception as exc:
        print(f"[mcu] pwm_write ERROR: {exc}")
        return False


def mcu_adc_read(pin: int) -> int:
    """Python → Arduino: analogRead(pin) → 0-4095 (12-bit STM32 ADC); -1 on error."""
    print(f"[mcu] adc_read(pin={pin})")
    try:
        return int(Bridge.call("adc_read", int(pin)))
    except Exception as exc:
        print(f"[mcu] adc_read ERROR: {exc}")
        return -1


def mcu_pin_mode(pin: int, mode: int) -> bool:
    """Python → Arduino: pinMode(pin, mode). mode: 0=INPUT, 1=OUTPUT, 2=INPUT_PULLUP."""
    print(f"[mcu] pin_mode(pin={pin}, mode={mode})")
    try:
        return bool(Bridge.call("pin_mode", int(pin), int(mode)))
    except Exception as exc:
        print(f"[mcu] pin_mode ERROR: {exc}")
        return False


def mcu_led_set(value: bool) -> bool:
    """Python → Arduino: set LED_BUILTIN HIGH/LOW."""
    print(f"[mcu] led_set({value})")
    try:
        return bool(Bridge.call("led_set", bool(value)))
    except Exception as exc:
        print(f"[mcu] led_set ERROR: {exc}")
        return False


def mcu_multi_write(pin_mask: int) -> bool:
    """Python → Arduino: set pins 0-7 from bitmask. Bit N high = pin N HIGH."""
    print(f"[mcu] multi_write(mask=0b{pin_mask:08b})")
    try:
        return bool(Bridge.call("multi_write", int(pin_mask)))
    except Exception as exc:
        print(f"[mcu] multi_write ERROR: {exc}")
        return False


def mcu_notify(method: str, *args) -> None:
    """Python → Arduino: fire-and-forget call (no result waited for)."""
    print(f"[mcu] notify({method!r}, {args})")
    try:
        Bridge.notify(method, *args)
    except Exception as exc:
        print(f"[mcu] notify ERROR: {exc}")


# ---------------------------------------------------------------------------
# Arduino → Python callbacks
#
# The sketch calls Bridge.notify("on_sensor_data", pin, value) to push
# readings without waiting for a result. Register these with Bridge.provide().
# ⚠️  Do NOT call mcu_* functions inside these callbacks — deadlock!
#     Set a flag and act in loop() instead.
# ---------------------------------------------------------------------------

_sensor_readings: dict = {}  # {pin: value} populated by Arduino push

def on_sensor_data(pin: int, value: int) -> None:
    """Receives ADC/digital readings pushed by the Arduino sketch."""
    _sensor_readings[int(pin)] = int(value)
    print(f"[mcu→py] on_sensor_data: pin={pin}, value={value}")


def on_gpio_event(pin: int, state: int) -> None:
    """Receives edge events (rising/falling) pushed by the Arduino sketch."""
    print(f"[mcu→py] on_gpio_event: pin={pin}, state={'HIGH' if state else 'LOW'}")
    # Store in thread-memory for later retrieval
    _call("threadmemory", "record",
          thread="gpio_events",
          message=f"pin={pin} state={'HIGH' if state else 'LOW'}",
          kind="event")


# Register each function so the sketch can call it with Bridge.call("name", ...)
Bridge.provide("fs_read_size",  fs_read_size)
Bridge.provide("fs_read_chunk", fs_read_chunk)
Bridge.provide("fs_read",    fs_read)
Bridge.provide("fs_write",   fs_write)
Bridge.provide("fs_exists",  fs_exists)
Bridge.provide("fs_delete",  fs_delete)
Bridge.provide("fs_search",  fs_search)
Bridge.provide("fs_list",    fs_list)
Bridge.provide("fs_hello",   fs_hello)
Bridge.provide("fs_mkdir",   fs_mkdir)
Bridge.provide("fs_move",    fs_move)
Bridge.provide("fs_copy",    fs_copy)
Bridge.provide("thread_submit",       thread_submit)
Bridge.provide("thread_status",       thread_status)
Bridge.provide("thread_result_size",  thread_result_size)
Bridge.provide("thread_result_chunk", thread_result_chunk)
Bridge.provide("thread_cancel",       thread_cancel)
Bridge.provide("thread_list",         thread_list)
Bridge.provide("thread_clear",        thread_clear)
Bridge.provide("tm_record",  tm_record)
Bridge.provide("tm_read",    tm_read)
Bridge.provide("net_ping",   net_ping)
Bridge.provide("net_check",  net_check)
Bridge.provide("net_get",    net_get)
Bridge.provide("net_post",   net_post)

# Arduino → Python push callbacks (sketch calls Bridge.notify to reach these)
Bridge.provide("on_sensor_data", on_sensor_data)
Bridge.provide("on_gpio_event",  on_gpio_event)
Bridge.provide("blink_enable",   lambda active: _set_blink(bool(active)))

# Initialise the thread-safe MCU dispatcher and register its push callbacks
_mcu.setup(Bridge, backends={})
for _name, _fn in _mcu.BRIDGE_FUNCTIONS.items():
    Bridge.provide(_name, _fn)

print("[UnoQ] Bridge handlers registered — container ready")
print(f"[UnoQ] Filesystem root: {_fs.root}")
print("[UnoQ] MCU functions available (sketch must provide_safe these):")
print("       pin_write, pin_read, pwm_write, adc_read, pin_mode, led_set, multi_write")

# Auto-discover and register any plugin handlers dropped into handlers/
# (skips the core handlers managed above; loads everything else automatically)
from plugin_loader import PluginLoader
_loader = PluginLoader(Bridge, backends={
    "file":          _fs,
    "threadmemory":  _tm,
    "networking":    _net,
    "threadmanager": _tmgr,
})
_loader.load_all()


# ---------------------------------------------------------------------------
# MCU output helpers callable from Python code
# ---------------------------------------------------------------------------

def mcu_set_led(state: bool) -> bool:
    """Python → Arduino: call set_led(bool) registered in BridgeBlink sketch."""
    print(f"[mcu] set_led({state})")
    return bool(_mcu.call_safe("set_led", bool(state)))


# ---------------------------------------------------------------------------
# Blink state — Python-driven LED toggling, independent of any sketch logic
# ---------------------------------------------------------------------------

_blink_active   = False
_blink_state    = False
_blink_interval = 1.0   # seconds between toggles

def _set_blink(active: bool) -> None:
    global _blink_active
    _blink_active = active
    print(f"[blink] {'started' if active else 'stopped'}")


# ---------------------------------------------------------------------------
# Main loop — time-based multitasking so every task runs at its own interval
# without blocking others.  Add new periodic work by tracking _last_* times.
# ---------------------------------------------------------------------------

_last_heartbeat = 0.0
_last_blink     = 0.0

def loop():
    global _blink_state, _last_heartbeat, _last_blink
    now = time.time()

    # Heartbeat every 30 s
    if now - _last_heartbeat >= 30.0:
        _last_heartbeat = now
        tm_record("system", "heartbeat", kind="event")
        print("[UnoQ] heartbeat")

    # Drain the mcu async call queue each tick (thread-safe dispatcher)
    _mcu.loop()

    # LED blink at _blink_interval when enabled
    if _blink_active and now - _last_blink >= _blink_interval:
        _last_blink  = now
        _blink_state = not _blink_state
        mcu_set_led(_blink_state)

    # Short sleep so loop runs ~20×/s without busy-spinning
    time.sleep(0.05)


# ---------------------------------------------------------------------------
# Start the LED Matrix web UI — runs in a background daemon thread on port 5000.
# Access from any device on the same network: http://<UnoQ-IP>:5000
# Requires matrixwebserver.py to be in the same folder as main.py (python/).
# ---------------------------------------------------------------------------
try:
    import matrixwebserver
    matrixwebserver.start()
except ImportError:
    print("[webserver] matrixwebserver.py not found in python/ — web UI disabled")

# ---------------------------------------------------------------------------
# App.run() — MUST be the last call. Keeps the container alive.
# ---------------------------------------------------------------------------
App.run(user_loop=loop)

