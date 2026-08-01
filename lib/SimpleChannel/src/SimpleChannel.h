#ifndef SIMPLECHANNEL_H
#define SIMPLECHANNEL_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
    #define SCHAN_HAS_MBED_RTOS 1
    #include <cmsis_os2.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #define SCHAN_HAS_FREERTOS 1
    extern "C" {
        #include <freertos/FreeRTOS.h>
        #include <freertos/semphr.h>
    }
#endif

template<typename T, size_t Capacity>
class SimpleChannel {
public:
    static_assert(Capacity > 0, "Capacity must be greater than zero");

    SimpleChannel() {
#if defined(SCHAN_HAS_MBED_RTOS)
        osMutexAttr_t attr;
        memset(&attr, 0, sizeof(attr));
        attr.attr_bits = osMutexPrioInherit;
        _stateMutex = osMutexNew(&attr);
#elif defined(SCHAN_HAS_FREERTOS)
        _stateMutex = xSemaphoreCreateMutex();
#endif
    }

    ~SimpleChannel() {
#if defined(SCHAN_HAS_MBED_RTOS)
        if (_stateMutex != nullptr) {
            osMutexDelete(_stateMutex);
            _stateMutex = nullptr;
        }
#elif defined(SCHAN_HAS_FREERTOS)
        if (_stateMutex != nullptr) {
            vSemaphoreDelete(_stateMutex);
            _stateMutex = nullptr;
        }
#endif
    }

    bool send(const T& item, unsigned long timeoutMs = 0) {
        const unsigned long start = millis();
        while (true) {
            if (!lockState(timeoutMs)) {
                return false;
            }

            if (_count < Capacity) {
                _buffer[_tail] = item;
                _tail = (_tail + 1) % Capacity;
                ++_count;
                unlockState();
                return true;
            }
            unlockState();

            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                return false;
            }

            ::yield();
            delay(1);
        }
    }

    bool trySend(const T& item) {
        if (!lockState(1)) {
            return false;
        }

        if (_count >= Capacity) {
            unlockState();
            return false;
        }

        _buffer[_tail] = item;
        _tail = (_tail + 1) % Capacity;
        ++_count;
        unlockState();
        return true;
    }

    bool receive(T& out, unsigned long timeoutMs = 0) {
        const unsigned long start = millis();
        while (true) {
            if (!lockState(timeoutMs)) {
                return false;
            }

            if (_count > 0) {
                out = _buffer[_head];
                _head = (_head + 1) % Capacity;
                --_count;
                unlockState();
                return true;
            }
            unlockState();

            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                return false;
            }

            ::yield();
            delay(1);
        }
    }

    bool tryReceive(T& out) {
        if (!lockState(1)) {
            return false;
        }

        if (_count == 0) {
            unlockState();
            return false;
        }

        out = _buffer[_head];
        _head = (_head + 1) % Capacity;
        --_count;
        unlockState();
        return true;
    }

    size_t size() const {
        return _count;
    }

    static constexpr size_t capacity() {
        return Capacity;
    }

    bool empty() const {
        return _count == 0;
    }

    bool full() const {
        return _count == Capacity;
    }

private:
    bool lockState(unsigned long timeoutMs) const {
#if defined(SCHAN_HAS_MBED_RTOS)
        if (_stateMutex == nullptr) {
            return false;
        }
        const uint32_t wait = (timeoutMs == 0) ? osWaitForever : static_cast<uint32_t>(timeoutMs);
        return osMutexAcquire(_stateMutex, wait) == osOK;
#elif defined(SCHAN_HAS_FREERTOS)
        if (_stateMutex == nullptr) {
            return false;
        }
        const TickType_t ticks =
            (timeoutMs == 0) ? portMAX_DELAY : pdMS_TO_TICKS(static_cast<uint32_t>(timeoutMs));
        return xSemaphoreTake(_stateMutex, ticks) == pdTRUE;
#else
        (void)timeoutMs;
        noInterrupts();
        return true;
#endif
    }

    void unlockState() const {
#if defined(SCHAN_HAS_MBED_RTOS)
        if (_stateMutex != nullptr) {
            osMutexRelease(_stateMutex);
        }
#elif defined(SCHAN_HAS_FREERTOS)
        if (_stateMutex != nullptr) {
            xSemaphoreGive(_stateMutex);
        }
#else
        interrupts();
#endif
    }

#if defined(SCHAN_HAS_MBED_RTOS)
    mutable osMutexId_t _stateMutex = nullptr;
#elif defined(SCHAN_HAS_FREERTOS)
    mutable SemaphoreHandle_t _stateMutex = nullptr;
#endif

    T _buffer[Capacity];
    volatile size_t _head = 0;
    volatile size_t _tail = 0;
    volatile size_t _count = 0;
};

#endif // SIMPLECHANNEL_H
