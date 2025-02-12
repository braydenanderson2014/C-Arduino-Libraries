#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include "ArrayList.h"
#include "Hashtable.h"
#include "JSON.h"
#include <SdFat.h>
#include "TypeTraits.h"
#include "ExtremeVariant.h"
#include "Optional.h"

#define DS_SUCCESS "DS0"             // Operation successful
#define DS_SD_ERROR "DS5"            // SD card failure
#define DS_FILE_NOT_FOUND "DS6"      // File not found
#define DS_FILE_OPEN_ERROR "DS7"     // Error opening file
#define DS_FILE_WRITE_ERROR "DS8"    // Error writing to file
#define DS_FILE_READ_ERROR "DS9"     // Error reading from file
#define DS_FILE_PARSE_ERROR "DS10"   // Error parsing JSON
#define DS_FILE_WRITE_SUCCESS "DS11" // File write successful
#define DS_FILE_READ_SUCCESS "DS12"  // File read successful

SdFat sdInstance;  // Use SdFat instead of Arduino's SD library

template <typename K, typename T>
class DynamicStorage {
public:
    enum Mode { RAM, SD, AUTO };

    void add(const T& value) {
        if (useSD()) {
            loadBlocksFromSD();

            if (!listBlocks.getList().hasValue()) {
                listBlocks.setList(SimpleVector<Block>());
            }

            auto& blockList = listBlocks.getList().getValue();
            if (blockList.isEmpty() || blockList.get(blockList.size() - 1).elements.getList().getValue().elements() >= blockSize) {
                blockList.put(Block(blockList.size()));
            }

            blockList.get(blockList.size() - 1).elements.getList().getValue().put(value);
            saveBlocksToSD();
        } else {
            if (!listBlocks.getList().hasValue()) {
                listBlocks.setList(SimpleVector<Block>());
            }

            auto& blockList = listBlocks.getList().getValue();
            if (blockList.isEmpty() || blockList.get(blockList.size() - 1).elements.getList().getValue().elements() >= blockSize) {
                blockList.put(Block(blockList.size()));
            }

            blockList.get(blockList.size() - 1).elements.getList().getValue().put(value);
        }
    }

    void put(const K& key, const T& value) {
        JSON json;
        
        // Handle JSON storage if using SD
        if (useSD()) {
            json.setString("map." + key, serialize(value));
            json.writeToFile(filename);
            return;
        }   

        // Ensure `mapBlocks` has key-value pairs initialized
        if (!mapBlocks.getKeyValuePairs().hasValue()) {
            Hashtable<K, Optional<ExtremeVariant<K, Block>>> newTable;
            ExtremeVariant<K, Block> initialVariant;
            initialVariant.setSingle(Block(0));  // Set initial single block    

            newTable.put(key, Optional<ExtremeVariant<K, Block>>(initialVariant));
            mapBlocks.setKeyValuePairs(newTable);
        }   

        // Retrieve the key-value table
        auto& keyValueMap = mapBlocks.getKeyValuePairs().getValue();    

        // If the key doesn't exist, add an initial value
        if (!keyValueMap.exists(key)) {
            ExtremeVariant<K, Block> newVariant;
            newVariant.setSingle(Block(1));
            keyValueMap.put(key, Optional<ExtremeVariant<K, Block>>(newVariant));
            return;
        }   

        // Retrieve the existing value (optional ExtremeVariant<K, Block>)
        auto* optVariant = keyValueMap.get(key); // Pointer to Optional<ExtremeVariant<K, Block>>   

        // Ensure the optional has a value before using it
        if (optVariant && optVariant->hasValue()) {
            auto& blockVariant = optVariant->getValue(); // Reference to ExtremeVariant<K, Block>   

            // If currently storing a single block, convert it into a list
            if (blockVariant.getType() == SINGLE_ELEMENT) {
                SimpleVector<Block> blockList;
                blockList.put(blockVariant.getSingle().getValue()); // Add existing block
                blockList.put(Block(3)); // Add new block   

                // Correctly capture `blockList` inside the lambda
                *optVariant = Optional<ExtremeVariant<K, Block>>(
                    [blockList]() mutable {
                        ExtremeVariant<K, Block> newVariant;
                        newVariant.setList(blockList);
                        return newVariant;
                    }()
                );
            }
        }
    }



private:
    struct Block {
        int id;
        ExtremeVariant<K, T> elements;
        Block(int blockID = 0) : id(blockID) {}
    };

    Mode mode;
    int blockSize = 4;
    ExtremeVariant<K, Block> listBlocks;
    ExtremeVariant<K, Optional<ExtremeVariant<K, Block>>> mapBlocks;
    String filename = "storage.json";

