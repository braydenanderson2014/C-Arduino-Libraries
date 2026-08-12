#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include "Hashtable.h"
#include "JSON.h"
#include "SimpleVector.h"
#include "TypeTraits.h"
#include <SdFat.h>
#include <stdlib.h>
#include "../../UnoQFileTransferClient.h"

// Define DYNAMIC_STORAGE_USE_BRIDGE before including this header to enable
// Bridge-backed save/load on boards that have Arduino_RouterBridge available.
#ifdef DYNAMIC_STORAGE_USE_BRIDGE
  #include <Arduino_RouterBridge.h>
  // Chunk size for chunked Bridge reads — keep well under RPC timeout
  #ifndef DS_BRIDGE_CHUNK_SIZE
    #define DS_BRIDGE_CHUNK_SIZE 128
  #endif
#endif

#define DS_SUCCESS "DS0"
#define DS_SD_ERROR "DS5"
#define DS_FILE_NOT_FOUND "DS6"
#define DS_FILE_OPEN_ERROR "DS7"
#define DS_FILE_WRITE_ERROR "DS8"
#define DS_FILE_READ_ERROR "DS9"
#define DS_FILE_PARSE_ERROR "DS10"
#define DS_FILE_WRITE_SUCCESS "DS11"
#define DS_FILE_READ_SUCCESS "DS12"

template <typename K, typename T>
class DynamicStorage {
public:
    enum Mode { RAM, SD, AUTO };

    DynamicStorage(Mode initialMode = AUTO, const String& customFilename = "storage.json")
        : mode(initialMode),
          blockSize(4),
          listStorage(),
          mapStorage(),
          filename(customFilename),
          sdInstance(),
                    _filesystem(nullptr),
          sdInitialized(false),
          sdCsPin(4) {}

    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    bool beginSD(uint8_t csPin) {
        _filesystem = nullptr;
        sdCsPin = csPin;
        sdInitialized = sdInstance.begin(csPin);
        if (sdInitialized && mode == AUTO) {
            loadBlocksFromSD();
        }
        return sdInitialized;
    }

    bool beginFS(fs::FS& filesystem) {
        _filesystem = &filesystem;
        sdInitialized = true;
        if (mode == AUTO) {
            loadBlocksFromSD();
        }
        return true;
    }

    void changeMode(Mode newMode) {
        if (mode == newMode) return;
        const bool targetUsesSD = (newMode == SD) || (newMode == AUTO && sdInitialized);
        if (targetUsesSD) {
            loadBlocksFromSD();
        }
        mode = newMode;
        if (useSD()) {
            saveBlocksToSD();
        }
    }

    void clear() {
        listStorage.clear();
        mapStorage.clear();
        if (useSD()) {
            saveBlocksToSD();
        }
    }

    void add(const T& value) {
        syncFromSDIfNeeded();
        listStorage.put(value);
        syncToSDIfNeeded();
    }

    void put(const K& key, const T& value) {
        syncFromSDIfNeeded();

        StoredValue* existing = mapStorage.get(key);
        if (!existing) {
            StoredValue holder;
            holder.isList = false;
            holder.singleValue = value;
            mapStorage.put(key, holder);
            syncToSDIfNeeded();
            return;
        }

        if (!existing->isList) {
            existing->listValues.clear();
            existing->listValues.put(existing->singleValue);
            existing->listValues.put(value);
            existing->isList = true;
        } else {
            existing->listValues.put(value);
        }

        syncToSDIfNeeded();
    }

    void put(const K& key, const SimpleVector<T>& values) {
        syncFromSDIfNeeded();
        StoredValue holder;
        holder.isList = true;
        holder.listValues = values;
        mapStorage.put(key, holder);
        syncToSDIfNeeded();
    }

    T get(size_t index) {
        syncFromSDIfNeeded();
        if (index >= listStorage.elements()) return T();
        return listStorage[index];
    }

