#include <Arduino.h>
#include "SimpleArduinoTimer.h"

Timer timer(false);

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    // --- 5-minute countdown, print remaining every 10 seconds ---
    timer.setTargetMinutes(5);
    timer.start();
    unsigned long nextPrint = 10000; // first print at 10 s
    while (!timer.hasReachedTarget()) {
        if (timer.checkTimer(nextPrint)) {
            // remainingTime() auto-scales: returns hours, minutes, or seconds
            // depending on how much time is left.
            Serial.println(String(timer.remainingTime()));
            nextPrint += 10000;
        }
    }
    Serial.println("Timer has reached target duration!");

    // --- Same 5-minute target again using printTimeRemaining() ---
    timer.clear();
    timer.setTargetMinutes(5);
    timer.start();
    nextPrint = 1000; // first print at 1 s
    while (!timer.hasReachedTarget()) {
        if (timer.checkTimer(nextPrint)) {
            timer.printTimeRemaining();
            nextPrint += 1000;
        }
    }
    Serial.println("Timer has reached target duration!");

    // --- 10-second countdown ---
    timer.clear();
    timer.setTargetSeconds(10);
    timer.start();
    nextPrint = 1000;
    while (!timer.hasReachedTarget()) {
        if (timer.checkTimer(nextPrint)) {
            timer.printTimeRemaining();
            nextPrint += 1000;
        }
    }
    Serial.println("Timer has reached target duration!");

    // --- 2-hour countdown, print every minute ---
    timer.clear();
    timer.setTargetHours(2);
    timer.start();
    nextPrint = 60000; // first print at 1 min
    while (!timer.hasReachedTarget()) {
        if (timer.checkTimer(nextPrint)) {
            timer.printTimeRemaining();
            nextPrint += 60000;
        }
    }
    Serial.println("Timer has reached target duration!");

    // Other available operations:
    //   timer.pause();    // Pause the timer
    //   timer.resume();   // Resume the timer
    //   timer.stop();     // Stop and accumulate elapsed time
    //   timer.reset();    // Zero elapsed time (only works when stopped)
    //   timer.clear();    // Unconditionally reset all timer state
    //   timer.remainingTimeMillis();  // Raw milliseconds remaining
}

void loop()
{
}
