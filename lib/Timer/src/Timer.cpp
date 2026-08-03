#include "SimpleArduinoTimer.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
Timer::Timer(bool debug)
    : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0),
      isRunning(false), isPaused(false), useRTC(false), rtcInitialized(false),
      debug(debug), remainingTimeOnTimer(0), timerMode(Seconds)
#if defined(useRTCModule) && defined(RTC_CHIP_DS1302)
    , _ds1302Wire(RTC_DS1302_IO, RTC_DS1302_SCLK, RTC_DS1302_CE)
    , rtc(_ds1302Wire)
#endif
{
    setTimerName("Timer");
}

// ---------------------------------------------------------------------------
// Name helpers
// ---------------------------------------------------------------------------
void Timer::setTimerName(String timerName) {
    this->TimerName = timerName;
}

String Timer::getTimerName() const {
    return TimerName;
}

// ---------------------------------------------------------------------------
// Private chip-abstraction helpers (compiled only when useRTCModule is defined)
// ---------------------------------------------------------------------------
#ifdef useRTCModule

// Returns true when the RTC was successfully initialised.
bool Timer::_rtcBeginImpl() {
#ifdef RTC_CHIP_DS1302
    // DS1302 has no I2C ACK mechanism; Begin() always succeeds if wired correctly.
    rtc.Begin();
    return true;
#else
    return rtc.begin();
#endif
}

// Returns the current RTC time as milliseconds since the Unix epoch.
unsigned long Timer::_rtcNowMs() const {
#ifdef RTC_CHIP_DS1302
    return (unsigned long)(rtc.GetDateTime().Unix32Time()) * 1000UL;
#else
    return (unsigned long)(rtc.now().unixtime()) * 1000UL;
#endif
}

// Adjusts the RTC to the specified date/time.
void Timer::_rtcAdjustImpl(int year, int month, int day,
                             int hour, int minute, int second) {
#ifdef RTC_CHIP_DS1302
    rtc.SetDateTime(RtcDateTime((uint16_t)year,  (uint8_t)month,
                                 (uint8_t)day,    (uint8_t)hour,
                                 (uint8_t)minute, (uint8_t)second));
#else
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
#endif
}

// Returns the current RTC time as a chip-agnostic TimerDateTime struct.
TimerDateTime Timer::_rtcNowDateTime() const {
    TimerDateTime dt;
#ifdef RTC_CHIP_DS1302
    RtcDateTime now = rtc.GetDateTime();
    dt.year   = now.Year();
    dt.month  = now.Month();
    dt.day    = now.Day();
    dt.hour   = now.Hour();
    dt.minute = now.Minute();
    dt.second = now.Second();
#else
    DateTime now = rtc.now();
    dt.year   = now.year();
    dt.month  = now.month();
    dt.day    = now.day();
    dt.hour   = now.hour();
    dt.minute = now.minute();
    dt.second = now.second();
#endif
    return dt;
}

// ---------------------------------------------------------------------------
// Public RTC helpers
// ---------------------------------------------------------------------------
void Timer::setUseRTC(bool useRTC) {
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting useRTC: " + String(useRTC));
    }
    if (useRTC) {
        // Initialise the RTC only if it has not been successfully initialised yet.
        if (!rtcInitialized) {
            if (_rtcBeginImpl()) {
                rtcInitialized = true;
            } else {
                rtcInitialized = false;
            }
        }
        if (rtcInitialized) {
            this->useRTC = true;
        } else {
            this->useRTC = false;
            if (debug) {
                Serial.println("[" + TimerName + "]: RTC not found - cannot enable RTC mode");
            }
        }
    } else {
        this->useRTC = false;
    }
}

bool Timer::getUseRTC() const {
    return useRTC;
}

void Timer::syncWithRTC() {
    if (!useRTC) {
        if (debug) {
            Serial.println("[" + TimerName + "]: syncWithRTC() ignored - useRTC is false");
        }
        return;
    }
    // Re-anchor the start reference so that an external RTC adjustment does
    // not cause a time-jump.  Accumulated elapsed time is preserved.
    if (isRunning) {
        unsigned long now = _rtcNowMs();
        elapsedTime += now - startTime;
        startTime = now;
        if (debug) {
            Serial.println("[" + TimerName + "]: Synced with RTC");
        }
    }
}

TimerDateTime Timer::getRTCTime() const {
    if (!rtcInitialized) {
        if (debug) {
            Serial.println("[" + TimerName + "]: getRTCTime() ignored - RTC not initialised");
        }
        TimerDateTime empty = {0, 0, 0, 0, 0, 0};
        return empty;
    }
    return _rtcNowDateTime();
}

void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    if (!rtcInitialized) {
        if (debug) {
            Serial.println("[" + TimerName + "]: setRTCTime() ignored - RTC not initialised");
        }
        return;
    }
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting RTC time");
    }
    _rtcAdjustImpl(year, month, day, hour, minute, second);
}
#endif // useRTCModule

// ---------------------------------------------------------------------------
// Initialisation
// ---------------------------------------------------------------------------
void Timer::begin() {
#ifdef useRTCModule
    if (!_rtcBeginImpl()) {
        useRTC = false;
        rtcInitialized = false;
        if (debug) {
            Serial.println("[" + TimerName + "]: RTC not found - falling back to millis()");
        }
    } else {
        useRTC = true;
        rtcInitialized = true;
        if (debug) {
            Serial.println("[" + TimerName + "]: RTC initialised");
        }
    }
#endif
}