    T get(const K& key) {
        syncFromSDIfNeeded();
        StoredValue* value = mapStorage.get(key);
        if (!value) return T();
        if (!value->isList) return value->singleValue;
        if (value->listValues.isEmpty()) return T();
        // For dynamic keys that have accumulated multiple values, return the newest one.
        return value->listValues[value->listValues.elements() - 1];
    }

    SimpleVector<T> getList(const K& key) {
        syncFromSDIfNeeded();
        StoredValue* value = mapStorage.get(key);
        if (!value) return SimpleVector<T>();
        if (value->isList) return value->listValues;

        SimpleVector<T> singleAsList;
        singleAsList.put(value->singleValue);
        return singleAsList;
    }

    size_t size() {
        syncFromSDIfNeeded();
        return listStorage.elements();
    }

    bool hasKey(const K& key) {
        syncFromSDIfNeeded();
        return mapStorage.exists(key);
    }

    String saveBlocksToSD() {
        if (!ensureSDReady()) return DS_SD_ERROR;

        JSON json = (_filesystem != nullptr) ? JSON(*_filesystem) : JSON(sdCsPin);
        json.setNumber("meta.blockSize", blockSize);

        json.setNumber("list.count", listStorage.elements());
        for (size_t i = 0; i < listStorage.elements(); ++i) {
            writeTypedValue(json, "list.values." + String(i), listStorage[i]);
        }

        SimpleVector<K> keys = mapStorage.keys();
        json.setNumber("map.count", keys.elements());
        for (size_t i = 0; i < keys.elements(); ++i) {
            const K& key = keys[i];
            const StoredValue* value = mapStorage.get(key);
            if (!value) continue;

            const String base = "map.entries." + String(i);
            json.setString(base + ".key", keyToString(key));
            json.setBool(base + ".isList", value->isList);

            if (value->isList) {
                json.setNumber(base + ".count", value->listValues.elements());
                for (size_t j = 0; j < value->listValues.elements(); ++j) {
                    writeTypedValue(json, base + ".values." + String(j), value->listValues[j]);
                }
            } else {
                writeTypedValue(json, base + ".single", value->singleValue);
            }
        }

        int status = json.writeToFile(filename);
        return status == JSON::JSON_WRITE_SUCCESS ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
    }

    String loadBlocksFromSD() {
        if (!ensureSDReady()) return DS_SD_ERROR;

        JSON json = (_filesystem != nullptr) ? JSON(*_filesystem) : JSON(sdCsPin);
        int status = json.readFromFile(filename);
        if (status == JSON::JSON_FILE_NOT_FOUND) {
            listStorage.clear();
            mapStorage.clear();
            return DS_FILE_NOT_FOUND;
        }
        if (status == JSON::JSON_FILE_PARSE_ERROR) return DS_FILE_PARSE_ERROR;
        if (status != JSON::JSON_READ_SUCCESS) return DS_FILE_READ_ERROR;

        listStorage.clear();
        mapStorage.clear();

        blockSize = static_cast<int>(json.getNumber("meta.blockSize", blockSize));

        size_t listCount = static_cast<size_t>(json.getNumber("list.count", 0));
        for (size_t i = 0; i < listCount; ++i) {
            listStorage.put(readTypedValue(json, "list.values." + String(i), T()));
        }

        size_t mapCount = static_cast<size_t>(json.getNumber("map.count", 0));
        for (size_t i = 0; i < mapCount; ++i) {
            const String base = "map.entries." + String(i);
            if (!json.hasKey(base + ".key")) continue;

            K key = stringToKey(json.getString(base + ".key", ""));
            StoredValue holder;
            holder.isList = json.getBool(base + ".isList", false);

            if (holder.isList) {
                size_t count = static_cast<size_t>(json.getNumber(base + ".count", 0));
                for (size_t j = 0; j < count; ++j) {
                    holder.listValues.put(readTypedValue(json, base + ".values." + String(j), T()));
                }
            } else {
                holder.singleValue = readTypedValue(json, base + ".single", T());
            }

            mapStorage.put(key, holder);
        }

        return DS_FILE_READ_SUCCESS;
    }

