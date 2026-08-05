# DynamicStorageLibrary for Arduino

## UnoQ Metadata

- Scope: UnoQ experimental branch
- Stability: Experimental
- Documentation label: UnoQ-Experimental-2026-07-29
- Library role: Hybrid RAM/SD/FS storage with UnoQ backend transfer helpers

## UnoQ Experimental Status

- This copy is part of the UnoQ experimental folder.
- Added filesystem injection support through `beginFS(fs::FS&)`.
- Added UnoQ bridge transfer helpers:
  - `saveBlocksToUnoQ(...)`
  - `loadBlocksFromUnoQ(...)`
- SD and FS-backed persistence both remain available.

## Overview
DynamicStorageLibrary is a flexible storage helper for Arduino projects that can keep data in RAM, on SD, or automatically switch between the two. It is designed for simple key/value storage, list storage, and lightweight persistence without requiring a full database layer.

## What's new in 1.0.0
- Added variant-style handling so repeated writes to the same key can grow from a single value into a list automatically.
- Improved support for mixed values and list-backed storage.
- Refined the SD-backed persistence flow and refreshed the public documentation.

## Features
- Automatic mode switching with RAM, SD, or AUTO modes
- Key/value storage plus list-style storage under the same key
- Batch-style insertion of values and lists
- Optional SD persistence using JSON-backed blocks
- Custom filenames per storage instance

## Installation
- Place the DynamicStorageLibrary folder in your Arduino libraries directory, or add it to a PlatformIO project.
- Include the header in your sketch:

```cpp
#include <DynamicStorageLibrary.h>
```

## Quick Start
```cpp
#include <DynamicStorageLibrary.h>

DynamicStorage<String, int> storage(DynamicStorage<String, int>::AUTO);

void setup() {
  Serial.begin(115200);
  delay(1000);

  storage.add(10);
  storage.add(20);
  storage.put("score", 42);

  Serial.println(storage.get(0));
  Serial.println(storage.get("score"));
}

void loop() {
}
```

## Example Sketch
A basic example is available in the examples folder:
- examples/BasicUsage/BasicUsage.ino

## Notes
- SD support requires a valid SD card and a configured chip-select pin when calling beginSD().
- For SD persistence, use string or numeric keys. Pointer-based keys are not supported.

## Changelog
### 1.0.0
- Initial packaged release for the updated DynamicStorageLibrary.
- Added variant-style behavior for repeated writes to the same key.
- Improved list and key/value support.
- Updated documentation and library metadata for Arduino and PlatformIO discovery.
