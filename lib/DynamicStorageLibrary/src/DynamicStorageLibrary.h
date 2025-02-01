#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#define DS_SUCCESS 0              // Operation successful
#define DS_ALREADY_FLATTENED 1    // Already in list mode
#define DS_ALREADY_UNFLATTENED 2  // Already in hashtable mode
#define DS_EMPTY_LIST 3           // No data to unflatten
#define DS_ODD_LIST_SIZE 4        // Flattened data is not in key-value pairs
#define DS_SD_ERROR 5             // SD card failure
#define DS_FILE_NOT_FOUND 6       // File not found
#define DS_FILE_OPEN_ERROR 7      // Error opening file
#define DS_FILE_WRITE_ERROR 8     // Error writing to file
#define DS_FILE_READ_ERROR 9      // Error reading from file
#define DS_FILE_PARSE_ERROR 10    // Error parsing JSON
#define DS_FILE_WRITE_SUCCESS 11  // File write successful
#define DS_FILE_READ_SUCCESS 12   // File read successful
#define DS_FILE_PARSE_SUCCESS 13  // JSON parse successful
#define DS_SERIALALIZE_ERROR 14   // Error serializing data
#define DS_DESERIALIZE_ERROR 15   // Error deserializing data
#define DS_GARBAGE_COLLECTED 16   // Garbage collection completed
#define DS_BLOCK_REMOVED 17       // Block removed
#define DS_BLOCK_NOT_FOUND 18     // Block not found
#define DS_BLOCK_FOUND 19         // Block found
#define DS_BLOCK_SCAN_SUCCESS 20  // Block scan successful
#define DS_BLOCK_SCAN_ERROR 21    // Block scan error
#define DS_BLOCK_SCAN_EMPTY 22    // Block scan empty


#include "ArrayList.h"
#include "Hashtable.h"
#include "JSON.h"
#include "MemoryManager.h"
#include <SD.h>
#include "TypeTraits.h"
#include "Variant.h"

template <typename K, typename T>
class DynamicStorage {
public:
    enum Mode { RAM, SD, AUTO };

private:
    struct Block {
        int id;
        ArrayList<T> elements;

        Block(int blockID = 0) : id(blockID) {}

        bool operator==(const Block& other) const { return id == other.id && elements == other.elements; }
        bool operator!=(const Block& other) const { return !(*this == other); }
    };

    Mode mode; // Mode of the Storage
    int blockSize = 4;
    ArrayList<Block> listBlocks;
    Hashtable<K, Variant<T>> mapBlocks;
    JSON json;
    MemoryManager memoryManager;
    String filename = "storage.json";  // Default filename

    bool useSD() { return mode == SD || (mode == AUTO && memoryManager.getFreeMemory() < 1024); }

public:
    DynamicStorage(Mode initialMode = AUTO, String customFilename = "storage.json")
        : mode(initialMode), filename(customFilename) {}

    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    void add(const T& value) {
        if (useSD()) {
            loadBlocksFromSD();
            if (listBlocks.isEmpty() || listBlocks.get(listBlocks.size() - 1).elements.size() >= blockSize) {
                listBlocks.add(Block(listBlocks.size()));
            }
            listBlocks.get(listBlocks.size() - 1).elements.add(value);
            saveBlocksToSD();
        } else {
            if (listBlocks.isEmpty() || listBlocks.get(listBlocks.size() - 1).elements.size() >= blockSize) {
                listBlocks.add(Block(listBlocks.size()));
            }
            listBlocks.get(listBlocks.size() - 1).elements.add(value);
        }
    }

    void put(const K& key, const T& value) {
        if (useSD()) {
            json.setString("map." + key, serialize(value));
            json.writeToFile(filename);
        } else {
            if (!mapBlocks.exists(key)) {
                mapBlocks.put(key, Variant<T>(value));
            } else {
                mapBlocks.get(key)->addValue(value);
            }
        }
    }

    void put(const K& key, const ArrayList<T>& values) {
        if (useSD()) {
            json.setString("map." + key, serialize(values));
            json.writeToFile(filename);
        } else {
            mapBlocks.put(key, Variant<T>(values));
        }
    }

    int garbageCollect() {
        int removedBlocks = 0;

        // Remove empty blocks from RAM
        for (int i = 0; i < listBlocks.size(); i++) {
            if (listBlocks.get(i).elements.isEmpty()) {
                listBlocks.remove(i);
                i--; // Adjust index after removal
                removedBlocks++;
            }
        }

        // Remove empty key-value pairs from map
        for (auto it = mapBlocks.begin(); it != mapBlocks.end();) {
            if (it->value.isArrayList() && it->value.getList().isEmpty()) {
                it = mapBlocks.erase(it); // Remove empty Variant
                removedBlocks++;
            } else {
                ++it;
            }
        }

        // Remove unused blocks from SD
        int sdResult = DS_SUCCESS;
        if (useSD()) {
            sdResult = removeUnusedBlocksFromSD();
        }

        // Free memory from MemoryManager
        memoryManager.dumpMemoryLeaks();
        memoryManager.writeMemoryLeaksToSerial();

        return removedBlocks > 0 ? DS_GARBAGE_COLLECTED : sdResult;
    }

    void scanFileSystemForBlocks() {
        Serial.println("Scanning SD card for orphaned blocks...");

        if (!SD.begin()) {
            Serial.println("SD card not found!");
            return;
        }

        File root = SD.open("/");
        while (File file = root.openNextFile()) {
            String filename = file.name();
            if (filename.startsWith("block_") && filename.endsWith(".json")) {
                Serial.println("Found storage block: " + filename);
            }
        }
    }

