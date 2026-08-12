#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <Arduino.h>
#include <SdFat.h>
#include <cstring>
#include <cstdlib>
#include "SimpleVector.h"

#if defined(__has_include) && __has_include(<FS.h>)
    #include <FS.h>
#else
    namespace fs {
        class FS {
        public:
            virtual ~FS() = default;
            virtual File open(const char* path, const char* mode) = 0;
            virtual bool exists(const char* path) = 0;
            virtual bool remove(const char* path) = 0;
        };
    }
#endif

#if defined(__has_include) && __has_include("../../UnoQFileTransferClient.h")
    #include "../../UnoQFileTransferClient.h"
    #define MM_HAS_UNOQ_TRANSFER 1
#else
    #define MM_HAS_UNOQ_TRANSFER 0
#endif

// Define MM_USE_BRIDGE before including to add Bridge-backed export/log methods.
#ifdef MM_USE_BRIDGE
  #include <Arduino_RouterBridge.h>
  #ifndef MM_BRIDGE_CHUNK_SIZE
    #define MM_BRIDGE_CHUNK_SIZE 128
  #endif
#endif

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
#define MM_BLOCK_HAS_DEPENDENTS 13
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
    bool ownsMemory;
    bool virtualSlice;
    unsigned int size;
    unsigned int id;
    unsigned int rootId;
    unsigned int line;
    char* file;
    void* data;
    String storageTag;
    uint8_t storageKind;
};

enum MMStorageKind : uint8_t {
    MM_STORAGE_RAM = 0,
    MM_STORAGE_SD = 1,
    MM_STORAGE_LITTLEFS = 2,
    MM_STORAGE_MBED = 3,
    MM_STORAGE_MSGPACK = 4
};

class MemoryManager {
private:
    SimpleVector<MemoryBlock*> memoryBlocks;
    bool useSDFile = false;
    bool useLittleFS = false;
    bool useMbedTracking = false;
    int lastError = MM_SUCCESS;
    unsigned int nextId = 1;
    SdFat sdFat;
    fs::FS* littleFS = nullptr;
    MMStorageKind defaultStorage = MM_STORAGE_RAM;

    unsigned long totalSDBytes = 0;
    unsigned long usedSDBytes = 0;

#if MM_HAS_UNOQ_TRANSFER
    UnoQFileTransferClient* remoteTransfer = nullptr;
#endif

    static const char* storageKindName(MMStorageKind kind) {
        switch (kind) {
            case MM_STORAGE_SD: return "SD";
            case MM_STORAGE_LITTLEFS: return "LittleFS";
            case MM_STORAGE_MBED: return "mbed";
            case MM_STORAGE_MSGPACK: return "MessagePack";
            case MM_STORAGE_RAM:
            default:
                return "RAM";
        }
    }