    bool useSD() { return mode == SD; }

public:
    DynamicStorage(Mode initialMode = AUTO, String customFilename = "storage.json")
        : mode(initialMode), filename(customFilename) {}

    void setFilename(const String& newFilename) { filename = newFilename; }
    String getFilename() const { return filename; }

    bool beginSD(uint8_t csPin) {
        return sdInstance.begin(csPin);
    }

    String saveBlocksToSD() {
        if (!sdInstance.begin()) return DS_SD_ERROR;
    
        JSON json;
    
        // ✅ Save `listBlocks` if available
        if (listBlocks.getList().hasValue()) {
            auto& blockList = listBlocks.getList().getValue();
            for (size_t i = 0; i < blockList.elements(); i++) {
                json.setString("blocks." + String(blockList[i].id), serialize(blockList[i].elements));
            }
        }
    
        // ✅ Save `mapBlocks` if available
        if (mapBlocks.getKeyValuePairs().hasValue()) {
            auto& keyValueMap = mapBlocks.getKeyValuePairs().getValue();
    
            // ✅ Iterate over `mapBlocks` and serialize each key-value pair
            SimpleVector<K> keys = keyValueMap.keys();  // Get all keys
            for (size_t i = 0; i < keys.elements(); i++) {
                K key = keys[i];
    
                // Get the optional variant
                auto* optVariant = keyValueMap.get(key);
                if (optVariant && optVariant->hasValue()) {
                    json.setString("map." + key, serialize(*optVariant));  // ✅ Now works!
                }
            }
        }
    
        // ✅ Write to file
        if (!json.writeToFile(filename)) return DS_FILE_WRITE_ERROR;
        return DS_FILE_WRITE_SUCCESS;
    }
    

    String loadBlocksFromSD() {
        JSON json;
        if (!sdInstance.begin()) return DS_SD_ERROR;
        listBlocks.clear();

        if (!json.readFromFile(filename)) return DS_FILE_READ_ERROR;

        int blockID = 0;
        SimpleVector<Block> newList;
        while (true) {
            String data = json.getString("blocks." + String(blockID), "");
            if (data == "") break;

            newList.put(Block(blockID));
            newList[newList.elements() - 1].elements = deserializeExtremeVariant(data);
            blockID++;
        }

        listBlocks.setList(newList);
        return DS_FILE_READ_SUCCESS;
    }

private:
    String serialize(const ExtremeVariant<K, T>& variant) {
        JSON json;

        switch (variant.getType()) {
            case SINGLE_ELEMENT:
                json.setString("single", serialize(variant.getSingle().getValue()));
                break;

            case SINGLE_LIST: {
                auto list = variant.getList().getValue();
                for (size_t i = 0; i < list.elements(); i++) {
                    json.setString("list." + String(i), serialize(list[i]));
                }
                break;
            }

            case KEY_VALUE_PAIRS: {
                auto table = variant.getKeyValuePairs().getValue();
                SimpleVector<K> keys = table.keys();  // Get all keys
                for (size_t i = 0; i < keys.elements(); i++) {
                    K key = keys[i];
                    json.setString("map." + key, serialize(table.getElement(key)));
                }
                break;
            }

            default:
                break;  // Do nothing for NONE type
        }

        return json.writeToString();
    }


    String serialize(const Block& block) {
        JSON json;
        json.setNumber("id", block.id);
        json.setString("elements", serialize(block.elements));
        return json.writeToString();
    }

    String serialize(const Optional<ExtremeVariant<K, Block>>& optVariant) {
        if (!optVariant.hasValue()) return "null";
        return serialize(optVariant.getValue());
    }


    ExtremeVariant<K, T> deserializeExtremeVariant(const String& jsonString) {
        ExtremeVariant<K, T> result;
        JSON json;
        json.readFromString(jsonString);
    
        if (json.hasKey("single")) {
            result.setSingle(deserialize(json.getString("single")));
        } 
        else if (json.hasKey("list")) {
            SimpleVector<T> list;
            int index = 0;
            while (true) {
                String data = json.getString(String(index), "");
                if (data == "") break;
                list.put(deserialize(data));
                index++;
            }
            result.setList(list);
        } 
        else if (json.hasKey("map")) {
            Hashtable<K, T> map;
    
            // ✅ Instead of iterating, manually extract key-value pairs
            SimpleVector<K> keys = json.getKeys();  // Assuming `getKeys()` is implemented in JSON
            for (int i = 0; i < keys.elements(); i++) {
                K key = keys[i];
                T value = deserialize(json.getString(key));
                map.put(key, value);
            }
    
            result.setKeyValuePairs(map);
        }
    
        return result;
    }


    String serialize(const String& value) {
        return "\"" + value + "\"";  // Ensure it's properly quoted for JSON
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
