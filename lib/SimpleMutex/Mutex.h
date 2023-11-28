#ifndef MUTEX_H
#define MUTEX_H

#include <Arduino.h>
//#include <ThreadManager.h>
#include "ThreadManager.h"
class Mutex {
public:
    Mutex() : _locked(false), _ownerThreadId(-1) {}

    bool lock(unsigned long timeout = 0) {
        unsigned long startTime = millis();
        while (true) {
            noInterrupts();
            if (!_locked || _ownerThreadId == currentThreadId()) {
                _locked = true;
                _ownerThreadId = currentThreadId();
                interrupts();
                return true;
            }
            interrupts();

            if (timeout > 0 && (millis() - startTime >= timeout)) {
                return false; // Timeout
            }

            // Optionally, yield to other threads or introduce a delay
            delay(1);
        }
    }

    void unlock() {
        if (currentThreadId() == _ownerThreadId) {
            noInterrupts();
            _locked = false;
            _ownerThreadId = -1;
            interrupts();
        }
    }

    bool tryLock() {
        noInterrupts();
        if (!_locked || _ownerThreadId == currentThreadId()) {
            _locked = true;
            _ownerThreadId = currentThreadId();
            interrupts();
            return true;
        }
        interrupts();
        return false;
    }

private:
    // ... [Other parts of the Mutex class] ...

private:
    volatile bool _locked;
    volatile int _ownerThreadId; // Thread ID of the current owner

    int currentThreadId() {
        // Here, integrate with your thread manager to get the current thread ID
        // For example:
        return ThreadManager::getCurrentThreadId();
    }
};

// ... [Rest of the Mutex class] ...



#endif // MUTEX_H
