# Arduino UnoQ Experimental Development Folder

Active experimental development for the Arduino Uno Q dual-core bridge workflow.
The sketch runs on the STM32 MCU; Python runs in a Linux container on the Qualcomm MPU.
Transport between them is handled by `Arduino_RouterBridge` / `Arduino_RPClite` (App Lab).

---

## Architecture

```
Arduino Sketch (STM32 MCU)              Linux Container (Qualcomm MPU)
──────────────────────────              ──────────────────────────────
#include <Arduino_RouterBridge.h>       main.py          ← API wrapper
Bridge.call("fs_write", ...)  ───────►  handlers/
Bridge.call("net_ping",  ...)            filesystem.py   sandboxed file I/O
Bridge.call("tm_record", ...)            threadmemory.py in-process records
                                         networking.py   TCP + HTTP
```

`UnoQBridgeService.py` and `UnoQBridgeClient.h` implement a separate
stdio/binary transport for non-App-Lab deployments (Yún, bare Linux).

---

## Python Files (App Lab container)

| File | Purpose |
|---|---|
| `main.py` | App Lab entry point — all `Bridge.provide()` registrations, full API docstring |
| `handlers/__init__.py` | Package marker |
| `handlers/filesystem.py` | `FileBackend` — sandboxed read/write/delete/move/copy/list/search |
| `handlers/threadmemory.py` | `ThreadMemoryBackend` — per-thread in-process record store |
| `handlers/networking.py` | `NetworkingBackend` — TCP check, HTTP GET/POST |
| `handlers/threadmanager.py` | `ThreadManagerBackend` — named background task queue |
| `UnoQBridgeService.py` | Standalone stdio/msgpack/binary transport (non-App-Lab) |
| `ThreadMemoryHandler.py` | Thin standalone wrapper; re-exports from `handlers/threadmemory` |

## C++ Files

| File | Purpose |
|---|---|
| `UnoQBridge/src/UnoQBridge.h` | Server-side protocol scaffold (sequence IDs, frame header, opcodes) |
| `UnoQBridge/src/UnoQBridgeClient.h` | Binary client for custom stdio/serial transports |
| `UnoQFileTransferClient.h` | Type alias for `UnoQBridgeClient` |

---

## Bridge API Summary

All paths are relative to `python/unoq_files/` (sandboxed). See `main.py` docstring for full reference.

### Thread Manager (async background tasks)
`thread_submit` · `thread_status` · `thread_result_size` · `thread_result_chunk` · `thread_cancel` · `thread_list` · `thread_clear`

> **Async pattern (solves Bridge timeout for any slow work):**
> 1. `thread_submit("net_fetch", "my_job", url)` → `"ok"` immediately
> 2. Poll `thread_status("my_job")` → `"running"` … `"done"`
> 3. `thread_result_size("my_job")` → int, then loop `thread_result_chunk("my_job", offset, 128)`

### Filesystem
`fs_read_size` · `fs_read_chunk` · `fs_read` · `fs_write` · `fs_exists` · `fs_delete` · `fs_mkdir` · `fs_move` · `fs_copy` · `fs_list` · `fs_search` · `fs_hello`

### Thread Memory
`tm_record` · `tm_read`

### Networking
`net_ping` · `net_check` · `net_get` · `net_post`

> **Bridge string timeout:** functions returning large strings can exceed the RPC timeout.
> Use `fs_read_size` + `fs_read_chunk` for files, `net_check` instead of `net_get` for HTTP.
> See `BridgeConnectivityTest.ino` for worked examples of every pattern.

---

## Included Libraries

| Library | Status | Notes |
|---|---|---|
| `UnoQBridge` | Active | Bridge scaffold, Python handlers, App Lab examples |
| `DynamicStorageLibrary` | Updated | FS backend support, UnoQ transfer helpers |
| `EthernetConnectionChecker` | Updated | Filesystem injection, UnoQ snapshot upload API |
| `EthernetMach2ConnectionChecker` | Updated | UnoQ and filesystem snapshot export APIs |
| `Hashtable` | Synced | Dependency used by Properties and others |
| `Json` | Updated | Injected FS backend, UnoQ read/write helpers |
| `MemoryManager` | Updated | Multi-source block tracking, shrink/split support |
| `Properties` | Updated | UnoQ save/load helpers over SD/LittleFS |
| `SimpleChannel` | Synced | Dependency |
| `SimpleCoreRuntime` | Synced | Dependency |
| `SimpleMutex` | Synced | Dependency |
| `SimpleRWLock` | Synced | Dependency |
| `SimpleSemaphore` | Synced | Dependency |
| `SimpleThreadManager` | Synced | Dependency |
| `SimpleVector` | Synced | Dependency used by most experimental libraries |

---

## Examples

| Sketch | Location | What it tests |
|---|---|---|
| `BasicUsage` | `UnoQBridge/examples/BasicUsage/` | `UnoQBridge.h` unit tests — runs standalone, no Python needed |
| `BridgeConnectivityTest` | `UnoQBridge/examples/BridgeConnectivityTest/` | Full end-to-end bridge: file lifecycle, chunked read, search, delete, networking |

---

## Notes

- Experimental — may change quickly.
- For stable behaviour use the main `lib/` folder.
- The `__pycache__/` folder is generated locally and is not deployed to App Lab.

