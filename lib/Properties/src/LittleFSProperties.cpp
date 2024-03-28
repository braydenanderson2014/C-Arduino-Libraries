#include <LittleFSProperties.h>
#include <Hashtable.h>
#include <Arduino.h>
//#include <SD.h>
//#include <FS.h>
#ifdef useLittleFS
    #ifdef ESP32
        #include <LittleFS.h>
    #endif
    #ifdef ESPRESSIF32
        #include <LittleFS.h>
    #endif
    #ifdef ESP8266
        #include <LittleFSWrapper.h>
        #define useLittleFSWrapper
    #endif
    #ifdef ESP32S2
        #include <LittleFS.h>
    #endif
    #ifdef ESP32C3
        #include <LittleFS.h>
    #endif
    #ifndef ARDUINO_ARCH_MBED
        #include <MbedLittleFSWrapper.h>
    #endif
#else
    #include <SD.h>
    #define useSD
#endif


/**
 * @brief Construct a new LittleFSProperties:: LittleFSProperties object <String, String>
*/
LittleFSProperties::LittleFSProperties() {}

/**
 * @brief Destroy the LittleFSProperties:: LittleFSProperties object
*/
LittleFSProperties::~LittleFSProperties() {
    table.clear();
}

/**
 * @brief setChipSelect (Set the chip select pin)
 * 
 * @param cs (Chip Select)
 * 
 * @details This method sets the chip select pin for the SD card module.
 * @return void
*/
void LittleFSProperties::setChipSelect(const size_t cs) {
    chipSelect = cs;
}

/**
 * @brief getChipSelect (Get the chip select pin)
 * 
 * @details This method returns the chip select pin for the SD card module.
 * @return size_t
*/
size_t LittleFSProperties::getChipSelect() {
    return chipSelect;
}

/**
 * @brief exists (Check if the key exists in the LittleFSProperties)
 * 
 * @param key (Variable Name)
 * 
 * @details This method returns true if the table contains the given key, false otherwise.
 * @return bool
*/
bool LittleFSProperties::exists(const String& key) {
    return table.get(key) != nullptr;
}

/**
 * @brief exists (Check if the key exists in the LittleFSProperties)
 * 
 * @overload
 * 
 * @param key (Variable Name)
 * @param value (Variable Value)
 * 
 * @details This method returns true if the table contains the given key with the given value, false otherwise.
 * @return bool
*/
bool LittleFSProperties::exists(const String& key, const String& value) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return false;
    }
    return *valuePtr == value;
}


/**
 * @brief Begin
 * 
 * @details This method initializes the SD card module.
 * @attention This method must be called before any other method in the class!
 * @return bool
*/
#ifdef useSD
bool LittleFSProperties::beginSD(size_t cs = 4 , IDENTIFIERTYPE identifierType = EQUALS) {
    chipSelect = cs;
    this->identifierType = identifierType;
    if (!SD.begin(chipSelect)) {
        return false;
    }
    return true;
}
#endif

/**
 * @brief Begin LFS
 * 
 * @details This method initializes the LittleFS file system.
 * @attention This method must be called before any other method in the class!
 * @return bool
*/
#ifdef useLittleFS
bool LittleFSProperties::beginLFS() {
    if (!LITTLEFS.begin()) {
        return false;
    }
    return true;
}
#endif
#ifdef useLittleFSWrapper
bool LittleFSProperties::beginLFS() {
    if (!LittleFS.begin()) {
        return false;
    }
    return true;
}
#endif

