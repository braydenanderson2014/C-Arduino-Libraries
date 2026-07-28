#ifndef SIMPLEARDUINOTIMER_h
#define SIMPLEARDUINOTIMER_h

#include <Arduino.h>

#ifdef useRTCModule
    #include <RTClib.h>
#endif

class Timer {
private:
    unsigned long startTime;
    unsigned long elapsedTime;
    unsigned long pauseTime;
    unsigned long targetDuration;
    bool isRunning;
    bool isPaused;
    bool useRTC;
    bool debug;
    unsigned long remainingTimeOnTimer;
    enum mode { Seconds, Minutes, Hours };
    mode timerMode;
    String TimerName;
#ifdef useRTCModule
    mutable RTC_DS3231 rtc;
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
    DateTime getRTCTime() const;
    void setRTCTime(int year, int month, int day, int hour, int minute, int second);
#endif
};

#endif // SIMPLEARDUINOTIMER_h
