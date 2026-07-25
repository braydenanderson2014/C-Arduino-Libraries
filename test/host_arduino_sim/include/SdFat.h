#ifndef HOST_ARDUINO_SIM_SDFAT_H
#define HOST_ARDUINO_SIM_SDFAT_H

#include "Arduino.h"

#include <cstddef>
#include <cstdint>

// Common open flags used by libraries in this repository.
constexpr uint8_t O_READ = 0x01;
constexpr uint8_t O_WRITE = 0x02;
constexpr uint8_t O_CREAT = 0x04;

#ifndef SD_CS_PIN
#define SD_CS_PIN 4
#endif

#ifndef SD_SCK_MHZ
#define SD_SCK_MHZ(x) (x)
#endif

class FsFile {
public:
    FsFile() = default;

    explicit operator bool() const {
        return true;
    }

    int read() {
        return -1;
    }

    int read(void*, size_t) {
        return 0;
    }

    size_t write(const void*, size_t size) {
        return size;
    }

    size_t write(uint8_t) {
        return 1;
    }

    void close() {}
};

class SdCardMock {
public:
    uint32_t sectorCount() const {
        return 4UL * 1024UL * 1024UL; // Simulated 2GB card (512-byte sectors)
    }
};

class SdVolumeMock {
public:
    uint32_t freeClusterCount() const {
        return 32768UL;
    }

    uint32_t clusterCount() const {
        return 65536UL;
    }

    uint32_t sectorsPerCluster() const {
        return 8UL;
    }
};

class SdFat {
public:
    SdFat() = default;

    bool begin() {
        return true;
    }

    bool begin(uint8_t, uint32_t = 0) {
        return true;
    }

    FsFile open(const char*, uint8_t = O_READ) {
        return FsFile();
    }

    FsFile open(const String&, uint8_t = O_READ) {
        return FsFile();
    }

    SdCardMock* card() {
        return &_card;
    }

    SdVolumeMock* vol() {
        return &_vol;
    }

private:
    SdCardMock _card;
    SdVolumeMock _vol;
};

// Compatibility type sometimes used by SdFat examples.
using SdFile = FsFile;

#endif
