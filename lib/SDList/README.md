# SDList

`SDList` is a paged, batch-buffered list for Arduino that offloads element storage
to an SD card (or LittleFS) while keeping only a single fixed-size window of
elements in RAM at a time.  It behaves like `ArrayList` / `SimpleVector` from the
user's perspective but lets you hold far more elements than would fit in RAM.

## How it works

* **Binary storage** — each element is written as raw `sizeof(T)` bytes.  No text
  serialisation means no data corruption for any trivially-copyable type (`int`,
  `float`, `struct`, etc.).
* **Fixed batch window** — one block of `BATCH_SIZE` elements (default 8) lives in
  RAM.  When you access an element outside that window the current batch is flushed
  to disk and the new batch is loaded.  The buffer is allocated once and never
  reallocated.
* **Pre-allocated file** — on `begin()` the file is created with
  `reserveCapacity` zero-filled slots up front.  This avoids filesystem
  fragmentation.  The reserved area grows automatically (by `BATCH_SIZE` slots at a
  time) if you add more elements than originally reserved.
* **LittleFS support** — compile with `-DUSE_LITTLEFS` to route all filesystem calls
  through `LittleFS` instead of the SD library.

### File format

```
Offset                Size      Field
0                     4         Magic  'S','D','L','2'
4                     4         count     (uint32_t, little-endian)
8                     4         reserved  (uint32_t, little-endian)
12 + i * sizeof(T)    sizeof(T) element i
```

---

## Installation

1. Copy the `SDList` folder into your project's `lib/` directory.
2. `#include "SDList.h"` in your sketch.

---

## Quick start

```cpp
#include "SDList.h"

// Store up to 32 ints, batch window = 8 elements
SDList<int, 8> myList(SDCARD, 32);

void setup() {
    Serial.begin(9600);
    if (!myList.begin(4, "ints.bin")) {   // CS pin 4
        Serial.println("SD failed – running in memory mode");
    }

    for (int i = 0; i < 20; i++) myList.append(i * 10);

    Serial.println(myList.get(5));   // 50
    myList.set(5, 999);
    myList.insert(0, -1);
    myList.remove(1);
    myList.flush();                  // explicit flush (also happens on destruction)
}
```

### LittleFS variant

```cpp
// Compile with -DUSE_LITTLEFS
SDList<float, 4> data(SDCARD, 64);
void setup() {
    LittleFS.begin();
    data.beginLFS("floats.bin");
    data.append(3.14f);
}
```

---

## API

| Method | Description |
|--------|-------------|
| `begin(csPin [, filename])` | Initialise SD card and open / create the backing file. Returns `false` and falls back to MEMORY mode if the card is unavailable. |
| `beginLFS([filename])` | Initialise using LittleFS (requires `-DUSE_LITTLEFS`). |
| `append(element)` | Add element to the end. Returns `bool`. |
| `add(element)` | Alias for `append()`. |
| `get(index)` | Return element at index (or `T()` if out of range). |
| `set(index, element)` | Overwrite element at index. Returns `bool`. |
| `insert(index, element)` | Insert element, shifting right. Returns `bool`. |
| `remove(index)` | Remove element, shifting left. Returns `bool`. |
| `clear()` | Remove all elements and reset the file to its reserved-capacity state. |
| `flush()` | Force-write the current in-memory batch to disk. |
| `size()` | Number of valid elements. |
| `capacity()` | Number of pre-allocated slots (SDCARD) or heap slots (MEMORY). |
| `isEmpty()` | `true` when `size() == 0`. |
| `isReady()` | `true` if the filesystem was initialised successfully. |
| `getMode()` | `SDCARD` or `MEMORY`. |
| `setMode(mode)` | Switch modes, migrating all data. |

---

## Template parameters

```cpp
SDList<T, BATCH_SIZE>
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| `T` | — | Element type.  Must be trivially copyable. |
| `BATCH_SIZE` | `8` | Elements held in RAM at once.  Larger → fewer file opens, more RAM. |

---

## Constructor

```cpp
SDList<T, BATCH_SIZE>(SDListMode mode = SDCARD, uint32_t reserveCapacity = 16)
```

`reserveCapacity` is the number of element slots written as zeros into the file
when it is first created.  It grows automatically but pre-sizing avoids
fragmentation on the FAT filesystem.

---

## Notes

* **Type constraint** — `T` must be trivially copyable (POD types, plain structs).
  `String` and other types that own heap memory are **not** supported in SDCARD mode
  because their binary representation cannot be meaningfully written to / read from
  a file.  Use MEMORY mode or store plain `char` arrays instead.
* `flush()` is called automatically when the batch window shifts and on object
  destruction.  For maximum durability after a burst of writes, call it explicitly.
* Each `SDList` instance should use a **unique filename** to avoid conflicts.
* The SD library version must be ≥ 1.3.0 (Arduino `FILE_WRITE` must not include
  `O_APPEND` so that in-place seeks work).

---

## Dependencies

* Arduino
* SD + SPI  (or LittleFS when compiled with `-DUSE_LITTLEFS`)

---

## ChangeLog

### Version 2.0.0
* **Complete rewrite** — binary storage replaces text serialisation; eliminates
  all data corruption for non-String types.
* Fixed batch window: one pre-allocated `T[BATCH_SIZE]` buffer, never reallocated.
* Pre-allocated file with grow-on-demand: no fragmentation.
* `set()` method added for direct element overwrite.
* `flush()` / `isReady()` added.
* `insert()` and `remove()` now correctly shift elements in binary mode.
* LittleFS support via `-DUSE_LITTLEFS` compile flag.
* Dropped `ArrayList` dependency.
* `SDListMode` enum replaces `Mode` to avoid name collisions.

### Version 1.0.5
* Added `getUnderlyingStructure()`.

### Version 1.0.3
* Reimplemented using ArrayList for in-memory storage.

### Version 1.0.0
* Initial release.
