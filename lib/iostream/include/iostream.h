#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SD.h>

class iostream {
public:
    void print(const char* str) {
        Serial.print(str);
    }
    
    void println(const char* str) {
        Serial.println(str);
    }
    
    void printLCD(const char* str, LiquidCrystal& lcd, int line, bool clear = false) {
        if (clear) {
            lcd.setCursor(0, line);
            lcd.print("                ");
        }
        lcd.setCursor(0, line);
        lcd.print(str);
    }

    void printI2CLCD(const char* str, LiquidCrystal_I2C& lcd, int line, bool clear = false) {
        if (clear) {
            lcd.setCursor(0, line);
            lcd.print("                ");
        }
        lcd.setCursor(0, line);
        lcd.print(str);
        if (strlen(str) > 16) {
            for (int i = 0; i < strlen(str) - 16; i++) {
                lcd.scrollDisplayLeft();
                delay(200);
            }
        }
    }

    bool saveToFile(const char* filename, const char* data) {
        File file = SD.open(filename, FILE_WRITE);
        if (!file) {
            return false;
        }
        file.print(data);
        file.close();
        return true;
    }
    
    // Load data from a file on the SD card
    bool loadFromFile(const char* filename, char* buffer, size_t bufferSize) {
        File file = SD.open(filename);
        if (!file) {
            return false;
        }
        size_t bytesRead = file.readBytes(buffer, bufferSize);
        buffer[bytesRead] = '\0';
        file.close();
        return true;
    }
    //... Add other I/O methods as needed
};

extern iostream cout; // Typically, cout is used for console output.
