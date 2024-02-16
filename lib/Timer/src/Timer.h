#ifndef TIMER_H
#define TIMER_H

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
    mutable RTC_DS3231 rtc;

public:
    Timer();

    void start();
    void stop();
    void reset();
    void clear();
    void pause();
    void resume();

    unsigned long elapsed() const;
    bool isTimerRunning() const;
    bool hasReachedTarget() const;
    unsigned long remainingTime() const;

    void syncWithRTC();
    DateTime getRTCTime() const;
    bool isTimerPaused() const;
    void setRTCTime(int year, int month, int day, int hour, int minute, int second);
    void setTargetDuration(unsigned long duration);
    void setTargetMinutes(unsigned long minutes);
    bool checkTimer(unsigned long duration) const;
    bool debug;
};

#endif // TIMER_H
