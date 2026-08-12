#ifndef SDList_h
#define SDList_h

/*
 * SDList  –  Paged / batched list backed by SD card or LittleFS
 * Version 2.0.0
 *
 * Elements are stored in raw binary (sizeof(T) bytes per slot), so ANY
 * trivially-copyable type T is preserved exactly — no text serialisation,
 * no data corruption.
 *
 * A single fixed-size in-memory window of BATCH_SIZE elements acts as the
 * RAM buffer.  Only that one batch lives in RAM; every other element stays on
 * the filesystem.  The batch buffer is allocated once at construction and is
 * never reallocated, giving stable, predictable memory usage.
 *
 * File layout
 * ───────────
 *   Offset               Size      Field
 *   0                    4         Magic  'S','D','L','2'
 *   4                    4         count     (uint32_t, little-endian)
 *   8                    4         reserved  (uint32_t, little-endian)
 *   12 + i * sizeof(T)   sizeof(T) element i  (i = 0 … reserved-1)
 *
 * Build-time option
 * ─────────────────
 *   -DUSE_LITTLEFS   Route filesystem calls through LittleFS instead of the
 *                    Arduino SD library.  Default: SD + SPI.
 *   UnoQBridgeClient.h auto-detected (if available) to enable beginUnoQ()
 *                    without creating a hard dependency.
 */

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

#if defined(USE_LITTLEFS)
  #include <LittleFS.h>
  #define SDLIST_FS  LittleFS
#else
  #include <SD.h>
  #include <SPI.h>
  #define SDLIST_FS  SD
#endif

#if defined(__has_include)
  #if __has_include(<UnoQBridgeClient.h>)
    #include <UnoQBridgeClient.h>
    #define SDLIST_HAS_UNOQ_BRIDGE 1
  #elif __has_include("../../UnoQBridge/sketch/src/UnoQBridgeClient.h")
    #include "../../UnoQBridge/sketch/src/UnoQBridgeClient.h"
    #define SDLIST_HAS_UNOQ_BRIDGE 1
  #else
    #define SDLIST_HAS_UNOQ_BRIDGE 0
  #endif
#else
  #define SDLIST_HAS_UNOQ_BRIDGE 0
#endif

enum SDListMode { SDCARD, MEMORY };

/**
 * @brief Fixed-batch paged list that offloads elements to SD card or LittleFS.
 *
 * @tparam T          Element type.  Must be trivially copyable (POD, structs of
 *                    PODs, etc.).  String and other heap-owning types are NOT
 *                    supported in SDCARD mode.
 * @tparam BATCH_SIZE Number of elements in the in-memory batch window.
 *                    Default: 8.  Larger values trade RAM for fewer file opens.
 */
template <typename T, size_t BATCH_SIZE = 8>
class SDList {

    // ── compile-time guard (works without C++11 static_assert in class body) ─
    typedef char _BatchSizeCheck[(BATCH_SIZE > 0) ? 1 : -1];

    // ── File layout ──────────────────────────────────────────────────────────
    static const uint32_t HDR_BYTES = 12u;  // 4 magic + 4 count + 4 reserved

    // ── In-memory batch ──────────────────────────────────────────────────────
    T        _batch[BATCH_SIZE]; ///< The one batch that lives in RAM (never reallocated)
    uint32_t _batchStart;        ///< Element index of _batch[0]
    uint32_t _batchLoaded;       ///< Valid elements currently in _batch
    bool     _batchDirty;        ///< _batch has unsaved modifications

    // ── List state ───────────────────────────────────────────────────────────
    uint32_t _count;             ///< Total valid elements stored
    uint32_t _reserved;          ///< Pre-allocated slots in the file
    uint32_t _initReserved;      ///< Reserved count used when (re)creating the file

    // ── Config ───────────────────────────────────────────────────────────────
    String     _filename;
    int        _csPin;
    bool       _ready;
    SDListMode _mode;

#if SDLIST_HAS_UNOQ_BRIDGE
    UnoQFileTransferClient* _unoqClient;
    bool _useUnoQ;
#endif

