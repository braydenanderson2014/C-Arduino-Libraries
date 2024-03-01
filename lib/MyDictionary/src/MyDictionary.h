#ifndef MY_DICTIONARY_H
#define MY_DICTIONARY_H

#include <Arduino.h>
#include "Dictionary.h"
#include "UnorderedMap.h"

/**
 * @brief UnorderedMap-based implementation of the Dictionary interface.
 * @interface Dictionary
 * @see UnorderedMap
 * 
 * @extends Dictionary
*/
class MyDictionary : public Dictionary<String, String> {
private:
    UnorderedMap<String, String> data = UnorderedMap<String, String>(); //Uses UnorderedMap as the underlying data structure

public:
    /**
     * @brief Returns the number of key-value mappings in this map.
     * @return the number of key-value mappings in this map
     * 
    */
    int size() const override {
        return data.getSize();
    }

    /**
     * @brief Returns true if this map contains no key-value mappings.
     * @return true if this map contains no key-value mappings
     * 
    */
    bool isEmpty() const override {
        return data.isEmpty();
    }

    /**
     * @brief Returns the value to which the specified key is mapped, or null if this map contains no mapping for the key.
     * @param key - the key whose associated value is to be returned
     * @return the value to which the specified key is mapped, or null if this map contains no mapping for the key
    */
    String get(const String& key) const override {
        String value;
        if (data.getValue(key)) {
            return value;
        } else {
            return "null"; // or some other error indicator
        }
    }

    /**
     * @brief Puts the specified value into the map, using the specified key. If the map previously contained a mapping for the key, the old value is replaced.
     * @param key - the key with which the specified value is to be associated
     * @param value - the value to be associated with the specified key
     * 
    */
    void put(const String& key, const String& value) override {
        data.put(key, value);
    }

    /**
     * @brief Removes the mapping for the specified key from this map if present.
     * @param key - the key whose mapping is to be removed from the map
     * @return the previous value associated with key, or null if there was no mapping for key.
     * 
    */
    String remove(const String& key) override {
        if (!data.remove(key)) {
            return "null"; // or some other error indicator
        }
        return "success"; // or some other success indicator
    }

    /**
     * @brief Removes all of the mappings from this map.
     * @see UnorderedMap::clear()
    */
    void clear()  {
        data.clear();
    }
};

#endif // MY_DICTIONARY_H