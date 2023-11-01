# iostream Class

The `iostream` class is a C++ class designed for simple input and output operations in embedded systems using Arduino. It provides methods for printing messages to the serial monitor, LCD displays (both standard and I2C), and saving/loading data to/from an SD card. This class aims to simplify common input and output tasks when working with Arduino-based projects.

## Features

- Print messages to the serial monitor.
- Print messages to standard LCD displays (LiquidCrystal).
- Print messages to I2C-enabled LCD displays (LiquidCrystal_I2C).
- Clear LCD display lines before printing (optional).
- Save data to a file on an SD card.
- Load data from a file on an SD card.

## Usage

To use the `iostream` class, follow these steps:

1. Include the necessary libraries (`Arduino.h`, `LiquidCrystal_I2C.h`, `LiquidCrystal.h`, `Wire.h`, `SD.h`) in your Arduino sketch.
2. Create an instance of the `iostream` class to perform input and output operations.
3. Use the provided methods to perform specific input and output tasks, such as printing messages or saving/loading data.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/iostream.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <iostream.h> 
```

## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1: 
* Dependency Update: Issues with SD, Wire, and SPI
### Version 1.0.2: (TO COME SOON)
* Update to README

    

## Example

Here's an example of how to use the `iostream` class for basic input and output operations:

```cpp
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SD.h>
#include "iostream.h"

// Create an instance of the iostream class
iostream cout;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize SD card
    if (!SD.begin(10)) {
        cout.println("SD card initialization failed!");
        return;
    }

    // Initialize LCD display
    LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address and dimensions as needed
    lcd.init();
    lcd.backlight();

    // Print messages to various outputs
    cout.print("Hello, Serial Monitor!");
    cout.printLCD("Hello, LCD!", lcd, 0, true);
    cout.printI2CLCD("This message is too long for the LCD and will scroll.", lcd, 1, true);

    // Save data to a file
    bool saved = cout.saveToFile("data.txt", "This is sample data.");
    if (saved) {
        cout.println("Data saved to file successfully.");
    } else {
        cout.println("Failed to save data to file.");
    }

    // Load data from a file
    char buffer[128];
    bool loaded = cout.loadFromFile("data.txt", buffer, sizeof(buffer));
    if (loaded) {
        cout.print("Loaded data from file: ");
        cout.println(buffer);
    } else {
        cout.println("Failed to load data from file.");
    }
}

void loop() {
    // Your main code here
}

```
