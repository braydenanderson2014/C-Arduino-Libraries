#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <Arduino.h>
#include <SdFat.h>
#include <cstring>
#include <cstdlib>
#include "SimpleVector.h"

// Error Codes
#define MM_SUCCESS 0
#define MM_MEMORY_LEAK_ERROR 1
#define MM_ALLOCATION_FAILED 2
#define MM_FILE_WRITE_ERROR 3
#define MM_FILE_READ_ERROR 4
#define MM_FILE_NOT_FOUND 5
#define MM_NO_AVAILABLE_BLOCKS 6
#define MM_BLOCK_NOT_FOUND 7
#define MM_SD_FOUND 8
#define MM_SD_NOT_FOUND 9
#define MM_TEST_FILE_ERROR 10
#define MM_ROOT_DIR_ERROR 11
#define MM_SD_OUT_OF_SPACE 12
#define MM_UNKNOWN_ERROR 99

#define MEMORYBLOCKSIZE 64

#ifndef DEBUG_MEMORY
#define DEBUG_MEMORY 0
#endif

#ifndef SD_CS_PIN
#define SD_CS_PIN 4
#endif

#if DEBUG_MEMORY
    #define DEBUG_PRINT(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
#endif

//#define ENABLE_GARBAGE_COLLECTION  // Uncomment to enable memory block reuse

struct MemoryBlock {
    bool freed;
    unsigned int size;
    unsigned int id;
    unsigned int line;
    char* file;
    void* data;
};

class MemoryManager {
private:
    SimpleVector<MemoryBlock*> memoryBlocks;
    bool useSDFile = false;
    int lastError = MM_SUCCESS;
    unsigned int nextId = 1;
    SdFat sdFat;

    unsigned long totalSDBytes = 0;
    unsigned long usedSDBytes = 0;

    MemoryBlock* findBlockByPtr(void* ptr) {
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && block->data == ptr) {
                return block;
            }
        }
        return nullptr;
    }

    void releaseBlock(MemoryBlock* block) {
        if (!block) {
            return;
        }
        if (block->data) {
            ::free(block->data);
            block->data = nullptr;
        }
        if (block->file) {
            delete[] block->file;
            block->file = nullptr;
        }
        ::free(block);
    }

