#ifndef HOST_ARDUINO_SIM_SD_H
#define HOST_ARDUINO_SIM_SD_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include "Arduino.h"

constexpr std::uint8_t FILE_READ = 0x01;
constexpr std::uint8_t FILE_WRITE = 0x02;

class File {
public:
    File() = default;
    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&& other) noexcept
        : stream_(std::move(other.stream_)) {}

    File& operator=(File&& other) noexcept {
        if (this != &other) {
            stream_ = std::move(other.stream_);
        }
        return *this;
    }

    explicit File(std::fstream&& stream)
        : stream_(std::move(stream)) {}

    explicit operator bool() const {
        return stream_.is_open();
    }

    bool seek(std::uint32_t pos) {
        if (!stream_.is_open()) {
            return false;
        }
        stream_.clear();
        stream_.seekg(static_cast<std::streamoff>(pos), std::ios::beg);
        stream_.seekp(static_cast<std::streamoff>(pos), std::ios::beg);
        return !stream_.fail();
    }

    std::size_t write(const std::uint8_t* data, std::size_t length) {
        if (!stream_.is_open()) {
            return 0;
        }
        stream_.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(length));
        if (stream_.fail()) {
            return 0;
        }
        return length;
    }

    std::size_t read(std::uint8_t* data, std::size_t length) {
        if (!stream_.is_open()) {
            return 0;
        }
        stream_.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(length));
        return static_cast<std::size_t>(stream_.gcount());
    }

    void close() {
        if (stream_.is_open()) {
            stream_.close();
        }
    }

private:
    std::fstream stream_;
};

class SDClass {
public:
    bool begin(int) {
        std::error_code ec;
        std::filesystem::create_directories(rootPath_, ec);
        return true;
    }

    void setRoot(const std::string& rootPath) {
        rootPath_ = rootPath;
    }

    bool exists(const String& filename) const {
        return std::filesystem::exists(resolvePath_(filename));
    }

    bool remove(const String& filename) const {
        std::error_code ec;
        return std::filesystem::remove(resolvePath_(filename), ec);
    }

    File open(const String& filename, std::uint8_t mode) const {
        const auto resolved = resolvePath_(filename);

        std::error_code ec;
        std::filesystem::create_directories(resolved.parent_path(), ec);

        std::ios::openmode openMode = std::ios::binary;
        if (mode == FILE_READ) {
            openMode |= std::ios::in;
        } else {
            openMode |= std::ios::in | std::ios::out;
            if (!std::filesystem::exists(resolved)) {
                std::ofstream createFile(resolved.string(), std::ios::binary);
            }
        }

        std::fstream stream(resolved.string(), openMode);
        return File(std::move(stream));
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
};

inline SDClass SD;

#endif
