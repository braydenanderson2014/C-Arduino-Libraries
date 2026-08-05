#ifndef SIMPLERWLOCK_H
#define SIMPLERWLOCK_H

#include <Arduino.h>
#include <stdint.h>

#if !defined(HOST_ARDUINO_SIM_ARDUINO_H) && \
    !defined(ESP32) && \
    !defined(ARDUINO_ARCH_ESP32) && \
    !defined(ARDUINO_ARCH_MBED) && \
    !defined(__MBED__) && \
    !defined(TARGET_RTOS_MBED) && \
    !defined(ARDUINO_ARCH_STM32) && \
    !defined(ARDUINO_UNO_Q) && \
    !defined(SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED)
    #error "SimpleRWLock targets ESP32, Arduino mbed cores (for example GIGA), and STM32-based targets (including UNO Q and VENTUNO Q class MCU-side builds). Define SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED to bypass this gate."
#endif

#if defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
    #define SRWL_HAS_MBED_RTOS 1
    #include <cmsis_os2.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #define SRWL_HAS_FREERTOS 1
    extern "C" {
        #include <freertos/FreeRTOS.h>
        #include <freertos/semphr.h>
    }
#endif

class SimpleRWLock {
public:
    SimpleRWLock() {
#if defined(SRWL_HAS_MBED_RTOS)
        osMutexAttr_t attr;
        memset(&attr, 0, sizeof(attr));
        attr.attr_bits = osMutexPrioInherit;
        _stateMutex = osMutexNew(&attr);
#elif defined(SRWL_HAS_FREERTOS)
        _stateMutex = xSemaphoreCreateMutex();
#endif
    }

    ~SimpleRWLock() {
#if defined(SRWL_HAS_MBED_RTOS)
        if (_stateMutex != nullptr) {
            osMutexDelete(_stateMutex);
            _stateMutex = nullptr;
        }
#elif defined(SRWL_HAS_FREERTOS)
        if (_stateMutex != nullptr) {
            vSemaphoreDelete(_stateMutex);
            _stateMutex = nullptr;
        }
#endif
    }

    bool readLock(unsigned long timeoutMs = 0) {
        const unsigned long start = millis();
        while (true) {
            if (!lockState(timeoutMs)) {
                return false;
            }

            const bool canRead = !_writerActive && _waitingWriters == 0;
            if (canRead) {
                ++_readers;
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

    void readUnlock() {
        if (!lockState(0)) {
            return;
        }
        if (_readers > 0) {
            --_readers;
        }
        unlockState();
    }

    bool writeLock(unsigned long timeoutMs = 0) {
        const unsigned long start = millis();

        if (!lockState(timeoutMs)) {
            return false;
        }
        ++_waitingWriters;
        unlockState();

        while (true) {
            if (!lockState(timeoutMs)) {
                return false;
            }

            const bool canWrite = !_writerActive && _readers == 0;
            if (canWrite) {
                _writerActive = true;
                if (_waitingWriters > 0) {
                    --_waitingWriters;
                }
                unlockState();
                return true;
            }
            unlockState();

            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                if (lockState(0)) {
                    if (_waitingWriters > 0) {
                        --_waitingWriters;
                    }
                    unlockState();
                }
                return false;
            }

            ::yield();
            delay(1);
        }
    }

    void writeUnlock() {
        if (!lockState(0)) {
            return;
        }
        _writerActive = false;
        unlockState();
    }

    uint16_t activeReaders() const {
        return _readers;
    }

    bool writerActive() const {
        return _writerActive;
    }

private:
    bool lockState(unsigned long timeoutMs) const {
#if defined(SRWL_HAS_MBED_RTOS)
        if (_stateMutex == nullptr) {
            return false;
        }
        const uint32_t wait = (timeoutMs == 0) ? osWaitForever : static_cast<uint32_t>(timeoutMs);
        return osMutexAcquire(_stateMutex, wait) == osOK;
#elif defined(SRWL_HAS_FREERTOS)
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
#if defined(SRWL_HAS_MBED_RTOS)
        if (_stateMutex != nullptr) {
            osMutexRelease(_stateMutex);
        }
#elif defined(SRWL_HAS_FREERTOS)
        if (_stateMutex != nullptr) {
            xSemaphoreGive(_stateMutex);
        }
#else
        interrupts();
#endif
    }

#if defined(SRWL_HAS_MBED_RTOS)
    mutable osMutexId_t _stateMutex = nullptr;
#elif defined(SRWL_HAS_FREERTOS)
    mutable SemaphoreHandle_t _stateMutex = nullptr;
#endif

    volatile uint16_t _readers = 0;
    volatile uint16_t _waitingWriters = 0;
    volatile bool _writerActive = false;
};

#endif // SIMPLERWLOCK_H
