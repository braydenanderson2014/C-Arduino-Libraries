#ifndef SIMPLEARDUINOTIMER_h
#define SIMPLEARDUINOTIMER_h

#include <Arduino.h>
#include <RTClib.h>

class Timer {
private:
    unsigned long startTime;
    unsigned long elapsedTime;
    unsigned long pauseTime;
    unsigned long targetDuration;
    bool isRunning;
    bool isPaused;
    bool useRTC;
    mutable RTC_DS3231 rtc;

public:
    Timer();

    void start();
    void stop();
    void reset();
    void clear();
    void pause();
    void resume();
    void setUseRTC(bool useRTC);
    void begin();
    
    unsigned long elapsed();
    bool isTimerRunning() const;
    bool hasReachedTarget();
    bool getUseRTC() const;
    unsigned long remainingTime();

    void syncWithRTC();
    DateTime getRTCTime() const;
    bool isTimerPaused() const;
    void setRTCTime(int year, int month, int day, int hour, int minute, int second);
    void setTargetDuration(unsigned long duration);
    void setTargetMinutes(unsigned long minutes);
    void setTargetHours(unsigned long hours);
    bool checkTimer(unsigned long duration);
    bool debug;
};

#endif // SIMPLEARDUINOTIMER_h
