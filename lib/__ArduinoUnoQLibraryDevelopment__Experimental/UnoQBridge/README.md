# UnoQBridge

## Experimental Scaffold

This library is an experimental scaffold for the Arduino-side bridge work used with UNO Q / VENTUNO Q style workflows.
It may not work yet, may change without notice, and should be treated as disposable until the protocol settles.

If a helper library needs edits, copy it into this folder before modifying it so the stable libraries stay untouched.

## Purpose

- Keep bridge experiments isolated from the production library set.
- Define a binary sketch-side API for Linux-side helper communication.
- Leave room for protocol work such as sequence IDs, resource IDs, and binary packet framing.

## Current Scope

- `UnoQBridge.h` provides the initial protocol scaffold.
- `UnoQBridgeClient.h` provides the binary client wrapper used by sketches.
- `UnoQBridgeService.py` now acts as a multipurpose file and message dispatcher.
- `ThreadMemoryHandler.py` holds thread-memory specific handling.
- No production transport, file service, or remote memory system is implemented yet.
- This folder is intentionally minimal so it can evolve safely.

## Notes

- Experimental only.
- May not compile or work on every target.
- Use the stable libraries in `lib/` for production work.
