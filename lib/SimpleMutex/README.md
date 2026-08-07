# SimpleMutex


<!-- HEALTH_BADGES_START -->
[![Health: Unsure](https://img.shields.io/badge/Health-Unsure-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Tested](https://img.shields.io/badge/Testing-Tested-2ea44f?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

SimpleMutex is a lightweight recursive mutex for Arduino projects that need safe shared-state access across tasks or cooperative loops.

## Supported targets

- ESP32 class boards.
- Arduino mbed class boards (including GIGA-class boards).
- STM32 class boards (including UNO Q and VENTUNO Q MCU-side sketch builds).

If your board is outside these targets, this library intentionally blocks compilation unless you define:

```cpp
#define SIMPLE_CONCURRENCY_ALLOW_UNSUPPORTED
```

## Header

```cpp
#include <Mutex.h>
```

## Best fit

- Shared counters, queues, and caches touched from multiple tasks.
- Protecting small critical sections that must be consistent.
- Boards that may run on Mbed RTOS, FreeRTOS, or no RTOS.

## Why this exists

- Protects critical sections on multithreaded targets.
- Supports recursive locking by the same owner.
- Keeps compatibility on non-RTOS boards with cooperative fallback behavior.

## Platform behavior

- Mbed RTOS targets: uses native recursive RTOS mutexes.
- ESP32 / FreeRTOS targets: uses native recursive FreeRTOS mutexes.
- Non-RTOS targets: uses a cooperative lock loop with interrupt-safe state updates.

## API

### Core methods

- `bool lock(unsigned long timeoutMs = 0)`
- `bool tryLock()`
- `void unlock()`

### Introspection

- `bool isLocked() const`
- `intptr_t ownerThreadId() const`

### RAII helper

- `Mutex::LockGuard guard(mutex);`
- `guard.locked()` reports whether lock acquisition succeeded.

## Timeout semantics

- `lock(0)` means wait forever.
- `lock(x)` waits up to `x` milliseconds.
- `tryLock()` never blocks.

## Usage examples

### Explicit lock/unlock

```cpp
Mutex sharedMutex;
volatile int sharedCounter = 0;

void incrementCounter() {
	if (!sharedMutex.lock(50)) {
		return;
	}
	sharedCounter++;
	sharedMutex.unlock();
}
```

### RAII lock guard

```cpp
Mutex sharedMutex;

void safeSection() {
	Mutex::LockGuard guard(sharedMutex, 100);
	if (!guard.locked()) {
		return;
	}

	// Critical section.
}
```

## Design notes

- Recursive behavior means the same owner can lock more than once.
- The lock is released only when matching unlock calls bring recursion to zero.
- Unlock from a non-owner is ignored for safety.

## Notes

- Keep critical sections short to avoid starvation.
- Avoid calling long blocking IO while holding the lock.
