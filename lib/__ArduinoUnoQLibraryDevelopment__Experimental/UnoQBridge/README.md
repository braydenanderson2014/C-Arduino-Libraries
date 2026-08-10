# UnoQBridge

Arduino-side bridge library for Arduino Uno Q App Lab projects.
Provides the sketch-side protocol scaffold and a Python API wrapper that runs in the Linux container.

---

## Architecture

```
Arduino Sketch (STM32 MCU)          Linux Container (Qualcomm MPU)
──────────────────────────          ──────────────────────────────
#include <Arduino_RouterBridge.h>   python/main.py   ← API wrapper
Bridge.call("fs_write", ...)  ───►  Bridge.provide("fs_write", fs_write)
Bridge.call("net_ping", ...)  ───►  Bridge.provide("net_ping",  net_ping)
                                        │
                                    handlers/
                                      filesystem.py   sandboxed file I/O
                                      threadmemory.py in-process records
                                      networking.py   TCP + HTTP
```

The transport layer (MsgPack IPC over internal UART) is handled entirely by
`Arduino_RouterBridge`. This library only contains the protocol scaffold and
the Python business-logic layer.

---

## Files

| File | Purpose |
|---|---|
| `src/UnoQBridge.h` | Server-side protocol scaffold (sequence IDs, frame header, opcodes) |
| `src/UnoQBridgeClient.h` | Binary client for custom stdio/serial transports (Yún pattern) |
| `src/UnoQFileTransferClient.h` | Type alias for `UnoQBridgeClient` |
| `python/main.py` | App Lab entry point — API wrapper, all `Bridge.provide()` calls |
| `python/handlers/filesystem.py` | `FileBackend` — sandboxed file I/O |
| `python/handlers/threadmemory.py` | `ThreadMemoryBackend` — per-thread record store |
| `python/handlers/networking.py` | `NetworkingBackend` — TCP check, HTTP GET/POST |
| `examples/BasicUsage/` | Unit tests for `UnoQBridge.h` (runs standalone, no Python needed) |
| `examples/BridgeConnectivityTest/` | End-to-end bridge test — file lifecycle + networking |

---

## Bridge API Quick Reference

All paths are relative to `python/unoq_files/` (sandboxed root).
Return types are what the sketch receives from `.result()`.

### Thread Manager (async background tasks)

| Function | Args | Returns | Notes |
|---|---|---|---|
| `thread_submit` | `task, job, arg` | `String` | `"ok"` or `"error: ..."` — returns immediately |
| `thread_status` | `job` | `String` | `pending`/`running`/`done`/`error`/`cancelled` |
| `thread_result_size` | `job` | `int` | char count of result; -1 if not ready |
| `thread_result_chunk` | `job, offset, length` | `String` | safe bounded slice of result |
| `thread_cancel` | `job` | `bool` | cooperative cancel signal |
| `thread_list` | — | `String` | `"job: status"` lines ≤200 chars |
| `thread_clear` | — | `bool` | remove done/error/cancelled jobs |

Built-in registered tasks: **`net_fetch`** (async HTTP GET), **`fs_scan`** (recursive listing).
Add custom tasks with `_tmgr.register("name", fn)` in `main.py`.

```cpp
// Async HTTP GET — never times out regardless of response size
String status;
Bridge.call("thread_submit", String("net_fetch"), String("gh_api"),
            String("https://api.github.com")).result(status);
// poll until done
do { delay(500); Bridge.call("thread_status", String("gh_api")).result(status); }
while (status == "running" || status == "pending");
// read result in chunks
int sz = 0;
Bridge.call("thread_result_size", String("gh_api")).result(sz);
String body = "";  int off = 0;
while (off < sz) {
    String chunk;
    Bridge.call("thread_result_chunk", String("gh_api"), off, 128).result(chunk);
    if (!chunk.length()) break;
    body += chunk;  off += chunk.length();
}
```

### Filesystem

