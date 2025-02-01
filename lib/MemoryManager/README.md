# MemoryManager

## Overview
The `MemoryManager` is a dynamic memory management system for embedded systems, offering memory allocation, garbage collection, and memory leak detection. It supports JSON-based storage for memory tracking and integrates with SD cards for logging.

## Features
- **Dynamic memory allocation** with block reuse optimization.
- **Garbage collection** to manage memory efficiently.
- **Memory leak detection and reporting** via serial or SD card.
- **JSON-based storage** for persistent memory tracking.
- **Configurable settings** via preprocessor directives.

## Configuration
You can enable or disable features using the following macros:

| Macro                      | Description |
|----------------------------|-------------|
| `ENABLE_JSON_STORAGE`      | Enables storing memory allocations in JSON format. |
| `ENABLE_GARBAGE_COLLECTION` | Enables reuse of freed memory blocks to improve efficiency. |

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
| `int saveMemoryLeaks(const char* filename)` | Saves memory leak information to an SD file. |
| `unsigned int getFreeMemory()` | Returns the total amount of free memory available. |
| `void setUseSDFile(bool useSD)` | Enables or disables logging memory leaks to an SD card. |
| `int storeMemoryBlockToFile(const char* filename)` | Saves memory block information in a JSON file. |
| `int loadMemoryBlockFromFile(const char* filename)` | Loads memory block information from a JSON file. |
| `~MemoryManager()` | Destructor that frees all allocated memory blocks. |

## Example Usage
```cpp
MemoryManager memoryManager;

void* ptr = memoryManager.malloc(128, __FILE__, __LINE__);
if (ptr) {
    // Use allocated memory
    memoryManager.free(ptr);
}

memoryManager.detectMemoryLeaks();
memoryManager.printMemoryLeaks();
```

## License
This project is open-source and available for modification and distribution.

