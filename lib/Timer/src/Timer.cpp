#include "SimpleArduinoTimer.h"
#include <RTClib.h>

RTC_DS3231 timerRTC; // Add an instance of the RTC library
DateTime timerNow = timerRTC.now();



Timer::Timer() : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0), isRunning(false), isPaused(false), debug(debug) {
  useRTC = false;
}

void Timer::begin() {
    if(!rtc.begin()){
        useRTC = false;
    }
    useRTC = true;
}

void Timer::setUseRTC(bool useRTC) {
    if(debug){
        Serial.println("[TIMER]: Setting useRTC : " + String(useRTC));
    }
    this->useRTC = useRTC;
}

bool Timer::getUseRTC() const {
    if(debug){
        Serial.println("[TIMER]: Getting useRTC : " + String(useRTC));
    }
    return useRTC;
}

void Timer::start() {
    if(debug){
        Serial.println("[TIMER]: Starting timer");
    }
    if (!isRunning && !isPaused) {
        if(useRTC){
            syncWithRTC();
            startTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
        }else{
            startTime = millis();
        }
        isRunning = true;
    }
}

void Timer::stop() {
    if(debug){
        Serial.println("[TIMER]: Stopping timer");
    }
    if (isRunning) {
        if(useRTC){
            syncWithRTC();
            elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
        }else{
            elapsedTime += millis() - startTime;
        }
        isRunning = false;
    }
}

void Timer::reset() {
    if(debug){
        Serial.println("[TIMER]: Resetting timer");
    }
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    if(debug){
        Serial.println("[TIMER]: Clearing timer");
    }
    elapsedTime = 0;
    startTime = 0;
    pauseTime = 0;
    isRunning = false;
    isPaused = false;
}

void Timer::pause() {
    if(debug){
        Serial.println("[TIMER]: Pausing timer");
    }
    if (isRunning && !isPaused) {
        if(useRTC){
            syncWithRTC();
            pauseTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
        }else{
            pauseTime = millis();
        }
        isPaused = true;
        isRunning = false;
        if(debug){
            Serial.println("[TIMER]: Paused timer");
        }
    } else {
        if(debug){
            Serial.println("[TIMER]: Timer is not running");
        }
    }
}

void Timer::resume() {
    if(debug){
        Serial.println("[TIMER]: Resuming timer");
    }
    if (isPaused) {
        if(useRTC){
            syncWithRTC();
            startTime += (timerNow.unixtime() * 1000 - pauseTime); // Adjusting the start time so that paused duration isn't counted.
        }else{
            startTime += (millis() - pauseTime); // Adjusting the start time so that paused duration isn't counted.
        }
        isRunning = true;
        isPaused = false;
        if(debug){
            Serial.println("[TIMER]: Resumed timer");
        }
    } else {
        if(debug){
            Serial.println("[TIMER]: Timer is not paused");
        }
    }
}

unsigned long Timer::elapsed() {
    if(debug){
        Serial.println("[TIMER]: Getting elapsed time");
    }
    if (isRunning) {
        if(debug){
            Serial.println("[TIMER]: (Timer Running) Elapsed Time: " + String(elapsedTime + (millis() - startTime)) + " milliseconds");
        }
        if(useRTC){
            syncWithRTC();
            return elapsedTime + (timerNow.unixtime() * 1000 - startTime);
        }else{
            return elapsedTime + (millis() - startTime);
        }
    } else if (isPaused) {
        if(debug){
            Serial.println("[TIMER]: (Timer Paused) Elapsed Time: " + String(elapsedTime + (pauseTime - startTime)) + " milliseconds");
        }
        return elapsedTime + (pauseTime - startTime);
    }
    if(debug){
        Serial.println("[TIMER]: Elapsed Time: " + String(elapsedTime) + " milliseconds");
    }
    return elapsedTime;
}

bool Timer::isTimerRunning() const {
    if(debug){
        Serial.println("[TIMER]: Checking if timer is running : " + String(isRunning));
    }
    return isRunning;
}

bool Timer::checkTimer(unsigned long duration)  {
    if(debug){
        Serial.println("[TIMER]: Checking if timer has reached duration : " + String(elapsed() >= duration) + " milliseconds");
    }
    return elapsed() >= duration;
}

void Timer::syncWithRTC() {
    if(debug){
        Serial.println("[TIMER]: Syncing with RTC");
    }
    // Logic to sync with RTC
    timerNow = timerRTC.now();
    elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
}

DateTime Timer::getRTCTime() const {
    if(debug){
        Serial.println("[TIMER]: Getting RTC time : " + String(timerRTC.now().timestamp()) + " milliseconds");
    }
    return timerRTC.now();
}

bool Timer::isTimerPaused() const { // Rename to isTimerPaused to avoid conflict
    if(debug){
        Serial.println("[TIMER]: Checking if timer is paused : " + String(isPaused));
    }
    return isPaused;
}

void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    if(debug){
        Serial.println("[TIMER]: Setting RTC time");
    }
    timerRTC.adjust(DateTime(year, month, day, hour, minute, second));
}

void Timer::setTargetDuration(unsigned long duration) {
    if(debug){
        Serial.println("[TIMER]: Setting target duration");
    }
    targetDuration = duration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    if(debug){
        Serial.println("[TIMER]: Setting target minutes");
    }
    targetDuration = minutes * 60000;  // 1 minute = 60,000 milliseconds
}

void Timer::setTargetHours(unsigned long hours) {
    if(debug){
        Serial.println("[TIMER]: Setting target hours");
    }
    targetDuration = hours * 3600000;  // 1 hour = 3,600,000 milliseconds
}

bool Timer::hasReachedTarget() {
    if(debug){
        Serial.println("[TIMER]: Checking if timer has reached target : " + String(elapsed() >= targetDuration));
    }
    return elapsed() >= targetDuration;
}

unsigned long Timer::remainingTime() {
    if(debug){
        Serial.println("[TIMER]: Getting remaining time : " + String(targetDuration - elapsed()) + " milliseconds");
    }
    if (elapsed() < targetDuration) {
        return targetDuration - elapsed();
    }
    return 0;  // No time left
}