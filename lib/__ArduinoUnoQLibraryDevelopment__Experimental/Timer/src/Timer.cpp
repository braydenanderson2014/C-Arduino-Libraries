#include "SimpleArduinoTimer.h"

// ---------------------------------------------------------------------------
// Locking helpers
// ---------------------------------------------------------------------------
// Both macros compile to nothing unless TIMER_THREAD_SAFE is defined, so the
// single threaded build behaves exactly as before.  On the Mutex backend the
// read variant is simply an exclusive lock; on the SimpleRWLock backend it is
// a shared lock, so concurrent pollers do not serialise on each other.
#if defined(TIMER_THREAD_SAFE) && defined(TIMER_LOCK_BACKEND_RWLOCK)
#  define TIMER_READ_LOCK()  SimpleRWLock::ReadGuard _timerLockGuard(_rwLock)
#  define TIMER_WRITE_LOCK() SimpleRWLock::WriteGuard _timerLockGuard(_rwLock)
#elif defined(TIMER_THREAD_SAFE)
#  define TIMER_READ_LOCK()  Mutex::LockGuard _timerLockGuard(_mutex)
#  define TIMER_WRITE_LOCK() Mutex::LockGuard _timerLockGuard(_mutex)
#else
#  define TIMER_READ_LOCK()  ((void)0)
#  define TIMER_WRITE_LOCK() ((void)0)
#endif

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
Timer::Timer(bool debug)
    : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0),
      isRunning(false), isPaused(false), useRTC(false), rtcInitialized(false),
      debug(debug), _repeating(false), remainingTimeOnTimer(0),
      _lapAnchor(0), _callback(nullptr), timerMode(Seconds)
#if defined(useRTCModule) && defined(RTC_CHIP_DS1302)
    , _timezoneOffsetHours(0)
    , _ds1302Wire(RTC_DS1302_IO, RTC_DS1302_SCLK, RTC_DS1302_CE)
    , rtc(_ds1302Wire)
#elif defined(useRTCModule)
    , _timezoneOffsetHours(0)
#endif
{
#ifdef TIMER_ENABLE_THREADING
    _monitorThread = ThreadManager::InvalidThreadId;
    _monitorRunning = false;
    _monitorStopRequested = false;
    _monitorIntervalMs = TIMER_MONITOR_INTERVAL_MS;
#endif
    setTimerName("Timer");
}

Timer::~Timer() {
#ifdef TIMER_ENABLE_THREADING
    stopMonitor();
#endif
}

// ---------------------------------------------------------------------------
// Name helpers
// ---------------------------------------------------------------------------
void Timer::setTimerName(String timerName) {
    TIMER_WRITE_LOCK();
    this->TimerName = timerName;
}

String Timer::getTimerName() const {
    TIMER_READ_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_READ_LOCK();
    return useRTC;
}

bool Timer::isRTCAvailable() const {
    TIMER_READ_LOCK();
    return rtcInitialized;
}

void Timer::setTimezone(int offsetHours) {
    TIMER_WRITE_LOCK();
    _timezoneOffsetHours = offsetHours;
}

void Timer::syncWithRTC() {
    TIMER_WRITE_LOCK();
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
    TIMER_READ_LOCK();
    if (!rtcInitialized) {
        if (debug) {
            Serial.println("[" + TimerName + "]: getRTCTime() ignored - RTC not initialised");
        }
        TimerDateTime empty = {0, 0, 0, 0, 0, 0};
        return empty;
    }
    TimerDateTime dt = _rtcNowDateTime();
    // Apply timezone offset (simple signed-hour shift, no DST)
    if (_timezoneOffsetHours != 0) {
        int totalMinutes = (int)dt.hour * 60 + (int)dt.minute
                           + _timezoneOffsetHours * 60;
        // Wrap into 0–1439 minute range
        totalMinutes = ((totalMinutes % 1440) + 1440) % 1440;
        dt.hour   = (uint8_t)(totalMinutes / 60);
        dt.minute = (uint8_t)(totalMinutes % 60);
    }
    return dt;
}