    // ── Memory-mode heap array ───────────────────────────────────────────────
    T*     _mem;                 ///< Heap array for MEMORY-mode fallback
    size_t _memCap;              ///< Allocated slots in _mem

    // ========================================================================
    //  Private helpers – file I/O
    // ========================================================================

    /** Byte offset in the file for element index i. */
    static uint32_t _elemOff(uint32_t i) {
        return HDR_BYTES + (uint32_t)sizeof(T) * i;
    }

    /** Aligned batch-window start index for element index. */
    static uint32_t _winStart(uint32_t index) {
        return (index / (uint32_t)BATCH_SIZE) * (uint32_t)BATCH_SIZE;
    }

    /**
     * Write the 12-byte header to an already-open file.
     * Seeks to offset 0 before writing.
     */
    bool _writeHeader(File& f) const {
        if (!f.seek(0)) return false;
        const uint8_t magic[4] = {'S', 'D', 'L', '2'};
        if ((size_t)f.write(magic, 4) != 4) return false;
        uint32_t c = _count;
        uint32_t r = _reserved;
        if ((size_t)f.write((const uint8_t*)&c, 4) != 4) return false;
        if ((size_t)f.write((const uint8_t*)&r, 4) != 4) return false;
        return true;
    }

    /**
     * Read and validate the 12-byte header from an already-open file.
     * Seeks to offset 0 before reading.
     */
    bool _readHeader(File& f, uint32_t& cnt, uint32_t& res) {
        if (!f.seek(0)) return false;
        uint8_t magic[4];
        if ((size_t)f.read(magic, 4) != 4) return false;
        if (magic[0] != 'S' || magic[1] != 'D' ||
            magic[2] != 'L' || magic[3] != '2') return false;
        if ((size_t)f.read((uint8_t*)&cnt, 4) != 4) return false;
        if ((size_t)f.read((uint8_t*)&res, 4) != 4) return false;
        return true;
    }

    /**
     * Flush the dirty in-memory batch and update the header in one
     * file open/close cycle.
     */
    bool _flushBatch() {
        if (!_batchDirty) return true;
        if (!_ready || _mode != SDCARD) { _batchDirty = false; return true; }

#if SDLIST_HAS_UNOQ_BRIDGE
        if (_usesUnoQ()) {
            size_t totalBytes = 0;
            uint8_t* fileData = _remoteReadAll(totalBytes);
            if (!fileData) return false;

            const size_t expected = (size_t)_elemOff(_reserved);
            if (totalBytes < expected) {
                uint8_t* grown = static_cast<uint8_t*>(malloc(expected));
                if (!grown) {
                    free(fileData);
                    return false;
                }
                memset(grown, 0, expected);
                if (totalBytes > 0) memcpy(grown, fileData, totalBytes);
                free(fileData);
                fileData = grown;
                totalBytes = expected;
            }

            fileData[0] = 'S';
            fileData[1] = 'D';
            fileData[2] = 'L';
            fileData[3] = '2';
            _writeU32LE(fileData + 4, _count);
            _writeU32LE(fileData + 8, _reserved);

            if (_batchLoaded > 0) {
                const uint32_t offset = _elemOff(_batchStart);
                const size_t batchBytes = (size_t)_batchLoaded * sizeof(T);
                if (offset + batchBytes <= totalBytes) {
                    memcpy(fileData + offset, reinterpret_cast<const uint8_t*>(_batch), batchBytes);
                }
            }

            bool ok = _remoteWriteAll(fileData, totalBytes);
            free(fileData);
            if (ok) _batchDirty = false;
            return ok;
        }
#endif

        File f = SDLIST_FS.open(_filename, FILE_WRITE);
        if (!f) return false;

        bool ok = _writeHeader(f);
        if (ok && _batchLoaded > 0) {
            ok = f.seek(_elemOff(_batchStart));
            if (ok) {
                uint32_t bytes = _batchLoaded * (uint32_t)sizeof(T);
                ok = ((uint32_t)f.write((const uint8_t*)_batch, bytes) == bytes);
            }
        }
        f.close();
        if (ok) _batchDirty = false;
        return ok;
    }

