#ifndef SIMPLECORERUNTIME_H
#define SIMPLECORERUNTIME_H

#include <Arduino.h>
#include <stdint.h>
#include "ThreadManager.h"

#if !defined(HOST_ARDUINO_SIM_ARDUINO_H) && \
    !defined(ESP32) && \
    !defined(ARDUINO_ARCH_ESP32) && \
    !defined(ARDUINO_ARCH_MBED) && \
    !defined(__MBED__) && \
    !defined(TARGET_RTOS_MBED) && \
    !defined(ARDUINO_ARCH_STM32) && \
    !defined(ARDUINO_UNO_Q) && \
    !defined(SIMPLE_CORE_RUNTIME_ALLOW_UNSUPPORTED)
    #error "SimpleCoreRuntime targets ESP32, Arduino mbed cores (for example GIGA), and STM32-based targets (including UNO Q and VENTUNO Q class MCU-side builds). Define SIMPLE_CORE_RUNTIME_ALLOW_UNSUPPORTED to bypass this gate."
#endif

class SimpleCoreRuntime {
public:
    typedef ThreadManager::ThreadId ThreadId;
    typedef ThreadManager::ThreadPriority ThreadPriority;

    struct RuntimeInfo {
        bool threading;
        bool multiCore;
        bool coreAffinity;
        int processorCount;
        int currentCore;
    };

    static RuntimeInfo info() {
        RuntimeInfo out;
        out.threading = ThreadManager::hasThreading();
        out.multiCore = hasMultiCore();
        out.coreAffinity = supportsCoreAffinity();
        out.processorCount = processorCount();
        out.currentCore = currentCore();
        return out;
    }

    static bool hasThreading() {
        return ThreadManager::hasThreading();
    }

    static bool hasMultiCore() {
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
        return ThreadManager::hasMultiCoreScheduling();
#elif defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
        // Most mbed-class Arduino targets are single-core, while H747 class boards are dual-core.
        #if defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
            return true;
        #else
            return false;
        #endif
#elif defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_UNO_Q)
        return false;
#else
        return false;
#endif
    }

    static bool supportsCoreAffinity() {
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
        return ThreadManager::hasMultiCoreScheduling();
#else
        return false;
#endif
    }

    static int processorCount() {
        if (!hasThreading()) {
            return 1;
        }
        const int reported = ThreadManager::processorCount();
        if (reported > 0) {
            return reported;
        }
        return hasMultiCore() ? 2 : 1;
    }

    static int currentCore() {
        return ThreadManager::currentCore();
    }

    static ThreadPriority defaultPriority() {
        return ThreadManager::defaultPriority();
    }

    static ThreadId invalidThreadId() {
        return ThreadManager::InvalidThreadId;
    }

    static ThreadId launchTask(
        ThreadManager::ThreadFunction function,
        void* argument = nullptr,
        const char* name = nullptr,
        uint32_t stackSize = 0,
        ThreadPriority priority = ThreadManager::defaultPriority()) {
        return ThreadManager::createThread(function, argument, name, stackSize, priority);
    }

    static ThreadId launchTaskOnCore(
        ThreadManager::ThreadFunction function,
        void* argument,
        int8_t core,
        const char* name = nullptr,
        uint32_t stackSize = 0,
        ThreadPriority priority = ThreadManager::defaultPriority()) {
        if (!supportsCoreAffinity()) {
            return ThreadManager::createThread(function, argument, name, stackSize, priority);
        }
        return ThreadManager::createThreadOnCore(function, argument, core, name, stackSize, priority);
    }

    static bool waitForTask(ThreadId threadId, uint32_t timeoutMs = 0) {
        return ThreadManager::joinThread(threadId, timeoutMs);
    }

    static void stopTask(ThreadId threadId) {
        ThreadManager::endThread(threadId);
    }

    static void yieldNow() {
        ThreadManager::yield();
    }

    static void delayMs(uint32_t ms) {
        ThreadManager::delayThread(ms);
    }
};

#endif // SIMPLECORERUNTIME_H
