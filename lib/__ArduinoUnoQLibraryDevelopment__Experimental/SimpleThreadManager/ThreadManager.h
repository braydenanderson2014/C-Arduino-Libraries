#ifndef THREADMANAGER_H
#define THREADMANAGER_H

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
    #error "SimpleThreadManager targets ESP32, Arduino mbed cores (for example GIGA), and STM32-based targets (including UNO Q and VENTUNO Q class MCU-side builds). Define SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED to bypass this gate."
#endif

#if defined(ARDUINO_ARCH_MBED) || defined(__MBED__) || defined(TARGET_RTOS_MBED)
    #define STM_HAS_MBED_RTOS 1
    #include <mbed.h>
    #include <cmsis_os2.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
    #define STM_HAS_FREERTOS 1
    extern "C" {
        #include <freertos/FreeRTOS.h>
        #include <freertos/task.h>
    }
#endif

class ThreadManager {
public:
    typedef void (*ThreadFunction)(void* argument);

#if defined(STM_HAS_MBED_RTOS)
    typedef osThreadId_t ThreadId;
    typedef osPriority_t ThreadPriority;
    static constexpr ThreadId InvalidThreadId = nullptr;
#elif defined(STM_HAS_FREERTOS)
    typedef TaskHandle_t ThreadId;
    typedef UBaseType_t ThreadPriority;
    static constexpr ThreadId InvalidThreadId = nullptr;
#else
    typedef int ThreadId;
    typedef int ThreadPriority;
    static constexpr ThreadId InvalidThreadId = -1;
#endif

    static bool hasThreading() {
#if defined(STM_HAS_MBED_RTOS) || defined(STM_HAS_FREERTOS)
        return true;
#else
        return false;
#endif
    }

    // True when the active runtime can schedule tasks across multiple cores.
    static bool hasMultiCoreScheduling() {
#if defined(STM_HAS_FREERTOS) && defined(portNUM_PROCESSORS)
        return portNUM_PROCESSORS > 1;
#else
        return false;
#endif
    }

    static int processorCount() {
#if defined(STM_HAS_FREERTOS) && defined(portNUM_PROCESSORS)
        return static_cast<int>(portNUM_PROCESSORS);
#else
        return 1;
#endif
    }

    static int currentCore() {
#if defined(STM_HAS_FREERTOS)
        return static_cast<int>(xPortGetCoreID());
#else
        return -1;
#endif
    }

    static ThreadPriority defaultPriority() {
#if defined(STM_HAS_MBED_RTOS)
        return osPriorityNormal;
#elif defined(STM_HAS_FREERTOS)
        return 1;
#else
        return 0;
#endif
    }

    static ThreadId createThread(
        ThreadFunction function,
        void* argument = nullptr,
        const char* name = nullptr,
        uint32_t stackSize = 0,
        ThreadPriority priority = defaultPriority(),
        int8_t coreAffinity = -1) {
        if (function == nullptr) {
            return InvalidThreadId;
        }

#if defined(STM_HAS_MBED_RTOS)
        osThreadAttr_t attr;
        memset(&attr, 0, sizeof(attr));
        attr.name = name;
        if (stackSize > 0) {
            attr.stack_size = stackSize;
        }
        attr.priority = priority;
        (void)coreAffinity;
        return osThreadNew(function, argument, &attr);
#elif defined(STM_HAS_FREERTOS)
        TaskHandle_t handle = nullptr;
        const uint32_t defaultStackWords = 4096 / sizeof(StackType_t);
        uint32_t stackWords =
            (stackSize == 0) ? defaultStackWords : (stackSize / sizeof(StackType_t));
        if (stackWords == 0) {
            stackWords = 1;
        }

        BaseType_t result;
        if (coreAffinity >= 0 && hasMultiCoreScheduling()) {
            result = xTaskCreatePinnedToCore(
                function,
                (name != nullptr) ? name : "Thread",
                stackWords,
                argument,
                priority,
                &handle,
                coreAffinity);
        } else {
            result = xTaskCreate(
                function,
                (name != nullptr) ? name : "Thread",
                stackWords,
                argument,
                priority,
                &handle);
        }
        return (result == pdPASS) ? handle : InvalidThreadId;
#else
        (void)argument;
        (void)name;
        (void)stackSize;
        (void)priority;
        (void)coreAffinity;
        return InvalidThreadId;
#endif
    }

