#include <Properties.h>
#include "Hashtable.h"
#include <Arduino.h>
#include <SD.h>

// ─── Private helpers ─────────────────────────────────────────────────────────

/**
 * @brief Calls SD.begin(chipSelect) unless bypassSDBegin is set.
 * @return true if SD is ready (or bypass is active), false on SD init failure.
 */
bool Properties::beginSDIfNeeded() {
    if (_bypassSDBegin) return true;
    return SD.begin(chipSelect);
}

/**
 * @brief Returns the separator character corresponding to the current identifierType.
 */
char Properties::getSeparator() const {
    switch (identifierType) {
        case COLEN:          return ':';
        case SEMICOLEN:      return ';';
        case HYPHEN:         return '-';
        case COMMA:          return ',';
        case FORWARD_SLASH:  return '/';
        case BACKWARD_SLASH: return '\\';
        case EQUALS:
        default:             return '=';
    }
}

/**
 * @brief Prepends a '/' to the filename if it does not already start with one.
 *        Required by LittleFS and other fs::FS implementations that expect absolute paths.
 */
String Properties::normalizePath(const String& filename) const {
    if (filename.startsWith("/")) {
        return filename;
    }
    return "/" + filename;
}

/**
 * @brief Opens the given file for reading.
 *        Uses _filesystem when set; falls back to SD.
 */
File Properties::openForRead(const String& filename) {
    String path = normalizePath(filename);
    if (_filesystem != nullptr) {
        return _filesystem->open(path.c_str(), "r");
    }
    return SD.open(path.c_str(), FILE_READ);
}

/**
 * @brief Opens the given file for writing (truncate).
 *        Uses _filesystem when set; falls back to SD (removing any existing file first).
 */
File Properties::openForWrite(const String& filename) {
    String path = normalizePath(filename);
    if (_filesystem != nullptr) {
        return _filesystem->open(path.c_str(), "w");
    }
    // SD.open with FILE_WRITE may append; remove first to ensure truncation
    if (SD.exists(path.c_str())) {
        SD.remove(path.c_str());
    }
    return SD.open(path.c_str(), FILE_WRITE);
}

/**
 * @brief Returns true if the given file exists.
 *        Uses _filesystem when set; falls back to SD.
 */
bool Properties::fileExists(const String& filename) {
    String path = normalizePath(filename);
    if (_filesystem != nullptr) {
        return _filesystem->exists(path.c_str());
    }
    return SD.exists(path.c_str());
}

/**
 * @brief Removes the given file.
 *        Uses _filesystem when set; falls back to SD.
 */
bool Properties::removeFile(const String& filename) {
    String path = normalizePath(filename);
    if (_filesystem != nullptr) {
        return _filesystem->remove(path.c_str());
    }
    return SD.remove(path.c_str());
}

// ─── Constructors / destructor ────────────────────────────────────────────────

/**
 * @brief Default constructor — uses SD card (SD.begin() called per operation unless bypassed).
 */
Properties::Properties() {}

/**
 * @brief Constructor that injects an external filesystem (e.g. LittleFS, SPIFFS, SD_MMC).
 *        When this constructor is used, SD.begin() is never called internally.
 */
Properties::Properties(fs::FS& filesystem) : _filesystem(&filesystem) {}

/**
 * @brief Destructor.
 */
Properties::~Properties() {
    table.clear();
}

// ─── Configuration ────────────────────────────────────────────────────────────

/**
 * @brief Sets the chip select pin used when initialising the SD card internally.
 */
void Properties::setChipSelect(const size_t cs) {
    chipSelect = cs;
}

/**
 * @brief Returns the chip select pin.
 */
size_t Properties::getChipSelect() {
    return chipSelect;
}

/**
 * @brief When bypass is true, SD.begin() will NOT be called before SD operations.
 *        Use this when the calling application has already initialised the SD card.
 */
void Properties::setBypassSDBegin(bool bypass) {
    _bypassSDBegin = bypass;
}

/**
 * @brief Returns the current bypassSDBegin flag.
 */
bool Properties::getBypassSDBegin() {
    return _bypassSDBegin;
}

/**
 * @brief Sets (or replaces) the filesystem used for all file operations.
 *        After calling this, SD.begin() is never invoked internally.
 */
void Properties::setFilesystem(fs::FS& filesystem) {
    _filesystem = &filesystem;
}

/**
 * @brief Sets the separator character used when reading/writing properties files.
 *        If not called, the default is EQUALS (=).
 */
void Properties::identify(const IDENTIFIERTYPE identifierType) {
    this->identifierType = identifierType;
}

// ─── In-memory operations ─────────────────────────────────────────────────────

/**
 * @brief Returns true if the key exists in the table.
 */
bool Properties::exists(const String& key) {
    return table.get(key) != nullptr;
}

/**
 * @brief Returns true if the key exists in the table with the given value.
 */
bool Properties::exists(const String& key, const String& value) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return false;
    }
    return *valuePtr == value;
}

/**
 * @brief Sets a property in memory.
 */
void Properties::setProperty(const String& key, const String& value) {
    table.put(key, value);
}

/**
 * @brief Loads properties from filePath, sets key=value, then saves back to filePath.
 */
void Properties::setProperty(const String& key, const String& value, const String& filePath) {
    load(filePath);
    table.put(key, value);
    save(filePath);
}

/**
 * @brief Returns the value for the given key, or an error string if not found.
 */
String Properties::getProperty(const String& key) {
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return "[SIMPLE PROPERTIES]: Property with key '" + key + "' not found.";
    }
    return *valuePtr;
}

/**
 * @brief Loads properties from filePath, then returns the value for key (or defaultValue).
 */