    /**
     * Load a batch window starting at startIdx from the file.
     * Flushes the current dirty batch first.
     */
    bool _loadBatch(uint32_t startIdx) {
        if (_batchDirty && !_flushBatch()) return false;

        _batchStart  = startIdx;
        _batchLoaded = 0;
        _batchDirty  = false;

        // Nothing to read when startIdx is at or beyond valid elements
        if (!_ready || _mode != SDCARD || startIdx >= _count) return true;

        uint32_t toRead = (uint32_t)BATCH_SIZE;
        if (startIdx + toRead > _count) toRead = _count - startIdx;

        uint32_t bytes = toRead * (uint32_t)sizeof(T);

#if SDLIST_HAS_UNOQ_BRIDGE
        if (_usesUnoQ()) {
            const uint32_t got = _remoteReadRange(_elemOff(startIdx), reinterpret_cast<uint8_t*>(_batch), bytes);
            _batchLoaded = got / (uint32_t)sizeof(T);
            return (_batchLoaded == toRead);
        }
#endif

        File f = SDLIST_FS.open(_filename, FILE_READ);
        if (!f) return false;
        if (!f.seek(_elemOff(startIdx))) { f.close(); return false; }

        uint32_t got = (uint32_t)f.read((uint8_t*)_batch, bytes);
        f.close();

        _batchLoaded = got / (uint32_t)sizeof(T);
        return (_batchLoaded == toRead);
    }

    /**
     * Ensure the in-memory batch window covers element index.
     * Loads a new batch if needed.
     */
    bool _ensureBatch(uint32_t index) {
        if (index >= _batchStart && index < _batchStart + _batchLoaded)
            return true;
        return _loadBatch(_winStart(index));
    }

    /**
     * Extend the pre-allocated area in the file by BATCH_SIZE additional
     * zero-valued slots.
     */
    bool _grow() {
        uint32_t newRes = _reserved + (uint32_t)BATCH_SIZE;

#if SDLIST_HAS_UNOQ_BRIDGE
        if (_usesUnoQ()) {
            size_t totalBytes = 0;
            uint8_t* fileData = _remoteReadAll(totalBytes);
            if (!fileData) return false;

            const size_t newSize = (size_t)_elemOff(newRes);
            uint8_t* grown = static_cast<uint8_t*>(malloc(newSize));
            if (!grown) {
                free(fileData);
                return false;
            }
            memset(grown, 0, newSize);
            if (totalBytes > 0) {
                size_t copyBytes = totalBytes < newSize ? totalBytes : newSize;
                memcpy(grown, fileData, copyBytes);
            }
            free(fileData);

            grown[0] = 'S';
            grown[1] = 'D';
            grown[2] = 'L';
            grown[3] = '2';
            _writeU32LE(grown + 4, _count);
            _writeU32LE(grown + 8, newRes);

            const bool ok = _remoteWriteAll(grown, newSize);
            free(grown);
            if (ok) _reserved = newRes;
            return ok;
        }
#endif

        File f = SDLIST_FS.open(_filename, FILE_WRITE);
        if (!f) return false;

        if (!f.seek(_elemOff(_reserved))) { f.close(); return false; }

        T zeroElem;
        memset(&zeroElem, 0, sizeof(T));
        bool ok = true;
        for (uint32_t i = 0; i < (uint32_t)BATCH_SIZE && ok; i++) {
            ok = ((size_t)f.write((const uint8_t*)&zeroElem, sizeof(T)) == sizeof(T));
        }
        if (ok) {
            _reserved = newRes;
            ok = _writeHeader(f);
        }
        f.close();
        return ok;
    }