    static ThreadId createThreadOnCore(
        ThreadFunction function,
        void* argument,
        int8_t core,
        const char* name = nullptr,
        uint32_t stackSize = 0,
        ThreadPriority priority = defaultPriority()) {
        return createThread(function, argument, name, stackSize, priority, core);
    }

    static void endThread(ThreadId threadId) {
#if defined(STM_HAS_MBED_RTOS)
        if (threadId != InvalidThreadId) {
            osThreadTerminate(threadId);
        }
#elif defined(STM_HAS_FREERTOS)
        if (threadId != InvalidThreadId) {
            vTaskDelete(threadId);
        }
#else
        (void)threadId;
#endif
    }

    // timeoutMs = 0 means wait forever.
    static bool joinThread(ThreadId threadId, uint32_t timeoutMs = 0) {
        if (threadId == InvalidThreadId) {
            return false;
        }

#if defined(STM_HAS_MBED_RTOS)
        const uint32_t start = millis();
        while (true) {
            const osThreadState_t state = osThreadGetState(threadId);
            if (state == osThreadTerminated || state == osThreadInactive || state == osThreadError) {
                return true;
            }
            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                return false;
            }
            osThreadYield();
            delay(1);
        }
#elif defined(STM_HAS_FREERTOS)
        const uint32_t start = millis();
        while (true) {
            const eTaskState state = eTaskGetState(threadId);
            if (state == eDeleted) {
                return true;
            }
            if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
                return false;
            }
            taskYIELD();
            delay(1);
        }
#else
        (void)timeoutMs;
        return false;
#endif
    }

    static intptr_t getCurrentThreadId() {
#if defined(STM_HAS_MBED_RTOS)
        return reinterpret_cast<intptr_t>(osThreadGetId());
#elif defined(STM_HAS_FREERTOS)
        return reinterpret_cast<intptr_t>(xTaskGetCurrentTaskHandle());
#else
        return 1;
#endif
    }

    static void yield() {
#if defined(STM_HAS_MBED_RTOS)
        osThreadYield();
#elif defined(STM_HAS_FREERTOS)
        taskYIELD();
#else
        ::yield();
#endif
    }

    static bool setThreadPriority(ThreadId threadId, ThreadPriority priority) {
#if defined(STM_HAS_MBED_RTOS)
        if (threadId == InvalidThreadId) {
            return false;
        }
        return osThreadSetPriority(threadId, priority) == osOK;
#elif defined(STM_HAS_FREERTOS)
        if (threadId == InvalidThreadId) {
            return false;
        }
        vTaskPrioritySet(threadId, priority);
        return true;
#else
        (void)threadId;
        (void)priority;
        return false;
#endif
    }

    static ThreadPriority getThreadPriority(ThreadId threadId) {
#if defined(STM_HAS_MBED_RTOS)
        if (threadId == InvalidThreadId) {
            return defaultPriority();
        }
        return osThreadGetPriority(threadId);
#elif defined(STM_HAS_FREERTOS)
        if (threadId == InvalidThreadId) {
            return defaultPriority();
        }
        return uxTaskPriorityGet(threadId);
#else
        (void)threadId;
        return defaultPriority();
#endif
    }

    static void delayThread(uint32_t ms) {
#if defined(STM_HAS_MBED_RTOS)
        osDelay(ms);
#elif defined(STM_HAS_FREERTOS)
        if (ms == 0) {
            taskYIELD();
            return;
        }
        vTaskDelay(pdMS_TO_TICKS(ms));
#else
        delay(ms);
#endif
    }
};

#endif // THREADMANAGER_H