String Timer::getRTCTimeString() const {
    TIMER_READ_LOCK();
    TimerDateTime dt = getRTCTime();
    // Format: "YYYY-MM-DD HH:MM:SS"
    char buf[20];
    snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u",
             (unsigned)dt.year,  (unsigned)dt.month,  (unsigned)dt.day,
             (unsigned)dt.hour,  (unsigned)dt.minute, (unsigned)dt.second);
    return String(buf);
}

void Timer::printRTCTime() const {
    TIMER_READ_LOCK();
    if (!rtcInitialized) {
        if (debug) {
            Serial.println("[" + TimerName + "]: printRTCTime() ignored - RTC not initialised");
        }
        return;
    }
    Serial.println("[" + TimerName + "] RTC: " + getRTCTimeString());
}

void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
        _lapAnchor = startTime;
        isRunning = true;
    }
}

void Timer::restart() {
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Restarting timer");
    }
    clear();
    start();
}

void Timer::stop() {
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Resetting timer");
    }
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target duration (ms): " + String(durationMs));
    }
    targetDuration = durationMs;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetSeconds(unsigned long seconds) {
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target seconds: " + String(seconds));
    }
    targetDuration = seconds * 1000UL;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target minutes: " + String(minutes));
    }
    targetDuration = minutes * 60000UL;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetHours(unsigned long hours) {
    TIMER_WRITE_LOCK();
    if (debug) {
        Serial.println("[" + TimerName + "]: Setting target hours: " + String(hours));
    }
    targetDuration = hours * 3600000UL;
    remainingTimeOnTimer = targetDuration;
}

// ---------------------------------------------------------------------------
// Target duration getters
// ---------------------------------------------------------------------------
unsigned long Timer::getTargetDuration() const {
    TIMER_READ_LOCK();
    return targetDuration;
}

unsigned long Timer::getTargetSeconds() const {
    TIMER_READ_LOCK();
    return targetDuration / 1000UL;
}

unsigned long Timer::getTargetMinutes() const {
    TIMER_READ_LOCK();
    return targetDuration / 60000UL;
}

unsigned long Timer::getTargetHours() const {
    TIMER_READ_LOCK();
    return targetDuration / 3600000UL;
}

