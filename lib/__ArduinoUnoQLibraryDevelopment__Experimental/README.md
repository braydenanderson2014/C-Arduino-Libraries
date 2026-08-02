# Arduino UnoQ Experimental Development Folder

## UnoQ Metadata

- Scope: UnoQ experimental branch
- Stability: Experimental
- Documentation label: UnoQ-Experimental-2026-07-29
- Primary integration: UnoQBridgeService.py, ThreadMemoryHandler.py, and UnoQBridgeClient.h
- Transport focus: binary frames, JSON, MessagePack, Yun Process, and Serial/Stream

This folder contains the active experimental branch of selected libraries for
Arduino UnoQ and related dual-core bridge workflows.

## Purpose

- Keep UnoQ-specific development isolated from the normal library set.
- Validate Linux-side file backend integration through `UnoQBridgeService.py`.
- Add multi-backend behavior for SD, LittleFS, mbed, and MessagePack bridge flows.

## Included Libraries

| Library | UnoQ Status | Notes |
|---|---|---|
| `DynamicStorageLibrary` | Updated | Added FS backend support and UnoQ transfer helpers. |
| `EthernetConnectionChecker` | Updated | Added filesystem injection and UnoQ snapshot upload API. |
| `EthernetMach2ConnectionChecker` | Updated | Added UnoQ and filesystem snapshot export APIs. |
| `Hashtable` | Synced | Dependency library used by Properties and others. |
| `Json` | Updated | Added injected FS backend and UnoQ read/write helpers. |
| `MemoryManager` | Updated | Added multi-source block tracking and shrink/split support. |
| `Properties` | Updated | Added UnoQ save/load helpers on top of SD/LittleFS support. |
| `SimpleVector` | Synced | Dependency library used by most experimental libraries. |
| `UnoQBridge` | Scaffolded | Experimental bridge shell for sketch-side Linux helper communication. |

## Shared UnoQ Components

- `UnoQBridgeService.py`: Linux-side backend service for file operations and message delegation.
- `ThreadMemoryHandler.py`: Linux-side helper for thread-memory record handling.
- `UnoQBridgeClient.h`: sketch-side binary bridge client with stream and Yun Process transports.
- `UnoQBridge/`: isolated bridge scaffold folder for the next protocol iteration.

## Notes

- This folder is experimental and may change quickly.
- If you need stable behavior, use the normal `lib` folder libraries.
