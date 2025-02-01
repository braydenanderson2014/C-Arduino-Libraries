#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MEMORYBLOCKSIZE 64
#include "BasicLinkedList.h"
#include <new>
#include <MathLib.h>
#include <SD.h>
#include <JSON.h>

// Error codes
#define MM_SUCCESS 0
#define MM_MEMORY_LEAK_ERROR 1
#define MM_ALLOCATION_FAILED 2
#define MM_FILE_WRITE_ERROR 3
#define MM_FILE_READ_ERROR 4
#define MM_FILE_NOT_FOUND 5
#define MM_NO_AVAILABLE_BLOCKS 6
#define MM_UNKNOWN_ERROR 99

#define ENABLE_JSON_STORAGE  // Uncomment to enable JSON-based storage tracking
#define ENABLE_GARBAGE_COLLECTION  // Uncomment to enable memory block reuse

struct MemoryBlock {
    bool free;
    unsigned int size;
    unsigned int id;
    unsigned int line;
    String file;
    void* data;
};

class MemoryManager {
private:
    LinkedList<MemoryBlock*> blocks;
    bool useSDFile = false;

public:
    MemoryManager() {}

    // Allocate Memory
    void* malloc(unsigned int size, const char* file, unsigned int line) {
        #ifdef ENABLE_GARBAGE_COLLECTION
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (block->free && block->size >= size) {
                block->free = false;
                block->line = line;
                block->file = file;
                return block->data;
            }
        }
        #endif

        // Allocate MemoryBlock and the data in one malloc call
        MemoryBlock* block = (MemoryBlock*)::malloc(sizeof(MemoryBlock) + size);
        if (!block) return nullptr;

        block->data = (void*)((char*)block + sizeof(MemoryBlock));
        block->free = false;
        block->size = size;
        block->id = static_cast<uint16_t>(random(1, 99999));
        block->line = line;
        block->file = file;

        blocks.add(block);
        debugPrintMemoryBlocks();
        return block->data;
    }


    void debugPrintMemoryBlocks() {
        Serial.println("---- Memory Block Debug ----");
        Serial.print("Total Blocks: ");
        Serial.println(blocks.size());

        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            Serial.print("Block ID: ");
            Serial.print(block->id);
            Serial.print(", Size: ");
            Serial.print(block->size);
            Serial.print(", Free: ");
            Serial.println(block->free ? "Yes" : "No");
        }
        Serial.println("---------------------------");
    }


    // Free Memory
    int free(void* ptr) {
        if (!ptr) return MM_ALLOCATION_FAILED;
    
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (block->data == ptr) {
                block->free = true;
                debugPrintMemoryBlocks();
                #ifndef ENABLE_GARBAGE_COLLECTION
                blocks.removeElement(block);
                ::free(block);
                #endif
                return MM_SUCCESS;
            }
        }
        return MM_UNKNOWN_ERROR;
    }


    // Reallocate Memory
    void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line) {
        if (!ptr) return malloc(size, file, line);

        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (block->data == ptr) {
                if (block->size >= size) return ptr;

                void* newPtr = malloc(size, file, line);
                if (!newPtr) return nullptr;

                memcpy(newPtr, ptr, block->size);
                free(ptr);
                return newPtr;
            }
        }

        return nullptr;
    }

    // Allocate Zero-Initialized Memory
    void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line) {
        void* ptr = malloc(num * size, file, line);
        if (ptr) memset(ptr, 0, num * size);
        return ptr;
    }

    

    int detectMemoryLeaks() {
        int leakCount = 0;
        Serial.println("=== Memory Leak Detection ===");
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (!block->free) {
                leakCount++;
                Serial.print("Leak: ");
                Serial.print(block->size);
                Serial.print(" bytes at ");
                Serial.print(block->file);
                Serial.print(" (line ");
                Serial.print(block->line);
                Serial.println(")");
            }
        }
        Serial.println("=============================");
        return leakCount > 0 ? MM_MEMORY_LEAK_ERROR : MM_SUCCESS;
    }


    // Destructor: Free All Blocks
    ~MemoryManager() {
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            ::free(block->data);
            ::free(block);
        }
    }

    // Print Memory Leaks
    void printMemoryLeaks() {
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (!block->free) {
                Serial.print("Memory Leak: ");
                Serial.print(block->size);
                Serial.print(" bytes at ");
                Serial.print(block->file);
                Serial.print(" (line ");
                Serial.print(block->line);
                Serial.println(")");
            }
        }

        if (blocks.size() == 0) {
            Serial.println("No memory leaks detected.");
        }
    }

    // Get Free Memory
    unsigned int getFreeMemory() {
        unsigned int freeMemory = 0;
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (block->free) freeMemory += block->size;
        }
        return freeMemory;
    }

};

#endif // MEMORY_MANAGER_H
