#include <Timer.h>

Timer myTimer;  // Create an instance of the Timer class

void setup() {
    Serial.begin(9600);
    myTimer.setTargetMinutes(5);  // Set the target duration to 5 minutes
    myTimer.start();  // Start the timer
}

void loop() {
    if (myTimer.checkTimer(5000)) {  // Check if 5 seconds have elapsed
        Serial.println("5 seconds have elapsed!");
        myTimer.reset();  // Reset the timer
    }

    if (myTimer.isTimerRunning()) {  // Check if the timer is running
        Serial.println("Timer is running");
    } else if (myTimer.isTimerPaused()) {  // Check if the timer is paused
        Serial.println("Timer is paused");
    } else {
        Serial.println("Timer is stopped");
    }

    if (myTimer.hasReachedTarget()) {  // Check if the target duration has been reached
        Serial.println("Target duration has been reached!");
    }

    unsigned long remaining = myTimer.remainingTime();  // Get the remaining time until the target duration is reached
    Serial.print("Remaining time: ");
    Serial.println(remaining);

    DateTime now = myTimer.getRTCTime();  // Get the current time from the RTC
    Serial.print("Current time: ");
    Serial.print(now.year());
    Serial.print("-");
    Serial.print(now.month());
    Serial.print("-");
    Serial.print(now.day());
    Serial.print(" ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    myTimer.setTargetMinutes(1);  // Set the target duration to 1 minute
    myTimer.pause();  // Pause the timer
    delay(5000);  // Wait for 5 seconds
    myTimer.resume();  // Resume the timer
    delay(60000);  // Wait for 1 minute
    myTimer.stop();  // Stop the timer
    myTimer.syncWithRTC();  // Sync the elapsed time with the RTC
    myTimer.setRTCTime(2022, 1, 1, 0, 0, 0);  // Set the RTC time to January 1, 2022 at midnight
    myTimer.clear();  // Clear the timer
}