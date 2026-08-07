# MemoryManager


<!-- HEALTH_BADGES_START -->
[![Health: Unsure](https://img.shields.io/badge/Health-Unsure-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Not Tested](https://img.shields.io/badge/Testing-Not%20Tested-6f42c1?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

## Overview
The `MemoryManager` tracks allocations made through its API so you can detect leaks and safely free/reallocate blocks in embedded projects. It optionally checks SD card space before allocating.

## Features
- **Tracked allocation APIs**: `malloc`, `calloc`, `realloc`, `free`.
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
| `MM_UNKNOWN_ERROR` | 99 | Unknown error occurred. |

## API Reference

| Function | Description |
|----------|-------------|
| `void* malloc(unsigned int size, const char* file, unsigned int line)` | Allocates memory and tracks allocation details. |
| `int free(void* ptr)` | Frees allocated memory and marks it for reuse (if enabled). |
| `void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line)` | Reallocates memory while preserving existing data. |
| `void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line)` | Allocates and zero-initializes memory. |
| `int detectMemoryLeaks()` | Checks for memory leaks and reports them. |
| `void printMemoryLeaks()` | Prints memory leak details to the serial output. |
| `unsigned int getTrackedMemoryInUse()` | Returns total bytes currently tracked as in-use. |
| `unsigned int getAllocationCount()` | Returns number of active tracked allocations. |
| `void setUseSDFile(bool useSD)` | Enables or disables logging memory leaks to an SD card. |
| `bool beginSD(uint8_t csPin)` | Initializes SD usage explicitly for capacity checks. |
| `~MemoryManager()` | Destructor that frees all allocated memory blocks. |

## Example Usage
```cpp
MemoryManager memoryManager;

void* ptr = memoryManager.malloc(128, __FILE__, __LINE__);
if (ptr) {
    // Use allocated memory
    memoryManager.free(ptr);
}

if (memoryManager.detectMemoryLeaks() == MM_MEMORY_LEAK_ERROR) {
    memoryManager.printMemoryLeaks();
}
```

## License
This project is open-source and available for modification and distribution.