    /**
     * (Re)create the file: header + _reserved pre-allocated zero slots.
     * Any existing file at _filename is removed first.
     */
    bool _initFile() {
#if SDLIST_HAS_UNOQ_BRIDGE
        if (_usesUnoQ()) {
            const size_t bytes = (size_t)_elemOff(_reserved);
            uint8_t* data = static_cast<uint8_t*>(malloc(bytes));
            if (!data) return false;
            memset(data, 0, bytes);
            data[0] = 'S'; data[1] = 'D'; data[2] = 'L'; data[3] = '2';
            _writeU32LE(data + 4, _count);
            _writeU32LE(data + 8, _reserved);
            bool ok = _remoteWriteAll(data, bytes);
            free(data);
            return ok;
        }
#endif

        if (SDLIST_FS.exists(_filename)) SDLIST_FS.remove(_filename);

        File f = SDLIST_FS.open(_filename, FILE_WRITE);
        if (!f) return false;

        bool ok = _writeHeader(f);
        if (ok) {
            T zeroElem;
            memset(&zeroElem, 0, sizeof(T));
            for (uint32_t i = 0; i < _reserved && ok; i++) {
                ok = ((size_t)f.write((const uint8_t*)&zeroElem, sizeof(T)) == sizeof(T));
            }
        }
        f.close();
        return ok;
    }

    /**
     * Try to open an existing file and validate its header.
     * If the file is missing or corrupt, creates a fresh one.
     */
    bool _openOrCreate() {
#if SDLIST_HAS_UNOQ_BRIDGE
        if (_usesUnoQ()) {
            bool exists = false;
            bool haveStatus = _unoqClient->exists(_filename, exists);
            if (!haveStatus) {
                _ready = false;
                return false;
            }

            if (exists) {
                uint8_t hdr[HDR_BYTES];
                if (_remoteReadRange(0, hdr, HDR_BYTES) == HDR_BYTES) {
                    const bool magicOk = (hdr[0] == 'S' && hdr[1] == 'D' && hdr[2] == 'L' && hdr[3] == '2');
                    const uint32_t cnt = _readU32LE(hdr + 4);
                    const uint32_t res = _readU32LE(hdr + 8);
                    if (magicOk && res >= (uint32_t)BATCH_SIZE) {
                        _count = cnt;
                        _reserved = res;
                        _ready = true;
                        _loadBatch(0);
                        return true;
                    }
                }
            }

            _count = 0;
            _reserved = _initReserved;
            _ready = _initFile();
            if (_ready) { _batchStart = 0; _batchLoaded = 0; }
            return _ready;
        }
#endif

        if (SDLIST_FS.exists(_filename)) {
            File f = SDLIST_FS.open(_filename, FILE_READ);
            if (f) {
                uint32_t cnt = 0, res = 0;
                bool ok = _readHeader(f, cnt, res);
                f.close();
                if (ok && res >= (uint32_t)BATCH_SIZE) {
                    _count    = cnt;
                    _reserved = res;
                    _ready    = true;
                    _loadBatch(0);
                    return true;
                }
            }
            // Corrupt file – remove and fall through to create
            SDLIST_FS.remove(_filename);
        }
        _count    = 0;
        _reserved = _initReserved;
        _ready    = _initFile();
        if (_ready) { _batchStart = 0; _batchLoaded = 0; }
        return _ready;
    }

    // ========================================================================
    //  Private helpers – memory mode
    // ========================================================================

