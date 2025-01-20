#include <Arduino.h>
#include "ArrayList.h"

template <typename K, typename V>
class OrderedMap {
private:
    ArrayList<K> internalKeys ;       // Holds all keys
    ArrayList<V> internalValues;     // Holds all values
    Mode mode = MEMORY;           // Use the global Mode enum

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

    bool exists(const String& key) { return internalKeys.contains(key); }

    bool exists(const String& key, const String& value) {
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

};
