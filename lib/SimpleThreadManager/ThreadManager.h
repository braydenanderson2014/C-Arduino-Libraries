#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <Arduino.h>
#include <mbed.h>

class ThreadManager {
public:
    static osThreadId_t createThread(void (*function)(void *argument), void *argument = nullptr) {
        // Create a new thread and start it
        osThreadId_t threadId = osThreadNew(function, argument, nullptr);
        return threadId; // Return the thread ID
    }

    static void endThread(osThreadId_t threadId) {
        // Terminate the specified thread
        osThreadTerminate(threadId);
    }

    static void joinThread(int threadId) {
        // Wait for a specific thread to finish.
        // This may involve polling or a more complex signaling mechanism.
        
    }

    static int getCurrentThreadId() {
        // Return the ID of the currently running thread.
        // This function will depend on how you're identifying active threads.
        return osThreadGetId();
    }

    static void yield() {
        // Allow the current thread to yield execution to other threads.
        // This is particularly useful in cooperative multitasking scenarios.
    }

    static void setThreadPriority(int threadId, int priority) {
        // Set the priority of a given thread.
        // Managing priorities requires a scheduler capable of priority-based scheduling.
    }

    static int getThreadPriority(int threadId) {
        // Get the priority of a given thread.
    }

    static void delayThread(int ms) {
        // Delay a thread's execution for a specified number of milliseconds.
    }

    // Add more thread management and utility functions as needed...

private:
    static const int MAX_THREADS = 10;
    static int _nextThreadId;
    static int _threadCount;
    // You might need a data structure to keep track of threads and their states.
};

int ThreadManager::_nextThreadId = 0;
int ThreadManager::_threadCount = 0;

#endif // THREADMANAGER_H
