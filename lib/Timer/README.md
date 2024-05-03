# Timer Class

The `Timer` class is a C++ class for managing time-related operations and timers. It allows you to create, start, stop, reset, and pause timers, making it useful for various timing and scheduling tasks in your Arduino projects.

## Features

- Start, stop, reset, and pause timers.
- Set target durations and check if a timer has reached its target.
- Synchronize with an RTC (Real-Time Clock) module.
- Get the current RTC time.
- Manage both running and paused states of timers.

## Usage

To use the `Timer` class, follow these steps:

1. Include the necessary header file (`Timer.h`) in your C++ program.
2. Create an instance of the `Timer` class.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Timer.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <Timer.h> 
```
# Arduino
## Changelog
### Version 1.0.0:
* Initial Untested Release
### Version 1.0.1 BETA:
* Tested, Fixed Version
* Fixes All known bugs at this time. This library has had a few tests now performed and is now working for basic timer functionality.
* Added the ability to name a timer. Though strictly not necessary, this allows more creative function.
* Added setTargetSeconds() so you can set a timer in seconds.
* Added printTimeRemaing() so you dont have to create your own interface if you dont want to.
* New setTimerName and getTimerName functions.
* Fixed the remainingTime function. [WARNING]: This function behaves differently then you might expect. It will auto adjust the unit of measurement being used without letting you know. This is a feature and not a bug.
* Moved RTC functionality into conditional. You need to define the ```cpp #define useRTCModule``` in order to use the rtc functions. [WARNING]: RTC FUNCTIONALITY IS NOT TESTED AND MAY NOT WORK AS INTENDED.
* [WARNING]: The pause and resume functionality have not been tested.
  


# Platformio 
## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Update to Libary.json
### Version 1.0.2:
* Added Debug Statements [TIMER]: 
### Version 1.0.3:
* Modified constructor to take in a ```bool```cpp to allow you to set whether or not the library displays debug messages.
### Version 1.0.4:
* Renamed Header File.
* Added new Functions setTargetHours(), setUseRTC, getUseRTC, begin(). Please note, unless you call the begin() function, the rtc will not function.the begin() function will try and start the rtc. if it fails it will remain using the millis() function instead.
* Modified some Functions to swap between millis() and the rtc Functions based off of the useRTC boolean.
### Version 1.0.5:
* Fixes All known bugs at this time. This library has had a few tests now performed and is now working for basic timer functionality.
* Added the ability to name a timer. Though strictly not necessary, this allows more creative function.
* Added setTargetSeconds() so you can set a timer in seconds.
* Added printTimeRemaing() so you dont have to create your own interface if you dont want to.
* New setTimerName and getTimerName functions.
* Fixed the remainingTime function. [WARNING]: This function behaves differently then you might expect. It will auto adjust the unit of measurement being used without letting you know. This is a feature and not a bug.
* Moved RTC functionality into conditional. You need to define the ```cpp #define useRTCModule``` in order to use the rtc functions. [WARNING]: RTC FUNCTIONALITY IS NOT TESTED AND MAY NOT WORK AS INTENDED.
* [WARNING]: The pause and resume functionality have not been tested.
### Version 1.0.6:
* Adjusted serial statements to now say the timers name instead of the timer class name.
* Fixed Documentation.
    

Here's an example of how to use the `Timer` class:

```cpp
#include <Arduino.h>
#include "Timer.h"

Timer myTimer;

void setup() {
    Serial.begin(9600);
    myTimer.setTargetMinutes(5);  // Set the target time to 5 minutes
    myTimer.start();  // Start the timer
}

void loop() {
    if (myTimer.hasReachedTarget()) {
        Serial.println("Timer has reached the target duration.");
        // Perform actions when the timer reaches the target.
        // You can also stop or reset the timer as needed.
    }
}
//SEE the Example.ino for a more detailed example.
```