// ---------------------------------------------------------------------------
// Elapsed / remaining queries
// ---------------------------------------------------------------------------
unsigned long Timer::elapsed() {
    TIMER_READ_LOCK();
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

unsigned long Timer::elapsedSeconds() {
    TIMER_READ_LOCK();
    return elapsed() / 1000UL;
}

unsigned long Timer::elapsedMinutes() {
    TIMER_READ_LOCK();
    return elapsed() / 60000UL;
}

unsigned long Timer::elapsedHours() {
    TIMER_READ_LOCK();
    return elapsed() / 3600000UL;
}

unsigned long Timer::lap() {
    TIMER_WRITE_LOCK();
    unsigned long now;
#ifdef useRTCModule
    if (useRTC) {
        now = _rtcNowMs();
    } else
#endif
    {
        now = millis();
    }
    // For a stopped/paused timer, use accumulated elapsed as the lap end point
    if (!isRunning) {
        now = elapsedTime + startTime;
    }
    unsigned long lapTime = now - _lapAnchor;
    _lapAnchor = now;
    if (debug) {
        Serial.println("[" + TimerName + "]: Lap: " + String(lapTime) + " ms");
    }
    return lapTime;
}

unsigned long Timer::remainingTimeMillis() {
    TIMER_READ_LOCK();
    unsigned long e = elapsed();
    if (e >= targetDuration) {
        return 0;
    }
    return targetDuration - e;
}

unsigned long Timer::remainingTime() {
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_WRITE_LOCK();
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
    TIMER_READ_LOCK();
    return isRunning;
}

bool Timer::isTimerPaused() const {
    TIMER_READ_LOCK();
    return isPaused;
}

bool Timer::hasReachedTarget() {
    TIMER_WRITE_LOCK();
    if (targetDuration == 0) {
        return false; // No target set; never treat as reached
    }
    if (elapsed() >= targetDuration) {
        if (_callback) {
            _callback();
        }
        if (_repeating) {
            restart();
        }
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Repeating mode
// ---------------------------------------------------------------------------
void Timer::setRepeating(bool repeating) {
    TIMER_WRITE_LOCK();
    _repeating = repeating;
}

bool Timer::getRepeating() const {
    TIMER_READ_LOCK();
    return _repeating;
}

// ---------------------------------------------------------------------------
// Callback
// ---------------------------------------------------------------------------
void Timer::onTargetReached(void (*callback)()) {
    TIMER_WRITE_LOCK();
    _callback = callback;
}

bool Timer::checkTimer(unsigned long durationMs) {
    TIMER_READ_LOCK();
    return elapsed() >= durationMs;
}

// ---------------------------------------------------------------------------
// Concurrency support
// ---------------------------------------------------------------------------
bool Timer::isThreadSafe() {
#ifdef TIMER_THREAD_SAFE
    return true;
#else
    return false;
#endif
}

#ifdef TIMER_THREAD_SAFE
bool Timer::lock(unsigned long timeoutMs) {
#ifdef TIMER_LOCK_BACKEND_RWLOCK
    return _rwLock.writeLock(timeoutMs);
#else
    return _mutex.lock(timeoutMs);
#endif
}

void Timer::unlock() {
#ifdef TIMER_LOCK_BACKEND_RWLOCK
    _rwLock.writeUnlock();
#else
    _mutex.unlock();
#endif
}

bool Timer::lockShared(unsigned long timeoutMs) {
#ifdef TIMER_LOCK_BACKEND_RWLOCK
    return _rwLock.readLock(timeoutMs);
#else
    return _mutex.lock(timeoutMs);
#endif
}

void Timer::unlockShared() {
#ifdef TIMER_LOCK_BACKEND_RWLOCK
    _rwLock.readUnlock();
#else
    _mutex.unlock();
#endif
}
#endif

#ifdef TIMER_ENABLE_THREADING
void Timer::_monitorEntry(void* argument) {
    Timer* self = static_cast<Timer*>(argument);
    if (self != nullptr) {
        self->_monitorLoop();
    }
#if defined(STM_HAS_FREERTOS)
    vTaskDelete(nullptr);
#endif
}

void Timer::_monitorLoop() {
    while (!_monitorStopRequested) {
        // hasReachedTarget() fires the callback and handles repeating restarts.
        hasReachedTarget();
        ThreadManager::delayThread(static_cast<uint32_t>(_monitorIntervalMs));
    }
    _monitorRunning = false;
}

bool Timer::startMonitor(unsigned long pollIntervalMs) {
    TIMER_WRITE_LOCK();
    if (_monitorRunning) {
        return true;
    }
    if (!ThreadManager::hasThreading()) {
        if (debug) {
            Serial.println("[" + TimerName + "]: Threading unavailable - monitor not started");
        }
        return false;
    }

    _monitorIntervalMs = (pollIntervalMs == 0) ? 1UL : pollIntervalMs;
    _monitorStopRequested = false;
    _monitorRunning = true;
    _monitorThread = ThreadManager::createThread(
        &Timer::_monitorEntry,
        this,
        "TimerMonitor",
        TIMER_MONITOR_STACK_SIZE);

    if (_monitorThread == ThreadManager::InvalidThreadId) {
        _monitorRunning = false;
        if (debug) {
            Serial.println("[" + TimerName + "]: Failed to create monitor thread");
        }
        return false;
    }
    if (debug) {
        Serial.println("[" + TimerName + "]: Monitor thread started");
    }
    return true;
}

void Timer::stopMonitor(unsigned long timeoutMs) {
    if (!_monitorRunning) {
        return;
    }
    _monitorStopRequested = true;

    const unsigned long start = millis();
    while (_monitorRunning && (millis() - start) < timeoutMs) {
        ThreadManager::delayThread(1);
    }
    if (_monitorRunning && _monitorThread != ThreadManager::InvalidThreadId) {
        ThreadManager::endThread(_monitorThread);
        _monitorRunning = false;
    }
    _monitorThread = ThreadManager::InvalidThreadId;
}

bool Timer::isMonitorRunning() const {
    return _monitorRunning;
}
#endif // TIMER_ENABLE_THREADING