    int removeUnusedBlocksFromSD() {
        if (!SD.begin()) return DS_SD_ERROR;

        int removedBlocks = 0;
        File root = SD.open("/");
        while (File file = root.openNextFile()) {
            String filename = file.name();
            if (filename.startsWith("block_") && filename.endsWith(".json")) {
                String blockID = filename.substring(6, filename.length() - 5);
                bool found = false;

                for (int i = 0; i < listBlocks.size(); i++) {
                    if (String(listBlocks.get(i).id) == blockID) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    if (!SD.remove(filename)) return DS_FILE_WRITE_ERROR;
                    removedBlocks++;
                }
            }
        }
        return (removedBlocks > 0) ? DS_GARBAGE_COLLECTED : DS_BLOCK_NOT_FOUND;
    }

    int saveBlocksToSD() {
        if (!SD.begin()) return DS_SD_ERROR;

        json = JSON();
        for (int i = 0; i < listBlocks.size(); i++) {
            json.setString("blocks." + String(listBlocks.get(i).id), serialize(listBlocks.get(i).elements));
        }
        return json.writeToFile(filename) ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
    }

    int loadBlocksFromSD() {
        if (!SD.begin()) return DS_SD_ERROR;

        listBlocks.clear();
        if (!json.readFromFile(filename)) return DS_FILE_READ_ERROR;

        int blockID = 0;
        while (true) {
            String data = json.getString("blocks." + String(blockID), "");
            if (data == "") break;
            listBlocks.add(Block(blockID));
            listBlocks.get(listBlocks.size() - 1).elements = deserializeArrayList(data);
            blockID++;
        }

        return DS_FILE_READ_SUCCESS;
    }

    /**
     * @brief Flattens the Hashtable to ArrayList/Variant
     */
    int flatten() {
        if (!mapBlocks.size()) {
            return DS_ALREADY_FLATTENED; // No need to flatten if we're already in list mode
        }

        listBlocks.clear();
        listBlocks.add(Block());

        ArrayList<T>& flatList = listBlocks.get(0).elements; // Store the flattened data

        if (useSD()) {
            if (!SD.begin()) return DS_SD_ERROR;

            json = JSON();
            for (auto it = mapBlocks.begin(); it != mapBlocks.end(); ++it) {
                flatList.add(serialize(it->key));
                if (it->value.isArrayList()) {
                    flatList.add(serialize(it->value.getList()));
                } else {
                    flatList.add(serialize(it->value.getSingle()));
                }
            }
            if (!json.writeToFile(filename)) return DS_FILE_WRITE_ERROR;
        } else {
            for (auto it = mapBlocks.begin(); it != mapBlocks.end(); ++it) {
                flatList.add(serialize(it->key));
                if (it->value.isArrayList()) {
                    flatList.add(serialize(it->value.getList()));
                } else {
                    flatList.add(serialize(it->value.getSingle()));
                }
            }
        }

        mapBlocks.clear(); // Convert hashtable mode to list mode
        return DS_SUCCESS;
    }

    int unflatten() {
        if (!listBlocks.size() || listBlocks.get(0).elements.size() == 0) {
            return DS_EMPTY_LIST; // No data to unflatten
        }

        ArrayList<T>& flatList = listBlocks.get(0).elements;

        if (flatList.size() % 2 != 0) {
            return DS_ODD_LIST_SIZE; // Ensure key-value pairs are balanced
        }

        if (useSD()) {
            if (!SD.begin()) return DS_SD_ERROR;

            json = JSON();
            for (int i = 0; i < flatList.size(); i += 2) {
                K key = deserializeKey(flatList.get(i));
                T value = deserialize(flatList.get(i + 1));
                json.setString("map." + key, serialize(value));
            }
            if (!json.writeToFile(filename)) return DS_FILE_WRITE_ERROR;
        } else {
            for (int i = 0; i < flatList.size(); i += 2) {
                K key = deserializeKey(flatList.get(i));
                T value = deserialize(flatList.get(i + 1));

                if (!mapBlocks.exists(key)) {
                    mapBlocks.put(key, Variant<T>(value));
                } else {
                    mapBlocks.get(key)->addValue(value);
                }
            }
        }

        listBlocks.clear(); // Convert list mode to hashtable mode
        return DS_SUCCESS;
    }

private:
    String serialize(const T& value) {
        if constexpr (is_same<T, int>::value) return String(value);
        if constexpr (is_same<T, float>::value) return String(value, 6);
        if constexpr (is_same<T, bool>::value) return value ? "true" : "false";
        if constexpr (is_same<T, String>::value) return value;
        return String(value);
    }

    String serialize(const ArrayList<T>& list) {
        JSON json;
        for (int i = 0; i < list.size(); i++) {
            json.setString(String(i), serialize(list.get(i)));
        }
        return json.writeToString();
    }

    ArrayList<T> deserializeArrayList(const String& jsonString) {
        ArrayList<T> result;
        JSON json;
        json.readFromString(jsonString);
        
        int index = 0;
        while (true) {
            String data = json.getString(String(index), "");
            if (data == "") break;
            result.add(deserialize(data));
            index++;
        }
        return result;
    }

    T deserialize(const String& value) {
        if constexpr (is_same<T, int>::value) return value.toInt();
        if constexpr (is_same<T, float>::value) return value.toFloat();
        if constexpr (is_same<T, bool>::value) return (value == "true");
        if constexpr (is_same<T, String>::value) return value;
        return T();
    }
};

#endif // DYNAMIC_STORAGE_H