/**
 * @brief Begin
 * 
 * @param identifierType (Identifier Type)
 * 
 * @details This method sets the identifier type for the LittleFSProperties. The identifier type is used to determine the type of the property separator.
 * @return void
*/
void LittleFSProperties::identify(const IDENTIFIERTYPE identifierType = EQUALS) {
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
void LittleFSProperties::setProperty(const String& key, const String& value) {
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
 * @details This method sets the property with the given key to the given value and saves the LittleFSProperties to the given file path.
 * @return void
*/
void LittleFSProperties::setProperty(const String& key, const String& value, const String& filePath) {
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
String LittleFSProperties::getProperty(const String& key) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return "[SIMPLE LittleFSProperties]: Property with key '" + key + "' not found.";
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
String LittleFSProperties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
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
void LittleFSProperties::removeProperty(const String& key) {
    table.remove(key);
}

/**
 * @brief Clear the LittleFSProperties object
 * 
 * @details This method removes all LittleFSProperties from the table.
 * @return void
*/
void LittleFSProperties::clear() {
    table.clear();
}

/**
 * @brief Get the Size object
 * 
 * @details This method returns the current capacity of the table.
 * @return int
*/
int LittleFSProperties::size() {
    return table.size();
}

/**
 * @brief Get the Elements object
 * 
 * @details This method returns the number of elements in the table.
 * @return int
*/
int LittleFSProperties::elements() {
    return table.elements();
}

/**
 * @brief Is Empty
 * 
 * @details This method returns true if the table is empty, false otherwise.
 * @return bool
*/
bool LittleFSProperties::isEmpty() {
    return table.isEmpty();
}

/**
 * @brief Save To SD
 * 
 * @param filename (File Name)
 * 
 * @details This method saves the LittleFSProperties to the given file name on the SD card.
 * @return bool
*/
bool LittleFSProperties::saveToSD(const String& filename) {
    #ifdef useSD
        beginSD(chipSelect, identifierType);

        // Remove the existing file if it exists to start fresh
        if (SD.exists(filename.c_str())) {
            if (!SD.remove(filename.c_str())) {
                return false;
            }
        }
        // Create a new file or replace the old one
        File file = SD.open(filename.c_str(), FILE_WRITE);
        if (file) {
            // Iterate through the LittleFSProperties using the custom iterator and write them to the file
            for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFS
        beginLFS();

        // Remove the existing file if it exists to start fresh
        if (LITTLEFS.exists(filename.c_str())) {
            if (!LITTLEFS.remove(filename.c_str())) {
                return false;
            }
        }

        // Create a new file or replace the old one
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        // Iterate through the LittleFSProperties using the custom iterator and write them to the file
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
        beginLFS();

        // Remove the existing file if it exists to start fresh
        if (LittleFS.exists(filename.c_str())) {
            if (!LittleFS.remove(filename.c_str())) {
                return false;
            }
        }
        // Create a new file or replace the old one
        File file = LittleFS.open(filename.c_str(), "w");
        if (file) {
            // Iterate through the LittleFSProperties using the custom iterator and write them to the file
            for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    
}

/**
 * @brief Load From SD
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card.
 * @return bool
*/
bool LittleFSProperties::loadFromSD(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_READ);;
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief Save
 * 
 * @param filename (File Name)
 * 
 * @details This method calls the saveToSD method with the given file name. It is a wrapper method to provide a more intuitive API.
 * @return bool
*/
bool LittleFSProperties::save(const String& filename){
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
bool LittleFSProperties::load(const String& filename){
    return loadFromSD(filename);
}

/**
 * @brief store the LittleFSProperties object (Similar to save, but with comments)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This function stores the LittleFSProperties to the given file name on the SD card with the given comments. Similar to the save method, but with comments.
 * @return bool
*/
bool LittleFSProperties::store(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("#" + String(millis()) + "\n");
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("#" + String(millis()) + "\n");
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("#" + String(millis()) + "\n");
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief containsKey (Check if the key exists in the LittleFSProperties)
 * 
 * @param key (Variable Name)
 * 
 * @details This method returns true if the table contains the given key, false otherwise.
 * @return bool
*/
bool LittleFSProperties::containsKey(const String& key) {
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
bool LittleFSProperties::deleteFile(const String& filename) {
    #ifdef useSD
        if(SD.exists(filename.c_str())){
            if(SD.remove(filename.c_str())){
                return true;
            }
            return false;
        }
    #endif
    #ifdef useLittleFS
    if (!LITTLEFS.begin()) {
        return false;
    }
    if (LITTLEFS.exists(filename.c_str())) {
        if (!LITTLEFS.remove(filename.c_str())) {
            return false;
        }
    }
    return true;
    #endif
}

/**
 * @brief storeToXML (Store the LittleFSProperties to an XML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in XML format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToXML(const String& filename, const String& comments) {
    #ifdef useSD
        beginSD(chipSelect, identifierType);
        File file = SD.open(filename.c_str(), FILE_WRITE);
        if (file) {
            file.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            file.print("<!-- " + comments + " -->\n");
            file.print("<LittleFSProperties>\n");
            for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
                if (it.value().length() > 0) { // Check that the string is not empty
                    file.print("  <property>\n");
                    file.print("    <key>" + it.key() + "</key>\n");
                    file.print("    <value>" + it.value() + "</value>\n");
                    file.print("  </property>\n");
                }else {
                    break;
                }
            }
            file.print("</LittleFSProperties>\n");
            file.close();
            return true;
        }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file.print("<!-- " + comments + " -->\n");
        file.print("<LittleFSProperties>\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print("  <property>\n");
                file.print("    <key>" + it.key() + "</key>\n");
                file.print("    <value>" + it.value() + "</value>\n");
                file.print("  </property>\n");
            }else {
                break;
            }
        }
        file.print("</LittleFSProperties>\n");
        file.close();
        return true;
    }
    return false;
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file.print("<!-- " + comments + " -->\n");
        file.print("<LittleFSProperties>\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print("  <property>\n");
                file.print("    <key>" + it.key() + "</key>\n");
                file.print("    <value>" + it.value() + "</value>\n");
                file.print("  </property>\n");
            }else {
                break;
            }
        }
        file.print("</LittleFSProperties>\n");
        file.close();
        return true;
    }
    return false;
    #endif
}

/**
 * @brief loadFromXML (Load the LittleFSProperties from an XML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in XML format.
 * @return bool
*/
bool LittleFSProperties::loadFromXML(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToMsgPack (Store the LittleFSProperties to a MsgPack file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in MsgPack format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToMsgPack(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + ":" + it.value() + "\n");
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromMsgPack (Load the LittleFSProperties from a MsgPack file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in MsgPack format.
 * @return bool
*/
bool LittleFSProperties::loadFromMsgPack(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToTOML (Store the LittleFSProperties to a TOML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in TOML format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToTOML(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + " = \"" + it.value() + "\"\n");
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromTOML (Load the LittleFSProperties from a TOML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in TOML format.
 * @return bool
*/
bool LittleFSProperties::loadFromTOML(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToCSV (Store the LittleFSProperties to a CSV file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in CSV format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToCSV(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + "," + it.value() + "\n");
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromCSV (Load the LittleFSProperties from a CSV file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in CSV format.
 * @return bool
*/
bool LittleFSProperties::loadFromCSV(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToJSON (Store the LittleFSProperties to a JSON file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in JSON format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToJSON(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("{\n");
        file.print("  \"comments\": \"" + comments + "\",\n");
        file.print("  \"LittleFSProperties\": [\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("{\n");
        file.print("  \"comments\": \"" + comments + "\",\n");
        file.print("  \"LittleFSProperties\": [\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("{\n");
        file.print("  \"comments\": \"" + comments + "\",\n");
        file.print("  \"LittleFSProperties\": [\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromJSON (Load the LittleFSProperties from a JSON file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in JSON format.
 * @return bool
*/
bool LittleFSProperties::loadFromJSON(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToYAML (Store the LittleFSProperties to a YAML file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in YAML format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToYAML(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + ": " + it.value() + "\n");
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("# " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromYAML (Load the LittleFSProperties from a YAML file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in YAML format.
 * @return bool
*/
bool LittleFSProperties::loadFromYAML(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}

/**
 * @brief storeToINI (Store the LittleFSProperties to an INI file)
 * 
 * @param filename (File Name)
 * @param comments (Comments)
 * 
 * @details This method stores the LittleFSProperties to the given file name on the SD card in INI format with the given comments.
 * @return bool
*/
bool LittleFSProperties::storeToINI(const String& filename, const String& comments) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);
    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (file) {
        file.print("; " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Check that the string is not empty
                file.print(it.key() + " = " + it.value() + "\n");
            }else {
                break;
            }
        }
        file.close();
        return true;
    }
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "w");
    if (file) {
        file.print("; " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "w");
    if (file) {
        file.print("; " + comments + "\n");
        for (LittleFSPropertiesIterator it = begin(); it != end(); ++it) {
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
    #endif
}

/**
 * @brief loadFromINI (Load the LittleFSProperties from an INI file)
 * 
 * @param filename (File Name)
 * 
 * @details This method loads the LittleFSProperties from the given file name on the SD card in INI format.
 * @return bool
*/
bool LittleFSProperties::loadFromINI(const String& filename) {
    #ifdef useSD
    beginSD(chipSelect, identifierType);    
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
    #endif
    #ifdef useLittleFS
    beginLFS();
    File file = LITTLEFS.open(filename.c_str(), "r");
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
    #endif
    #ifdef useLittleFSWrapper
    beginLFS();
    File file = LittleFS.open(filename.c_str(), "r");
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
    #endif
}