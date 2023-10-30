#include "Properties.h"
#include "Hashtable.h"
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
    String value = table.get(key);
    if (value == "") { // Assuming an empty String indicates a lack of the property
        Serial.println("Error: Property with key '" + key + "' not found.");
        return "[ERROR]";  // Return a special error value
    }
    return value;
}

String Properties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
    loadFromSD(filePath);
    String value = table.get(key);
    if (value == "") { // Assuming an empty String indicates a lack of the property
        return defaultValue;
    }
    return value;
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

bool Properties::isEmpty() {
    return table.isEmpty();
}

void Properties::saveToSD(const String& filename) {
    if (!SD.begin(4)) {
        Serial.println("Failed to initialize SD card.");
        return;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);

    if (file) {
        for (auto& key : table.keys()) {  // Assuming you have a keys() method in Hashtable
            String line = key + "=" + table.get(key) + "\n";
            file.print(line);
        }
        file.close();
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
    return table.get(key) != "";
}
