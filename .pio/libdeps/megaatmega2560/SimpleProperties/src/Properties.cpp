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
 * @brief exists (Check if the key exists in the properties)
 * 
 * @param key (Variable Name)
 * 
 * @details This method returns true if the table contains the given key, false otherwise.
 * @return bool
*/
bool Properties::exists(const String& key) {
    return table.get(key) != nullptr;
}

/**
 * @brief exists (Check if the key exists in the properties)
 * 
 * @overload
 * 
 * @param key (Variable Name)
 * @param value (Variable Value)
 * 
 * @details This method returns true if the table contains the given key with the given value, false otherwise.
 * @return bool
*/
bool Properties::exists(const String& key, const String& value) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return false;
    }
    return *valuePtr == value;
}

/**
 * @brief Begin
 * 
 * @param identifierType (Identifier Type)
 * 
 * @details This method sets the identifier type for the properties. The identifier type is used to determine the type of the property separator.
 * @return void
*/
void Properties::identify(const IDENTIFIERTYPE identifierType = EQUALS) {
    this->identifierType = identifierType;
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
                if(identifierType == IDENTIFIERTYPE::EQUALS){
                    String line = it.key() + "=" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::COLEN){
                    String line = it.key() + ":" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::SEMICOLEN){
                    String line = it.key() + ";" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::HYPHEN){
                    String line = it.key() + "-" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::COMMA){
                    String line = it.key() + "," + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::FORWARD_SLASH){
                    String line = it.key() + "/" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::BACKWARD_SLASH){
                    String line = it.key() + "\\" + it.value() + "\n";
                    file.print(line);
                }
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
            int separatorIndex;
            if(identifierType == IDENTIFIERTYPE::EQUALS){
                separatorIndex = line.indexOf('=');
            } else if(identifierType == IDENTIFIERTYPE::COLEN){
                separatorIndex = line.indexOf(':');
            } else if(identifierType == IDENTIFIERTYPE::SEMICOLEN){
                separatorIndex = line.indexOf(';');
            } else if(identifierType == IDENTIFIERTYPE::HYPHEN){
                separatorIndex = line.indexOf('-');
            } else if(identifierType == IDENTIFIERTYPE::COMMA){
                separatorIndex = line.indexOf(',');
            } else if(identifierType == IDENTIFIERTYPE::FORWARD_SLASH){
                separatorIndex = line.indexOf('/');
            } else if(identifierType == IDENTIFIERTYPE::BACKWARD_SLASH){
                separatorIndex = line.indexOf('\\');
            }
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
                if(identifierType == IDENTIFIERTYPE::EQUALS){
                    String line = it.key() + "=" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::COLEN){
                    String line = it.key() + ":" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::SEMICOLEN){
                    String line = it.key() + ";" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::HYPHEN){
                    String line = it.key() + "-" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::COMMA){
                    String line = it.key() + "," + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::FORWARD_SLASH){
                    String line = it.key() + "/" + it.value() + "\n";
                    file.print(line);
                } else if(identifierType == IDENTIFIERTYPE::BACKWARD_SLASH){
                    String line = it.key() + "\\" + it.value() + "\n";
                    file.print(line);
                }
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

/**
 * @brief storeToXML (Store the properties to an XML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in XML format with the given comments.
 * @return bool
*/
bool Properties::storeToXML(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file.print("<!-- " + comments + " -->\n");
        file.print("<properties>\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print("  <property>\n");
                file.print("    <key>" + it.key() + "</key>\n");
                file.print("    <value>" + it.value() + "</value>\n");
                file.print("  </property>\n");
            }else {
                break;
            }
        }
        file.print("</properties>\n");
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief loadFromXML (Load the properties from an XML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in XML format.
 * @return bool
*/
bool Properties::loadFromXML(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_READ);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int keyStartIndex = line.indexOf("<key>");
            int keyEndIndex = line.indexOf("</key>");
            int valueStartIndex = line.indexOf("<value>");
            int valueEndIndex = line.indexOf("</value>");
            if (keyStartIndex != -1 && keyEndIndex != -1 && valueStartIndex != -1 && valueEndIndex != -1) {
                String key = line.substring(keyStartIndex + 5, keyEndIndex);
                String value = line.substring(valueStartIndex + 7, valueEndIndex);
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief storeToMsgPack (Store the properties to a MsgPack file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in MsgPack format with the given comments.
 * @return bool
*/
bool Properties::storeToMsgPack(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + ":" + it.value() + "\n");
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
 * @brief loadFromMsgPack (Load the properties from a MsgPack file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in MsgPack format.
 * @return bool
*/
bool Properties::loadFromMsgPack(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_READ);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int separatorIndex = line.indexOf(':');
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
 * @brief storeToTOML (Store the properties to a TOML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in TOML format with the given comments.
 * @return bool
*/
bool Properties::storeToTOML(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + " = \"" + it.value() + "\"\n");
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
 * @brief loadFromTOML (Load the properties from a TOML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in TOML format.
 * @return bool
*/
bool Properties::loadFromTOML(const String& filename) {
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
                String value = line.substring(separatorIndex + 3, line.length() - 2);
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief storeToCSV (Store the properties to a CSV file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in CSV format with the given comments.
 * @return bool
*/
bool Properties::storeToCSV(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + "," + it.value() + "\n");
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
 * @brief loadFromCSV (Load the properties from a CSV file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in CSV format.
 * @return bool
*/
bool Properties::loadFromCSV(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_READ);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int separatorIndex = line.indexOf(',');
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
 * @brief storeToJSON (Store the properties to a JSON file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in JSON format with the given comments.
 * @return bool
*/
bool Properties::storeToJSON(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("{\n");
        file.print("  \"comments\": \"" + comments + "\",\n");
        file.print("  \"properties\": [\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print("    {\n");
                file.print("      \"key\": \"" + it.key() + "\",\n");
                file.print("      \"value\": \"" + it.value() + "\"\n");
                file.print("    }");
                if (it != end()) {
                    file.print(",");
                }
                file.print("\n");
            }else {
                break;
            }
        }
        file.print("  ]\n");
        file.print("}\n");
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief loadFromJSON (Load the properties from a JSON file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in JSON format.
 * @return bool
*/
bool Properties::loadFromJSON(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_READ);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int keyStartIndex = line.indexOf("\"key\": \"");
            int keyEndIndex = line.indexOf("\",");
            int valueStartIndex = line.indexOf("\"value\": \"");
            int valueEndIndex = line.indexOf("\"", valueStartIndex + 9);
            if (keyStartIndex != -1 && keyEndIndex != -1 && valueStartIndex != -1 && valueEndIndex != -1) {
                String key = line.substring(keyStartIndex + 8, keyEndIndex);
                String value = line.substring(valueStartIndex + 9, valueEndIndex);
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief storeToYAML (Store the properties to a YAML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in YAML format with the given comments.
 * @return bool
*/
bool Properties::storeToYAML(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + ": " + it.value() + "\n");
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
 * @brief loadFromYAML (Load the properties from a YAML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in YAML format.
 * @return bool
*/
bool Properties::loadFromYAML(const String& filename) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_READ);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            int separatorIndex = line.indexOf(':');
            if (separatorIndex != -1) {
                String key = line.substring(0, separatorIndex);
                String value = line.substring(separatorIndex + 2);
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief storeToINI (Store the properties to an INI file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the properties to the given file name on the SD card in INI format with the given comments.
 * @return bool
*/
bool Properties::storeToINI(const String& filename, const String& comments) {
    if (!SD.begin(4)) {
        return false;
    }
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("; " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + " = " + it.value() + "\n");
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
 * @brief loadFromINI (Load the properties from an INI file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the properties from the given file name on the SD card in INI format.
 * @return bool
*/
bool Properties::loadFromINI(const String& filename) {
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
                String value = line.substring(separatorIndex + 2);
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}