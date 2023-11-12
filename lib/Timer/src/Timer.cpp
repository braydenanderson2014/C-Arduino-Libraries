#include "Timer.h"
#include <RTClib.h>

RTC_DS3231 timerRTC; // Add an instance of the RTC library
DateTime timerNow = timerRTC.now();

Timer::Timer() : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0), isRunning(false), isPaused(false) {
    if (!rtc.begin()) {
        Serial.println("[TIMER]: Couldn't find RTC");
        while (1);
    }
}


void Timer::start() {
    Serial.println("[TIMER]: Starting timer");
    if (!isRunning && !isPaused) {
        startTime = millis();
        isRunning = true;
    }
}

void Timer::stop() {
    Serial.println("[TIMER]: Stopping timer");
    if (isRunning) {
        elapsedTime += millis() - startTime;
        isRunning = false;
    }
}

void Timer::reset() {
    Serial.println("[TIMER]: Resetting timer");
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    Serial.println("[TIMER]: Clearing timer");
    elapsedTime = 0;
    startTime = 0;
    pauseTime = 0;
    isRunning = false;
    isPaused = false;
}

void Timer::pause() {
    Serial.println("[TIMER]: Pausing timer");
    if (isRunning && !isPaused) {
        pauseTime = millis();
        isPaused = true;
        isRunning = false;
    }
}

void Timer::resume() {
    Serial.println("[TIMER]: Resuming timer");
    if (isPaused) {
        startTime += (millis() - pauseTime); // Adjusting the start time so that paused duration isn't counted.
        isRunning = true;
        isPaused = false;
    }
}

unsigned long Timer::elapsed() const {
    Serial.println("[TIMER]: Getting elapsed time");
    if (isRunning) {
        return elapsedTime + (millis() - startTime);
    } else if (isPaused) {
        return elapsedTime + (pauseTime - startTime);
    }
    return elapsedTime;
}

bool Timer::isTimerRunning() const {
    Serial.println("[TIMER]: Checking if timer is running");
    return isRunning;
}

bool Timer::checkTimer(unsigned long duration) const {
    Serial.println("[TIMER]: Checking if timer has reached duration");
    return elapsed() >= duration;
}

void Timer::syncWithRTC() {
    Serial.println("[TIMER]: Syncing with RTC");
    // Logic to sync with RTC
    timerNow = timerRTC.now();
    elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
}

DateTime Timer::getRTCTime() const {
    Serial.println("[TIMER]: Getting RTC time");
    return timerRTC.now();
}

bool Timer::isTimerPaused() const { // Rename to isTimerPaused to avoid conflict
    Serial.println("[TIMER]: Checking if timer is paused");
    return isPaused;
}

void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    Serial.println("[TIMER]: Setting RTC time");
    timerRTC.adjust(DateTime(year, month, day, hour, minute, second));
}

void Timer::setTargetDuration(unsigned long duration) {
    Serial.println("[TIMER]: Setting target duration");
    targetDuration = duration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    Serial.println("[TIMER]: Setting target minutes");
    targetDuration = minutes * 60000;  // 1 minute = 60,000 milliseconds
}

bool Timer::hasReachedTarget() const {
    Serial.println("[TIMER]: Checking if timer has reached target");
    return elapsed() >= targetDuration;
}

unsigned long Timer::remainingTime() const {
    Serial.println("[TIMER]: Getting remaining time");
    if (elapsed() < targetDuration) {
        return targetDuration - elapsed();
    }
    return 0;  // No time left
}