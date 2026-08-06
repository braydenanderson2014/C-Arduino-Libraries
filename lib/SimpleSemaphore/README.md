# SimpleSemaphore


<!-- HEALTH_BADGES_START -->
[![Health: Unknown](https://img.shields.io/badge/Health-Unknown-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Unmanaged](https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

SimpleSemaphore is a counting semaphore library for Arduino projects that need task coordination.

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
#include <SimpleSemaphore.h>
```

## Best fit

- Controlling access to a finite resource pool.
- Limiting concurrent work units.
- Signaling work readiness between producer/consumer tasks.

## Why this exists

- Limits access to a finite resource pool.
- Coordinates producer and consumer task handoff.
- Works with Mbed RTOS, ESP32 FreeRTOS, and non-RTOS targets.

## Platform behavior

- Mbed RTOS targets: uses CMSIS semaphore primitives.
- ESP32 / FreeRTOS targets: uses FreeRTOS counting semaphores.
- Non-RTOS targets: uses cooperative fallback with interrupt-safe count updates.

## API

- `SimpleSemaphore(uint16_t maxCount, uint16_t initialCount = 0)`
- `bool acquire(unsigned long timeoutMs = 0)`
- `bool tryAcquire()`
- `bool release(uint16_t amount = 1)`
- `uint16_t available() const`
- `uint16_t maxCount() const`
- `bool isRuntimeBacked() const`

## Timeout semantics

- `acquire(0)` means wait forever.
- `acquire(x)` waits up to `x` milliseconds.
- `tryAcquire()` is immediate and non-blocking.

## Usage examples

### Limit concurrent workers

```cpp
SimpleSemaphore slots(2, 2); // At most two workers at once.

void runJob() {
	if (!slots.acquire(100)) {
		return;
	}

	// Protected resource usage.

	slots.release();
}
```

### Producer signals consumer

```cpp
SimpleSemaphore ready(16, 0);

void producerWorkReady() {
	ready.release();
}

bool waitForWork() {
	return ready.acquire(200);
}
```

## Design notes

- `release(amount)` fails if it would exceed `maxCount` in fallback mode.
- On RTOS targets, semaphore saturation behavior follows backend limits.

## Notes

- Keep semaphore counts small and intentional to simplify debugging.
