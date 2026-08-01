#ifndef SIMPLESEMAPHORE_H
#define SIMPLESEMAPHORE_H

#include <Arduino.h>
#include <stdint.h>

#if defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
    #define SSEM_HAS_MBED_RTOS 1
    #include <cmsis_os2.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #define SSEM_HAS_FREERTOS 1
    extern "C" {
        #include <freertos/FreeRTOS.h>
        #include <freertos/semphr.h>
    }
#endif

class SimpleSemaphore {
public:
    SimpleSemaphore(uint16_t maxCount, uint16_t initialCount = 0)
        : _maxCount(maxCount), _initialCount(initialCount)
#if !defined(SSEM_HAS_MBED_RTOS) && !defined(SSEM_HAS_FREERTOS)
        , _count(initialCount)
#endif
    {
#if defined(SSEM_HAS_MBED_RTOS)
        _semaphore = osSemaphoreNew(maxCount, initialCount, nullptr);
#elif defined(SSEM_HAS_FREERTOS)
        _semaphore = xSemaphoreCreateCounting(maxCount, initialCount);
#endif
    }

    ~SimpleSemaphore() {
#if defined(SSEM_HAS_MBED_RTOS)
        if (_semaphore != nullptr) {
            osSemaphoreDelete(_semaphore);
            _semaphore = nullptr;
        }
#elif defined(SSEM_HAS_FREERTOS)
        if (_semaphore != nullptr) {
            vSemaphoreDelete(_semaphore);
            _semaphore = nullptr;
        }
#endif
    }

    bool acquire(unsigned long timeoutMs = 0) {
#if defined(SSEM_HAS_MBED_RTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        const uint32_t wait = (timeoutMs == 0) ? osWaitForever : static_cast<uint32_t>(timeoutMs);
        return osSemaphoreAcquire(_semaphore, wait) == osOK;
#elif defined(SSEM_HAS_FREERTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        const TickType_t ticks =
            (timeoutMs == 0) ? portMAX_DELAY : pdMS_TO_TICKS(static_cast<uint32_t>(timeoutMs));
        return xSemaphoreTake(_semaphore, ticks) == pdTRUE;
#else
        const unsigned long start = millis();
        while (true) {
            noInterrupts();
            if (_count > 0) {
                --_count;
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

    bool tryAcquire() {
#if defined(SSEM_HAS_MBED_RTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        return osSemaphoreAcquire(_semaphore, 0) == osOK;
#elif defined(SSEM_HAS_FREERTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        return xSemaphoreTake(_semaphore, 0) == pdTRUE;
#else
        noInterrupts();
        if (_count > 0) {
            --_count;
            interrupts();
            return true;
        }
        interrupts();
        return false;
#endif
    }

    bool release(uint16_t amount = 1) {
        if (amount == 0) {
            return true;
        }

#if defined(SSEM_HAS_MBED_RTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        for (uint16_t i = 0; i < amount; ++i) {
            if (osSemaphoreRelease(_semaphore) != osOK) {
                return false;
            }
        }
        return true;
#elif defined(SSEM_HAS_FREERTOS)
        if (_semaphore == nullptr) {
            return false;
        }
        for (uint16_t i = 0; i < amount; ++i) {
            if (xSemaphoreGive(_semaphore) != pdTRUE) {
                return false;
            }
        }
        return true;
#else
        noInterrupts();
        if (_count + amount > _maxCount) {
            interrupts();
            return false;
        }
        _count += amount;
        interrupts();
        return true;
#endif
    }

    uint16_t available() const {
#if defined(SSEM_HAS_MBED_RTOS)
        if (_semaphore == nullptr) {
            return 0;
        }
        return static_cast<uint16_t>(osSemaphoreGetCount(_semaphore));
#elif defined(SSEM_HAS_FREERTOS)
        if (_semaphore == nullptr) {
            return 0;
        }
        return static_cast<uint16_t>(uxSemaphoreGetCount(_semaphore));
#else
        return _count;
#endif
    }

    uint16_t maxCount() const {
        return _maxCount;
    }

    bool isRuntimeBacked() const {
#if defined(SSEM_HAS_MBED_RTOS) || defined(SSEM_HAS_FREERTOS)
        return true;
#else
        return false;
#endif
    }

private:
    uint16_t _maxCount;
    uint16_t _initialCount;

#if defined(SSEM_HAS_MBED_RTOS)
    osSemaphoreId_t _semaphore = nullptr;
#elif defined(SSEM_HAS_FREERTOS)
    SemaphoreHandle_t _semaphore = nullptr;
#else
    volatile uint16_t _count;
#endif
};

#endif // SIMPLESEMAPHORE_H
