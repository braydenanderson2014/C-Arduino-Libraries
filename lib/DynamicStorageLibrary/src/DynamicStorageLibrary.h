#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include "ArrayList.h"
#include "Hashtable.h"
#include "JSON.h"
#include "MemoryManager.h"
#include <SdFat.h>
#include "TypeTraits.h"
#include "Variant.h"

#define DS_SUCCESS 0              // Operation successful
#define DS_SD_ERROR 5             // SD card failure
#define DS_FILE_NOT_FOUND 6       // File not found
#define DS_FILE_OPEN_ERROR 7      // Error opening file
#define DS_FILE_WRITE_ERROR 8     // Error writing to file
#define DS_FILE_READ_ERROR 9      // Error reading from file
#define DS_FILE_PARSE_ERROR 10    // Error parsing JSON
#define DS_FILE_WRITE_SUCCESS 11  // File write successful
#define DS_FILE_READ_SUCCESS 12   // File read successful

SdFat SD;  // Use SdFat instead of Arduino's SD library

template <typename K, typename T>
class DynamicStorage {
public:
    enum Mode { RAM, SD, AUTO };

private:
    struct Block {
        int id;
        ArrayList<T> elements;
        Block(int blockID = 0) : id(blockID) {}
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

    bool beginSD(uint8_t csPin) {
        return SD.begin(csPin);
    }

    int saveBlocksToSD() {
        if (!SD.begin()) return DS_SD_ERROR;

        json = JSON();
        for (int i = 0; i < listBlocks.size(); i++) {
            json.setString("blocks." + String(listBlocks.get(i).id), serialize(listBlocks.get(i).elements));
        }

        FsFile file = SD.open(filename.c_str(), O_WRITE | O_CREAT);
        if (!file) return DS_FILE_OPEN_ERROR;

        if (!json.writeToFile(filename.c_str())) return DS_FILE_WRITE_ERROR;
        file.close();
        return DS_FILE_WRITE_SUCCESS;
    }

    int loadBlocksFromSD() {
        if (!SD.begin()) return DS_SD_ERROR;
        listBlocks.clear();

        FsFile file = SD.open(filename.c_str(), O_READ);
        if (!file) return DS_FILE_NOT_FOUND;

        if (!json.readFromFile(filename.c_str())) return DS_FILE_READ_ERROR;
        file.close();

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
