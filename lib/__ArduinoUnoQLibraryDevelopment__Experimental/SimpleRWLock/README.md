# SimpleRWLock


<!-- HEALTH_BADGES_START -->
[![Health: Unknown](https://img.shields.io/badge/Health-Unknown-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Unmanaged](https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

SimpleRWLock is a lightweight reader-writer lock.

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
#include <SimpleRWLock.h>
```

## Best fit

- Config/state objects read frequently but updated occasionally.
- Concurrent read-heavy algorithms where writer exclusion is required.
- Thread-safe access patterns without forcing full serialization for readers.

## Why this exists

- Lets many readers access shared data at once.
- Keeps writes exclusive.
- Fits cache/state-heavy data structures where reads dominate writes.

## Platform behavior

- Mbed RTOS targets: state protection uses CMSIS mutex backing.
- ESP32 / FreeRTOS targets: state protection uses FreeRTOS mutex backing.
- Non-RTOS targets: cooperative fallback guarded by interrupt control.

## Fairness behavior

- New readers pause when writers are waiting.
- This helps avoid writer starvation in read-heavy workloads.
- A thread that already holds a read lock is exempt: it re-enters immediately
  instead of waiting on a queued writer (which would be waiting on itself).

## Reentrancy

The lock is reentrant per thread:

- `writeLock()` from the thread that already owns the write lock increments a
  recursion count; the lock is released when the matching number of
  `writeUnlock()` calls have been made.
- `readLock()` from a thread that already holds a read lock re-enters.
- `readLock()` from the current writer succeeds and is released by the matching
  `readUnlock()`.
- `writeLock()` from a thread that only holds a read lock returns `false`
  immediately rather than deadlocking; a read-to-write upgrade is not supported.

Read reentrancy is tracked in a fixed-size table (`SRWL_MAX_TRACKED_READERS`,
default `8`). Readers beyond that limit still acquire the lock normally, they
just cannot re-enter.

## API

- `bool readLock(unsigned long timeoutMs = 0)`
- `void readUnlock()`
- `bool writeLock(unsigned long timeoutMs = 0)`
- `void writeUnlock()`
- `bool holdsReadLock() const`
- `bool holdsWriteLock() const`
- `uint16_t activeReaders() const`
- `bool writerActive() const`
- `intptr_t writerThreadId() const`
- `SimpleRWLock::ReadGuard` / `SimpleRWLock::WriteGuard` — scoped RAII guards
  with a `locked()` accessor.

## Timeout semantics

- `readLock(0)` and `writeLock(0)` mean wait forever.
- Passing a timeout enforces bounded wait behavior.

## Usage examples

### Shared config map pattern

```cpp
SimpleRWLock rwLock;
int sharedValue = 0;

int readValue() {
	if (!rwLock.readLock(50)) {
		return -1;
	}
	const int value = sharedValue;
	rwLock.readUnlock();
	return value;
}

bool writeValue(int v) {
	if (!rwLock.writeLock(50)) {
		return false;
	}
	sharedValue = v;
	rwLock.writeUnlock();
	return true;
}
```

### Scoped guards

```cpp
int readValueGuarded() {
	SimpleRWLock::ReadGuard guard(rwLock, 50);
	if (!guard.locked()) {
		return -1;
	}
	return sharedValue;
}
```

## Design notes

- Always match each successful lock with the corresponding unlock.
- Keep write sections short to reduce read blocking.
- Recursive acquisition is safe; read-to-write upgrades still are not, and fail
  fast so the caller can restructure instead of hanging.

## Notes

- On non-RTOS targets, lock behavior is cooperative rather than preemptive.