| Function | Args | Returns | Notes |
|---|---|---|---|
| `fs_read_size` | `path` | `int` | char count; -1 on error |
| `fs_read_chunk` | `path, offset, length` | `String` | safe bounded slice |
| `fs_read` | `path` | `String` | full file; use chunked for large files |
| `fs_write` | `path, content` | `bool` | create or overwrite |
| `fs_exists` | `path` | `bool` | file or directory |
| `fs_delete` | `path, recursive` | `bool` | set recursive=1 for directories |
| `fs_mkdir` | `path` | `bool` | creates parent dirs |
| `fs_move` | `src, dst` | `bool` | move or rename |
| `fs_copy` | `src, dst` | `bool` | copy file or directory tree |
| `fs_list` | `path, recursive` | `String` | `"Nf Md"` summary; full tree in Python tab |
| `fs_search` | `pattern, path` | `String` | newline-joined matches ≤200 chars |
| `fs_hello` | `message` | `String` | writes timestamped helloworld.txt, returns contents |

### Thread Memory

| Function | Args | Returns | Notes |
|---|---|---|---|
| `tm_record` | `thread, message` | `bool` | appends a record |
| `tm_read` | `thread, limit` | `String` | last N records, newline-joined |

### Networking

| Function | Args | Returns | Notes |
|---|---|---|---|
| `net_ping` | `host` | `bool` | TCP check on ports 53/80/443 |
| `net_check` | `url` | `bool` | HTTP 2xx (fast — no body returned) |
| `net_get` | `url` | `String` | HTTP GET body ≤2048 chars |
| `net_post` | `url, body` | `String` | HTTP POST body ≤2048 chars |

> **Bridge string timeout rule:** functions that do slow work (network I/O) or return large
> strings can exceed the RPC timeout. Prefer `bool`-returning functions where possible.
> Use `fs_read_size` + `fs_read_chunk` in a loop for files larger than ~200 chars.

---

## Sketch-Side Usage

```cpp
#include <Arduino_RouterBridge.h>

// Write a file
bool ok = false;
Bridge.call("fs_write", String("sensors/temp.txt"), String("22.5")).result(ok);

// Read a file in safe chunks (handles any size)
// fs_read_full() helper is provided in BridgeConnectivityTest.ino
String content = fs_read_full("sensors/temp.txt");

// List a directory — summary to sketch, full tree to Python tab
String summary;
Bridge.call("fs_list", String("sensors"), (int)1).result(summary);
// summary == "3f 0d"

// Search for files
String matches;
Bridge.call("fs_search", String(".txt"), String("sensors")).result(matches);

// Delete a file
Bridge.call("fs_delete", String("sensors/temp.txt"), (int)0).result(ok);

// TCP ping
bool reachable = false;
Bridge.call("net_ping", String("8.8.8.8")).result(reachable);

// HTTP check (fast, bool only)
bool up = false;
Bridge.call("net_check", String("http://example.com")).result(up);
```

---

## Adding a New Handler

1. Create `python/handlers/myfeature.py`:
   ```python
   class MyFeatureBackend:
       def my_op(self, arg): ...

   def dispatch(backend, request):
       # map op names to backend methods
       ...
   ```

2. In `main.py`, import and register:
   ```python
   from handlers.myfeature import MyFeatureBackend, dispatch as mf_dispatch
   _mf = MyFeatureBackend()
   # add to _call() dispatchers: "myfeature": (mf_dispatch, _mf)

   def my_action(arg: str) -> bool:
       ...
   Bridge.provide("my_action", my_action)
   ```

---

## Notes

- `UnoQBridgeClient.h` / `UnoQBridgeService.py` implement a custom stdio/binary transport
  for non-App-Lab deployments (Yún, bare Linux). These are not used when running under App Lab.
- All file paths are sandboxed — attempts to escape the root are rejected with `PATH_ESCAPE`.
- Thread-memory records are in-process only and do not survive a container restart.
