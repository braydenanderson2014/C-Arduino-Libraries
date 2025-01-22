#include <Arduino.h>
#include "ArrayList.h"
#include <SD.h>
#include "JSON.h"

template <typename K, typename V>
class OrderedMap {
private:
    ArrayList<K> internalKeys ;       // Holds all keys
    ArrayList<V> internalValues;     // Holds all values

    JSON json;
    //Mode mode = MEMORY;           // Use the global Mode enum

    // Recursive helper that does the real flattening
    void flattenNodeRecursive(const JSON::Node& node,
                              const String& prefix)
    {
        // Distinguish by node type
        if (node.type == JSON::ValueType::Object) {
            // Iterate children
            for (int i = 0; i < node.children.size(); i++) {
                const JSON::Node& child = node.children[i];

                // Build nextKey: either child.key or prefix + "." + child.key
                String nextKey;
                if (prefix.length() == 0) {
                    nextKey = child.key;
                } else {
                    nextKey = prefix + "." + child.key.C_STR(); // Convert child from Custom_String to C_String which is compatible with Arduino String
                }
                // If child is another object or an array, recurse
                if (child.type == JSON::ValueType::Object ||
                    child.type == JSON::ValueType::Array) 
                {
                    flattenNodeRecursive(child, nextKey);
                } else {
                    // It's a leaf: convert to string, insert into map
                    String leafVal = nodeToString(child);
                    this->insert(nextKey, leafVal);
                }
            }
        }
        else if (node.type == JSON::ValueType::Array) {
            // Flatten arrays by numeric indices
            for (int i = 0; i < node.children.size(); i++) {
                const JSON::Node& child = node.children[i];
                
                // Build something like: prefix + "." + i
                // e.g. "myArray.0", "myArray.1", etc.
                String indexKey = prefix + ".";
                indexKey += i; // automatically convert 'i' to string
                
                if (child.type == JSON::ValueType::Object ||
                    child.type == JSON::ValueType::Array)
                {
                    flattenNodeRecursive(child, indexKey);
                } else {
                    // Leaf
                    String leafVal = nodeToString(child);
                    this->insert(indexKey, leafVal);
                }
            }
        }
        else {
            // If node is a top-level leaf (rare if root isn't an object or array)
            // Just store it at 'prefix'
            String leafVal = nodeToString(node);
            this->insert(prefix, leafVal);
        }
    }

    String nodeToString(const JSON::Node& node) {
        switch (node.type) {
            case JSON::ValueType::Null:
                return "null";
            case JSON::ValueType::Bool:
                return node.boolValue ? "true" : "false";
            case JSON::ValueType::Number: {
                char buf[32];
                dtostrf(node.numberValue, 0, 6, buf); 
                return String(buf);
            }
            case JSON::ValueType::String:
                return node.stringValue;
            default:
                return "";
        }
    }

public:
    OrderedMap() {
            internalKeys.setSizeType(ArrayList<K>::DYNAMIC2);
            internalValues.setSizeType(ArrayList<V>::DYNAMIC2);
            
        }

    OrderedMap(size_t initialCapacity)
        : internalKeys(initialCapacity), internalValues(initialCapacity) {
            internalKeys.setSizeType(ArrayList<K>::DYNAMIC2);
            internalValues.setSizeType(ArrayList<V>::DYNAMIC2);
    }

    void insert(K key, V value, size_t index = size()){
        internalKeys.insert(internalKeys.size() + 1, key);
        internalValues.insert(internalValues.size() + 1, key);
    }

    void put(K key, V value) {
        internalKeys.add(key);
        internalValues.add(value);
    }

    V get(K key) {
        for (int i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key) {
                return internalValues.get(i);
            }
        }
        return V(); // Default value of V
    }

    void remove(K key) {
        for (int i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key) {
                internalKeys.remove(i);
                internalValues.remove(i);
                return;
            }
        }
    }

    bool containsKey(K key) {
        for (int i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key) {
                return true;
            }
        }
        return false;
    }

    size_t size() { return internalKeys.size(); }

    size_t capacity() { return internalKeys.capacity(); }

    bool isEmpty() { return internalKeys.isEmpty(); }

    void clear() {
        internalKeys.clear();
        internalValues.clear();
    }

    ArrayList<K> keys() { return internalKeys; }

    ArrayList<V> values() { return internalValues; }

    bool exists(const K& key) { return internalKeys.contains(key); }

    bool exists(const K& key, const String& value) {
        for (int i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key && internalValues.get(i) == value) {
                return true;
            }
        }
        return false;
    }

    bool fileExists(const String& filename) { return SD.exists(filename); }

    bool deleteFile(const String& filename) { return SD.remove(filename); }

    bool renameFile(const String& oldFilename, const String& newFilename) {
        File oldFile = SD.open(oldFilename, FILE_READ);
        if (!oldFile) {
            // Failed to open the old file
            return false;
        }

        File newFile = SD.open(newFilename, FILE_WRITE);
        if (!newFile) {
            // Failed to create the new file
            oldFile.close();
            return false;
        }

        // Copy the contents of the old file to the new file
        while (oldFile.available()) {
            newFile.write(oldFile.read());
        }

        // Close both files
        oldFile.close();
        newFile.close();

        // Delete the old file
        return SD.remove(oldFilename);
    }

    void saveToJson(){
        for(size_t i = 0; i <= internalKeys.size(); i++){
            json.setString(internalKeys[i], internalValues[i]);
        }
        json.writeToFile("OrderedMap.txt");
    }

    void loadFromJson() {
        json.readFromFile("OrderedMap.json");
        fromJsonNode(json.getRoot());
        
    }

    void fromJsonNode(const JSON::Node& rootNode, const String& prefix = "") {
        flattenNodeRecursive(rootNode, prefix);
    }

};