    /** Grow the heap array to hold at least minCap elements. */
    bool _memGrow(size_t minCap) {
        if (minCap <= _memCap) return true;
        size_t newCap = (_memCap == 0) ? (size_t)BATCH_SIZE : _memCap;
        while (newCap < minCap) newCap *= 2;
        T* buf = new T[newCap];
        if (!buf) return false;
        for (size_t i = 0; i < (size_t)_count && _mem; i++) buf[i] = _mem[i];
        delete[] _mem;
        _mem    = buf;
        _memCap = newCap;
        return true;
    }

#if SDLIST_HAS_UNOQ_BRIDGE
    static uint8_t _decodeB64Char(char c) {
        if (c >= 'A' && c <= 'Z') return (uint8_t)(c - 'A');
        if (c >= 'a' && c <= 'z') return (uint8_t)(c - 'a' + 26);
        if (c >= '0' && c <= '9') return (uint8_t)(c - '0' + 52);
        if (c == '+') return 62;
        if (c == '/') return 63;
        return 255;
    }

    static bool _encodeBase64(const uint8_t* data, size_t len, String& out) {
        static const char* BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        out = "";
        out.reserve(((len + 2) / 3) * 4);
        for (size_t i = 0; i < len; i += 3) {
            uint32_t triple = (uint32_t)data[i] << 16;
            bool have2 = (i + 1) < len;
            bool have3 = (i + 2) < len;
            if (have2) triple |= (uint32_t)data[i + 1] << 8;
            if (have3) triple |= (uint32_t)data[i + 2];
            out += BASE64[(triple >> 18) & 0x3F];
            out += BASE64[(triple >> 12) & 0x3F];
            out += have2 ? BASE64[(triple >> 6) & 0x3F] : '=';
            out += have3 ? BASE64[triple & 0x3F] : '=';
        }
        return true;
    }

    static bool _decodeBase64(const String& text, uint8_t* out, size_t maxOut, size_t& written) {
        written = 0;
        size_t n = text.length();
        if (n == 0) return true;
        if ((n % 4) != 0) return false;

        for (size_t i = 0; i < n; i += 4) {
            char c0 = text[i + 0];
            char c1 = text[i + 1];
            char c2 = text[i + 2];
            char c3 = text[i + 3];
            uint8_t v0 = _decodeB64Char(c0);
            uint8_t v1 = _decodeB64Char(c1);
            if (v0 == 255 || v1 == 255) return false;

            uint8_t v2 = 0;
            uint8_t v3 = 0;
            bool pad2 = (c2 == '=');
            bool pad3 = (c3 == '=');
            if (pad2 && !pad3) return false;
            if (!pad2) {
                v2 = _decodeB64Char(c2);
                if (v2 == 255) return false;
            }
            if (!pad3) {
                v3 = _decodeB64Char(c3);
                if (v3 == 255) return false;
            }

            uint32_t chunk = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12) | ((uint32_t)v2 << 6) | (uint32_t)v3;
            if (written >= maxOut) return false;
            out[written++] = (uint8_t)((chunk >> 16) & 0xFF);
            if (!pad2) {
                if (written >= maxOut) return false;
                out[written++] = (uint8_t)((chunk >> 8) & 0xFF);
            }
            if (!pad3) {
                if (written >= maxOut) return false;
                out[written++] = (uint8_t)(chunk & 0xFF);
            }
        }
        return true;
    }

    static void _writeU32LE(uint8_t* out, uint32_t value) {
        out[0] = (uint8_t)(value & 0xFF);
        out[1] = (uint8_t)((value >> 8) & 0xFF);
        out[2] = (uint8_t)((value >> 16) & 0xFF);
        out[3] = (uint8_t)((value >> 24) & 0xFF);
    }

    static uint32_t _readU32LE(const uint8_t* in) {
        return (uint32_t)in[0]
            | ((uint32_t)in[1] << 8)
            | ((uint32_t)in[2] << 16)
            | ((uint32_t)in[3] << 24);
    }

    bool _usesUnoQ() const {
        return _useUnoQ && _unoqClient != nullptr;
    }

    uint32_t _remoteReadRange(uint32_t offset, uint8_t* dest, uint32_t size) {
        if (size == 0) return 0;
        if (!_usesUnoQ() || dest == nullptr) return 0;

        String b64;
        if (!_unoqClient->readBytesBase64(_filename, b64, (int)offset, (int)size)) {
            return 0;
        }

        size_t decoded = 0;
        if (!_decodeBase64(b64, dest, size, decoded)) {
            return 0;
        }

        return (uint32_t)decoded;
    }

    uint8_t* _remoteReadAll(size_t& outSize) {
        outSize = 0;
        if (!_usesUnoQ()) return nullptr;

        String b64;
        if (!_unoqClient->readBytesBase64(_filename, b64, 0, -1)) {
            return nullptr;
        }

        const size_t inputLen = b64.length();
        if (inputLen == 0) {
            uint8_t* empty = static_cast<uint8_t*>(malloc(1));
            if (!empty) return nullptr;
            return empty;
        }

        size_t maxDecoded = (inputLen / 4) * 3;
        if (b64[inputLen - 1] == '=') maxDecoded--;
        if (inputLen > 1 && b64[inputLen - 2] == '=') maxDecoded--;

        uint8_t* data = static_cast<uint8_t*>(malloc(maxDecoded));
        if (!data) return nullptr;

        size_t decoded = 0;
        if (!_decodeBase64(b64, data, maxDecoded, decoded)) {
            free(data);
            return nullptr;
        }

        outSize = decoded;
        return data;
    }

    bool _remoteWriteAll(const uint8_t* data, size_t size) {
        if (!_usesUnoQ()) return false;
        String b64;
        if (!_encodeBase64(data, size, b64)) return false;
        return _unoqClient->writeBytesBase64(_filename, b64, false);
    }
