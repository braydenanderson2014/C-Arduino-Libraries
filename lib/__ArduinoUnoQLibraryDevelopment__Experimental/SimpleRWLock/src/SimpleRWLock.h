#ifndef SIMPLERWLOCK_H
#define SIMPLERWLOCK_H

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

// Maximum number of distinct threads whose read locks can be tracked for
// reentrancy.  Extra concurrent readers still work, they just cannot re-enter.
#ifndef SRWL_MAX_TRACKED_READERS
#  define SRWL_MAX_TRACKED_READERS 8
#endif

class SimpleRWLock {
public:
    class ReadGuard {
    public:
        explicit ReadGuard(SimpleRWLock& lock, unsigned long timeoutMs = 0)
            : _lock(lock), _held(lock.readLock(timeoutMs)) {}

        ~ReadGuard() {
            if (_held) {
                _lock.readUnlock();
            }
        }

        bool locked() const {
            return _held;
        }

    private:
        SimpleRWLock& _lock;
        bool _held;
    };

    class WriteGuard {
    public:
        explicit WriteGuard(SimpleRWLock& lock, unsigned long timeoutMs = 0)
            : _lock(lock), _held(lock.writeLock(timeoutMs)) {}

        ~WriteGuard() {
            if (_held) {
                _lock.writeUnlock();
            }
        }

        bool locked() const {
            return _held;
        }

    private:
        SimpleRWLock& _lock;
        bool _held;
    };

    SimpleRWLock() {
        memset(const_cast<ReaderEntry*>(_readerTable), 0, sizeof(_readerTable));
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

    // Reentrant: a thread that already holds this lock (for reading or for
    // writing) acquires it again without blocking.
    bool readLock(unsigned long timeoutMs = 0) {
        const intptr_t id = currentThreadId();
        const unsigned long start = millis();

        while (true) {
            if (!lockState(timeoutMs)) {
                return false;
            }

            // Already the writer on this thread: count it as write recursion.
            if (_writerActive && _writerThreadId == id) {
                ++_writerRecursion;
                unlockState();
                return true;
            }

            // Already a reader on this thread: re-enter, ignoring queued
            // writers, otherwise the thread would be waiting on itself.
            ReaderEntry* entry = findReader(id);
            if (entry != nullptr) {
                ++entry->count;
                ++_readers;
                unlockState();
                return true;
            }

            if (!_writerActive && _waitingWriters == 0) {
                if (!trackReader(id)) {
                    ++_untrackedReaders;
                }
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
        const intptr_t id = currentThreadId();

        if (!lockState(0)) {
            return;
        }

        ReaderEntry* entry = findReader(id);
        if (entry != nullptr) {
            if (--entry->count == 0) {
                entry->id = 0;
            }
            if (_readers > 0) {
                --_readers;
            }
        } else if (_writerActive && _writerThreadId == id) {
            releaseWriteRecursion();
        } else if (_untrackedReaders > 0) {
            --_untrackedReaders;
            if (_readers > 0) {
                --_readers;
            }
        }
        unlockState();
    }

    // Reentrant for the owning thread.  Upgrading from a read lock to a write
    // lock is not supported and fails fast instead of deadlocking.
    bool writeLock(unsigned long timeoutMs = 0) {
        const intptr_t id = currentThreadId();
        const unsigned long start = millis();

        if (!lockState(timeoutMs)) {
            return false;
        }
        if (_writerActive && _writerThreadId == id) {
            ++_writerRecursion;
            unlockState();
            return true;
        }
        if (findReader(id) != nullptr) {
            unlockState();
            return false; // read-to-write upgrade would deadlock
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
                _writerThreadId = id;
                _writerRecursion = 1;
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
        const intptr_t id = currentThreadId();

        if (!lockState(0)) {
            return;
        }
        if (_writerActive && _writerThreadId == id) {
            releaseWriteRecursion();
        }
        unlockState();
    }

    bool holdsReadLock() const {
        return const_cast<SimpleRWLock*>(this)->findReader(currentThreadId()) != nullptr;
    }

    bool holdsWriteLock() const {
        return _writerActive && _writerThreadId == currentThreadId();
    }

    uint16_t activeReaders() const {
        return _readers;
    }

    bool writerActive() const {
        return _writerActive;
    }

    intptr_t writerThreadId() const {
        return _writerThreadId;
    }

private:
    struct ReaderEntry {
        intptr_t id;
        uint16_t count;
    };

    // The helpers below assume the state mutex is already held.
    ReaderEntry* findReader(intptr_t id) {
        for (uint8_t i = 0; i < SRWL_MAX_TRACKED_READERS; ++i) {
            ReaderEntry* entry = const_cast<ReaderEntry*>(&_readerTable[i]);
            if (entry->count > 0 && entry->id == id) {
                return entry;
            }
        }
        return nullptr;
    }

    bool trackReader(intptr_t id) {
        for (uint8_t i = 0; i < SRWL_MAX_TRACKED_READERS; ++i) {
            ReaderEntry* entry = const_cast<ReaderEntry*>(&_readerTable[i]);
            if (entry->count == 0) {
                entry->id = id;
                entry->count = 1;
                return true;
            }
        }
        return false;
    }

    void releaseWriteRecursion() {
        if (_writerRecursion > 0 && --_writerRecursion == 0) {
            _writerActive = false;
            _writerThreadId = 0;
        }
    }

    static intptr_t currentThreadId() {
#if defined(SRWL_HAS_MBED_RTOS)
        return reinterpret_cast<intptr_t>(osThreadGetId());
#elif defined(SRWL_HAS_FREERTOS)
        return reinterpret_cast<intptr_t>(xTaskGetCurrentTaskHandle());
#else
        return 1;
#endif
    }

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

    volatile ReaderEntry _readerTable[SRWL_MAX_TRACKED_READERS];
    volatile uint16_t _readers = 0;
    volatile uint16_t _untrackedReaders = 0;
    volatile uint16_t _waitingWriters = 0;
    volatile uint16_t _writerRecursion = 0;
    volatile intptr_t _writerThreadId = 0;
    volatile bool _writerActive = false;
};

#endif // SIMPLERWLOCK_H