    String saveBlocksToUnoQ(UnoQFileTransferClient& client, const String& remoteFilename = "") {
        JSON json;
        json.setNumber("meta.blockSize", blockSize);

        json.setNumber("list.count", listStorage.elements());
        for (size_t i = 0; i < listStorage.elements(); ++i) {
            writeTypedValue(json, "list.values." + String(i), listStorage[i]);
        }

        SimpleVector<K> keys = mapStorage.keys();
        json.setNumber("map.count", keys.elements());
        for (size_t i = 0; i < keys.elements(); ++i) {
            const K& key = keys[i];
            const StoredValue* value = mapStorage.get(key);
            if (!value) continue;

            const String base = "map.entries." + String(i);
            json.setString(base + ".key", keyToString(key));
            json.setBool(base + ".isList", value->isList);

            if (value->isList) {
                json.setNumber(base + ".count", value->listValues.elements());
                for (size_t j = 0; j < value->listValues.elements(); ++j) {
                    writeTypedValue(json, base + ".values." + String(j), value->listValues[j]);
                }
            } else {
                writeTypedValue(json, base + ".single", value->singleValue);
            }
        }

        const String target = remoteFilename.length() > 0 ? remoteFilename : filename;
        const int status = json.writeToUnoQ(client, target, true);
        return status == JSON::JSON_WRITE_SUCCESS ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
    }

    String loadBlocksFromUnoQ(UnoQFileTransferClient& client, const String& remoteFilename = "") {
        JSON json;
        const String target = remoteFilename.length() > 0 ? remoteFilename : filename;
        int status = json.readFromUnoQ(client, target);
        if (status == JSON::JSON_FILE_NOT_FOUND) {
            listStorage.clear();
            mapStorage.clear();
            return DS_FILE_NOT_FOUND;
        }
        if (status == JSON::JSON_FILE_PARSE_ERROR) return DS_FILE_PARSE_ERROR;
        if (status != JSON::JSON_READ_SUCCESS) return DS_FILE_READ_ERROR;

        listStorage.clear();
        mapStorage.clear();

        blockSize = static_cast<int>(json.getNumber("meta.blockSize", blockSize));

        size_t listCount = static_cast<size_t>(json.getNumber("list.count", 0));
        for (size_t i = 0; i < listCount; ++i) {
            listStorage.put(readTypedValue(json, "list.values." + String(i), T()));
        }

        size_t mapCount = static_cast<size_t>(json.getNumber("map.count", 0));
        for (size_t i = 0; i < mapCount; ++i) {
            const String base = "map.entries." + String(i);
            if (!json.hasKey(base + ".key")) continue;

            K key = stringToKey(json.getString(base + ".key", ""));
            StoredValue holder;
            holder.isList = json.getBool(base + ".isList", false);

            if (holder.isList) {
                size_t count = static_cast<size_t>(json.getNumber(base + ".count", 0));
                for (size_t j = 0; j < count; ++j) {
                    holder.listValues.put(readTypedValue(json, base + ".values." + String(j), T()));
                }
            } else {
                holder.singleValue = readTypedValue(json, base + ".single", T());
            }

            mapStorage.put(key, holder);
        }

        return DS_FILE_READ_SUCCESS;
    }

#ifdef DYNAMIC_STORAGE_USE_BRIDGE
    // ── Bridge-backed persistence (Uno Q App Lab) ────────────────────────────
    // Requires DYNAMIC_STORAGE_USE_BRIDGE to be defined before including this header.
    // Python container must have handlers/filesystem.py loaded (standard main.py setup).

