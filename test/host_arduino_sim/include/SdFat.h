#ifndef HOST_ARDUINO_SIM_SDFAT_H
#define HOST_ARDUINO_SIM_SDFAT_H

#include "Arduino.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

constexpr uint8_t O_READ = 0x01;
constexpr uint8_t O_WRITE = 0x02;
constexpr uint8_t O_CREAT = 0x04;
constexpr uint8_t O_TRUNC = 0x08;

#ifndef SD_CS_PIN
#define SD_CS_PIN 4
#endif

#ifndef SD_SCK_MHZ
#define SD_SCK_MHZ(x) (x)
#endif

class FsFile {
public:
    FsFile() = default;
    FsFile(const FsFile&) = delete;
    FsFile& operator=(const FsFile&) = delete;

    FsFile(FsFile&& other) noexcept
        : stream_(std::move(other.stream_)) {}

    FsFile& operator=(FsFile&& other) noexcept {
        if (this != &other) {
            stream_ = std::move(other.stream_);
        }
        return *this;
    }

    explicit FsFile(std::fstream&& stream)
        : stream_(std::move(stream)) {}

    explicit operator bool() const {
        return stream_.is_open();
    }

    int read() {
        if (!stream_.is_open()) {
            return -1;
        }

        const int value = stream_.get();
        return stream_.good() ? value : -1;
    }

    int read(void* buffer, size_t length) {
        if (!stream_.is_open() || !buffer) {
            return 0;
        }

        stream_.read(static_cast<char*>(buffer), static_cast<std::streamsize>(length));
        return static_cast<int>(stream_.gcount());
    }

    size_t write(const void* buffer, size_t size) {
        if (!stream_.is_open()) {
            return 0;
        }

        stream_.write(static_cast<const char*>(buffer), static_cast<std::streamsize>(size));
        return stream_.fail() ? 0 : size;
    }

    size_t write(uint8_t value) {
        return write(&value, 1);
    }

    void close() {
        if (stream_.is_open()) {
            stream_.close();
        }
    }

private:
    std::fstream stream_;
};

class SdCardMock {
public:
    uint32_t sectorCount() const {
        return 4UL * 1024UL * 1024UL;
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
        std::error_code ec;
        std::filesystem::create_directories(rootPath_, ec);
        return true;
    }

    bool begin(uint8_t, uint32_t = 0) {
        return begin();
    }

    void setRoot(const std::string& rootPath) {
        rootPath_ = rootPath;
    }

    FsFile open(const char* filename, uint8_t mode = O_READ) {
        return open(String(filename ? filename : ""), mode);
    }

    FsFile open(const String& filename, uint8_t mode = O_READ) {
        const std::filesystem::path resolved = resolvePath_(filename);

        std::error_code ec;
        std::filesystem::create_directories(resolved.parent_path(), ec);

        std::ios::openmode openMode = std::ios::binary;
        if ((mode & O_READ) != 0) {
            openMode |= std::ios::in;
        }
        if ((mode & O_WRITE) != 0) {
            openMode |= std::ios::out;
        }
        if ((mode & O_TRUNC) != 0) {
            openMode |= std::ios::trunc;
        }

        if ((mode & O_CREAT) != 0 && !std::filesystem::exists(resolved)) {
            std::ofstream createFile(resolved.string(), std::ios::binary);
        }

        std::fstream stream(resolved.string(), openMode);
        return FsFile(std::move(stream));
    }

    SdCardMock* card() {
        return &_card;
    }

    SdVolumeMock* vol() {
        return &_vol;
    }

private:
    std::filesystem::path resolvePath_(const String& filename) const {
        const std::filesystem::path path(filename.c_str());
        if (path.is_absolute()) {
            return path;
        }
        return std::filesystem::path(rootPath_) / path;
    }

    std::string rootPath_ = ".";
    SdCardMock _card;
    SdVolumeMock _vol;
};

using SdFile = FsFile;

#endif
