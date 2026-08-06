# SimpleChannel


<!-- HEALTH_BADGES_START -->
[![Health: Unknown](https://img.shields.io/badge/Health-Unknown-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Unmanaged](https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

SimpleChannel is a fixed-capacity queue for message passing between tasks.

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
#include <SimpleChannel.h>
```

## Why this exists

- Gives you bounded producer-consumer communication.
- Avoids unbounded dynamic allocation in task handoff paths.
- Works on RTOS-backed targets and simpler cooperative targets.

## API

- `bool send(const T& item, unsigned long timeoutMs = 0)`
- `bool trySend(const T& item)`
- `bool receive(T& out, unsigned long timeoutMs = 0)`
- `bool tryReceive(T& out)`
- `size_t size() const`
- `static constexpr size_t capacity()`
- `bool empty() const`
- `bool full() const`

## Notes

`timeoutMs = 0` means wait forever on RTOS targets.

Example usage:

```cpp
SimpleChannel<int, 16> channel;
channel.send(42);
int value = 0;
if (channel.receive(value, 100)) {
  // Use value.
}
```