// ---------------------------------------------------------------------------
// Core timer control
// ---------------------------------------------------------------------------
void Timer::start() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Starting timer");
    }
    if (!isRunning && !isPaused) {
#ifdef useRTCModule
        if (useRTC) {
            startTime = _rtcNowMs();
        } else
#endif
        {
            startTime = millis();
        }
        isRunning = true;
    }
}

void Timer::stop() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Stopping timer");
    }
    if (isRunning) {
#ifdef useRTCModule
        if (useRTC) {
            elapsedTime += _rtcNowMs() - startTime;
        } else
#endif
        {
            elapsedTime += millis() - startTime;
        }
        isRunning = false;
    }
}

void Timer::reset() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Resetting timer");
    }
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Clearing timer");
    }
    elapsedTime = 0;
    startTime = 0;
    pauseTime = 0;
    isRunning = false;
    isPaused = false;
}

void Timer::pause() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Pausing timer");
    }
    if (isRunning && !isPaused) {
#ifdef useRTCModule
        if (useRTC) {
            pauseTime = _rtcNowMs();
        } else
#endif
        {
            pauseTime = millis();
        }
        isPaused = true;
        isRunning = false;
        if (debug) {
            Serial.println("[" + TimerName + "]: Timer paused");
        }
    } else {
        if (debug) {
            Serial.println("[" + TimerName + "]: Timer is not running");
        }
    }
}

void Timer::resume() {
    if (debug) {
        Serial.println("[" + TimerName + "]: Resuming timer");
    }
    if (isPaused) {
#ifdef useRTCModule
        if (useRTC) {
            startTime += _rtcNowMs() - pauseTime;
        } else
#endif
        {
            startTime += millis() - pauseTime;
        }
        isPaused = false;
        isRunning = true;
        if (debug) {
            Serial.println("[" + TimerName + "]: Timer resumed");
        }
    } else {
        if (debug) {
            Serial.println("[" + TimerName + "]: Timer is not paused");
        }
    }
}

// ---------------------------------------------------------------------------
// Target duration setters
// ---------------------------------------------------------------------------
void Timer::setTargetDuration(unsigned long durationMs) {
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target duration (ms): " + String(durationMs));
    }
    targetDuration = durationMs;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetSeconds(unsigned long seconds) {
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target seconds: " + String(seconds));
    }
    targetDuration = seconds * 1000UL;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target minutes: " + String(minutes));
    }
    targetDuration = minutes * 60000UL;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetHours(unsigned long hours) {
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target hours: " + String(hours));
    }
    targetDuration = hours * 3600000UL;
    remainingTimeOnTimer = targetDuration;
}

// ---------------------------------------------------------------------------
// Elapsed / remaining queries
// ---------------------------------------------------------------------------
unsigned long Timer::elapsed() {
    unsigned long e;
    if (isRunning) {
#ifdef useRTCModule
        if (useRTC) {
            e = elapsedTime + (_rtcNowMs() - startTime);
        } else
#endif
        {
            e = elapsedTime + (millis() - startTime);
        }
    } else if (isPaused) {
        e = elapsedTime + (pauseTime - startTime);
    } else {
        e = elapsedTime;
    }
    if (debug) {
        Serial.println("[" + TimerName + "]: Elapsed: " + String(e) + " ms");
    }
    return e;
}

unsigned long Timer::remainingTimeMillis() {
    unsigned long e = elapsed();
    if (e >= targetDuration) {
        return 0;
    }
    return targetDuration - e;
}

unsigned long Timer::remainingTime() {
    unsigned long remainingMillis = remainingTimeMillis();

    if (remainingMillis >= 3600000UL) {
        timerMode = Hours;
        return remainingMillis / 3600000UL;
    } else if (remainingMillis >= 60000UL) {
        timerMode = Minutes;
        return remainingMillis / 60000UL;
    } else {
        timerMode = Seconds;
        return remainingMillis / 1000UL;
    }
}

void Timer::updateRemainingTime() {
    unsigned long remainingMillis = remainingTimeMillis();

    if (remainingMillis >= 3600000UL) {
        timerMode = Hours;
    } else if (remainingMillis >= 60000UL) {
        timerMode = Minutes;
    } else {
        timerMode = Seconds;
    }

    remainingTimeOnTimer = remainingMillis / 1000UL; // total seconds remaining
}

void Timer::printTimeRemaining() {
    updateRemainingTime(); // refreshes remainingTimeOnTimer (seconds) and timerMode

    unsigned long hours   = remainingTimeOnTimer / 3600UL;
    unsigned long minutes = (remainingTimeOnTimer % 3600UL) / 60UL;
    unsigned long seconds = remainingTimeOnTimer % 60UL;

    Serial.print("[" + TimerName + "] Remaining Time: ");
    switch (timerMode) {
        case Hours:
            Serial.print(hours);
            Serial.print(":");
            if (minutes < 10) Serial.print("0");
            Serial.print(minutes);
            Serial.print(":");
            if (seconds < 10) Serial.print("0");
            Serial.println(seconds);
            break;
        case Minutes:
            Serial.print(minutes);
            Serial.print(":");
            if (seconds < 10) Serial.print("0");
            Serial.println(seconds);
            break;
        case Seconds:
            Serial.println(seconds);
            break;
    }
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------
bool Timer::isTimerRunning() const {
    return isRunning;
}

bool Timer::isTimerPaused() const {
    return isPaused;
}

bool Timer::hasReachedTarget() {
    if (targetDuration == 0) {
        return false; // No target set; never treat as reached
    }
    return elapsed() >= targetDuration;
}

bool Timer::checkTimer(unsigned long durationMs) {
    return elapsed() >= durationMs;
}