    MemoryBlock* findBlockById(unsigned int id) {
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && block->id == id) {
                return block;
            }
        }
        return nullptr;
    }

    MemoryBlock* findBlockByPtr(void* ptr) {
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed && block->data == ptr) {
                return block;
            }
        }
        return nullptr;
    }

    bool hasActiveDependentSlices(unsigned int rootId) {
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (!block) {
                continue;
            }
            if (block->rootId == rootId && !block->ownsMemory && !block->freed) {
                return true;
            }
        }
        return false;
    }

    void eraseSlicesForRoot(unsigned int rootId) {
        for (int i = static_cast<int>(memoryBlocks.elements()) - 1; i >= 0; --i) {
            MemoryBlock* block = memoryBlocks.get(static_cast<unsigned int>(i));
            if (!block) {
                continue;
            }
            if (block->rootId == rootId && !block->ownsMemory) {
                memoryBlocks.erase(static_cast<unsigned int>(i));
                if (block->file) {
                    delete[] block->file;
                    block->file = nullptr;
                }
                ::free(block);
            }
        }
    }

    MemoryBlock* createBlockMetadata(
        void* data,
        unsigned int size,
        const char* file,
        unsigned int line,
        MMStorageKind storage,
        const String& storageTag,
        bool ownsMemory,
        bool virtualSlice,
        unsigned int rootId) {
        MemoryBlock* block = static_cast<MemoryBlock*>(::malloc(sizeof(MemoryBlock)));
        if (!block) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        block->freed = false;
        block->ownsMemory = ownsMemory;
        block->virtualSlice = virtualSlice;
        block->size = size;
        block->id = nextId++;
        block->rootId = rootId == 0 ? block->id : rootId;
        block->line = line;
        block->data = data;
        block->storageKind = static_cast<uint8_t>(storage);
        block->storageTag = storageTag;

        const char* sourceFile = file ? file : "unknown";
        size_t fileLen = std::strlen(sourceFile);
        block->file = new char[fileLen + 1];
        std::strcpy(block->file, sourceFile);
        return block;
    }

    bool validateBackendForStorage(MMStorageKind storage) {
        if (storage == MM_STORAGE_SD && !useSDFile) {
            lastError = MM_SD_NOT_FOUND;
            return false;
        }
        if (storage == MM_STORAGE_LITTLEFS && (!useLittleFS || littleFS == nullptr)) {
            lastError = MM_FILE_NOT_FOUND;
            return false;
        }
        if (storage == MM_STORAGE_MBED && !useMbedTracking) {
            lastError = MM_UNKNOWN_ERROR;
            return false;
        }
        return true;
    }

    MemoryBlock* tryAllocateFromFreeSlice(
        unsigned int size,
        const char* file,
        unsigned int line,
        MMStorageKind storage,
        const String& storageTag) {
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* candidate = memoryBlocks.get(i);
            if (!candidate || !candidate->freed || candidate->size < size) {
                continue;
            }

            if (candidate->storageKind != static_cast<uint8_t>(storage)) {
                continue;
            }

            if (candidate->size >= size + 8U) {
                uint8_t* oldPtr = static_cast<uint8_t*>(candidate->data);
                unsigned int oldSize = candidate->size;

                candidate->size = size;
                candidate->freed = false;
                candidate->line = line;
                candidate->storageTag = storageTag;
                delete[] candidate->file;
                size_t fileLen = std::strlen(file ? file : "unknown");
                candidate->file = new char[fileLen + 1];
                std::strcpy(candidate->file, file ? file : "unknown");

                MemoryBlock* remainder = createBlockMetadata(
                    oldPtr + size,
                    oldSize - size,
                    "split-fragment",
                    line,
                    static_cast<MMStorageKind>(candidate->storageKind),
                    candidate->storageTag,
                    false,
                    true,
                    candidate->rootId);
                if (remainder) {
                    remainder->freed = true;
                    memoryBlocks.put(remainder);
                }

                return candidate;
            }

            candidate->freed = false;
            candidate->line = line;
            candidate->storageTag = storageTag;
            delete[] candidate->file;
            size_t fileLen = std::strlen(file ? file : "unknown");
            candidate->file = new char[fileLen + 1];
            std::strcpy(candidate->file, file ? file : "unknown");
            return candidate;
        }

        return nullptr;
    }

    void releaseBlock(MemoryBlock* block) {
        if (!block) {
            return;
        }
        if (block->ownsMemory && block->data) {
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
            defaultStorage = MM_STORAGE_SD;
        } else {
            if (autoInitSD) {
                DEBUG_PRINT("MM_SD_NOT_FOUND");
            }
            useSDFile = false;
            defaultStorage = MM_STORAGE_RAM;
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
        defaultStorage = MM_STORAGE_SD;
        lastError = MM_SUCCESS;
        return true;
    }

    bool beginLittleFS(fs::FS& filesystem) {
        littleFS = &filesystem;
        useLittleFS = true;
        defaultStorage = MM_STORAGE_LITTLEFS;
        lastError = MM_SUCCESS;
        return true;
    }

    bool beginMbed() {
#if defined(ARDUINO_ARCH_MBED)
        useMbedTracking = true;
        defaultStorage = MM_STORAGE_MBED;
        lastError = MM_SUCCESS;
        return true;
#else
        lastError = MM_UNKNOWN_ERROR;
        return false;
#endif
    }

#if MM_HAS_UNOQ_TRANSFER
    void setRemoteTransfer(UnoQFileTransferClient& transfer) {
        remoteTransfer = &transfer;
    }

    bool exportAllocationsToMsgPack(const String& remotePath = "/memory_manager/allocations.msgpack") {
        if (remoteTransfer == nullptr) {
            lastError = MM_FILE_WRITE_ERROR;
            return false;
        }

        const size_t count = memoryBlocks.elements();
        size_t capacity = 128 + (count * 220);
        uint8_t* payload = static_cast<uint8_t*>(::malloc(capacity));
        if (!payload) {
            lastError = MM_ALLOCATION_FAILED;
            return false;
        }

        size_t offset = 0;
        auto putByte = [&](uint8_t v) -> bool {
            if (offset >= capacity) return false;
            payload[offset++] = v;
            return true;
        };
        auto putRaw = [&](const uint8_t* p, size_t len) -> bool {
            if (!p || offset + len > capacity) return false;
            for (size_t i = 0; i < len; ++i) payload[offset++] = p[i];
            return true;
        };
        auto putMapHdr = [&](uint16_t n) -> bool {
            if (n <= 15) return putByte(static_cast<uint8_t>(0x80 | n));
            return putByte(0xDE) && putByte(static_cast<uint8_t>((n >> 8) & 0xFF)) && putByte(static_cast<uint8_t>(n & 0xFF));
        };
        auto putArrayHdr = [&](uint16_t n) -> bool {
            if (n <= 15) return putByte(static_cast<uint8_t>(0x90 | n));
            return putByte(0xDC) && putByte(static_cast<uint8_t>((n >> 8) & 0xFF)) && putByte(static_cast<uint8_t>(n & 0xFF));
        };
        auto putUInt = [&](uint32_t n) -> bool {
            if (n <= 127) return putByte(static_cast<uint8_t>(n));
            return putByte(0xCE)
                && putByte(static_cast<uint8_t>((n >> 24) & 0xFF))
                && putByte(static_cast<uint8_t>((n >> 16) & 0xFF))
                && putByte(static_cast<uint8_t>((n >> 8) & 0xFF))
                && putByte(static_cast<uint8_t>(n & 0xFF));
        };
        auto putBool = [&](bool b) -> bool {
            return putByte(b ? 0xC3 : 0xC2);
        };
        auto putStr = [&](const char* s) -> bool {
            if (!s) s = "";
            size_t len = std::strlen(s);
            if (len <= 31) {
                if (!putByte(static_cast<uint8_t>(0xA0 | len))) return false;
            } else if (len <= 255) {
                if (!putByte(0xD9) || !putByte(static_cast<uint8_t>(len))) return false;
            } else if (len <= 65535) {
                if (!putByte(0xDA) || !putByte(static_cast<uint8_t>((len >> 8) & 0xFF)) || !putByte(static_cast<uint8_t>(len & 0xFF))) return false;
            } else {
                return false;
            }
            return putRaw(reinterpret_cast<const uint8_t*>(s), len);
        };

        bool ok = true;
        ok = ok && putMapHdr(3);
        ok = ok && putStr("version") && putUInt(1);
        ok = ok && putStr("defaultStorage") && putStr(storageKindName(defaultStorage));
        ok = ok && putStr("blocks") && putArrayHdr(static_cast<uint16_t>(count));

        for (size_t i = 0; ok && i < count; ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (!block) {
                ok = false;
                break;
            }

            unsigned int offsetFromRoot = 0;
            MemoryBlock* root = findBlockById(block->rootId);
            if (root && root->data && block->data) {
                offsetFromRoot = static_cast<unsigned int>(
                    reinterpret_cast<uint8_t*>(block->data) - reinterpret_cast<uint8_t*>(root->data));
            }

            ok = ok && putMapHdr(10);
            ok = ok && putStr("id") && putUInt(block->id);
            ok = ok && putStr("rootId") && putUInt(block->rootId);
            ok = ok && putStr("size") && putUInt(block->size);
            ok = ok && putStr("offset") && putUInt(offsetFromRoot);
            ok = ok && putStr("freed") && putBool(block->freed);
            ok = ok && putStr("ownsMemory") && putBool(block->ownsMemory);
            ok = ok && putStr("virtualSlice") && putBool(block->virtualSlice);
            ok = ok && putStr("storage") && putStr(storageKindName(static_cast<MMStorageKind>(block->storageKind)));
            ok = ok && putStr("tag") && putStr(block->storageTag.c_str());
            ok = ok && putStr("file") && putStr(block->file ? block->file : "unknown");
        }

        if (!ok) {
            ::free(payload);
            lastError = MM_FILE_WRITE_ERROR;
            return false;
        }

        static const char* BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        String b64;
        b64.reserve(((offset + 2) / 3) * 4);
        for (size_t i = 0; i < offset; i += 3) {
            uint32_t triple = static_cast<uint32_t>(payload[i]) << 16;
            bool have2 = (i + 1) < offset;
            bool have3 = (i + 2) < offset;
            if (have2) triple |= static_cast<uint32_t>(payload[i + 1]) << 8;
            if (have3) triple |= static_cast<uint32_t>(payload[i + 2]);

            b64 += BASE64[(triple >> 18) & 0x3F];
            b64 += BASE64[(triple >> 12) & 0x3F];
            b64 += have2 ? BASE64[(triple >> 6) & 0x3F] : '=';
            b64 += have3 ? BASE64[triple & 0x3F] : '=';
        }

        ::free(payload);
        bool wrote = remoteTransfer->writeBytesBase64(remotePath, b64, false);
        lastError = wrote ? MM_SUCCESS : MM_FILE_WRITE_ERROR;
        return wrote;
    }
#endif

#ifdef MM_USE_BRIDGE
    // Export allocation table as JSON to the Python container filesystem.
    bool exportAllocationsToBridge(const String& remotePath = "memory_manager/allocations.json") {
        String json;
        json.reserve(256 + memoryBlocks.elements() * 120);
        json += "{\"blocks\":[";
        bool first = true;
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* b = memoryBlocks.get(i);
            if (!b) continue;
            if (!first) json += ',';
            first = false;
            json += "{\"id\":"  + String(b->id)
                 + ",\"size\":" + String(b->size)
                 + ",\"freed\":" + String(b->freed ? "true" : "false")
                 + ",\"storage\":\"" + String(storageKindName(static_cast<MMStorageKind>(b->storageKind))) + "\""
                 + ",\"tag\":\"" + b->storageTag + "\""
                 + ",\"file\":\"" + String(b->file ? b->file : "") + "\""
                 + ",\"line\":" + String(b->line)
                 + "}";
        }
        json += "],\"count\":" + String(memoryBlocks.elements()) + "}";

        bool wrote = false;
        bool ok = Bridge.call("fs_write", remotePath, json).result(wrote) && wrote;
        lastError = ok ? MM_SUCCESS : MM_FILE_WRITE_ERROR;
        return ok;
    }

    // Push a one-line allocation summary to Python thread memory for live monitoring.
    bool logAllocToBridge(const String& tag, unsigned int bytes) {
        String msg = String("alloc tag=") + tag
                   + String(" bytes=") + bytes
                   + String(" total=") + memoryBlocks.elements();
        bool ok = false;
        Bridge.call("tm_record", String("mm"), msg).result(ok);
        return ok;
    }