public:
    explicit MemoryManager(bool autoInitSD = true) {
        if (autoInitSD && sdFat.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
            useSDFile = true;
            totalSDBytes = getTotalSDSize();
        } else {
            if (autoInitSD) {
                DEBUG_PRINT("MM_SD_NOT_FOUND");
            }
            useSDFile = false;
        }
    }

    unsigned long getTotalSDSize() {
        if (!useSDFile) return 0;
        if (!sdFat.card()) {
            lastError = MM_SD_NOT_FOUND;
            return 0;
        }

        uint32_t sectorCount = sdFat.card()->sectorCount();
        if (sectorCount == 0) {
            lastError = MM_TEST_FILE_ERROR;
            return 0;
        }

        totalSDBytes = sectorCount * 512;  // ✅ 512 bytes per sector

        return totalSDBytes;
    }

    unsigned long getUsedSDBytes() {
        if (!useSDFile) return 0;
        if (!sdFat.vol()) {
            lastError = MM_SD_NOT_FOUND;
            return 0;
        }

        uint32_t freeClusters = sdFat.vol()->freeClusterCount();
        uint32_t totalClusters = sdFat.vol()->clusterCount();
        uint32_t usedClusters = totalClusters - freeClusters;

        usedSDBytes = usedClusters * sdFat.vol()->sectorsPerCluster() * 512;  // ✅ Convert to bytes

        return usedSDBytes;
    }

    unsigned long getFreeSDBytes() {
        if (!useSDFile) {
            return 0;
        }
        return totalSDBytes - getUsedSDBytes();
    }

    bool beginSD(uint8_t csPin = SD_CS_PIN) {
        useSDFile = sdFat.begin(csPin, SD_SCK_MHZ(50));
        if (!useSDFile) {
            lastError = MM_SD_NOT_FOUND;
            return false;
        }
        totalSDBytes = getTotalSDSize();
        lastError = MM_SUCCESS;
        return true;
    }

    void setUseSDFile(bool useSD) {
        if (!useSD) {
            useSDFile = false;
            return;
        }
        beginSD(SD_CS_PIN);
    }

    void* malloc(unsigned int size, const char* file, unsigned int line) {
        lastError = MM_SUCCESS;

        if (size == 0) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        DEBUG_PRINT("Allocating memory: " + String(size) + " bytes...");

        if (useSDFile) {
            unsigned long freeSpace = getFreeSDBytes();
            if (size > freeSpace) {
                DEBUG_PRINT("MM_SD_OUT_OF_SPACE");
                lastError = MM_SD_OUT_OF_SPACE;
                return nullptr;
            }
        }

        MemoryBlock* block = static_cast<MemoryBlock*>(::malloc(sizeof(MemoryBlock)));
        if (!block) {
            DEBUG_PRINT("MM_ALLOCATION_FAILED");
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        block->data = ::malloc(size);
        if (!block->data) {
            DEBUG_PRINT("MM_ALLOCATION_FAILED");
            ::free(block);
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        block->freed = false;
        block->size = size;
        block->id = nextId++;
        block->line = line;
        const char* sourceFile = file ? file : "unknown";
        size_t fileLen = std::strlen(sourceFile);
        block->file = new char[fileLen + 1];
        std::strcpy(block->file, sourceFile);

        DEBUG_PRINT("MM_SUCCESS");
        memoryBlocks.put(block);
        return block->data;
    }

    void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line) {
        if (num == 0 || size == 0) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        if (num > (0xFFFFFFFFu / size)) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        unsigned int total = num * size;
        void* ptr = malloc(total, file, line);
        if (!ptr) {
            return nullptr;
        }

        std::memset(ptr, 0, total);
        return ptr;
    }

    void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line) {
        if (ptr == nullptr) {
            return malloc(size, file, line);
        }
        if (size == 0) {
            free(ptr);
            return nullptr;
        }

        MemoryBlock* block = findBlockByPtr(ptr);
        if (!block) {
            lastError = MM_BLOCK_NOT_FOUND;
            return nullptr;
        }

        void* newData = ::realloc(block->data, size);
        if (!newData) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        block->data = newData;
        block->size = size;
        block->line = line;

        if (file) {
            delete[] block->file;
            size_t fileLen = std::strlen(file);
            block->file = new char[fileLen + 1];
            std::strcpy(block->file, file);
        }

        lastError = MM_SUCCESS;
        return block->data;
    }


    int free(void* ptr) {
        if (!ptr) {
            lastError = MM_ALLOCATION_FAILED;
            return MM_ALLOCATION_FAILED;
        }

        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && block->data == ptr) {
                block->freed = true;
                memoryBlocks.remove(block);
                releaseBlock(block);
                lastError = MM_SUCCESS;
                return MM_SUCCESS;
            }
        }

        lastError = MM_BLOCK_NOT_FOUND;
        return MM_BLOCK_NOT_FOUND;
    }

    unsigned int getTrackedMemoryInUse() {
        unsigned int inUse = 0;
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed) {
                inUse += block->size;
            }
        }
        return inUse;
    }

    unsigned int getFreeMemory() {
        // This manager only tracks allocations made through itself.
        // "Free memory" in this context means bytes released from tracked blocks.
        return 0;
    }

    unsigned int getAllocationCount() {
        return memoryBlocks.elements();
    }

    int getLastError() {
        return lastError;
    }

    unsigned long getTotalSDMemory() {
        return totalSDBytes;
    }

    ~MemoryManager() {
        DEBUG_PRINT("Destroying MemoryManager...");
        while (!memoryBlocks.isEmpty()) {
            MemoryBlock* block = memoryBlocks.get(0);
            memoryBlocks.erase(0);
            releaseBlock(block);
        }
    }

    int detectMemoryLeaks() {
        DEBUG_PRINT("Checking for memory leaks...");
        unsigned long leakedMemory = 0;

        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed) {
                Serial.print("Memory leak detected: ");
                Serial.print(block->size);
                Serial.print(" bytes at ");
                Serial.print(block->file);
                Serial.print(":");
                Serial.println(block->line);
                leakedMemory += block->size;
            }
        }

        Serial.print("Total Leaked Memory: ");
        Serial.print(leakedMemory);
        Serial.println(" bytes.");

        if (leakedMemory > 0) {
            lastError = MM_MEMORY_LEAK_ERROR;
            return MM_MEMORY_LEAK_ERROR;
        }

        lastError = MM_SUCCESS;
        return MM_SUCCESS;
    }

    void printMemoryLeaks() {
        (void)detectMemoryLeaks();
    }
};

#endif // MEMORY_MANAGER_H
