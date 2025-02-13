#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "BasicLinkedList.h"
#include <new>
#include <SdFat.h>   // ✅ Use Adafruit SdFat library
#include <JSON.h>
#include "ArrayList.h"
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
#define ENABLE_JSON_STORAGE  // Uncomment to enable JSON-based storage tracking
#define DEBUG_MEMORY 1 // Set to 1 to enable debug prints

#if DEBUG_MEMORY
    #define DEBUG_PRINT(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
#endif

//#define ENABLE_GARBAGE_COLLECTION  // Uncomment to enable memory block reuse
//#define SD_CS_PIN 10
SdFat sdFat;  // ✅ Use SdFat instead of SD

struct MemoryBlock {
    bool free;
    unsigned int size;
    unsigned int id;
    unsigned int line;
    char* file;
    void* data;
};

class MemoryManager {
private:
    SimpleVector<MemoryBlock*>* memoryBlocks;
    bool useSDFile = false;
    int lastError = MM_SUCCESS;
    static unsigned int idCounter;

    unsigned long totalSDBytes = 0;
    unsigned long usedSDBytes = 0;

public:
    MemoryManager() {
        if (sdFat.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {  // ✅ Initialize SD
            useSDFile = true;
            totalSDBytes = getTotalSDSize();
        } else {
            Serial.println("MM_SD_NOT_FOUND");
            useSDFile = false;
        }
    }

    // **Get Total SD Card Size**
    unsigned long getTotalSDSize() {
        if (!useSDFile) return 0;

        uint32_t sectorCount = sdFat.card()->sectorCount();
        if (sectorCount == 0) {
            lastError = MM_TEST_FILE_ERROR;
            return 0;
        }

        totalSDBytes = sectorCount * 512;  // ✅ 512 bytes per sector

        return totalSDBytes;
    }

    // **Calculate Used Space on SD Card**
    unsigned long getUsedSDBytes() {
        if (!useSDFile) return 0;

        uint32_t freeClusters = sdFat.vol()->freeClusterCount();
        uint32_t totalClusters = sdFat.vol()->clusterCount();
        uint32_t usedClusters = totalClusters - freeClusters;

        usedSDBytes = usedClusters * sdFat.vol()->sectorsPerCluster() * 512;  // ✅ Convert to bytes

        return usedSDBytes;
    }

    unsigned long getFreeSDBytes() {
        return totalSDBytes - getUsedSDBytes();
    }

    void* malloc(unsigned int size, const char* file, unsigned int line) {
        lastError = MM_SUCCESS;

        DEBUG_PRINT("Allocating memory: " + String(size) + " bytes...");

        // Perform memory check before allocation
        if (useSDFile) {
            unsigned long freeSpace = getFreeSDBytes();
            if (size > freeSpace) {
                DEBUG_PRINT("MM_SD_OUT_OF_SPACE");
                lastError = MM_SD_OUT_OF_SPACE;
                return nullptr;
            }
        }

        MemoryBlock* block = (MemoryBlock*)::malloc(sizeof(MemoryBlock));
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

        block->free = false;
        block->size = size;
        block->id = idCounter++;
        block->line = line;
        block->file = new char[strlen(file) + 1];
        strcpy(block->file, file);

        DEBUG_PRINT("MM_SUCCESS");
        memoryBlocks->put(block);
        return block->data;
    }


    int free(void* ptr) {
        if (!ptr) return MM_ALLOCATION_FAILED;

        for (size_t i = 0; i < memoryBlocks->size(); ++i) {
            Optional<MemoryBlock*> optionalBlock = memoryBlocks->get(i);
            if (!optionalBlock.hasValue()) continue;
            MemoryBlock* block = optionalBlock.getValue();

            if (block->data == ptr) {
                block->free = true;
                memoryBlocks->remove(block);
                ::free(block->data);
                delete[] block->file;
                ::free(block);
                return MM_SUCCESS;
            }
        }

        lastError = MM_UNKNOWN_ERROR;
        return MM_UNKNOWN_ERROR;
    }

    unsigned int getFreeMemory() {
        unsigned int freeMemory = 0;
        for (size_t i = 0; i < memoryBlocks->size(); ++i) {
            Optional<MemoryBlock*> optionalBlock = memoryBlocks->get(i);
            if (!optionalBlock.hasValue()) continue;
            MemoryBlock* block = optionalBlock.getValue();
            if (block->free) freeMemory += block->size;
        }
        return freeMemory;
    }

    int getLastError() {
        return lastError;
    }

    unsigned long getTotalSDMemory() {
        return totalSDBytes;
    }

    ~MemoryManager() {
        Serial.println("Destroying MemoryManager...");
        while (!memoryBlocks->isEmpty()) {
            Optional<MemoryBlock*> optionalBlock = memoryBlocks->get(0);
            if (!optionalBlock.hasValue()) continue;
            MemoryBlock* block = optionalBlock.getValue();
            if (block) {
                memoryBlocks->remove(0);
                ::free(block->data);
                delete[] block->file;
                ::free(block);
            }
        }
    }

    void detectMemoryLeaks() {
        Serial.println("Checking for memory leaks...");
        unsigned long leakedMemory = 0;

        for (size_t i = 0; i < memoryBlocks->size(); ++i) {
            Optional<MemoryBlock*> optionalBlock = memoryBlocks->get(i);
            if (!optionalBlock.hasValue()) continue;
            MemoryBlock* block = optionalBlock.getValue();
            if (!block->free) {
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
    }
};

// Initialize static ID counter
unsigned int MemoryManager::idCounter = 1;

#endif // MEMORY_MANAGER_H