#endif

    void setDefaultStorage(MMStorageKind kind) {
        defaultStorage = kind;
    }

    MMStorageKind getDefaultStorage() const {
        return defaultStorage;
    }

    void setUseSDFile(bool useSD) {
        if (!useSD) {
            useSDFile = false;
            if (defaultStorage == MM_STORAGE_SD) {
                defaultStorage = MM_STORAGE_RAM;
            }
            return;
        }
        beginSD(SD_CS_PIN);
    }

    void* malloc(unsigned int size, const char* file, unsigned int line) {
        return mallocFrom(defaultStorage, size, file, line);
    }

    void* mallocFrom(MMStorageKind storage, unsigned int size, const char* file, unsigned int line) {
        lastError = MM_SUCCESS;

        if (size == 0) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        DEBUG_PRINT("Allocating memory: " + String(size) + " bytes...");

        if (!validateBackendForStorage(storage) && storage != MM_STORAGE_RAM && storage != MM_STORAGE_MSGPACK) {
            return nullptr;
        }

        String tag = String(storageKindName(storage));

        MemoryBlock* reused = tryAllocateFromFreeSlice(size, file, line, storage, tag);
        if (reused) {
            lastError = MM_SUCCESS;
            return reused->data;
        }

        if (storage == MM_STORAGE_SD && useSDFile) {
            unsigned long freeSpace = getFreeSDBytes();
            if (size > freeSpace) {
                DEBUG_PRINT("MM_SD_OUT_OF_SPACE");
                lastError = MM_SD_OUT_OF_SPACE;
                return nullptr;
            }
        }

        void* rawData = ::malloc(size);
        if (!rawData) {
            DEBUG_PRINT("MM_ALLOCATION_FAILED");
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        MemoryBlock* meta = createBlockMetadata(
            rawData,
            size,
            file,
            line,
            storage,
            tag,
            true,
            false,
            0);

        if (!meta) {
            ::free(rawData);
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        DEBUG_PRINT("MM_SUCCESS");
        memoryBlocks.put(meta);
        return meta->data;
    }

    void* calloc(unsigned int num, unsigned int size, const char* file, unsigned int line) {
        return callocFrom(defaultStorage, num, size, file, line);
    }

    void* callocFrom(MMStorageKind storage, unsigned int num, unsigned int size, const char* file, unsigned int line) {
        if (num == 0 || size == 0) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        if (num > (0xFFFFFFFFu / size)) {
            lastError = MM_ALLOCATION_FAILED;
            return nullptr;
        }

        unsigned int total = num * size;
        void* ptr = mallocFrom(storage, total, file, line);
        if (!ptr) {
            return nullptr;
        }

        std::memset(ptr, 0, total);
        return ptr;
    }

    void* realloc(void* ptr, unsigned int size, const char* file, unsigned int line) {
        return reallocFrom(defaultStorage, ptr, size, file, line);
    }

    void* reallocFrom(MMStorageKind storage, void* ptr, unsigned int size, const char* file, unsigned int line) {
        if (ptr == nullptr) {
            return mallocFrom(storage, size, file, line);
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

        if (storage != static_cast<MMStorageKind>(block->storageKind)) {
            block->storageKind = static_cast<uint8_t>(storage);
            block->storageTag = storageKindName(storage);
        }

        if (hasActiveDependentSlices(block->rootId)) {
            lastError = MM_BLOCK_HAS_DEPENDENTS;
            return nullptr;
        }

        eraseSlicesForRoot(block->rootId);

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

    int shrink(void* ptr, unsigned int newSize, const char* file, unsigned int line, bool splitInsteadOfRealloc = true) {
        if (!ptr) {
            lastError = MM_ALLOCATION_FAILED;
            return MM_ALLOCATION_FAILED;
        }

        MemoryBlock* block = findBlockByPtr(ptr);
        if (!block) {
            lastError = MM_BLOCK_NOT_FOUND;
            return MM_BLOCK_NOT_FOUND;
        }

        if (newSize == 0) {
            return free(ptr);
        }

        if (newSize >= block->size) {
            lastError = MM_SUCCESS;
            return MM_SUCCESS;
        }

        if (splitInsteadOfRealloc) {
            unsigned int tailSize = block->size - newSize;
            if (tailSize >= 8U) {
                uint8_t* basePtr = static_cast<uint8_t*>(block->data);
                MemoryBlock* tail = createBlockMetadata(
                    basePtr + newSize,
                    tailSize,
                    "split-tail",
                    line,
                    static_cast<MMStorageKind>(block->storageKind),
                    block->storageTag,
                    false,
                    true,
                    block->rootId);
                if (tail) {
                    tail->freed = true;
                    memoryBlocks.put(tail);
                }
            }

            block->size = newSize;
            block->line = line;
            if (file) {
                delete[] block->file;
                size_t fileLen = std::strlen(file);
                block->file = new char[fileLen + 1];
                std::strcpy(block->file, file);
            }
            lastError = MM_SUCCESS;
            return MM_SUCCESS;
        }

        void* reduced = reallocFrom(static_cast<MMStorageKind>(block->storageKind), ptr, newSize, file, line);
        if (!reduced) {
            return lastError;
        }
        lastError = MM_SUCCESS;
        return MM_SUCCESS;
    }


    int free(void* ptr) {
        if (!ptr) {
            lastError = MM_ALLOCATION_FAILED;
            return MM_ALLOCATION_FAILED;
        }

        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && block->data == ptr) {
                if (!block->ownsMemory) {
                    block->freed = true;
                    lastError = MM_SUCCESS;
                    return MM_SUCCESS;
                }

                if (hasActiveDependentSlices(block->rootId)) {
                    lastError = MM_BLOCK_HAS_DEPENDENTS;
                    return MM_BLOCK_HAS_DEPENDENTS;
                }

                eraseSlicesForRoot(block->rootId);
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
        unsigned int count = 0;
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed) {
                ++count;
            }
        }
        return count;
    }

    unsigned int getAllocationCountByStorage(MMStorageKind storage) {
        unsigned int count = 0;
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed && block->storageKind == static_cast<uint8_t>(storage)) {
                ++count;
            }
        }
        return count;
    }

    unsigned int getTrackedMemoryByStorage(MMStorageKind storage) {
        unsigned int total = 0;
        for (size_t i = 0; i < memoryBlocks.elements(); ++i) {
            MemoryBlock* block = memoryBlocks.get(i);
            if (block && !block->freed && block->storageKind == static_cast<uint8_t>(storage)) {
                total += block->size;
            }
        }
        return total;
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
                Serial.print(block->line);
                Serial.print(" [");
                Serial.print(storageKindName(static_cast<MMStorageKind>(block->storageKind)));
                Serial.println("]");
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
