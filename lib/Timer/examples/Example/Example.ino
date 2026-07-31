#include <Arduino.h>
#include "SimpleArduinoTimer.h"

// ---------------------------------------------------------------------------
// Non-blocking example: all timer sequences are driven from loop() using a
// simple state machine so the MCU is never blocked inside setup().
// ---------------------------------------------------------------------------

enum AppState {
    STATE_5MIN_RUN,
    STATE_5MIN_DONE,
    STATE_5MIN_PRINT_RUN,
    STATE_5MIN_PRINT_DONE,
    STATE_10SEC_RUN,
    STATE_10SEC_DONE,
    STATE_2HR_RUN,
    STATE_2HR_DONE,
    STATE_IDLE
};

Timer timer(false);
AppState state = STATE_5MIN_RUN;
unsigned long nextPrint = 0;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    // --- 5-minute countdown, print remaining every 10 seconds ---
    timer.setTargetMinutes(5);
    timer.start();
    nextPrint = 10000; // first print at 10 s
}

void loop()
{
    switch (state) {
        case STATE_5MIN_RUN:
            if (timer.checkTimer(nextPrint)) {
                // remainingTime() auto-scales: returns hours, minutes, or seconds
                // depending on how much time is left.
                Serial.println(String(timer.remainingTime()));
                nextPrint += 10000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("Timer has reached target duration!");
                state = STATE_5MIN_DONE;
            }
            break;

        case STATE_5MIN_DONE:
            // --- Same 5-minute target again using printTimeRemaining() ---
            timer.clear();
            timer.setTargetMinutes(5);
            timer.start();
            nextPrint = 1000; // first print at 1 s
            state = STATE_5MIN_PRINT_RUN;
            break;

        case STATE_5MIN_PRINT_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 1000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("Timer has reached target duration!");
                state = STATE_5MIN_PRINT_DONE;
            }
            break;

        case STATE_5MIN_PRINT_DONE:
            // --- 10-second countdown ---
            timer.clear();
            timer.setTargetSeconds(10);
            timer.start();
            nextPrint = 1000;
            state = STATE_10SEC_RUN;
            break;

        case STATE_10SEC_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 1000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("Timer has reached target duration!");
                state = STATE_10SEC_DONE;
            }
            break;

        case STATE_10SEC_DONE:
            // --- 2-hour countdown, print every minute ---
            timer.clear();
            timer.setTargetHours(2);
            timer.start();
            nextPrint = 60000; // first print at 1 min
            state = STATE_2HR_RUN;
            break;

        case STATE_2HR_RUN:
            if (timer.checkTimer(nextPrint)) {
                timer.printTimeRemaining();
                nextPrint += 60000;
            }
            if (timer.hasReachedTarget()) {
                Serial.println("Timer has reached target duration!");
                state = STATE_2HR_DONE;
            }
            break;

        case STATE_2HR_DONE:
            timer.clear();
            state = STATE_IDLE;
            break;

        case STATE_IDLE:
            // All countdowns complete; MCU continues running other tasks.
            // Other available operations:
            //   timer.pause();               // Pause the timer
            //   timer.resume();              // Resume the timer
            //   timer.stop();                // Stop and accumulate elapsed time
            //   timer.reset();               // Zero elapsed time (only when stopped)
            //   timer.clear();               // Unconditionally reset all timer state
            //   timer.remainingTimeMillis(); // Raw milliseconds remaining
            break;
    }
}
