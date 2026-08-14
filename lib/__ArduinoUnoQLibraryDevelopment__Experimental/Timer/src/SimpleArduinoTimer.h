#ifndef SIMPLEARDUINOTIMER_h
#define SIMPLEARDUINOTIMER_h

#include <Arduino.h>

#ifdef TIMER_USE_BRIDGE
  #include <Arduino_RouterBridge.h>
#endif

// ---------------------------------------------------------------------------
// Concurrency macros (opt-in)
// ---------------------------------------------------------------------------
// By default Timer is compiled exactly as before: no locking, no threads and
// no extra dependencies.  These compile-time switches change that:
//
//   TIMER_THREAD_SAFE       — guard every public method with a lock so a Timer
//                             instance can be shared between threads/cores.
//                             Default backend is the recursive Mutex from
//                             SimpleMutex.
//   TIMER_LOCK_BACKEND_RWLOCK — use SimpleRWLock instead of Mutex.  Read-only
//                             methods take a shared read lock so several
//                             threads can poll the Timer concurrently, while
//                             mutators take an exclusive write lock.  Implies
//                             TIMER_THREAD_SAFE.
//   TIMER_ENABLE_THREADING  — implies TIMER_THREAD_SAFE and adds
//                             startMonitor()/stopMonitor(), which poll the
//                             Timer on a background thread
//                             (SimpleThreadManager).
//
// All switches require the UNO Q / GIGA / ESP32 class concurrency libraries
// (SimpleMutex or SimpleRWLock, plus SimpleThreadManager) on the include path.
// ---------------------------------------------------------------------------
#if (defined(TIMER_ENABLE_THREADING) || defined(TIMER_LOCK_BACKEND_RWLOCK)) && \
    !defined(TIMER_THREAD_SAFE)
#  define TIMER_THREAD_SAFE 1
#endif

#ifdef TIMER_THREAD_SAFE
#  ifdef TIMER_LOCK_BACKEND_RWLOCK
#    include <SimpleRWLock.h>
#  else
#    include <Mutex.h>
#  endif
#endif

#ifdef TIMER_ENABLE_THREADING
#  include <ThreadManager.h>
#  ifndef TIMER_MONITOR_STACK_SIZE
#    define TIMER_MONITOR_STACK_SIZE 2048
#  endif
#  ifndef TIMER_MONITOR_INTERVAL_MS
#    define TIMER_MONITOR_INTERVAL_MS 10
#  endif
#endif

// ---------------------------------------------------------------------------
// RTC chip-selection macros
// ---------------------------------------------------------------------------
// Define useRTCModule to enable RTC support, then define ONE of the
// chip-selection macros below.  If none is chosen the default is DS3231.
//
//   RTC_CHIP_DS3231  — DS3231 via RTClib (default)
//   RTC_CHIP_DS1307  — DS1307 via RTClib
//   RTC_CHIP_PCF8523 — PCF8523 via RTClib
//   RTC_CHIP_PCF8563 — PCF8563 via RTClib
//   RTC_CHIP_MCP7940 — MCP7940N via RTClib
//   RTC_CHIP_DS1302  — DS1302 via Makuna RTC library (3-wire SPI)
//
// DS1302 pin defaults (override before including this header):
//   #define RTC_DS1302_IO    4
//   #define RTC_DS1302_SCLK  5
//   #define RTC_DS1302_CE    2
// ---------------------------------------------------------------------------

#ifdef useRTCModule
    #if defined(RTC_CHIP_DS1302)
        // DS1302 uses a 3-wire SPI interface; requires the Makuna RTC library
        #include <ThreeWire.h>
        #include <RtcDS1302.h>
        #ifndef RTC_DS1302_IO
        #  define RTC_DS1302_IO   4
        #endif
        #ifndef RTC_DS1302_SCLK
        #  define RTC_DS1302_SCLK 5
        #endif
        #ifndef RTC_DS1302_CE
        #  define RTC_DS1302_CE   2
        #endif
    #else
        // All other supported chips use the RTClib I2C driver
        #include <RTClib.h>
        #if   defined(RTC_CHIP_DS1307)
        #  define _RTC_IMPL_TYPE RTC_DS1307
        #elif defined(RTC_CHIP_PCF8523)
        #  define _RTC_IMPL_TYPE RTC_PCF8523
        #elif defined(RTC_CHIP_PCF8563)
        #  define _RTC_IMPL_TYPE RTC_PCF8563
        #elif defined(RTC_CHIP_MCP7940)
        #  define _RTC_IMPL_TYPE RTC_MCP7940N
        #else
        #  define _RTC_IMPL_TYPE RTC_DS3231  // default
        #endif
    #endif

    // ---------------------------------------------------------------------------
    // Chip-agnostic date/time struct returned by getRTCTime().
    // Replaces the RTClib-specific DateTime type so that all supported chips
    // share the same public API.
    // ---------------------------------------------------------------------------
    struct TimerDateTime {
        uint16_t year;
        uint8_t  month;
        uint8_t  day;
        uint8_t  hour;
        uint8_t  minute;
        uint8_t  second;
    };
#endif // useRTCModule

class Timer {
private:
    unsigned long startTime;
    unsigned long elapsedTime;
    unsigned long pauseTime;
    unsigned long targetDuration;
    bool isRunning;
    bool isPaused;
    bool useRTC;
    bool rtcInitialized;
    bool debug;
    bool _repeating;
    unsigned long remainingTimeOnTimer;
    unsigned long _lapAnchor;
    void (*_callback)();
    enum mode { Seconds, Minutes, Hours };
    mode timerMode;
    String TimerName;

#ifdef TIMER_USE_BRIDGE
    static Timer* _bridgeInstance;
    static String _bridgePrefix;
    static bool _bridgeRegistered;

