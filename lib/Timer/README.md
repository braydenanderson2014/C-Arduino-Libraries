# Timer Class

The `Timer` class is a C++ class for managing time-related operations and timers. It allows you to create, start, stop, reset, and pause timers, making it useful for various timing and scheduling tasks in your Arduino projects.

## Features

- Start, stop, reset, clear, and pause/resume timers — all **non-blocking**.
- Set target durations in milliseconds, seconds, minutes, or hours.
- `hasReachedTarget()` — check if the timer has expired.
- `checkTimer(ms)` — one-shot elapsed check, great for periodic actions.
- `remainingTime()` — auto-scales to hours, minutes, or seconds.
- `remainingTimeMillis()` — raw milliseconds remaining.
- `printTimeRemaining()` — pretty-prints remaining time to Serial.
- Optional RTC support — define `useRTCModule` to enable.
- Supported RTC chips: **DS3231** (default), **DS1307**, **PCF8523**, **PCF8563**, **MCP7940N** (all via RTClib), and **DS1302** (via Makuna RTC library).
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
### Version 1.1.0:
* **New**: Multi-RTC support — the library now supports DS3231, DS1307, PCF8523, PCF8563, and MCP7940N via Adafruit RTClib, plus DS1302 via the Makuna RTC library.
* **New**: Chip-selection macros — define one of `RTC_CHIP_DS3231` (default), `RTC_CHIP_DS1307`, `RTC_CHIP_PCF8523`, `RTC_CHIP_PCF8563`, `RTC_CHIP_MCP7940`, or `RTC_CHIP_DS1302` before including the header.
* **New**: DS1302 pin macros — `RTC_DS1302_IO` (default 4), `RTC_DS1302_SCLK` (default 5), `RTC_DS1302_CE` (default 2); override before the include.
* **New**: `TimerDateTime` struct — chip-agnostic date/time type returned by `getRTCTime()`, replacing the RTClib-specific `DateTime` type. Fields: `year`, `month`, `day`, `hour`, `minute`, `second`.
* **Refactor**: Private chip-abstraction helpers (`_rtcBeginImpl`, `_rtcNowMs`, `_rtcAdjustImpl`, `_rtcNowDateTime`) isolate chip-specific API differences; all public methods now call these helpers instead of accessing `rtc` directly.
* **Breaking change**: `getRTCTime()` now returns `TimerDateTime` instead of RTClib's `DateTime`. Update any code that assigned the result to a `DateTime` variable.