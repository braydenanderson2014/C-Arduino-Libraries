#include "Properties.h"
#include <Hashtable.h>
#include <Arduino.h>
#include <SD.h>

Properties::Properties() {}

Properties::~Properties() {
    table.clear();
}

void Properties::setProperty(const String& key, const String& value) {
    table.put(key, value);
}

// Overloaded setProperty
void Properties::setProperty(const String& key, const String& value, const String& filePath) {
    loadFromSD(filePath);
    table.put(key, value);
    saveToSD(filePath);
}

String Properties::getProperty(const String& key) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        Serial.println("Error: Property with key '" + key + "' not found.");
        return "[ERROR]";
    }
    return *valuePtr;
}

String Properties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
    loadFromSD(filePath);
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return defaultValue;
    }
    return *valuePtr;
}

void Properties::removeProperty(const String& key) {
    table.remove(key);
}

void Properties::clear() {
    table.clear();
}

int Properties::size() {
    return table.size();
}

int Properties::elements() {
    return table.elements();
}

bool Properties::isEmpty() {
    return table.isEmpty();
}

void Properties::saveToSD(const String& filename) {
    if (!SD.begin(4)) {
        Serial.println("Failed to initialize SD card.");
        return;
    }

    // Remove the existing file if it exists to start fresh
    if (SD.exists(filename.c_str())) {
        if (!SD.remove(filename.c_str())) {
            Serial.println("Error removing existing file.");
            return;
        }
    }

    // Create a new file or replace the old one
    File file = SD.open(filename.c_str(), FILE_WRITE);

    if (file) {
        // Iterate through the properties using the custom iterator and write them to the file
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                String line = it.key() + "=" + it.value() + "\n";
                file.print(line);
                Serial.println("Writing Line to file: " + line);
            }else {
                break;
            }
        }
        file.close(); // Make sure to close the file to save the data
        Serial.println("Properties saved successfully.");
    } else {
        Serial.println("Error opening file for writing.");
    }
}




void Properties::loadFromSD(const String& filename) {
    if (!SD.begin(4)) {
        Serial.println("Failed to initialize SD card.");
        return;
    }
    File file = SD.open(filename.c_str(), FILE_READ);

    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int separatorIndex = line.indexOf('=');
            if (separatorIndex != -1) {
                String key = line.substring(0, separatorIndex);
                String value = line.substring(separatorIndex + 1);
                table.put(key, value);
            }
        }
        file.close();
    } else {
        Serial.println("Error opening file for reading.");
    }
}

bool Properties::containsKey(const String& key) {
    return table.get(key) != nullptr;
}
