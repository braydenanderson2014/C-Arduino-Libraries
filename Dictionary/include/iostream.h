#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SD.h>

class IOStream {
private:
    File file;

public:
    IOStream() {}

    bool openFile(const char* fileName, uint8_t  mode) {
        file = SD.open(fileName, mode);
        return file;
    }

    void closeFile() {
        file.close();
    }

    size_t write(const uint8_t* buffer, size_t size) {
        return file.write(buffer, size);
    }

    size_t write(const char* str) {
        return file.print(str);
    }

    size_t writeLine(const char* str) {
        size_t bytesWritten = file.println(str);
        return bytesWritten;
    }

    size_t writeFormatted(File& file, const char* format, ...) {
        char buffer[256]; // Adjust the buffer size as needed
        va_list args;
        va_start(args, format);
        int len = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        if (len > 0) {
            return file.write(reinterpret_cast<const uint8_t*>(buffer), len);
        } else {
            return 0; // Error occurred during formatting
        }
    }

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

extern IOStream cout; // Typically, cout is used for console output.
