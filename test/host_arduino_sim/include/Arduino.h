#ifndef HOST_ARDUINO_SIM_ARDUINO_H
#define HOST_ARDUINO_SIM_ARDUINO_H

#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

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

inline unsigned long millis() {
    static const auto start = std::chrono::steady_clock::now();
    const auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count()
    );
}

inline void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

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

class HostSerial {
public:
    void begin(unsigned long) {}
    void flush() {}

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
