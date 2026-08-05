# SimpleCoreRuntime

SimpleCoreRuntime is a capability-aware runtime wrapper for boards that can support multicore and/or threaded execution.

## Supported targets

- ESP32 class boards (`esp32` architecture).
- Arduino mbed class boards (`mbed` architecture), including GIGA-class targets.
- STM32 class targets (`stm32` architecture), including UNO Q and VENTUNO Q MCU-side sketch builds.

Q-family hybrid note:
- UNO Q and VENTUNO Q are hybrid Linux + MCU boards.
- This library is for the Arduino MCU-side sketch environment.
- Linux-side multiprocessing and high-level orchestration are handled in Debian/Ubuntu user space and are outside this Arduino library.

If your board is outside these targets, this library intentionally blocks compilation unless you define:

```cpp
#define SIMPLE_CORE_RUNTIME_ALLOW_UNSUPPORTED
```

## Header

```cpp
#include <SimpleCoreRuntime.h>
```

## Why this exists

- Gives one API surface for runtime capability checks.
- Exposes task launch and optional core affinity where supported.
- Keeps code portable across supported architecture families.

## API highlights

- `SimpleCoreRuntime::info()`
- `SimpleCoreRuntime::hasThreading()`
- `SimpleCoreRuntime::hasMultiCore()`
- `SimpleCoreRuntime::supportsCoreAffinity()`
- `SimpleCoreRuntime::processorCount()`
- `SimpleCoreRuntime::currentCore()`
- `SimpleCoreRuntime::launchTask(...)`
- `SimpleCoreRuntime::launchTaskOnCore(...)`
- `SimpleCoreRuntime::waitForTask(...)`
- `SimpleCoreRuntime::stopTask(...)`
- `SimpleCoreRuntime::yieldNow()`
- `SimpleCoreRuntime::delayMs(...)`

## Example

```cpp
#include <SimpleCoreRuntime.h>

void worker(void* arg) {
  (void)arg;
  while (true) {
    // Do work.
    SimpleCoreRuntime::delayMs(10);
  }
}

void setup() {
  const auto rt = SimpleCoreRuntime::info();
  if (!rt.threading) {
    return;
  }

  if (rt.coreAffinity) {
    (void)SimpleCoreRuntime::launchTaskOnCore(worker, nullptr, 1, "worker-core1", 4096);
  } else {
    (void)SimpleCoreRuntime::launchTask(worker, nullptr, "worker", 4096);
  }
}
```

## Notes

- Core affinity is currently exposed only on ESP32-class FreeRTOS targets.
- On mbed/STM32 families, tasking support depends on the board core implementation.
- This library depends on SimpleThreadManager.