#else
    bool _usesUnoQ() const { return false; }
#endif

public:
    // ========================================================================
    //  Construction / destruction
    // ========================================================================

    /**
     * @brief Construct an SDList.
     *
     * @param mode            SDCARD (default) or MEMORY.
     * @param reserveCapacity Number of element slots to pre-allocate in the
     *                        backing file on first begin().  Clamped to a
     *                        minimum of BATCH_SIZE.
     */
    explicit SDList(SDListMode mode = SDCARD, uint32_t reserveCapacity = 16)
        : _batchStart(0), _batchLoaded(0), _batchDirty(false),
          _count(0),
          _reserved(reserveCapacity >= (uint32_t)BATCH_SIZE
                    ? reserveCapacity : (uint32_t)BATCH_SIZE),
          _initReserved(reserveCapacity >= (uint32_t)BATCH_SIZE
                        ? reserveCapacity : (uint32_t)BATCH_SIZE),
          _filename("sdlist.bin"), _csPin(4),
          _ready(false), _mode(mode),
#if SDLIST_HAS_UNOQ_BRIDGE
          _unoqClient(nullptr), _useUnoQ(false),
#endif
          _mem(nullptr), _memCap(0)
    {}

    ~SDList() {
        if (_mode == SDCARD && _batchDirty) _flushBatch();
        delete[] _mem;
        _mem = nullptr;
    }

    // ========================================================================
    //  Initialisation
    // ========================================================================

    /**
     * @brief Initialise in SD-card mode.
     *
     * @param csPin    Chip-select pin for the SD card.
     * @param filename Binary file on the card (default "sdlist.bin").
     *                 Use a unique name per SDList instance.
     * @return true on success.  Returns false if the SD card could not be
     *         initialised; the object automatically falls back to MEMORY mode
     *         in that case (check getMode()).
     *
     * @note Call this before any list operations in SDCARD mode.
     */
    bool begin(int csPin = 4, const String& filename = "sdlist.bin") {
        _csPin    = csPin;
        _filename = filename;
#if SDLIST_HAS_UNOQ_BRIDGE
        _useUnoQ = false;
        _unoqClient = nullptr;
#endif

        if (_mode == MEMORY) {
            _ready = true;
            return true;
        }

#if !defined(USE_LITTLEFS)
        if (!SD.begin(csPin)) {
            _mode  = MEMORY;
            _ready = true;
            return false;   // caller can check getMode() == MEMORY
        }
#endif
        return _openOrCreate();
    }

    /**
     * @brief begin() overload that accepts a C-string filename.
     */
    bool begin(int csPin, const char* filename) {
        return begin(csPin, String(filename));
    }

    /**
     * @brief Initialise in LittleFS mode.
     *
     * Compile the project with -DUSE_LITTLEFS so that SDLIST_FS resolves to
     * LittleFS.  No chip-select pin is needed.
     *
     * @param filename File path on the LFS volume (default "sdlist.bin").
     */
    bool beginLFS(const String& filename = "sdlist.bin") {
        _filename = filename;
        _mode     = SDCARD;   // shares the same internal code-path
        _ready    = true;
#if SDLIST_HAS_UNOQ_BRIDGE
        _useUnoQ = false;
        _unoqClient = nullptr;
#endif
        return _openOrCreate();
    }

