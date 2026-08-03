#ifndef SIMPLEARDUINOTIMER_h
#define SIMPLEARDUINOTIMER_h

#include <Arduino.h>

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
    unsigned long remainingTimeOnTimer;
    enum mode { Seconds, Minutes, Hours };
    mode timerMode;
    String TimerName;

#ifdef useRTCModule
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
    void setTimerName(String timerName);
    String getTimerName() const;

    void start();
    void stop();
    void reset();
    void clear();
    void pause();
    void resume();
    void begin();

    void setTargetDuration(unsigned long durationMs);
    void setTargetSeconds(unsigned long seconds);
    void setTargetMinutes(unsigned long minutes);
    void setTargetHours(unsigned long hours);

    unsigned long elapsed();
    unsigned long remainingTime();
    unsigned long remainingTimeMillis();
    void updateRemainingTime();
    void printTimeRemaining();

    bool isTimerRunning() const;
    bool isTimerPaused() const;
    bool hasReachedTarget();
    bool checkTimer(unsigned long durationMs);

#ifdef useRTCModule
    void setUseRTC(bool useRTC);
    bool getUseRTC() const;
    void syncWithRTC();
    TimerDateTime getRTCTime() const;
    void setRTCTime(int year, int month, int day, int hour, int minute, int second);
#endif
};

#endif // SIMPLEARDUINOTIMER_h
