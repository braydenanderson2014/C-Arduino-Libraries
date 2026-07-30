# MemoryManager

## UnoQ Metadata

- Scope: UnoQ experimental branch
- Stability: Experimental
- Documentation label: UnoQ-Experimental-2026-07-29
- Library role: Multi-backend allocation tracker with shrink/split and MessagePack export

## UnoQ Experimental Status

- This README documents the UnoQ development copy of MemoryManager.
- This branch supports mixed backend tracking (RAM, SD, LittleFS, mbed, MessagePack export).
- Allocation metadata tracks source backend per block.
- Block shrink operations can split/reuse tail fragments.

## Overview
The experimental `MemoryManager` tracks allocations made through its API so you can detect leaks and safely free/reallocate blocks in embedded projects. It supports mixed backend tagging for RAM, SD, LittleFS, mbed, and MessagePack export so you can track which block came from which backend.

## Features
- **Tracked allocation APIs**: `malloc`, `calloc`, `realloc`, `free`.
- **Per-block storage identity**: each allocation is tagged with backend/source metadata.
- **Mixed backend operation**: SD, LittleFS, mbed tracking, and RAM allocations can coexist.
- **MessagePack export**: tracked allocations can be exported through `UnoQFileTransferClient`.
- **Shrink support with split**: blocks can shrink by splitting and reusing tail fragments.
- **Memory leak detection** with file/line source reporting.
- **Error code reporting** via `getLastError()`.
- **Optional SD capacity gate** before allocation attempts.

## Configuration
You can configure behavior using these macros:

| Macro                      | Description |
|----------------------------|-------------|
| `DEBUG_MEMORY`             | Set to `1` to enable debug output, `0` to disable. |
| `SD_CS_PIN`                | Chip-select pin used for optional SD initialization. |

## Error Codes

| Error Code | Value | Description |
|------------|-------|-------------|
| `MM_SUCCESS` | 0 | Operation successful. |
| `MM_MEMORY_LEAK_ERROR` | 1 | Memory leak detected. |
| `MM_ALLOCATION_FAILED` | 2 | Memory allocation failed. |
| `MM_FILE_WRITE_ERROR` | 3 | Failed to write to file. |
| `MM_FILE_READ_ERROR` | 4 | Failed to read from file. |
| `MM_FILE_NOT_FOUND` | 5 | File not found. |
| `MM_NO_AVAILABLE_BLOCKS` | 6 | No available memory blocks. |
| `MM_BLOCK_NOT_FOUND` | 7 | Pointer was not found in tracked allocations. |
| `MM_SD_OUT_OF_SPACE` | 12 | Optional SD free-space check failed. |
| `MM_BLOCK_HAS_DEPENDENTS` | 13 | Root block cannot be freed/reallocated while active split fragments exist. |
| `MM_UNKNOWN_ERROR` | 99 | Unknown error occurred. |

## API Reference

| Function | Description |
|----------|-------------|
| `void* malloc(unsigned int size, const char* file, unsigned int line)` | Allocates memory and tracks allocation details. |
| `void* mallocFrom(MMStorageKind storage, unsigned int size, const char* file, unsigned int line)` | Allocates memory and tags it with the chosen backend. |
| `int free(void* ptr)` | Frees allocated memory and marks it for reuse (if enabled). |
| `void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line)` | Reallocates memory while preserving existing data. |
| `void* reallocFrom(MMStorageKind storage, void* ptr, unsigned int size, const char* file, unsigned int line)` | Reallocates and updates backend metadata. |
| `void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line)` | Allocates and zero-initializes memory. |
| `int shrink(void* ptr, unsigned int newSize, const char* file, unsigned int line, bool splitInsteadOfRealloc)` | Shrinks a block, optionally splitting off reusable tail fragments. |
| `int detectMemoryLeaks()` | Checks for memory leaks and reports them. |
| `void printMemoryLeaks()` | Prints memory leak details to the serial output. |
| `unsigned int getTrackedMemoryInUse()` | Returns total bytes currently tracked as in-use. |
| `unsigned int getAllocationCount()` | Returns number of active tracked allocations. |
| `unsigned int getAllocationCountByStorage(MMStorageKind storage)` | Returns active allocation count for one backend. |
| `unsigned int getTrackedMemoryByStorage(MMStorageKind storage)` | Returns tracked bytes for one backend. |
| `void setUseSDFile(bool useSD)` | Enables or disables logging memory leaks to an SD card. |
| `bool beginSD(uint8_t csPin)` | Initializes SD usage explicitly for capacity checks. |
| `bool beginLittleFS(fs::FS& filesystem)` | Enables LittleFS-backed tracking mode. |
| `bool beginMbed()` | Enables mbed backend tracking (on mbed cores). |
| `void setDefaultStorage(MMStorageKind kind)` | Sets backend used by default `malloc/calloc/realloc`. |
| `MMStorageKind getDefaultStorage() const` | Returns current default backend. |
| `bool exportAllocationsToMsgPack(const String& remotePath)` | Exports allocation metadata as MessagePack through UnoQ transfer client. |
| `~MemoryManager()` | Destructor that frees all allocated memory blocks. |

## Example Usage
```cpp
#include "MemoryManager.h"

MemoryManager memoryManager;

memoryManager.beginSD(4);
memoryManager.setDefaultStorage(MM_STORAGE_SD);

void* ptr = memoryManager.mallocFrom(MM_STORAGE_LITTLEFS, 128, __FILE__, __LINE__);
memoryManager.shrink(ptr, 64, __FILE__, __LINE__, true);
memoryManager.free(ptr);

if (memoryManager.detectMemoryLeaks() == MM_MEMORY_LEAK_ERROR) {
    memoryManager.printMemoryLeaks();
}
```

## License
This project is open-source and available for modification and distribution.

