#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include "ArrayList.h"
#include "Hashtable.h"
#include "JSON.h"
#include "MemoryManager.h"
#include <SD.h>
#include "TypeTraits.h"
#include "Variant.h"

template <typename K, typename T>
/**
 * @brief A class for dynamic storage of elements.
 *
 * @tparam T The type of the stored elements in Hashtable and ArrayList.
 * @tparam K The type of the keys in the Hashtable.
 */
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
    String filename = "storage.json";  // Default filename

    bool useSD() { return mode == SD || (mode == AUTO && memoryManager.getFreeMemory() < 1024); }

public:
    DynamicStorage(Mode initialMode = AUTO, String customFilename = "storage.json")
        : mode(initialMode), filename(customFilename) {}

    // 📌 Set and Get Filename
    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    // 📌 Add a Single Element (Uses ArrayList)
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

    // 📌 Store Single Key-Value Pair or a Key with an ArrayList
    void put(const K& key, const T& value) {
        if (useSD()) {
            json.setString("map." + key, serialize(value));
            json.writeToFile(filename);
        } else {
            if (!mapBlocks.exists(key)) {
                mapBlocks.put(key, Variant<T>(value)); // Store as single value
            } else {
                mapBlocks.get(key)->addValue(value); // Convert to list if needed
            }
        }
    }

    void put(const K& key, const ArrayList<T>& values) {
        if (useSD()) {
            json.setString("map." + key, serialize(values));
            json.writeToFile(filename);
        } else {
            mapBlocks.put(key, Variant<T>(values)); // Store list directly
        }
    }


    // 📌 Retrieve a Single Value from an ArrayList
    T get(int index) {
        int blockIndex = index / blockSize;
        int elementIndex = index % blockSize;

        if (useSD()) loadBlocksFromSD();
        if (blockIndex >= listBlocks.size()) return T();

        return listBlocks.get(blockIndex).elements.get(elementIndex);
    }

    T get(const K& key) {
        if (useSD()) {
            return deserialize(json.getString("map." + key, ""));
        }
        if (!mapBlocks.exists(key)) return T();
        return mapBlocks.get(key)->getSingle();
    }

    // 📌 Retrieve a Single Value from a Key-Value Store
    T get(const K& key, int index = 0) {
        if (useSD()) return deserialize(json.getString("map." + key + "." + String(index), ""));
        if (!mapBlocks.exists(key)) return T();
        return mapBlocks.get(key)->get(index);
    }

    // 📌 Retrieve an Entire List from a Key
    ArrayList<T> getList(const K& key) {
        if (useSD()) return deserializeArrayList(json.getString("map." + key, ""));
        if (!mapBlocks.exists(key)) return ArrayList<T>();
        return mapBlocks.get(key)->getList();
    }

    // 📌 Switch storage mode dynamically
    void changeMode(Mode newMode) {
        if (mode == newMode) return;

        if (newMode == SD) {
            saveBlocksToSD();
        } else {
            loadBlocksFromSD();
        }
        mode = newMode;
    }

    bool isListEmpty() { return listBlocks.isEmpty(); }
    bool isMapEmpty() { return mapBlocks.isEmpty(); }
    int size() { return listBlocks.size() * blockSize + mapBlocks.size(); }
    int blockCount() { return listBlocks.size(); }
    int mapCount() { return mapBlocks.size(); }

    void clear() {
        listBlocks.clear();
        mapBlocks.clear();
        if (useSD()) {
            json = JSON();
            json.writeToFile(filename);
        }
    }

    void remove(const K& key) {
        if (useSD()) {
            json.setNull("map." + key);
            json.writeToFile(filename);
        } else {
            mapBlocks.remove(key);
        }
    }

    void remove(int index) {
        if (useSD()) {
            loadBlocksFromSD();
            if (index >= listBlocks.size() * blockSize) return;
            listBlocks.get(index / blockSize).elements.remove(index % blockSize);
            saveBlocksToSD();
        } else {
            if (index >= listBlocks.size() * blockSize) return;
            listBlocks.get(index / blockSize).elements.remove(index % blockSize);
        }
    }

private:
    // 📌 Serialization: Convert any type to String for SD storage
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

    String serialize(const Variant<T>& variant) {
        JSON json;
        if (variant.isSingle()) {
            return serialize(variant.getSingle()); // Serialize single value
        } else {
            return serialize(variant.getList()); // Serialize list
        }
    }

    Variant<T> deserializeVariant(const String& jsonString) {
        JSON json;
        json.readFromString(jsonString);

        if (json.getRoot().type == JSON::ValueType::Array) {
            return Variant<T>(deserializeArrayList(jsonString)); // Convert JSON to list
        } else {
            return Variant<T>(deserialize(jsonString)); // Convert JSON to single value
        }
    }


    // 📌 Deserialization: Convert String back to original type
    T deserialize(const String& value) {
        if constexpr (is_same<T, int>::value) return value.toInt();
        if constexpr (is_same<T, float>::value) return value.toFloat();
        if constexpr (is_same<T, bool>::value) return (value == "true");
        if constexpr (is_same<T, String>::value) return value;
        return T();
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

    void saveBlocksToSD() {
        json = JSON();
        for (int i = 0; i < listBlocks.size(); i++) {
            json.setString("blocks." + String(listBlocks.get(i).id), serialize(listBlocks.get(i).elements));
        }
        for (auto it = mapBlocks.begin(); it != mapBlocks.end(); ++it) {
            json.setString("map." + it->key, serialize(it->value));
        }
        json.writeToFile(filename);
    }

    void loadBlocksFromSD() {
        listBlocks.clear();
        json.readFromFile(filename);

        int blockID = 0;
        while (true) {
            String data = json.getString("blocks." + String(blockID), "");
            if (data == "") break;
            listBlocks.add(Block(blockID));
            listBlocks.get(listBlocks.size() - 1).elements = deserializeArrayList(data);
            blockID++;
        }

        for (auto it = json.getRoot().children->begin(); it != json.getRoot().children->end(); ++it) {
            mapBlocks.put((*it).key, deserializeVariant((*it).stringValue)); 
        }
    }



};

#endif // DYNAMIC_STORAGE_H
