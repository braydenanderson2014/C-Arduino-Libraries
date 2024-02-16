#include "Properties.h"
#include "Hashtable.h"
#include <Arduino.h>
#include <SD.h>

/**
 * @brief Construct a new Properties:: Properties object <String, String>
*/
Properties::Properties() {}

/**
 * @brief Destroy the Properties:: Properties object
*/
Properties::~Properties() {
    table.clear();
}

/**
 * @brief Set the Property object
 * 
 * @param key (Variable Name)
 * @param value (Variable Value)
 * 
 * @details This method sets the property with the given key to the given value.
 * @return void
*/
void Properties::setProperty(const String& key, const String& value) {
    table.put(key, value);
}

/**
 * @brief Set the Property object
 * 
 * @overload 
 * 
 * @param key (Variable Name)
 * @param value (Variable Value)
 * @param filePath (File Path)
 * 
 * @details This method sets the property with the given key to the given value and saves the properties to the given file path.
 * @return void
*/
void Properties::setProperty(const String& key, const String& value, const String& filePath) {
    loadFromSD(filePath);
    table.put(key, value);
    saveToSD(filePath);
}

/**
 * @brief Get the Property object
 * 
 * @param key (Variable Name)
 * 
 * @details This method returns the value of the property with the given key.
 * @return String
*/
String Properties::getProperty(const String& key) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return "[SIMPLE PROPERTIES]: Property with key '" + key + "' not found.";
    }
    return *valuePtr;
}

/**
 * @brief Get the Property object
 * 
 * @overload 
 * 
 * @param key (Variable Name)
 * @param defaultValue (Default Value)
 * @param filePath (File Path)
 * 
 * @details This method returns the value of the property with the given key. If the property does not exist, it returns the given default value.
 * @return String
*/
String Properties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
    loadFromSD(filePath);
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return defaultValue;
    }
    return *valuePtr;
}

/**
 * @brief Remove the Property object
 * 
 * @param key (Variable Name)
 * 
 * @details This method removes the property with the given key.
 * @return void
*/
void Properties::removeProperty(const String& key) {
    table.remove(key);
}

/**
 * @brief Clear the Properties object
 * 
 * @details This method removes all properties from the table.
 * @return void
*/
void Properties::clear() {
    table.clear();
}

/**
 * @brief Get the Size object
 * 
 * @details This method returns the current capacity of the table.
 * @return int
*/
int Properties::size() {
    return table.size();
}

/**
 * @brief Get the Elements object
 * 
 * @details This method returns the number of elements in the table.
 * @return int
*/
int Properties::elements() {
    return table.elements();
}

/**
 * @brief Is Empty
 * 
 * @details This method returns true if the table is empty, false otherwise.
 * @return bool
*/
bool Properties::isEmpty() {
    return table.isEmpty();
}

/**
 * @brief Save To SD
 * 
 * @param filename (File Name)
 * 
 * @details This method saves the properties to the given file name on the SD card.
 * @return bool
*/
bool Properties::saveToSD(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }

    // Remove the existing file if it exists to start fresh
    if (SD.exists(filename.c_str())) {
        if (!SD.remove(filename.c_str())) {
            return false;
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
            }else {
                break;
            }
        }
        file.close(); // Make sure to close the file to save the data
        return true;
    } 
    return false;
}

/**
 * @brief Load From SD
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card.
 * @return bool
*/
bool Properties::loadFromSD(const String& filename) {
    if (!SD.begin(4)) {
        return false;
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
        return true;
    } 
    return false;
}

/**
 * @brief Save
 * 
 * @param filename (File Name)
 * 
 * @details This method calls the saveToSD method with the given file name. It is a wrapper method to provide a more intuitive API.
 * @return bool
*/
bool Properties::save(const String& filename){
    return saveToSD(filename);
}

/**
 * @brief Load
 * 
 * @param filename (File Name)
 * 
 * @details This method calls the loadFromSD method with the given file name. It is a wrapper method to provide a more intuitive API.
 * @return bool
*/
bool Properties::load(const String& filename){
    return loadFromSD(filename);
}

/**
 * @brief store the Properties object (Similar to save, but with comments)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This function stores the properties to the given file name on the SD card with the given comments. Similar to the save method, but with comments.
 * @return bool
*/
bool Properties::store(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("#" + String(millis()) + "\n");
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                String line = it.key() + "=" + it.value() + "\n";
                file.print(line);
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief containsKey (Check if the key exists in the properties)
 * 
 * @param key (Variable Name)
 * 
 * @details This method returns true if the table contains the given key, false otherwise.
 * @return bool
*/
bool Properties::containsKey(const String& key) {
    return table.get(key) != nullptr;
}

/**
 * @brief deleteFile (Delete the file from the SD card)
 * 
 * @param filename (File Name)
 * 
 * @details This method deletes the file with the given name from the SD card.
 * @return bool
*/
bool Properties::deleteFile(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    if (SD.exists(filename.c_str())) {
        if (!SD.remove(filename.c_str())) {
            return false;
        }
    }
    return true;
}