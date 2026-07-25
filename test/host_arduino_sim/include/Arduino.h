#ifndef HOST_ARDUINO_SIM_ARDUINO_H
#define HOST_ARDUINO_SIM_ARDUINO_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

using byte = std::uint8_t;
using String = std::string;

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

    void println() {
        std::cout << std::endl;
    }

    explicit operator bool() const {
        return true;
    }
};

inline HostSerial Serial;

#endif
