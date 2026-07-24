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
 */

#include <Arduino.h>
#include <string.h>

#if defined(USE_LITTLEFS)
  #include <LittleFS.h>
  #define SDLIST_FS  LittleFS
#else
  #include <SD.h>
  #include <SPI.h>
  #define SDLIST_FS  SD
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

        File f = SDLIST_FS.open(_filename, FILE_READ);
        if (!f) return false;
        if (!f.seek(_elemOff(startIdx))) { f.close(); return false; }

        uint32_t toRead = (uint32_t)BATCH_SIZE;
        if (startIdx + toRead > _count) toRead = _count - startIdx;

        uint32_t bytes = toRead * (uint32_t)sizeof(T);
        uint32_t got   = (uint32_t)f.read((uint8_t*)_batch, bytes);
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
        for (size_t i = 0; i < (size_t)_count; i++) buf[i] = _mem[i];
        delete[] _mem;
        _mem    = buf;
        _memCap = newCap;
        return true;
    }

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
        return _openOrCreate();
    }

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
            set(i, val);
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