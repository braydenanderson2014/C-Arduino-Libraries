#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MEMORYBLOCKSIZE 64
#include <CustomString.h>
#include "BasicLinkedList.h"
#include <new>
#include <MathLib.h>
#include <SD.h>
#include <JSON.h>

struct MemoryBlock {
    bool free; // Is the block free?
    unsigned int size; // Size of the allocation
    unsigned int id; // Unique identifier for the block
    unsigned int line; // Line number of the allocation
    Custom_String::String file; // File name of the allocation
    void* data; // Pointer to the user data portion
    void* baseAddress; // Store the base address of the allocation
};

class MemoryManager {
private:
    LinkedList<MemoryBlock*> blocks; // LinkedList of allocated MemoryBlocks
    bool useSDFile = false;

public:
    MemoryManager() {}

    void* malloc(unsigned int size, const char* file, unsigned int line) {
        MemoryBlock* block = (MemoryBlock*)::malloc(size + sizeof(MemoryBlock));
        if (block) {
            block->free = false;
            block->size = size;
            block->data = (char*)block + sizeof(MemoryBlock);
            block->baseAddress = block;
            block->id = Random(1, 99999);
            block->line = line;
            block->file = file;
            blocks.add(block); // Add to linked list
        }
        return block ? block->data : nullptr;
    }

    ~MemoryManager() {
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            ::free(block);
        }
    }

    void free(void* ptr) {
        MemoryBlock* block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
        block->free = true;
        blocks.removeElement(block);
        ::free(block);
    }

    void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line) {
        MemoryBlock* block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
        void* newPtr = malloc(size, file, line);
        if (newPtr) {
            memcpy(newPtr, ptr, block->size);
            free(ptr);
        }
        return newPtr;
    }

    void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line) {
        void* ptr = malloc(num * size, file, line);
        if (ptr) {
            memset(ptr, 0, num * size);
        }
        return ptr;
    }

    void dumpMemoryLeaks() {
        if (blocks.size() > 0) {
            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                if (!block->free) {
                    Serial.print("Memory leak detected: ");
                    Serial.print(block->file.C_STR());
                    Serial.print(" (line ");
                    Serial.print(block->line);
                    Serial.print(") ");
                    Serial.print(block->size);
                    Serial.println(" bytes");
                }
            }
        }
    }

    void setUseSDFile(bool useSD) {
        useSDFile = useSD;
    }

    void writeMemoryLeaksToFile(const char* filename) {
        if (useSDFile) {
            File file = SD.open(filename, FILE_WRITE);
            if (file) {
                if (blocks.size() > 0) {
                    for (size_t i = 0; i < blocks.size(); ++i) {
                        MemoryBlock* block = *(blocks.get(i));
                        if (!block->free) {
                            file.print("Memory leak detected: ");
                            file.print(block->file.C_STR());
                            file.print(" (line ");
                            file.print(block->line);
                            file.print(") ");
                            file.print(block->size);
                            file.println(" bytes");
                        }
                    }
                }
                file.close();
            }
        }
    }

    void writeMemoryLeaksToSerial() {
        dumpMemoryLeaks();
    }

    void writeMemoryLeaksToSerialAndFile(const char* filename) {
        writeMemoryLeaksToSerial();
        writeMemoryLeaksToFile(filename);
    }

    // Store memory blocks in JSON format
    void storeMemoryBlockToFile(const char* filename) {
        if (useSDFile) {
            JSON json;

            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                String blockPath = "blocks." + String(block->id);

                json.setBool(blockPath + ".free", block->free);
                json.setNumber(blockPath + ".size", block->size);
                json.setNumber(blockPath + ".line", block->line);
                json.setString(blockPath + ".file", block->file.C_STR());
            }

            json.writeToFile(filename, true);
        }
    } 

    // Load memory blocks from JSON format
    void loadMemoryBlockFromFile(const char* filename) {
        if (useSDFile) {
            JSON json;
            if (!json.readFromFile(filename)) {
                Serial.println("Failed to load JSON file.");
                return;
            }

            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                String blockPath = "blocks." + String(block->id);

                block->free = json.getBool(blockPath + ".free", false);
                block->size = json.getNumber(blockPath + ".size", 0);
                block->line = json.getNumber(blockPath + ".line", 0);
                block->file = json.getString(blockPath + ".file", "").c_str();
            }
        }
    }

    //getfreememory
    uint32_t getFreeMemory() {
        uint32_t freeMemory = 0;
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            if (block->free) {
                freeMemory += block->size;
            }
        }
        return freeMemory;
    }
};

#endif // MEMORY_MANAGER_H
