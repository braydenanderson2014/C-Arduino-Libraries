#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include "Hashtable.h"
#include "JSON.h"
#include "SimpleVector.h"
#include "TypeTraits.h"
#include <SdFat.h>

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
        : mode(initialMode), filename(customFilename) {}

    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    bool beginSD(uint8_t csPin) {
        sdCsPin = csPin;
        sdInitialized = sdInstance.begin(csPin);
        if (sdInitialized && mode == AUTO) {
            loadBlocksFromSD();
        }
        return sdInitialized;
    }

    void changeMode(Mode newMode) {
        if (mode == newMode) return;
        if (useSD()) {
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

        int status = json.writeToFile(filename);
        return status == JSON::JSON_WRITE_SUCCESS ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
    }

    String loadBlocksFromSD() {
        if (!ensureSDReady()) return DS_SD_ERROR;

        JSON json;
        int status = json.readFromFile(filename);
        if (status == JSON::JSON_FILE_NOT_FOUND) {
            listStorage.clear();
            mapStorage.clear();
            return DS_FILE_NOT_FOUND;
        }
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

private:
    struct StoredValue {
        bool isList = false;
        T singleValue = T();
        SimpleVector<T> listValues;
    };

    Mode mode = AUTO;
    int blockSize = 4;
    SimpleVector<T> listStorage;
    Hashtable<K, StoredValue> mapStorage;
    String filename = "storage.json";
    SdFat sdInstance;
    bool sdInitialized = false;
    uint8_t sdCsPin = 4;

    bool useSD() const {
        return mode == SD || (mode == AUTO && sdInitialized);
    }

    bool ensureSDReady() {
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

    String keyToString(const K& key) const {
        if constexpr (is_same<K, String>::value) return key;
        if constexpr (is_same<K, const char*>::value || is_same<K, char*>::value) return String(key);
        if constexpr (is_same<K, bool>::value) return key ? "true" : "false";
        return String(key);
    }

    K stringToKey(const String& key) const {
        if constexpr (is_same<K, String>::value) return key;
        if constexpr (is_same<K, int>::value) return key.toInt();
        if constexpr (is_same<K, long>::value) return key.toInt();
        if constexpr (is_same<K, float>::value) return key.toFloat();
        if constexpr (is_same<K, double>::value) return key.toFloat();
        if constexpr (is_same<K, bool>::value) return key == "true";
        return K();
    }

    void writeTypedValue(JSON& json, const String& base, const T& value) const {
        if constexpr (is_same<T, String>::value) {
            json.setString(base + ".type", "string");
            json.setString(base + ".value", value);
        } else if constexpr (is_same<T, bool>::value) {
            json.setString(base + ".type", "bool");
            json.setBool(base + ".value", value);
        } else if constexpr (is_arithmetic<T>::value) {
            json.setString(base + ".type", "number");
            json.setNumber(base + ".value", value);
        } else {
            json.setString(base + ".type", "string");
            json.setString(base + ".value", String(value));
        }
    }

    T readTypedValue(const JSON& json, const String& base, const T& defaultValue) const {
        if (!json.hasKey(base + ".type")) return defaultValue;

        String type = json.getString(base + ".type", "");
        if (type == "string") {
            if constexpr (is_same<T, String>::value) {
                return json.getString(base + ".value", "");
            } else {
                return defaultValue;
            }
        }
        if (type == "bool") {
            if constexpr (is_same<T, bool>::value) {
                return json.getBool(base + ".value", false);
            } else {
                return defaultValue;
            }
        }
        if (type == "number") {
            if constexpr (is_same<T, int>::value) return static_cast<int>(json.getNumber(base + ".value", 0));
            if constexpr (is_same<T, long>::value) return static_cast<long>(json.getNumber(base + ".value", 0));
            if constexpr (is_same<T, float>::value) return static_cast<float>(json.getNumber(base + ".value", 0));
            if constexpr (is_same<T, double>::value) return static_cast<double>(json.getNumber(base + ".value", 0));
        }
        return defaultValue;
    }
};

#endif // DYNAMIC_STORAGE_H