    static bool _bridgeStart();
    static bool _bridgeStop();
    static bool _bridgeReset();
    static bool _bridgeClear();
    static bool _bridgePause();
    static bool _bridgeResume();
    static bool _bridgeRestart();
    static bool _bridgeSetTargetDuration(unsigned long durationMs);
    static bool _bridgeSetTargetSeconds(unsigned long seconds);
    static bool _bridgeSetTargetMinutes(unsigned long minutes);
    static bool _bridgeSetTargetHours(unsigned long hours);
    static unsigned long _bridgeElapsed();
    static unsigned long _bridgeElapsedSeconds();
    static unsigned long _bridgeElapsedMinutes();
    static unsigned long _bridgeElapsedHours();
    static unsigned long _bridgeRemainingTime();
    static unsigned long _bridgeRemainingTimeMillis();
    static bool _bridgeHasReachedTarget();
    static bool _bridgeIsRunning();
    static bool _bridgeIsPaused();
    static bool _bridgeSetRepeating(bool repeating);
    static bool _bridgeGetRepeating();
#endif

#ifdef TIMER_THREAD_SAFE
#  ifdef TIMER_LOCK_BACKEND_RWLOCK
    mutable SimpleRWLock _rwLock;
#  else
    mutable Mutex _mutex;   // recursive, so nested public calls are safe
#  endif
#endif

#ifdef TIMER_ENABLE_THREADING
    ThreadManager::ThreadId _monitorThread;
    volatile bool _monitorRunning;
    volatile bool _monitorStopRequested;
    unsigned long _monitorIntervalMs;

    static void _monitorEntry(void* argument);
    void _monitorLoop();
#endif

#ifdef useRTCModule
    int _timezoneOffsetHours;

    #if defined(RTC_CHIP_DS1302)
        mutable ThreeWire            _ds1302Wire;
        mutable RtcDS1302<ThreeWire> rtc;
    #else
        mutable _RTC_IMPL_TYPE rtc;
    #endif

    // Private helpers that abstract chip-specific API differences
    bool          _rtcBeginImpl();
    unsigned long _rtcNowMs() const;
    void          _rtcAdjustImpl(int year, int month, int day,
                                  int hour, int minute, int second);
    TimerDateTime _rtcNowDateTime() const;
#endif

public:
    Timer(bool debug = false);
    ~Timer();
    void setTimerName(String timerName);
    String getTimerName() const;

    // Core control
    void start();
    void stop();
    void reset();
    void clear();
    void pause();
    void resume();
    void restart();
    void begin();

    // Target duration setters
    void setTargetDuration(unsigned long durationMs);
    void setTargetSeconds(unsigned long seconds);
    void setTargetMinutes(unsigned long minutes);
    void setTargetHours(unsigned long hours);

    // Target duration getters
    unsigned long getTargetDuration() const;
    unsigned long getTargetSeconds() const;
    unsigned long getTargetMinutes() const;
    unsigned long getTargetHours() const;

    // Elapsed helpers
    unsigned long elapsed();
    unsigned long elapsedSeconds();
    unsigned long elapsedMinutes();
    unsigned long elapsedHours();

    // Remaining helpers
    unsigned long remainingTime();
    unsigned long remainingTimeMillis();
    void updateRemainingTime();
    void printTimeRemaining();

    // Lap / split
    unsigned long lap();

    // State queries
    bool isTimerRunning() const;
    bool isTimerPaused() const;
    bool hasReachedTarget();
    bool checkTimer(unsigned long durationMs);

    // Repeating mode
    void setRepeating(bool repeating);
    bool getRepeating() const;

    // Callback
    void onTargetReached(void (*callback)());

#ifdef TIMER_USE_BRIDGE
    // UnoQ / App Lab bridge helpers. Register a default timer instance so
    // Python can call Bridge.call("timer_start", ...) and similar methods.
    bool beginBridge(const String& prefix = "timer");
    bool isBridgeRegistered() const;
    String getBridgePrefix() const;
#endif

    // Concurrency helpers
    static bool isThreadSafe();

#ifdef TIMER_THREAD_SAFE
    // Manual locking for callers that need several operations to be atomic.
    bool lock(unsigned long timeoutMs = 0);
    void unlock();
    // Shared/read acquisition; identical to lock() on the Mutex backend.
    bool lockShared(unsigned long timeoutMs = 0);
    void unlockShared();
#endif

#ifdef TIMER_ENABLE_THREADING
    // Polls hasReachedTarget() on a background thread so callbacks and
    // repeating restarts fire without the sketch calling into the Timer.
    bool startMonitor(unsigned long pollIntervalMs = TIMER_MONITOR_INTERVAL_MS);
    void stopMonitor(unsigned long timeoutMs = 1000);
    bool isMonitorRunning() const;
#endif

#ifdef useRTCModule
    void setUseRTC(bool useRTC);
    bool getUseRTC() const;
    bool isRTCAvailable() const;
    void syncWithRTC();
    TimerDateTime getRTCTime() const;
    String getRTCTimeString() const;
    void printRTCTime() const;
    void setTimezone(int offsetHours);
    void setRTCTime(int year, int month, int day, int hour, int minute, int second);
#endif
};

#endif // SIMPLEARDUINOTIMER_h
