#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#define DS_SUCCESS 0
#define DS_ALREADY_FLATTENED 1
#define DS_ALREADY_UNFLATTENED 2
#define DS_EMPTY_LIST 3
#define DS_ODD_LIST_SIZE 4
#define DS_SD_ERROR 5
#define DS_FILE_NOT_FOUND 6
#define DS_FILE_OPEN_ERROR 7
#define DS_FILE_WRITE_ERROR 8
#define DS_FILE_READ_ERROR 9
#define DS_FILE_PARSE_ERROR 10
#define DS_FILE_WRITE_SUCCESS 11
#define DS_FILE_READ_SUCCESS 12
#define DS_FILE_PARSE_SUCCESS 13
#define DS_SERIALIZE_ERROR 14
#define DS_DESERIALIZE_ERROR 15
#define DS_GARBAGE_COLLECTED 16
#define DS_BLOCK_REMOVED 17
#define DS_BLOCK_NOT_FOUND 18
#define DS_BLOCK_FOUND 19
#define DS_BLOCK_SCAN_SUCCESS 20
#define DS_BLOCK_SCAN_ERROR 21
#define DS_BLOCK_SCAN_EMPTY 22
#define DS_KEY_EXISTS 23
#define DS_KEY_NOT_FOUND 24
#define DS_INVALID_OPERATION 25


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

    Mode mode;
    int blockSize = 4;
    ArrayList<Block> listBlocks;
    Hashtable<K, Variant<T>> mapBlocks;
    JSON json;
    MemoryManager memoryManager;
    String filename = "storage.json";  

    bool useSD() { return mode == SD || (mode == AUTO && memoryManager.getFreeMemory() < 1024); }

public:
    DynamicStorage(Mode initialMode = AUTO, String customFilename = "storage.json")
        : mode(initialMode), filename(customFilename) {}

    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    int add(const T& value) {
        if (useSD()) {
            loadBlocksFromSD();
        }
        
        if (listBlocks.isEmpty() || listBlocks.get(listBlocks.size() - 1).elements.size() >= blockSize) {
            listBlocks.add(Block(listBlocks.size()));
        }
        listBlocks.get(listBlocks.size() - 1).elements.add(value);
        
        if (useSD()) saveBlocksToSD();

        return DS_SUCCESS;
    }

    int put(const K& key, const T& value) {
        if (useSD()) {
            json.setString("map." + key, serialize(value));
            return json.writeToFile(filename) ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
        } else {
            if (!mapBlocks.exists(key)) {
                mapBlocks.put(key, Variant<T>(value));
            } else {
                mapBlocks.get(key)->addValue(value);
            }
        }
        return DS_SUCCESS;
    }

    int put(const K& key, const ArrayList<T>& values) {
        if (useSD()) {
            json.setString("map." + key, serialize(values));
            return json.writeToFile(filename) ? DS_FILE_WRITE_SUCCESS : DS_FILE_WRITE_ERROR;
        } else {
            mapBlocks.put(key, Variant<T>(values));
        }
        return DS_SUCCESS;
    }

    int garbageCollect() {
        int removedBlocks = 0;

        for (int i = 0; i < listBlocks.size(); i++) {
            if (listBlocks.get(i).elements.isEmpty()) {
                listBlocks.remove(i);
                i--;
                removedBlocks++;
            }
        }

        for (auto it = mapBlocks.begin(); it != mapBlocks.end();) {
            if (it->value.isArrayList() && it->value.getList().isEmpty()) {
                it = mapBlocks.erase(it);
                removedBlocks++;
            } else {
                ++it;
            }
        }

        int sdResult = DS_SUCCESS;
        if (useSD()) {
            sdResult = removeUnusedBlocksFromSD();
        }

        memoryManager.detectMemoryLeaks();
        memoryManager.printMemoryLeaks();

        return removedBlocks > 0 ? DS_GARBAGE_COLLECTED : sdResult;
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
        if (json.readFromFile(filename) != JSON_SUCCESS) return DS_FILE_READ_ERROR;
    
        int blockID = 0;
        while (true) {
            String blockPath = String("blocks.") + String(blockID);  // ✅ Ensure proper String type
            String data = json.getString(blockPath, "");
    
            if (data == "") break;
            listBlocks.add(Block(blockID));
            listBlocks.get(listBlocks.size() - 1).elements = deserializeArrayList(data);
            blockID++;
        }
    
        return DS_FILE_READ_SUCCESS;
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
