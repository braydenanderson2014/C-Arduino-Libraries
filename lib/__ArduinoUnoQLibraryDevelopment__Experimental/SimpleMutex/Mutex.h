#ifndef MUTEX_H
#define MUTEX_H

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#if !defined(HOST_ARDUINO_SIM_ARDUINO_H) && \
    !defined(ESP32) && \
    !defined(ARDUINO_ARCH_ESP32) && \
    !defined(ARDUINO_ARCH_MBED) && \
    !defined(__MBED__) && \
    !defined(TARGET_RTOS_MBED) && \
    !defined(ARDUINO_ARCH_STM32) && \
    !defined(ARDUINO_UNO_Q) && \
    !defined(SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED)
    #error "SimpleMutex targets ESP32, Arduino mbed cores (for example GIGA), and STM32-based targets (including UNO Q and VENTUNO Q class MCU-side builds). Define SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED to bypass this gate."
#endif

#if defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
    #define SMUTEX_HAS_MBED_RTOS 1
    #include <cmsis_os2.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #define SMUTEX_HAS_FREERTOS 1
    extern "C" {
        #include <freertos/FreeRTOS.h>
        #include <freertos/semphr.h>
    }
#endif

class Mutex {
public:
    class LockGuard {
    public:
        explicit LockGuard(Mutex& mutex, unsigned long timeoutMs = 0)
            : _mutex(mutex), _locked(mutex.lock(timeoutMs)) {}

        ~LockGuard() {
            if (_locked) {
                _mutex.unlock();
            }
        }

        bool locked() const {
            return _locked;
        }

    private:
        Mutex& _mutex;
        bool _locked;
    };

    Mutex() : _ownerThreadId(0), _recursionCount(0) {
#if defined(SMUTEX_HAS_MBED_RTOS)
        osMutexAttr_t attr;
        memset(&attr, 0, sizeof(attr));
        attr.attr_bits = osMutexRecursive | osMutexPrioInherit;
        _nativeMutex = osMutexNew(&attr);
#elif defined(SMUTEX_HAS_FREERTOS)
        _nativeMutex = xSemaphoreCreateRecursiveMutex();
#else
        _locked = false;
#endif
    }

    ~Mutex() {
#if defined(SMUTEX_HAS_MBED_RTOS)
        if (_nativeMutex != nullptr) {
            osMutexDelete(_nativeMutex);
            _nativeMutex = nullptr;
        }
#elif defined(SMUTEX_HAS_FREERTOS)
        if (_nativeMutex != nullptr) {
            vSemaphoreDelete(_nativeMutex);
            _nativeMutex = nullptr;
        }
#endif
    }

    // timeoutMs = 0 means wait forever.
    bool lock(unsigned long timeoutMs = 0) {
        const intptr_t currentId = currentThreadId();

#if defined(SMUTEX_HAS_MBED_RTOS)
        if (_nativeMutex == nullptr) {
            return false;
        }
        const uint32_t wait = (timeoutMs == 0) ? osWaitForever : static_cast<uint32_t>(timeoutMs);
        if (osMutexAcquire(_nativeMutex, wait) != osOK) {
            return false;
        }
        noInterrupts();
        _ownerThreadId = currentId;
        ++_recursionCount;
        interrupts();
        return true;
#elif defined(SMUTEX_HAS_FREERTOS)
        if (_nativeMutex == nullptr) {
            return false;
        }
        const TickType_t ticks =
            (timeoutMs == 0) ? portMAX_DELAY : pdMS_TO_TICKS(static_cast<uint32_t>(timeoutMs));
        if (xSemaphoreTakeRecursive(_nativeMutex, ticks) != pdTRUE) {
            return false;
        }
        noInterrupts();
        _ownerThreadId = currentId;
        ++_recursionCount;
        interrupts();
        return true;
#else
        const unsigned long start = millis();
        while (true) {
            noInterrupts();
            if (!_locked || _ownerThreadId == currentId) {
                _locked = true;
                _ownerThreadId = currentId;
                ++_recursionCount;
                interrupts();
                return true;
            }
            interrupts();

            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                return false;
            }

            ::yield();
            delay(1);
        }
#endif
    }

    bool tryLock() {
        const intptr_t currentId = currentThreadId();

#if defined(SMUTEX_HAS_MBED_RTOS)
        if (_nativeMutex == nullptr) {
            return false;
        }
        if (osMutexAcquire(_nativeMutex, 0) != osOK) {
            return false;
        }
        noInterrupts();
        _ownerThreadId = currentId;
        ++_recursionCount;
        interrupts();
        return true;
#elif defined(SMUTEX_HAS_FREERTOS)
        if (_nativeMutex == nullptr) {
            return false;
        }
        if (xSemaphoreTakeRecursive(_nativeMutex, 0) != pdTRUE) {
            return false;
        }
        noInterrupts();
        _ownerThreadId = currentId;
        ++_recursionCount;
        interrupts();
        return true;
#else
        noInterrupts();
        if (!_locked || _ownerThreadId == currentId) {
            _locked = true;
            _ownerThreadId = currentId;
            ++_recursionCount;
            interrupts();
            return true;
        }
        interrupts();
        return false;
#endif
    }

    void unlock() {
        const intptr_t currentId = currentThreadId();

        noInterrupts();
        if (_ownerThreadId != currentId || _recursionCount == 0) {
            interrupts();
            return;
        }
        --_recursionCount;
        const bool fullyReleased = (_recursionCount == 0);
        if (fullyReleased) {
            _ownerThreadId = 0;
#if !defined(SMUTEX_HAS_MBED_RTOS) && !defined(SMUTEX_HAS_FREERTOS)
            _locked = false;
#endif
        }
        interrupts();

#if defined(SMUTEX_HAS_MBED_RTOS)
        if (_nativeMutex != nullptr) {
            osMutexRelease(_nativeMutex);
        }
#elif defined(SMUTEX_HAS_FREERTOS)
        if (_nativeMutex != nullptr) {
            xSemaphoreGiveRecursive(_nativeMutex);
        }
#endif
    }

    bool isLocked() const {
        return _recursionCount > 0;
    }

    intptr_t ownerThreadId() const {
        return _ownerThreadId;
    }

private:
    static intptr_t currentThreadId() {
#if defined(SMUTEX_HAS_MBED_RTOS)
    return reinterpret_cast<intptr_t>(osThreadGetId());
#elif defined(SMUTEX_HAS_FREERTOS)
    return reinterpret_cast<intptr_t>(xTaskGetCurrentTaskHandle());
#else
    return 1;
#endif
    }

#if defined(SMUTEX_HAS_MBED_RTOS)
    osMutexId_t _nativeMutex;
#elif defined(SMUTEX_HAS_FREERTOS)
    SemaphoreHandle_t _nativeMutex;
#else
    volatile bool _locked;
#endif

    volatile intptr_t _ownerThreadId;
    volatile uint16_t _recursionCount;
};

#endif // MUTEX_H