    String saveBlocksToBridge(const String& remoteFilename = "") {
        const String target = remoteFilename.length() > 0 ? remoteFilename : filename;

        JSON json;
        json.setNumber("meta.blockSize", blockSize);

        json.setNumber("list.count", listStorage.elements());
        for (size_t i = 0; i < listStorage.elements(); ++i) {
            writeTypedValue(json, "list.values." + String(i), listStorage[i]);
        }

        SimpleVector<K> keys = mapStorage.keys();
        json.setNumber("map.count", keys.elements());
        for (size_t i = 0; i < keys.elements(); ++i) {
            const K& key = keys[i];
            const StoredValue* value = mapStorage.get(key);
            if (!value) continue;

            const String base = "map.entries." + String(i);
            json.setString(base + ".key", keyToString(key));
            json.setBool(base + ".isList", value->isList);

            if (value->isList) {
                json.setNumber(base + ".count", value->listValues.elements());
                for (size_t j = 0; j < value->listValues.elements(); ++j) {
                    writeTypedValue(json, base + ".values." + String(j), value->listValues[j]);
                }
            } else {
                writeTypedValue(json, base + ".single", value->singleValue);
            }
        }

        char* raw = json.writeToString(false);  // compact (no pretty-print) saves Bridge bandwidth
        if (!raw) return DS_FILE_WRITE_ERROR;

        bool wrote = false;
        bool ok = Bridge.call("fs_write", target, String(raw)).result(wrote);
        free(raw);
        return (ok && wrote) ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
    }

    String loadBlocksFromBridge(const String& remoteFilename = "") {
        const String target = remoteFilename.length() > 0 ? remoteFilename : filename;

        // Check file exists and get its size
        int fileSize = -1;
        if (!Bridge.call("fs_read_size", target).result(fileSize) || fileSize <= 0) {
            listStorage.clear();
            mapStorage.clear();
            return DS_FILE_NOT_FOUND;
        }

        // Assemble file content in chunks to avoid Bridge RPC timeout
        String content;
        content.reserve(static_cast<unsigned int>(fileSize) + 1);
        int offset = 0;
        while (offset < fileSize) {
            String chunk;
            if (!Bridge.call("fs_read_chunk", target, offset, (int)DS_BRIDGE_CHUNK_SIZE).result(chunk)) {
                return DS_FILE_READ_ERROR;
            }
            if (chunk.length() == 0) break;
            content += chunk;
            offset += static_cast<int>(chunk.length());
        }

        JSON json;
        if (!json.readFromString(content.c_str())) return DS_FILE_PARSE_ERROR;

        listStorage.clear();
        mapStorage.clear();

        blockSize = static_cast<int>(json.getNumber("meta.blockSize", blockSize));

        size_t listCount = static_cast<size_t>(json.getNumber("list.count", 0));
        for (size_t i = 0; i < listCount; ++i) {
            listStorage.put(readTypedValue(json, "list.values." + String(i), T()));
        }

        size_t mapCount = static_cast<size_t>(json.getNumber("map.count", 0));
        for (size_t i = 0; i < mapCount; ++i) {
            const String base = "map.entries." + String(i);
            if (!json.hasKey(base + ".key")) continue;

            K key = stringToKey(json.getString(base + ".key", ""));
            StoredValue holder;
            holder.isList = json.getBool(base + ".isList", false);

            if (holder.isList) {
                size_t count = static_cast<size_t>(json.getNumber(base + ".count", 0));
                for (size_t j = 0; j < count; ++j) {
                    holder.listValues.put(readTypedValue(json, base + ".values." + String(j), T()));
                }
            } else {
                holder.singleValue = readTypedValue(json, base + ".single", T());
            }

            mapStorage.put(key, holder);
        }

        return DS_FILE_READ_SUCCESS;
    }
#endif  // DYNAMIC_STORAGE_USE_BRIDGE

private:
    template <typename U>
    String encodeValue(const U&) const { static_assert(sizeof(U) == 0, "DynamicStorage: unsupported value type for SD persistence"); return ""; }
    String encodeValue(const String& value) const { return value; }
    String encodeValue(const int& value) const { return String(value); }
    String encodeValue(const long& value) const { return String(value); }
    String encodeValue(const float& value) const { return String(value); }
    String encodeValue(const double& value) const { return String(value); }
    String encodeValue(const bool& value) const { return value ? "true" : "false"; }

