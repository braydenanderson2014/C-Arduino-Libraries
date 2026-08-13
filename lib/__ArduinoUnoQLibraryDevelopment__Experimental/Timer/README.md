# Timer Class


<!-- HEALTH_BADGES_START -->
[![Health: Unsure](https://img.shields.io/badge/Health-Unsure-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Failed Testing](https://img.shields.io/badge/Testing-Failed%20Testing-d73a49?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

The `Timer` class is a C++ class for managing time-related operations and timers. It allows you to create, start, stop, reset, and pause timers, making it useful for various timing and scheduling tasks in your Arduino projects.

## Features

- Start, stop, reset, clear, pause/resume, and `restart()` timers — all **non-blocking**.
- Set target durations in milliseconds, seconds, minutes, or hours.
- Get target durations back with `getTargetDuration/Seconds/Minutes/Hours()`.
- `hasReachedTarget()` — check if the timer has expired; fires callback and auto-restarts when configured.
- `checkTimer(ms)` — one-shot elapsed check, great for periodic actions.
- `remainingTime()` — auto-scales to hours, minutes, or seconds.
- `remainingTimeMillis()` — raw milliseconds remaining.
- `printTimeRemaining()` — pretty-prints remaining time to Serial.
- `elapsedSeconds()` / `elapsedMinutes()` / `elapsedHours()` — convenience elapsed helpers.
- `lap()` — lap/split timer; returns time since last lap (or `start()`).
- `setRepeating(true)` — auto-restart mode; timer loops automatically.
- `onTargetReached(callback)` — register a callback fired when the target is reached.
- Optional RTC support — define `useRTCModule` to enable.
- Supported RTC chips: **DS3231** (default), **DS1307**, **PCF8523**, **PCF8563**, **MCP7940N** (all via RTClib), and **DS1302** (via Makuna RTC library).
- `isRTCAvailable()` — check RTC health at runtime.
- `getRTCTimeString()` — returns current RTC time as `"YYYY-MM-DD HH:MM:SS"` string.
- `printRTCTime()` — prints current RTC time to Serial.
- `setTimezone(offsetHours)` — apply a UTC offset to all RTC time reads.
- Works without any RTC; uses `millis()` by default.
- Debug output via `Timer timer(true)`.

## Usage

```cpp
#include "SimpleArduinoTimer.h"

Timer myTimer;   // debug off

void setup() {
    Serial.begin(9600);
    myTimer.setTargetMinutes(5);
    myTimer.start();
}

void loop() {
    if (myTimer.hasReachedTarget()) {
        Serial.println("Done!");
        myTimer.clear();
    }
}
```

## RTC Support

### Selecting an RTC chip

Define `useRTCModule` **and** an optional chip macro **before** the `#include`:

| Macro | Chip | Library required |
|---|---|---|
| *(none / default)* or `RTC_CHIP_DS3231` | DS3231 | Adafruit RTClib |
| `RTC_CHIP_DS1307` | DS1307 | Adafruit RTClib |
| `RTC_CHIP_PCF8523` | PCF8523 | Adafruit RTClib |
| `RTC_CHIP_PCF8563` | PCF8563 | Adafruit RTClib |
| `RTC_CHIP_MCP7940` | MCP7940N | Adafruit RTClib |
| `RTC_CHIP_DS1302` | DS1302 | Makuna RTC library |

### RTClib chips (I2C) — DS3231, DS1307, PCF8523, PCF8563, MCP7940N

Install the **Adafruit RTClib** library, then:

```cpp
#define useRTCModule
#define RTC_CHIP_PCF8523        // omit for default DS3231
#include "SimpleArduinoTimer.h"

Timer myTimer;

void setup() {
    myTimer.begin();             // initialises RTC; falls back to millis() on failure
    myTimer.setTargetMinutes(5);
    myTimer.start();
}
```

### DS1302 (3-wire SPI) — Makuna RTC library

Install the **Makuna RTC** library (`Makuna/RTC`), then define pin assignments before the include (shown here with defaults):

```cpp
#define useRTCModule
#define RTC_CHIP_DS1302
#define RTC_DS1302_IO    4      // default 4  — override as needed
#define RTC_DS1302_SCLK  5      // default 5
#define RTC_DS1302_CE    2      // default 2
#include "SimpleArduinoTimer.h"

Timer myTimer;

void setup() {
    myTimer.begin();
    myTimer.setTargetMinutes(5);
    myTimer.start();
}
```

### getRTCTime() — TimerDateTime struct

`getRTCTime()` returns a chip-agnostic `TimerDateTime` struct (replaces the
RTClib-specific `DateTime` from earlier versions):

```cpp
TimerDateTime now = myTimer.getRTCTime();
Serial.print(now.year);   Serial.print('-');
Serial.print(now.month);  Serial.print('-');
Serial.println(now.day);
```

Fields: `year` (uint16_t), `month`, `day`, `hour`, `minute`, `second` (all uint8_t).

### getRTCTimeString() — formatted string

Returns the current RTC time as a pre-formatted `String` — no manual field unpacking needed:

```cpp
Serial.println(myTimer.getRTCTimeString()); // "2026-08-03 19:47:22"
```

### printRTCTime() — Serial helper

Prints the current RTC date+time to Serial in the same format:

```cpp
myTimer.printRTCTime(); // [MyTimer] RTC: 2026-08-03 19:47:22
```

### isRTCAvailable() — health check

Check whether the RTC was successfully initialised (without relying on debug output):

```cpp
if (!myTimer.isRTCAvailable()) {
    Serial.println("Warning: RTC not found, using millis()");
}
```

### setTimezone(offsetHours) — UTC offset

Apply a signed hour offset to all time reads from `getRTCTime()` and `getRTCTimeString()`:

```cpp
myTimer.setTimezone(-5);  // UTC-5 (Eastern Standard Time)
```

## QOL Features

### restart()

`clear()` + `start()` in a single call — the most common pattern in `loop()`:

```cpp
myTimer.restart();  // instead of: myTimer.clear(); myTimer.start();
```

### setRepeating(bool) — auto-restart mode

When enabled the timer restarts automatically from zero every time the target is reached.
`hasReachedTarget()` still returns `true` on each expiry before the restart:

```cpp
myTimer.setTargetSeconds(30);
myTimer.setRepeating(true);
myTimer.start();

void loop() {
    if (myTimer.hasReachedTarget()) {
        // runs every 30 s — timer auto-restarts, no state machine needed
        doWork();
    }
}
```

### onTargetReached(callback) — callback support

Register a function that is called automatically inside `hasReachedTarget()` whenever the timer expires:

```cpp
void flashLED() { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); }

myTimer.setTargetSeconds(1);
myTimer.setRepeating(true);
myTimer.onTargetReached(flashLED);
myTimer.start();

void loop() {
    myTimer.hasReachedTarget(); // triggers callback + auto-repeat when repeating is enabled
}
```

### lap() — lap/split timer

Returns milliseconds since the last `lap()` call (or `start()` if no prior lap) and resets the lap anchor:

```cpp
myTimer.start();
doStageOne();
unsigned long stage1Ms = myTimer.lap();  // time for stage 1
doStageTwo();
unsigned long stage2Ms = myTimer.lap();  // time for stage 2
```

### elapsedSeconds() / elapsedMinutes() / elapsedHours()

Convenience wrappers — no more manual division:

```cpp
Serial.println(myTimer.elapsedSeconds());  // instead of elapsed() / 1000UL
```

### getTargetDuration() / getTargetSeconds() / getTargetMinutes() / getTargetHours()

Getter counterparts for the existing setters:

```cpp
Serial.println("Target: " + String(myTimer.getTargetSeconds()) + "s");
```

## Multithreading (opt-in)

By default the Timer compiles exactly as before: no locking, no threads, no
extra dependencies. Two compile-time macros enable concurrency support on
UNO Q / GIGA (mbed) / ESP32 class targets:

| Macro | Effect | Requires |
| --- | --- | --- |
| `TIMER_THREAD_SAFE` | Every public method is guarded by a recursive `Mutex`, so one Timer instance can be shared by several threads. | `SimpleMutex` |
| `TIMER_LOCK_BACKEND_RWLOCK` | Uses `SimpleRWLock` instead of `Mutex`: read-only methods (`elapsed()`, `remainingTimeMillis()`, `isTimerRunning()`, the getters...) take a shared read lock so several threads can poll concurrently, while mutators take an exclusive write lock. Implies `TIMER_THREAD_SAFE`. | `SimpleRWLock` |
| `TIMER_ENABLE_THREADING` | Implies `TIMER_THREAD_SAFE` and adds `startMonitor()` / `stopMonitor()`, which poll `hasReachedTarget()` on a background thread. | `SimpleMutex` (or `SimpleRWLock`), `SimpleThreadManager` |

Both lock backends are reentrant, so nested internal calls (for example
`hasReachedTarget()` → `elapsed()` → `restart()`) never self-deadlock.

Optional tuning macros: `TIMER_MONITOR_INTERVAL_MS` (default `10`) and
`TIMER_MONITOR_STACK_SIZE` (default `2048`).

```ini
; platformio.ini
build_flags =
    -D TIMER_ENABLE_THREADING
    -D TIMER_LOCK_BACKEND_RWLOCK   ; optional: read-heavy workloads
lib_deps =
    SimpleRWLock                   ; or SimpleMutex for the default backend
    SimpleThreadManager
```

```cpp
#include <SimpleArduinoTimer.h>

Timer myTimer;

void onDone() { Serial.println("Target reached"); }

void setup() {
    myTimer.onTargetReached(onDone);
    myTimer.setRepeating(true);
    myTimer.setTargetSeconds(5);
    myTimer.start();

#ifdef TIMER_ENABLE_THREADING
    myTimer.startMonitor();   // callback now fires without loop() polling
#endif
}
```

Additional API when the macros are enabled:

- `Timer::isThreadSafe()` — always available; reports how the library was built.
- `lock(timeoutMs)` / `unlock()` — exclusive; group several calls into one atomic block.
- `lockShared(timeoutMs)` / `unlockShared()` — shared read acquisition on the
  RWLock backend, identical to `lock()` / `unlock()` on the Mutex backend.
- `startMonitor(pollIntervalMs)` / `stopMonitor(timeoutMs)` / `isMonitorRunning()`.

`startMonitor()` returns `false` when the target has no RTOS threading, so the
sketch can fall back to polling `hasReachedTarget()` from `loop()`. The target
callback runs while the Timer lock is held; calling back into the same Timer is
safe (the mutex is recursive), but avoid blocking on other locks inside it.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries.git"
```

## Header

```cpp
#include <SimpleArduinoTimer.h>
```

# Arduino
## Changelog
### Version 1.0.0:
* Initial Untested Release
### Version 1.0.1 BETA:
* Tested, Fixed Version
* Fixes All known bugs at this time. This library has had a few tests now performed and is now working for basic timer functionality.
* Added the ability to name a timer. Though strictly not necessary, this allows more creative function.
* Added setTargetSeconds() so you can set a timer in seconds.
* Added printTimeRemaining() so you don't have to create your own interface if you don't want to.
* New setTimerName and getTimerName functions.
* Fixed the remainingTime function. [WARNING]: This function behaves differently than you might expect. It will auto-adjust the unit of measurement being used without letting you know. This is a feature and not a bug.
* Moved RTC functionality into conditional. You need to define `#define useRTCModule` in order to use the RTC functions.
* Adjusted serial statements to now say the timer's name instead of the timer class name.
### Version 1.2.0
* **New**: `restart()` — `clear()` + `start()` in a single call; the most common `loop()` pattern.
* **New**: `setRepeating(bool)` / `getRepeating()` — auto-restart mode; the timer reloads from zero every time the target is reached.
* **New**: `onTargetReached(callback)` — register a `void (*)()` function pointer that fires automatically inside `hasReachedTarget()` on expiry.
* **New**: `lap()` — lap/split timer; returns milliseconds since the last `lap()` call (or `start()`).
* **New**: `elapsedSeconds()` / `elapsedMinutes()` / `elapsedHours()` — convenience wrappers around `elapsed()` in named units.
* **New**: `getTargetDuration()` / `getTargetSeconds()` / `getTargetMinutes()` / `getTargetHours()` — getter counterparts for the existing duration setters.
* **New (RTC)**: `isRTCAvailable()` — exposes `rtcInitialized` so code can check RTC health without debug output.
* **New (RTC)**: `getRTCTimeString()` — returns the current RTC date+time as a pre-formatted `String` (`"YYYY-MM-DD HH:MM:SS"`).
* **New (RTC)**: `printRTCTime()` — prints the current RTC date+time to Serial.
* **New (RTC)**: `setTimezone(int offsetHours)` — apply a signed UTC hour offset to all `getRTCTime()` / `getRTCTimeString()` reads.
* * **New**: Multi-RTC support — the library now supports DS3231, DS1307, PCF8523, PCF8563, and MCP7940N via Adafruit RTClib, plus DS1302 via the Makuna RTC library.
* **New**: Chip-selection macros — define one of `RTC_CHIP_DS3231` (default), `RTC_CHIP_DS1307`, `RTC_CHIP_PCF8523`, `RTC_CHIP_PCF8563`, `RTC_CHIP_MCP7940`, or `RTC_CHIP_DS1302` before including the header.
* **New**: DS1302 pin macros — `RTC_DS1302_IO` (default 4), `RTC_DS1302_SCLK` (default 5), `RTC_DS1302_CE` (default 2); override before the include.
* **New**: `TimerDateTime` struct — chip-agnostic date/time type returned by `getRTCTime()`, replacing the RTClib-specific `DateTime` type. Fields: `year`, `month`, `day`, `hour`, `minute`, `second`.
* **Refactor**: Private chip-abstraction helpers (`_rtcBeginImpl`, `_rtcNowMs`, `_rtcAdjustImpl`, `_rtcNowDateTime`) isolate chip-specific API differences; all public methods now call these helpers instead of accessing `rtc` directly.
* **Breaking change**: `getRTCTime()` now returns `TimerDateTime` instead of RTClib's `DateTime`. Update any code that assigned the result to a `DateTime` variable.

# Platformio
## ChangeLog
### Version 1.0.0:
* Initial Release
### Version 1.0.1:
* Update to Library.json
### Version 1.0.2:
* Added Debug Statements
### Version 1.0.3:
* Modified constructor to take a `bool` to allow setting whether the library displays debug messages.
### Version 1.0.4:
* Renamed Header File.
* Added new functions: setTargetHours(), setUseRTC(), getUseRTC(), begin(). Note: unless you call begin(), the RTC will not function. begin() will try to start the RTC; if it fails it will remain using millis() instead.
* Modified some functions to swap between millis() and the RTC functions based on the useRTC boolean.
### Version 1.0.5:
* Fixes All known bugs. This library has had a few tests performed and is now working for basic timer functionality.
* Added the ability to name a timer.
* Added setTargetSeconds() so you can set a timer in seconds.
* Added printTimeRemaining() so you don't have to create your own interface if you don't want to.
* New setTimerName and getTimerName functions.
* Fixed the remainingTime function.
* Moved RTC functionality into conditional.
### Version 1.0.6:
* Adjusted serial statements to now say the timer's name instead of the timer class name.
* Fixed Documentation.
### Version 1.0.7:
* **Bug fix**: `#include <RTClib.h>` in the header is now conditional on `#define useRTCModule` — the library compiles without RTClib installed.
* **Bug fix**: Constructor now correctly stores the `debug` parameter; all debug output now works as expected.
* **Bug fix**: `begin()` now correctly falls back to `millis()` when the RTC fails to initialise (missing `else` branch).
* **Bug fix**: Removed erroneous global `timerRTC`/`timerNow` variables; all RTC calls now use the class member `rtc`.
* **Bug fix**: `syncWithRTC()` no longer corrupts `elapsedTime` with an absolute unix timestamp; it now properly re-anchors the start reference.
* **Bug fix**: `stop()` RTC path now accumulates a delta instead of writing an absolute timestamp to `elapsedTime`.
* **Bug fix**: `remainingTime()`, `remainingTimeMillis()`, and `updateRemainingTime()` now guard against unsigned underflow when the timer has exceeded its target — they return 0 instead of wrapping to a huge value.
* **Bug fix**: `remainingTime()` no longer shadows the class member `remainingTimeOnTimer` with a local variable.
* **Bug fix**: `printTimeRemaining()` now calls `updateRemainingTime()` first so `timerMode` is always up to date.
* **Bug fix**: `hasReachedTarget()` now returns `false` when no target has been set (`targetDuration == 0`) instead of immediately returning `true`.
* **Bug fix**: `timerMode` and `remainingTimeOnTimer` are now properly initialised in the constructor.
* **Bug fix**: `setUseRTC()` and `getUseRTC()` are now declared and compiled only when `useRTCModule` is defined, eliminating the header/cpp guard mismatch.
* **New**: `remainingTimeMillis()` — always returns the exact number of milliseconds remaining, regardless of the auto-scaling done by `remainingTime()`.
* **Improvement**: Unsigned-long literals use `UL` suffix to prevent overflow on 32-bit targets when multiplying hours/minutes/seconds.
* **Example**: Fixed `Timer timer = new Timer(false)` (invalid C++) to `Timer timer(false)`.
* **Example**: Fixed runs that called `timer.reset()` while the timer was still running (silently did nothing); now uses `timer.clear()` between runs.
* **Metadata**: Removed hard `RTClib` dependency from library.json and library.properties since RTC support is optional.
### Version 1.2.0:
* **New**: `restart()` — `clear()` + `start()` in a single call; the most common `loop()` pattern.
* **New**: `setRepeating(bool)` / `getRepeating()` — auto-restart mode; the timer reloads from zero every time the target is reached.
* **New**: `onTargetReached(callback)` — register a `void (*)()` function pointer that fires automatically inside `hasReachedTarget()` on expiry.
* **New**: `lap()` — lap/split timer; returns milliseconds since the last `lap()` call (or `start()`).
* **New**: `elapsedSeconds()` / `elapsedMinutes()` / `elapsedHours()` — convenience wrappers around `elapsed()` in named units.
* **New**: `getTargetDuration()` / `getTargetSeconds()` / `getTargetMinutes()` / `getTargetHours()` — getter counterparts for the existing duration setters.
* **New (RTC)**: `isRTCAvailable()` — exposes `rtcInitialized` so code can check RTC health without debug output.
* **New (RTC)**: `getRTCTimeString()` — returns the current RTC date+time as a pre-formatted `String` (`"YYYY-MM-DD HH:MM:SS"`).
* **New (RTC)**: `printRTCTime()` — prints the current RTC date+time to Serial.
* **New (RTC)**: `setTimezone(int offsetHours)` — apply a signed UTC hour offset to all `getRTCTime()` / `getRTCTimeString()` reads.
* * **New**: Multi-RTC support — the library now supports DS3231, DS1307, PCF8523, PCF8563, and MCP7940N via Adafruit RTClib, plus DS1302 via the Makuna RTC library.
* **New**: Chip-selection macros — define one of `RTC_CHIP_DS3231` (default), `RTC_CHIP_DS1307`, `RTC_CHIP_PCF8523`, `RTC_CHIP_PCF8563`, `RTC_CHIP_MCP7940`, or `RTC_CHIP_DS1302` before including the header.
* **New**: DS1302 pin macros — `RTC_DS1302_IO` (default 4), `RTC_DS1302_SCLK` (default 5), `RTC_DS1302_CE` (default 2); override before the include.
* **New**: `TimerDateTime` struct — chip-agnostic date/time type returned by `getRTCTime()`, replacing the RTClib-specific `DateTime` type. Fields: `year`, `month`, `day`, `hour`, `minute`, `second`.
* **Refactor**: Private chip-abstraction helpers (`_rtcBeginImpl`, `_rtcNowMs`, `_rtcAdjustImpl`, `_rtcNowDateTime`) isolate chip-specific API differences; all public methods now call these helpers instead of accessing `rtc` directly.
* **Breaking change**: `getRTCTime()` now returns `TimerDateTime` instead of RTClib's `DateTime`. Update any code that assigned the result to a `DateTime` variable.
