#ifndef HOST_ARDUINO_SIM_ARDUINO_H
#define HOST_ARDUINO_SIM_ARDUINO_H

#include <cctype>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

using byte = std::uint8_t;

class String : public std::string {
public:
    using std::string::string;

    String() : std::string() {}
    String(const char* s) : std::string(s ? s : "") {}
    String(const std::string& s) : std::string(s) {}
    String(char c) : std::string(1, c) {}
    String(int v) : std::string(std::to_string(v)) {}
    String(unsigned int v) : std::string(std::to_string(v)) {}
    String(long v) : std::string(std::to_string(v)) {}
    String(unsigned long v) : std::string(std::to_string(v)) {}
    String(float v) : std::string(std::to_string(v)) {}
    String(double v) : std::string(std::to_string(v)) {}

    long toInt() const {
        char* end = nullptr;
        const long value = std::strtol(this->c_str(), &end, 10);
        return (end == this->c_str()) ? 0L : value;
    }

    float toFloat() const {
        char* end = nullptr;
        const float value = std::strtof(this->c_str(), &end);
        return (end == this->c_str()) ? 0.0f : value;
    }

    String substring(unsigned int from) const {
        if (from >= this->size()) {
            return String("");
        }
        return String(this->substr(from));
    }

    String substring(unsigned int from, unsigned int to) const {
        if (from >= this->size()) {
            return String("");
        }
        if (to > this->size()) {
            to = static_cast<unsigned int>(this->size());
        }
        if (to <= from) {
            return String("");
        }
        return String(this->substr(from, to - from));
    }

    void trim() {
        size_t begin = 0;
        while (begin < this->size() && std::isspace(static_cast<unsigned char>((*this)[begin]))) {
            ++begin;
        }

        size_t end = this->size();
        while (end > begin && std::isspace(static_cast<unsigned char>((*this)[end - 1]))) {
            --end;
        }

        if (begin == 0 && end == this->size()) {
            return;
        }

        *this = this->substr(begin, end - begin);
    }

    int indexOf(char ch, unsigned int fromIndex = 0) const {
        if (fromIndex >= this->size()) {
            return -1;
        }

        for (size_t i = fromIndex; i < this->size(); ++i) {
            if ((*this)[i] == ch) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    int indexOf(const char* needle, unsigned int fromIndex = 0) const {
        if (!needle || fromIndex >= this->size()) {
            return -1;
        }

        const size_t needleLen = std::strlen(needle);
        if (needleLen == 0 || needleLen > (this->size() - fromIndex)) {
            return -1;
        }

        for (size_t i = fromIndex; i + needleLen <= this->size(); ++i) {
            bool matched = true;
            for (size_t j = 0; j < needleLen; ++j) {
                if ((*this)[i + j] != needle[j]) {
                    matched = false;
                    break;
                }
            }
            if (matched) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    int indexOf(const String& needle, unsigned int fromIndex = 0) const {
        return indexOf(needle.c_str(), fromIndex);
    }

    bool startsWith(const char* prefix, unsigned int offset = 0) const {
        if (!prefix || offset > this->size()) {
            return false;
        }

        const size_t prefixLen = std::strlen(prefix);
        if (offset + prefixLen > this->size()) {
            return false;
        }

        for (size_t i = 0; i < prefixLen; ++i) {
            if ((*this)[offset + i] != prefix[i]) {
                return false;
            }
        }

        return true;
    }

    bool startsWith(const String& prefix, unsigned int offset = 0) const {
        return startsWith(prefix.c_str(), offset);
    }
};

class Stream {
public:
    virtual ~Stream() = default;
    virtual int available() { return 0; }
    virtual int read() { return -1; }
    virtual size_t write(uint8_t) { return 1; }
    virtual size_t write(const uint8_t*, size_t size) { return size; }
    virtual void flush() {}
};

constexpr uint8_t LOW = 0;
constexpr uint8_t HIGH = 1;
constexpr uint8_t INPUT = 0;
constexpr uint8_t OUTPUT = 1;
constexpr uint8_t LED_BUILTIN = 13;
constexpr int DEC = 10;
constexpr int HEX = 16;
constexpr int OCT = 8;
constexpr int BIN = 2;

inline void pinMode(uint8_t, uint8_t) {}
inline void digitalWrite(uint8_t, uint8_t) {}

template <typename T>
inline T min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template <typename T>
inline T max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

inline unsigned long millis() {
    static const std::clock_t start = std::clock();
    const std::clock_t now = std::clock();
    if (now < start) {
        return 0UL;
    }
    const double elapsedTicks = static_cast<double>(now - start);
    const double elapsedMs = (elapsedTicks * 1000.0) / static_cast<double>(CLOCKS_PER_SEC);
    return static_cast<unsigned long>(elapsedMs);
}

inline void delay(unsigned long ms) {
    const unsigned long begin = millis();
    while ((millis() - begin) < ms) {
        // Busy-wait fallback for host shim portability.
    }
}

inline void yield() {
    // Cooperative no-op in host shim.
}

inline void noInterrupts() {}
inline void interrupts() {}

inline void randomSeed(unsigned long seed) {
    std::srand(static_cast<unsigned int>(seed));
}

inline long random(long max) {
    if (max <= 0) {
        return 0;
    }
    return static_cast<long>(std::rand() % max);
}

inline long random(long min, long max) {
    if (max <= min) {
        return min;
    }
    return min + random(max - min);
}

class HostSerial : public Stream {
public:
    void begin(unsigned long) {}

    int available() override { return 0; }
    int read() override { return -1; }
    size_t write(uint8_t) override { return 1; }
    size_t write(const uint8_t*, size_t size) override { return size; }
    void flush() override {}

    template <typename T>
    void print(const T& value) {
        std::cout << value;
    }

    template <typename T>
    void println(const T& value) {
        std::cout << value << std::endl;
    }

    template <typename T>
    void print(const T& value, int base) {
        if (base == HEX) {
            std::cout << std::hex << value << std::dec;
            return;
        }
        if (base == OCT) {
            std::cout << std::oct << value << std::dec;
            return;
        }
        std::cout << value;
    }

    template <typename T>
    void println(const T& value, int base) {
        print(value, base);
        std::cout << std::endl;
    }

    void println() {
        std::cout << std::endl;
    }

    explicit operator bool() const {
        return true;
    }
};

inline HostSerial Serial;

#endif
