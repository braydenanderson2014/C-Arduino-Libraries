#include <Arduino.h>
#include "SDList.h"

template <typename K, typename V>
class OrderedMap {
private:
    SDList<K> internalKeys ;       // Holds all keys
    SDList<V> internalValues;     // Holds all values
    Mode mode = MEMORY;           // Use the global Mode enum

public:
    OrderedMap()
        : internalKeys(MEMORY, 8), internalValues(MEMORY, 1), mode(MEMORY) {
            internalKeys.getUnderlyingStructure().setSizeType(ArrayList<K>::DYNAMIC2);
            internalValues.getUnderlyingStructure().setSizeType(ArrayList<V>::DYNAMIC2);
            internalValues.setMode(MEMORY);
            internalKeys.setMode(MEMORY);
        }

    OrderedMap(size_t initialCapacity, Mode mode = MEMORY)
        : internalKeys(mode, initialCapacity), internalValues(mode, initialCapacity), mode(mode) {
            internalKeys.getUnderlyingStructure().setSizeType(ArrayList<K>::DYNAMIC2);
            internalValues.getUnderlyingStructure().setSizeType(ArrayList<V>::DYNAMIC2);
            internalValues.setMode(MEMORY);
            internalKeys.setMode(MEMORY);
        }

    void put(K key, V value) {
        internalKeys.append(key);
        internalValues.append(value);
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

    SDList<K> keys() { return internalKeys; }

    SDList<V> values() { return internalValues; }

    bool save(const String& filename) {
        internalKeys.dumpMemoryToFile();
        internalValues.dumpMemoryToFile();
        return true;
    }

    bool load(const String& filename) {
        internalKeys.readFileIntoMemory();
        internalValues.readFileIntoMemory();
        return true;
    }

    bool store(const String& filename, const String& comments) {
        internalKeys.dumpMemoryToFile();
        internalValues.dumpMemoryToFile();
        return true;
    }

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

    

    void swapMode() {
        mode = (mode == MEMORY) ? SDCARD : MEMORY;
        setMode(mode);
    }

    Mode getMode() const { return mode; }

    Mode getKeyMode() const { return internalKeys.getMode(); }  

    Mode getValueMode() const { return internalValues.getMode(); }

    void setMode(Mode newMode) {
        internalKeys.setMode(newMode);
        internalValues.setMode(newMode);
        mode = newMode;
    }

    void begin() {
        internalKeys.begin();
        internalValues.begin();
    }
};
