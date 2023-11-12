#include "Properties.h"
#include <Hashtable.h>
#include <Arduino.h>
#include <SD.h>

Properties::Properties() {}

Properties::~Properties() {
    table.clear();
}

void Properties::setProperty(const String& key, const String& value) {
    Serial.println("[SIMPLE PROPERTIES]: Setting property: " + key + " = " + value);
    table.put(key, value);
}

// Overloaded setProperty
void Properties::setProperty(const String& key, const String& value, const String& filePath) {
    Serial.println("[SIMPLE PROPERTIES]: Setting property: " + key + " = " + value);
    loadFromSD(filePath);
    table.put(key, value);
    saveToSD(filePath);
}

String Properties::getProperty(const String& key) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        Serial.println("[SIMPLE PROPERTIES]: Property with key '" + key + "' not found.");
        return "[ERROR]";
    }
    return *valuePtr;
}

String Properties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
    loadFromSD(filePath);
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        Serial.println("[SIMPLE PROPERTIES]: Property with key '" + key + "' not found.");
        return defaultValue;
    }
    return *valuePtr;
}

void Properties::removeProperty(const String& key) {
    Serial.println("[SIMPLE PROPERTIES]: Removing property: " + key);
    table.remove(key);
}

void Properties::clear() {
    Serial.println("[SIMPLE PROPERTIES]: Clearing all properties.");
    table.clear();
}

int Properties::size() {
    Serial.println("[SIMPLE PROPERTIES]: Getting size of properties.");
    return table.size();
}

int Properties::elements() {
    Serial.println("[SIMPLE PROPERTIES]: Getting number of elements in properties.");
    return table.elements();
}

bool Properties::isEmpty() {
    Serial.println("[SIMPLE PROPERTIES]: Checking if properties are empty.");
    return table.isEmpty();
}

void Properties::saveToSD(const String& filename) {
    Serial.println("[SIMPLE PROPERTIES]: Saving properties to SD card.");
    if (!SD.begin(4)) {
        Serial.println("[SIMPLE PROPERTIES]: Failed to initialize SD card.");
        return;
    }

    // Remove the existing file if it exists to start fresh
    if (SD.exists(filename.c_str())) {
        if (!SD.remove(filename.c_str())) {
            Serial.println("[SIMPLE PROPERTIES]: Error removing existing file.");
            return;
        }
    }

    Serial.println("[SIMPLE PROPERTIES]: Creating new file.");
    // Create a new file or replace the old one
    File file = SD.open(filename.c_str(), FILE_WRITE);
    Serial.println("[SIMPLE PROPERTIES]: Writing properties to file.");
    if (file) {
        // Iterate through the properties using the custom iterator and write them to the file
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                String line = it.key() + "=" + it.value() + "\n";
                file.print(line);
                Serial.println("[SIMPLE PROPERTIES]: Writing Line to file: " + line);
            }else {
                break;
            }
        }
        file.close(); // Make sure to close the file to save the data
        Serial.println("[SIMPLE PROPERTIES]: Properties saved successfully.");
    } else {
        Serial.println("[SIMPLE PROPERTIES]: Error opening file for writing.");
    }
}


void Properties::loadFromSD(const String& filename) {
    Serial.println("[SIMPLE PROPERTIES]: Loading properties from SD card.");
    if (!SD.begin(4)) {
        Serial.println("[SIMPLE PROPERTIES]: Failed to initialize SD card.");
        return;
    }
    Serial.println("[SIMPLE PROPERTIES]: Opening file.");
    File file = SD.open(filename.c_str(), FILE_READ);
    Serial.println("[SIMPLE PROPERTIES]: Reading file.");
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int separatorIndex = line.indexOf('=');
            if (separatorIndex != -1) {
                String key = line.substring(0, separatorIndex);
                String value = line.substring(separatorIndex + 1);
                Serial.println("[SIMPLE PROPERTIES]: Loading property: " + key + " = " + value);
                table.put(key, value);
            }
        }
        file.close();
        Serial.println("[SIMPLE PROPERTIES]: Properties loaded successfully.");
    } else {
        Serial.println("[SIMPLE PROPERTIES]: Error opening file for reading.");
    }
}

bool Properties::containsKey(const String& key) {
    Serial.println("[SIMPLE PROPERTIES]: Checking if properties contain key: " + key);
    return table.get(key) != nullptr;
}
