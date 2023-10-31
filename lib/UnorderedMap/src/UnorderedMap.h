#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <Arduino.h>

template <typename KeyType, typename ValueType>
class UnorderedMap {
private:
    struct KeyValuePair {
        KeyType key;
        ValueType value;
    };

    KeyValuePair* data;
    size_t capacity;
    size_t size;

    void resize(size_t newCapacity) {
        KeyValuePair* newData = new KeyValuePair[newCapacity];
        if (!newData) {
            // Handle allocation failure (this is critical on Arduino due to limited memory)
            Serial.println("Error: Unable to allocate more memory.");
            return;
        }

        // Copy data to new array
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        // Delete old data array and assign new array to data
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
public:
    UnorderedMap(size_t initialCapacity = 16) 
      : capacity(initialCapacity), size(0) {
        data = new KeyValuePair[capacity];
    }

    ~UnorderedMap() {
        delete[] data;
    }

    void insert(const KeyType& key, const ValueType& value) {
        // Check for duplicates first
        for (size_t i = 0; i < size; ++i) {
            if (data[i].key == key) {
                // Key already exists, update the value and return
                data[i].value = value;
                return;
            }
        }

        // Insert new element if there's space and no duplicate was found
        if (size < capacity) {
            data[size++] = {key, value};
        } else {
            resize(capacity * 2);
        }
    }


    bool get(const KeyType& key, ValueType& value) const {
        for (size_t i = 0; i < size; ++i) {
            if (data[i].key == key) {
                value = data[i].value;
                return true;
            }
        }
        return false;
    }

    bool remove(const KeyType& key) {
        for (size_t i = 0; i < size; ++i) {
            if (data[i].key == key) {
                // Move the last element to the current position
                data[i] = data[--size];
                return true;
            }
        }
        return false;
    }

    size_t getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }
};

#endif // UNORDERED_MAP_H
