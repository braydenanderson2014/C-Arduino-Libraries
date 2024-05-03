#include "SimpleArduinoTimer.h"

#ifdef useRTCModule
    #include <RTClib.h>
    
RTC_DS3231 timerRTC; // Add an instance of the RTC library
DateTime timerNow = timerRTC.now();
#endif



Timer::Timer(bool debug = false) : startTime(0), elapsedTime(0), pauseTime(0), targetDuration(0), isRunning(false), isPaused(false){
  setTimerName("Timer");
  useRTC = false;
}

void Timer::setTimerName(String timerName){
    this ->TimerName = timerName;
}

String Timer::getTimerName() const{
    return TimerName;
}

#ifdef useRTCModule
void Timer::begin() {
    if(!rtc.begin()){
        useRTC = false;
    }
    useRTC = true;
}

void Timer::setUseRTC(bool useRTC) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting useRTC : " + String(useRTC));
    }
    this->useRTC = useRTC;
}

bool Timer::getUseRTC() const {
    if(debug){
        Serial.println("[" + TimerName + "]: Getting useRTC : " + String(useRTC));
    }
    return useRTC;
}
#endif
void Timer::start() {
    if(debug){
        Serial.println("[" + TimerName + "]: Starting timer");
    }
    if (!isRunning && !isPaused) {
        if(useRTC){
            #ifdef useRTCModule
            syncWithRTC();
            startTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
            #endif
        }else{
            startTime = millis();
        }
        isRunning = true;
    }
}

void Timer::stop() {
    if(debug){
        Serial.println("[" + TimerName + "]: Stopping timer");
    }
    if (isRunning) {
        if(useRTC){
            #ifdef useRTCModule

            syncWithRTC();
            elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
            #endif
        }else{
            elapsedTime += millis() - startTime;
        }
        isRunning = false;
    }
}

void Timer::reset() {
    if(debug){
        Serial.println("[" + TimerName + "]: Resetting timer");
    }
    if (!isRunning) {
        elapsedTime = 0;
    }
}

void Timer::clear() {
    if(debug){
        Serial.println("[" + TimerName + "]: Clearing timer");
    }
    elapsedTime = 0;
    startTime = 0;
    pauseTime = 0;
    isRunning = false;
    isPaused = false;
}

void Timer::pause() {
    if(debug){
        Serial.println("[" + TimerName + "]: Pausing timer");
    }
    if (isRunning && !isPaused) {
        if(useRTC){
            #ifdef useRTCModule
            syncWithRTC();
            pauseTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
            #endif
        }else{
            pauseTime = millis();
        }
        isPaused = true;
        isRunning = false;
        if(debug){
            Serial.println("[" + TimerName + "]: Paused timer");
        }
    } else {
        if(debug){
            Serial.println("[" + TimerName + "]: Timer is not running");
        }
    }
}

void Timer::resume() {
    if(debug){
        Serial.println("[" + TimerName + "]: Resuming timer");
    }
    if (isPaused) {
        if(useRTC){
            #ifdef useRTCModule
            syncWithRTC();
            startTime += (timerNow.unixtime() * 1000 - pauseTime); // Adjusting the start time so that paused duration isn't counted.
            #endif
        }else{
            startTime += (millis() - pauseTime); // Adjusting the start time so that paused duration isn't counted.
        }
        isRunning = true;
        isPaused = false;
        if(debug){
            Serial.println("[" + TimerName + "]: Resumed timer");
        }
    } else {
        if(debug){
            Serial.println("[" + TimerName + "]: Timer is not paused");
        }
    }
}

unsigned long Timer::elapsed() {
    if(debug){
        Serial.println("[" + TimerName + "]: Getting elapsed time");
    }
    if (isRunning) {
        if(debug){
            Serial.println("[" + TimerName + "]: (Timer Running) Elapsed Time: " + String(elapsedTime + (millis() - startTime)) + " milliseconds");
        }
        if(useRTC){
            #ifdef useRTCModule
            syncWithRTC();
            return elapsedTime + (timerNow.unixtime() * 1000 - startTime);
            #else
            return elapsedTime + (millis() - startTime);
            #endif
        }else{
            return elapsedTime + (millis() - startTime);
        }
    } else if (isPaused) {
        if(debug){
            Serial.println("[" + TimerName + "]: (Timer Paused) Elapsed Time: " + String(elapsedTime + (pauseTime - startTime)) + " milliseconds");
        }
        return elapsedTime + (pauseTime - startTime);
    }
    if(debug){
        Serial.println("[" + TimerName + "]: Elapsed Time: " + String(elapsedTime) + " milliseconds");
    }
    return elapsedTime;
}

bool Timer::isTimerRunning() const {
    if(debug){
        Serial.println("[" + TimerName + "]: Checking if timer is running : " + String(isRunning));
    }
    return isRunning;
}