#if SDLIST_HAS_UNOQ_BRIDGE
    /**
     * @brief Initialise in UnoQ bridge mode.
     *
     * Uses UnoQBridgeClient remote filesystem operations and does not require
     * SD or LittleFS on the MCU side.
     */
    bool beginUnoQ(UnoQFileTransferClient& client, const String& filename = "sdlist.bin") {
        _filename = filename;
        _mode = SDCARD;
        _ready = true;
        _unoqClient = &client;
        _useUnoQ = true;
        return _openOrCreate();
    }

    bool beginUnoQ(UnoQFileTransferClient& client, const char* filename) {
        return beginUnoQ(client, String(filename));
    }
#endif

    // ========================================================================
    //  Core list API
    // ========================================================================

    /**
     * @brief Force-flush the in-memory batch to disk.
     *
     * Normally the batch is flushed automatically when the window moves or
     * on destruction.  Call this explicitly after a burst of writes if you
     * want the data persisted immediately.
     *
     * @return true on success.
     */
    bool flush() { return _flushBatch(); }

    /**
     * @brief Append an element to the end of the list.
     *
     * @return true on success; false on I/O error or allocation failure.
     */
    bool append(const T& element) {
        if (_mode == MEMORY) {
            if (!_memGrow(_count + 1)) return false;
            _mem[_count++] = element;
            return true;
        }
        if (!_ready) return false;

        // Grow the pre-allocated area if full
        if (_count >= _reserved && !_grow()) return false;

        // Make sure the batch window covers the slot at _count
        if (!(_count >= _batchStart &&
              _count <  _batchStart + (uint32_t)BATCH_SIZE)) {
            if (!_loadBatch(_winStart(_count))) return false;
        }

        uint32_t slot = _count - _batchStart;
        _batch[slot] = element;
        if (slot + 1 > _batchLoaded) _batchLoaded = slot + 1;
        _batchDirty = true;
        _count++;
        return true;
    }

    /** Alias for append(). */
    bool add(const T& element) { return append(element); }

    /**
     * @brief Retrieve the element at index.
     *
     * @return The element value, or a zero-initialised T() if index is out of
     *         range or an I/O error occurs.
     */
    T get(uint32_t index) {
        if (index >= _count) return T();
        if (_mode == MEMORY) return _mem[index];
        if (!_ready) return T();
        if (!_ensureBatch(index)) return T();
        return _batch[index - _batchStart];
    }

    /**
     * @brief Overwrite the element at index.
     *
     * @return true on success; false if index is out of range or I/O error.
     */
    bool set(uint32_t index, const T& element) {
        if (index >= _count) return false;
        if (_mode == MEMORY) { _mem[index] = element; return true; }
        if (!_ready) return false;
        if (!_ensureBatch(index)) return false;
        _batch[index - _batchStart] = element;
        _batchDirty = true;
        return true;
    }

    /**
     * @brief Insert an element at index, shifting subsequent elements right.
     *
     * Inserting at or beyond size() is equivalent to append().
     *
     * @return true on success.
     */
    bool insert(uint32_t index, const T& element) {
        if (index >= _count) return append(element);

        if (_mode == MEMORY) {
            if (!_memGrow(_count + 1)) return false;
            for (size_t i = _count; i > (size_t)index; i--)
                _mem[i] = _mem[i - 1];
            _mem[index] = element;
            _count++;
            return true;
        }
        if (!_ready) return false;
        if (_count >= _reserved && !_grow()) return false;

        _count++;  // claim the new slot before shifting
        for (uint32_t i = _count - 1; i > index; i--) {
            T val = get(i - 1);
            if (!set(i, val)) { _count--; return false; }
        }
        if (!set(index, element)) { _count--; return false; }
        return _flushBatch();
    }

    /**
     * @brief Remove element at index, shifting subsequent elements left.
     *
     * @return true on success; false if index is out of range.
     */
    bool remove(uint32_t index) {
        if (index >= _count) return false;

        if (_mode == MEMORY) {
            for (size_t i = index; i < (size_t)_count - 1; i++)
                _mem[i] = _mem[i + 1];
            _count--;
            return true;
        }
        if (!_ready) return false;

        for (uint32_t i = index; i < _count - 1; i++) {
            T val = get(i + 1);
            if (!set(i, val)) return false;
        }
        _count--;
        return _flushBatch();
    }

    /**
     * @brief Clear all elements.
     *
     * In SDCARD mode the file is reset to an empty, pre-allocated state (the
     * reserved-capacity block is rewritten as zeros, count is set to 0).
     * No reallocation occurs.
     */
    void clear() {
        _count       = 0;
        _batchStart  = 0;
        _batchLoaded = 0;
        _batchDirty  = false;
        if (_mode == MEMORY) return;  // keep heap array; just reset count
        _reserved = _initReserved;
        if (_ready) _initFile();
    }

    /**
     * @brief Switch between SDCARD and MEMORY modes, migrating all data.
     *
     * When switching SDCARD → MEMORY the file content is loaded into a heap
     * array.  When switching MEMORY → SDCARD all heap elements are written to
     * the file (requires begin() to have succeeded previously).
     */
    void setMode(SDListMode newMode) {
        if (newMode == _mode) return;

        if (newMode == MEMORY) {
            // SD → Memory: pull all elements into the heap array
            uint32_t cnt = _count;
            if (!_memGrow(cnt)) return;
            for (uint32_t i = 0; i < cnt; i++) _mem[i] = get(i);
            _mode  = MEMORY;
            _count = cnt;
        } else {
            // Memory → SD: push all heap elements into the file
            if (!_ready) return;
            uint32_t cnt = _count;
            T* tmp   = _mem;
            _mem     = nullptr;
            _memCap  = 0;
            _mode    = SDCARD;
            _count   = 0;
            _batchStart  = 0;
            _batchLoaded = 0;
            _batchDirty  = false;
            _reserved    = _initReserved;
            _initFile();
            for (uint32_t i = 0; i < cnt; i++) append(tmp[i]);
            _flushBatch();
            delete[] tmp;
        }
    }

    // ========================================================================
    //  Query
    // ========================================================================

    /** @return Number of valid elements. */
    size_t size() const { return (size_t)_count; }

    /**
     * @return Number of pre-allocated slots (SDCARD mode) or heap-allocated
     *         slots (MEMORY mode).
     */
    size_t capacity() const {
        return (_mode == MEMORY) ? _memCap : (size_t)_reserved;
    }

    /** @return true if the list contains no elements. */
    bool isEmpty() const { return _count == 0; }

    /**
     * @return true if begin() (or beginLFS()) completed successfully.
     *         Always true in MEMORY mode.
     */
    bool isReady() const { return _ready; }

    /** @return Current operating mode (SDCARD or MEMORY). */
    SDListMode getMode() const { return _mode; }
};

#endif // SDList_h