    template <typename U>
    U decodeValueAs(const String&, const U& defaultValue) const { return defaultValue; }
    String decodeValueAs(const String& value, const String&) const { return value; }
    int decodeValueAs(const String& value, const int&) const { return value.toInt(); }
    long decodeValueAs(const String& value, const long&) const { return atol(value.c_str()); }
    float decodeValueAs(const String& value, const float&) const { return value.toFloat(); }
    double decodeValueAs(const String& value, const double&) const { return atof(value.c_str()); }
    bool decodeValueAs(const String& value, const bool&) const { return value == "true"; }

    template <typename U>
    String encodeKey(const U&) const { static_assert(sizeof(U) == 0, "DynamicStorage: unsupported key type for SD persistence"); return ""; }
    String encodeKey(const String& value) const { return value; }
    String encodeKey(const int& value) const { return String(value); }
    String encodeKey(const long& value) const { return String(value); }
    String encodeKey(const float& value) const { return String(value); }
    String encodeKey(const double& value) const { return String(value); }
    String encodeKey(const bool& value) const { return value ? "true" : "false"; }
    // Pointer key types cannot be round-tripped safely (no owned storage / lifetime).
    // Require String (or another value type) for SD persistence.
    String encodeKey(char* const&) const { static_assert(!is_pointer<K>::value, "DynamicStorage: pointer key types cannot be persisted; use String"); return ""; }
    String encodeKey(const char* const&) const { static_assert(!is_pointer<K>::value, "DynamicStorage: pointer key types cannot be persisted; use String"); return ""; }

    template <typename U>
    U decodeKeyAs(const String&, const U& defaultValue) const { return defaultValue; }
    String decodeKeyAs(const String& value, const String&) const { return value; }
    int decodeKeyAs(const String& value, const int&) const { return value.toInt(); }
    long decodeKeyAs(const String& value, const long&) const { return atol(value.c_str()); }
    float decodeKeyAs(const String& value, const float&) const { return value.toFloat(); }
    double decodeKeyAs(const String& value, const double&) const { return atof(value.c_str()); }
    bool decodeKeyAs(const String& value, const bool&) const { return value == "true"; }
    char* decodeKeyAs(const String&, char* const& defaultValue) const { static_assert(!is_pointer<K>::value, "DynamicStorage: pointer key types cannot be persisted; use String"); return defaultValue; }
    const char* decodeKeyAs(const String&, const char* const& defaultValue) const { static_assert(!is_pointer<K>::value, "DynamicStorage: pointer key types cannot be persisted; use String"); return defaultValue; }

    struct StoredValue {
        bool isList = false;
        T singleValue = T();
        SimpleVector<T> listValues;
    };

    Mode mode;
    int blockSize;
    SimpleVector<T> listStorage;
    Hashtable<K, StoredValue> mapStorage;
    String filename;
    SdFat sdInstance;
    fs::FS* _filesystem;
    bool sdInitialized;
    uint8_t sdCsPin;

    bool useSD() const {
        return mode == SD || (mode == AUTO && sdInitialized);
    }

    bool ensureSDReady() {
        if (_filesystem != nullptr) {
            return true;
        }
        if (sdInitialized) return true;
        sdInitialized = sdInstance.begin(sdCsPin);
        return sdInitialized;
    }

    void syncFromSDIfNeeded() {
        if (useSD()) {
            loadBlocksFromSD();
        }
    }

    void syncToSDIfNeeded() {
        if (useSD()) {
            saveBlocksToSD();
        }
    }

    String keyToString(const K& key) const { return encodeKey(key); }

    K stringToKey(const String& key) const { return decodeKeyAs(key, K()); }

    void writeTypedValue(JSON& json, const String& base, const T& value) const {
        json.setString(base + ".value", encodeValue(value));
    }

    T readTypedValue(const JSON& json, const String& base, const T& defaultValue) const {
        if (!json.hasKey(base + ".value")) return defaultValue;
        return decodeValueAs(json.getString(base + ".value", ""), defaultValue);
    }
};

#endif // DYNAMIC_STORAGE_H
