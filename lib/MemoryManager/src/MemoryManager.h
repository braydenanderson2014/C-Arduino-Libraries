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
        LinkedList<MemoryBlock*> blocks; // Assuming LinkedList can hold MemoryBlock pointers
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
            block->id = Random(1, (int)99999);
            block->line = line;
            block->file = file;

            // Additional initialization...
        }
        return block ? block->data : nullptr;
    }

    ~MemoryManager() {
        // Iterate over blocks and free them
        for (size_t i = 0; i < blocks.size(); ++i) {
            MemoryBlock* block = *(blocks.get(i));
            ::operator delete(block); // Adjust to just use free on the block
        }
    }

    void free(void* ptr) {
        MemoryBlock* block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
        block->free = true;
        // Additional cleanup...
        blocks.removeElement(block);
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
                if (block->free == false) {
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
                        if (block->free == false) {
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
        if (blocks.size() > 0) {
            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                if (block->free == false) {
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

    void writeMemoryLeaksToSerialAndFile(const char* filename) {
        writeMemoryLeaksToSerial();
        writeMemoryLeaksToFile(filename);
    }

    //store using json format
    void storeMemoryBlockToFile(const char* filename) {
        if (useSDFile) {
            JSON json;
            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                Custom_String::String blockPath = (Custom_String::String)"blocks." + Custom_String::String(int(block->id));
                json.set(blockPath + ".free", block->free ? "true" : "false");
                json.set(blockPath + ".size", Custom_String::String(float(block->size)));
                json.set(blockPath + ".line", Custom_String::String(float(block->line)));
                json.set(blockPath + ".file", block->file);
                // Include other MemoryBlock attributes as needed
            }
            json.writeToFile(filename);
        }
    } 

    //load using json format
    void loadMemoryBlockFromFile(const char* filename) {
        if (useSDFile) {
            JSON json;
            json.readFromFile(filename);
            for (size_t i = 0; i < blocks.size(); ++i) {
                MemoryBlock* block = *(blocks.get(i));
                Custom_String::String blockPath = (Custom_String::String)"blocks." + Custom_String::String(int(block->id));
                block->free = json.get(blockPath + ".free") == "true";
                block->size = json.get(blockPath + ".size").toInt();
                block->line = json.get(blockPath + ".line").toInt();
                block->file = json.get(blockPath + ".file");
                // Include other MemoryBlock attributes as needed
            }
        }
    }
    

};

#endif // MEMORY_MANAGER_H