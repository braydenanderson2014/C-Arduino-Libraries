# SimpleThreadManager

SimpleThreadManager is a cross-platform thread/task wrapper for Arduino-friendly environments.

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
#include <ThreadManager.h>
```

## Best fit

- Portable task creation across Mbed and FreeRTOS boards.
- Optional core pinning on multicore-capable targets.
- A single API surface for demos, examples, and production sketches.

## Why this exists

- Gives one API for Mbed RTOS and FreeRTOS targets.
- Exposes multicore-aware controls where available.
- Provides predictable fallback behavior for non-threaded boards.

## Platform behavior

- Mbed RTOS targets: backed by CMSIS-RTOS2 thread APIs.
- ESP32 / FreeRTOS targets: backed by FreeRTOS task APIs, with optional core pinning.
- Non-RTOS targets: thread creation is unavailable and returns invalid IDs; utility methods remain safe.

## Capabilities

- `hasThreading()` tells you if runtime thread creation is supported.
- `hasMultiCoreScheduling()` reports if tasks can be spread across cores.
- `processorCount()` and `currentCore()` expose runtime core info when available.

## API highlights

### Runtime capability checks

- `hasThreading()`
- `hasMultiCoreScheduling()`
- `processorCount()`
- `currentCore()`

### Thread lifecycle

- `createThread(function, argument, name, stackSize, priority, coreAffinity)`
- `createThreadOnCore(function, argument, core, name, stackSize, priority)`
- `endThread(threadId)`
- `joinThread(threadId, timeoutMs)`

### Utilities

- `getCurrentThreadId()`
- `yield()`
- `setThreadPriority(threadId, priority)`
- `getThreadPriority(threadId)`
- `delayThread(ms)`

## Usage examples

### Basic task creation

```cpp
#include <ThreadManager.h>

void worker(void* arg) {
	(void)arg;
	while (true) {
		// Do work.
		ThreadManager::delayThread(10);
	}
}

void setup() {
	if (!ThreadManager::hasThreading()) {
		return;
	}

	auto tid = ThreadManager::createThread(worker, nullptr, "worker", 4096);
	(void)tid;
}
```

### Optional multicore pinning

```cpp
if (ThreadManager::hasThreading() && ThreadManager::hasMultiCoreScheduling()) {
	auto tid = ThreadManager::createThreadOnCore(worker, nullptr, 1, "core1-worker", 4096);
	(void)tid;
}
```

## Design notes

- Stack size is passed in bytes and adapted per backend.
- `joinThread` uses backend-friendly polling to stay portable.
- On non-threaded targets, creation returns `InvalidThreadId` rather than crashing.

## Notes

- `timeoutMs = 0` in `joinThread` means wait forever.
- `createThreadOnCore` only pins when multicore scheduling is available.
- Keep thread stack sizes conservative on memory-limited boards.