String Properties::getProperty(const String& key, const String& defaultValue, const String& filePath) {
    load(filePath);
    String* valuePtr = table.get(key);
    if (!valuePtr) {
        return defaultValue;
    }
    return *valuePtr;
}

/**
 * @brief Removes the property with the given key from memory.
 */
void Properties::removeProperty(const String& key) {
    table.remove(key);
}

/**
 * @brief Removes all properties from memory.
 */
void Properties::clear() {
    table.clear();
}

/**
 * @brief Returns the current capacity of the internal table.
 */
int Properties::size() {
    return table.size();
}

/**
 * @brief Returns the number of key-value pairs in the table.
 */
int Properties::elements() {
    return table.elements();
}

/**
 * @brief Returns true if the table is empty.
 */
bool Properties::isEmpty() {
    return table.isEmpty();
}

/**
 * @brief Returns true if the table contains the given key.
 */
bool Properties::containsKey(const String& key) {
    return table.get(key) != nullptr;
}

// ─── File operations ──────────────────────────────────────────────────────────

/**
 * @brief Saves properties to the SD card (or injected filesystem) in key=value format.
 */
bool Properties::saveToSD(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        char sep = getSeparator();
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) { // Properties with empty values are not persisted
                file.print(it.key() + sep + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from the SD card (or injected filesystem).
 *        Blank lines and lines starting with '#' are skipped.
 */
bool Properties::loadFromSD(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) {
                continue;
            }
            int separatorIndex = line.indexOf(getSeparator());
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
 * @brief Wrapper for saveToSD (more intuitive name).
 */
bool Properties::save(const String& filename) {
    return saveToSD(filename);
}

/**
 * @brief Wrapper for loadFromSD (more intuitive name).
 */
bool Properties::load(const String& filename) {
    return loadFromSD(filename);
}

/**
 * @brief Saves properties with a timestamp comment and user-provided comment header.
 */
bool Properties::store(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("#" + String(millis()) + "\n");
        file.print("# " + comments + "\n");
        char sep = getSeparator();
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + sep + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Deletes the given file from the SD card (or injected filesystem).
 */
bool Properties::deleteFile(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    if (fileExists(filename)) {
        if (!removeFile(filename)) {
            return false;
        }
    }
    return true;
}

// ─── XML ─────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in XML format.
 */
bool Properties::storeToXML(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        file.print("<!-- " + comments + " -->\n");
        file.print("<properties>\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print("  <property>\n");
                file.print("    <key>" + it.key() + "</key>\n");
                file.print("    <value>" + it.value() + "</value>\n");
                file.print("  </property>\n");
            }
        }
        file.print("</properties>\n");
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from an XML file.
 */
bool Properties::loadFromXML(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
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

// ─── JSON ─────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in JSON format.
 */
bool Properties::storeToJSON(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("{\n");
        file.print("  \"comments\": \"" + comments + "\",\n");
        file.print("  \"properties\": [\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print("    {\n");
                file.print("      \"key\": \"" + it.key() + "\",\n");
                file.print("      \"value\": \"" + it.value() + "\"\n");
                file.print("    }");
                if (it != end()) {
                    file.print(",");
                }
                file.print("\n");
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
 * @brief Loads properties from a JSON file.
 */
bool Properties::loadFromJSON(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
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

// ─── YAML ─────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in YAML format.
 */
bool Properties::storeToYAML(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + ": " + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from a YAML file.
 */
bool Properties::loadFromYAML(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) {
                continue;
            }
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

// ─── INI ──────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in INI format.
 */
bool Properties::storeToINI(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("; " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + " = " + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from an INI file.
 */
bool Properties::loadFromINI(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith(";") || line.startsWith("#")) {
                continue;
            }
            int separatorIndex = line.indexOf('=');
            if (separatorIndex != -1) {
                String key = line.substring(0, separatorIndex);
                String value = line.substring(separatorIndex + 1);
                key.trim();
                value.trim();
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

// ─── CSV ──────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in CSV format.
 */
bool Properties::storeToCSV(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + "," + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from a CSV file.
 */
bool Properties::loadFromCSV(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) {
                continue;
            }
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

// ─── TOML ─────────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in TOML format.
 */
bool Properties::storeToTOML(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + " = \"" + it.value() + "\"\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from a TOML file.
 */
bool Properties::loadFromTOML(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) {
                continue;
            }
            int separatorIndex = line.indexOf('=');
            if (separatorIndex != -1) {
                String key = line.substring(0, separatorIndex);
                key.trim();
                // TOML string values are quoted: key = "value"
                int quoteStart = line.indexOf('"', separatorIndex);
                int quoteEnd   = line.lastIndexOf('"');
                String value;
                if (quoteStart != -1 && quoteEnd > quoteStart) {
                    value = line.substring(quoteStart + 1, quoteEnd);
                } else {
                    // Unquoted value fallback
                    value = line.substring(separatorIndex + 1);
                    value.trim();
                }
                table.put(key, value);
            }
        }
        file.close();
        return true;
    }
    return false;
}

// ─── MsgPack ──────────────────────────────────────────────────────────────────

/**
 * @brief Stores properties in MsgPack-like format (key:value per line).
 */
bool Properties::storeToMsgPack(const String& filename, const String& comments) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForWrite(filename);
    if (file) {
        file.print("# " + comments + "\n");
        for (PropertiesIterator it = begin(); it != end(); ++it) {
            if (it.value().length() > 0) {
                file.print(it.key() + ":" + it.value() + "\n");
            }
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief Loads properties from a MsgPack-like file.
 */
bool Properties::loadFromMsgPack(const String& filename) {
    if (_filesystem == nullptr && !beginSDIfNeeded()) {
        return false;
    }
    File file = openForRead(filename);
    if (file) {
        while (file.available()) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() == 0 || line.startsWith("#")) {
                continue;
            }
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
