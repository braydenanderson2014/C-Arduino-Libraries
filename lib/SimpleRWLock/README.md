# SimpleRWLock

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

## API

- `bool readLock(unsigned long timeoutMs = 0)`
- `void readUnlock()`
- `bool writeLock(unsigned long timeoutMs = 0)`
- `void writeUnlock()`
- `uint16_t activeReaders() const`
- `bool writerActive() const`

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

## Design notes

- Always match each successful lock with the corresponding unlock.
- Keep write sections short to reduce read blocking.
- Avoid lock upgrades (read-to-write) within one flow unless explicitly designed.

## Notes

- On non-RTOS targets, lock behavior is cooperative rather than preemptive.