bool Timer::checkTimer(unsigned long duration)  {
    if(debug){
        Serial.println("[" + TimerName + "]: Checking if timer has reached duration : " + String(elapsed() >= duration) + " milliseconds");
    }
    return elapsed() >= duration;
}

#ifdef useRTCModule
void Timer::syncWithRTC() {
    if(debug){
        Serial.println("[" + TimerName + "]: Syncing with RTC");
    }
    // Logic to sync with RTC
    timerNow = timerRTC.now();
    elapsedTime = timerNow.unixtime() * 1000; // Convert seconds to milliseconds
}

DateTime Timer::getRTCTime() const {
    if(debug){
        Serial.println("[" + TimerName + "]: Getting RTC time : " + String(timerRTC.now().timestamp()) + " milliseconds");
    }
    return timerRTC.now();
}
#endif
bool Timer::isTimerPaused() const { // Rename to isTimerPaused to avoid conflict
    if(debug){
        Serial.println("[" + TimerName + "]: Checking if timer is paused : " + String(isPaused));
    }
    return isPaused;
}
#ifdef useRTCModule
void Timer::setRTCTime(int year, int month, int day, int hour, int minute, int second) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting RTC time");
    }
    timerRTC.adjust(DateTime(year, month, day, hour, minute, second));
}
#endif
void Timer::setTargetDuration(unsigned long duration) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting target duration");
    }
    targetDuration = duration;
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetSeconds(unsigned long seconds) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting target seconds");
    }
    targetDuration = seconds * 1000;  // 1 second = 1,000 milliseconds
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetMinutes(unsigned long minutes) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting target minutes");
    }
    targetDuration = minutes * 60000;  // 1 minute = 60,000 milliseconds
    remainingTimeOnTimer = targetDuration;
}

void Timer::setTargetHours(unsigned long hours) {
    if(debug){
        Serial.println("[" + TimerName + "]: Setting target hours");
    }
    targetDuration = hours * 3600000;  // 1 hour = 3,600,000 milliseconds
    remainingTimeOnTimer = targetDuration;
}

bool Timer::hasReachedTarget() {
    if(debug){
        Serial.println("[" + TimerName + "]: Checking if timer has reached target : " + String(elapsed() >= targetDuration));
    }
    return elapsed() >= targetDuration;
}
unsigned long Timer::remainingTime() {
    unsigned long remainingMillis = targetDuration - elapsed(); // Calculate remaining milliseconds once

    // Determine the timer mode based on the remaining milliseconds
    if (remainingMillis >= 3600000) { // More than or equal to an hour left
        timerMode = Hours;
    } else if (remainingMillis >= 60000) { // More than or equal to a minute left
        timerMode = Minutes;
    } else { // Less than a minute left
        timerMode = Seconds;
    }

    // Calculate remaining time based on the timer mode
    unsigned long remainingTimeOnTimer = 0;
    switch (timerMode) {
        case Hours:
            remainingTimeOnTimer = remainingMillis / 3600000; // Convert milliseconds to hours
            break;
        case Minutes:
            remainingTimeOnTimer = remainingMillis / 60000; // Convert milliseconds to minutes
            break;
        case Seconds:
            remainingTimeOnTimer = remainingMillis / 1000; // Convert milliseconds to seconds
            break;
    }

    return remainingTimeOnTimer;
}

void Timer::printTimeRemaining() {
    updateRemainingTime();  // Ensure the latest time is calculated before printing

    unsigned long hours = remainingTimeOnTimer / 3600; // Calculate hours
    unsigned long minutes = (remainingTimeOnTimer % 3600) / 60; // Calculate minutes
    unsigned long seconds = remainingTimeOnTimer % 60; // Calculate seconds

    // Decide the formatting based on the timer mode
    switch (timerMode) {
        case Hours:
            Serial.print("[" + TimerName + "] Remaining Time: ");
            Serial.print(hours);
            Serial.print(":");
            if (minutes < 10) Serial.print("0"); // Leading zero for minutes
            Serial.print(minutes);
            Serial.print(":");
            if (seconds < 10) Serial.print("0"); // Leading zero for seconds
            Serial.println(seconds);
            break;
        case Minutes:
            Serial.print("[" + TimerName + "] Remaining Time: ");
            Serial.print(minutes);
            Serial.print(":");
            if (seconds < 10) Serial.print("0"); // Leading zero for seconds
            Serial.println(seconds);
            break;
        case Seconds:
            Serial.print("[" + TimerName + "] Remaining Time: ");
            Serial.println(seconds);
            break;
    }
}


void Timer::updateRemainingTime() {
    unsigned long remainingMillis = targetDuration - elapsed(); // Calculate remaining milliseconds once

    // We need to convert milliseconds directly to total remaining seconds here
    remainingTimeOnTimer = remainingMillis / 1000; // Convert milliseconds to seconds
}
