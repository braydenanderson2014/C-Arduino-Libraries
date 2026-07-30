# JSON Library (UnoQ Experimental Copy)

## UnoQ Metadata

- Scope: UnoQ experimental branch
- Stability: Experimental
- Documentation label: UnoQ-Experimental-2026-07-29
- Library role: JSON document and file layer with SD/FS/UnoQ bridge support

## UnoQ Experimental Status

- This copy is maintained in the UnoQ experimental folder.
- Supports classic SD-backed file operations through `SdFat`.
- Supports injected filesystem backends (`fs::FS`) for LittleFS and related targets.
- Supports UnoQ bridge helpers for Linux-side file backend integration:
  - `readFromUnoQ(UnoQFileTransferClient&, const String& remotePath)`
  - `writeToUnoQ(UnoQFileTransferClient&, const String& remotePath, bool pretty)`

## Overview

`JSON` is a lightweight JSON document helper for Arduino-class systems with:

- object and array construction
- key-path get/set helpers
- file read/write support
- optional compression/decompression path via LZ4

## Backend Modes

### SD mode

```cpp
#include <JSON.h>

JSON json;
json.beginSD(4);
json.setString("device.name", "unoq");
json.writeToFile("/config.json");
```

### LittleFS or injected FS mode

```cpp
#include <JSON.h>
#include <LittleFS.h>

JSON json;
if (LittleFS.begin()) {
    json.begin(LittleFS);
    json.setNumber("meta.version", 1);
    json.writeToFile("/config.json");
}
```

### UnoQ backend mode

```cpp
#include <JSON.h>
#include "../UnoQFileTransferClient.h"

// Transport implementation omitted for brevity
// UnoQFileTransferClient client(transport);

JSON json;
json.setBool("flags.ready", true);
json.writeToUnoQ(client, "/unoq/config.json", true);
```

## Notes

- This is an experimental branch copy intended for UnoQ bring-up and integration testing.
- API details may evolve as bridge and backend behavior is refined.
