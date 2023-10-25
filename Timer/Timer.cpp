#include "Timer.h"
#include <RTClib.h>
RTC_DS3231 timerRTC; // Add an instance of the RTC library
DateTime timerNow = timerRTC.now();

Timer::Timer() : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0), isRunning(false), isPaused(false) {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }
}


void Timer::start() {
    if (!isRunning && !isPaused) {
        startTime = millis();
        isRunning = true;
    }
}

void Timer::stop() {
    if (isRunning) {
        elapsedTime += millis() - startTime;
        isRunning = false;
    }
}

void Timer::reset() {
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    elapsedTime = 0;
    startTime = 0;
    pauseTime = 0;
    isRunning = false;
    isPaused = false;
}

void Timer::pause() {
    if (isRunning && !isPaused) {
        pauseTime = millis();
        isPaused = true;
        isRunning = false;
    }
}

void Timer::resume() {
    if (isPaused) {
        startTime += (millis() - pauseTime); // Adjusting the start time so that paused duration isn't counted.
        isRunning = true;
        isPaused = false;
    }
}

unsigned long Timer::elapsed() const {
    if (isRunning) {
        return elapsedTime + (millis() - startTime);
    } else if (isPaused) {
        return elapsedTime + (pauseTime - startTime);
    }
    return elapsedTime;
}

bool Timer::isTimerRunning() const {
    return isRunning;
}

bool Timer::checkTimer(unsigned long duration) const {
    return elapsed() >= duration;
}

void Timer::syncWithRTC() {
    // Logic to sync with RTC
    timerNow = timerRTC.now();
    elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
}

DateTime Timer::getRTCTime() const {
    return timerRTC.now();
}

bool Timer::isTimerPaused() const { // Rename to isTimerPaused to avoid conflict
    return isPaused;
}

void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    timerRTC.adjust(DateTime(year, month, day, hour, minute, second));
}

void Timer::setTargetDuration(unsigned long duration) {
    targetDuration = duration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    targetDuration = minutes * 60000;  // 1 minute = 60,000 milliseconds
}

bool Timer::hasReachedTarget() const {
    return elapsed() >= targetDuration;
}

unsigned long Timer::remainingTime() const {
    if (elapsed() < targetDuration) {
        return targetDuration - elapsed();
    }
    return 0;  // No time left
}