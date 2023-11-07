#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <Arduino.h>

template <typename KeyType, typename ValueType>
class UnorderedMapIterator; // Forward declaration.

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
            Serial.println("Error: Unable to allocate more memory.");
            return;
        }

        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    UnorderedMap(size_t initialCapacity = 16) 
        : data(new KeyValuePair[initialCapacity]), capacity(initialCapacity), size(0) {}

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
    // Declaration of begin() and end() will be after the definition of UnorderedMapIterator.
    UnorderedMapIterator<KeyType, ValueType> begin();
    UnorderedMapIterator<KeyType, ValueType> end();

    // ... Rest of UnorderedMap methods

    friend class UnorderedMapIterator<KeyType, ValueType>; // Make iterator a friend.
};

template <typename KeyType, typename ValueType>
class UnorderedMapIterator {
private:
    typename UnorderedMap<KeyType, ValueType>::KeyValuePair* current;
    typename UnorderedMap<KeyType, ValueType>::KeyValuePair* end;

public:
    UnorderedMapIterator(typename UnorderedMap<KeyType, ValueType>::KeyValuePair* start, typename UnorderedMap<KeyType, ValueType>::KeyValuePair* stop)
        : current(start), end(stop) {}

    // Iterator methods...
    // ...
    // Dereference operator
    ValueType& operator*() {
        return current->value;
    }

    // Arrow operator
    ValueType* operator->() {
        return &(current->value);
    }

    // Prefix increment operator
    UnorderedMapIterator& operator++() {
        if (current != end) {
            ++current;
        }
        return *this;
    }

    // Postfix increment operator (usually less efficient than prefix)
    UnorderedMapIterator operator++(int) {
        UnorderedMapIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Equality operator
    bool operator==(const UnorderedMapIterator& other) const {
        return current == other.current;
    }

    // Inequality operator
    bool operator!=(const UnorderedMapIterator& other) const {
        return current != other.current;
    }
    friend class UnorderedMap<KeyType, ValueType>; // Make UnorderedMap a friend if needed.
};

// Definitions of begin() and end() that depend on UnorderedMapIterator.
template <typename KeyType, typename ValueType>
UnorderedMapIterator<KeyType, ValueType> UnorderedMap<KeyType, ValueType>::begin() {
    return UnorderedMapIterator<KeyType, ValueType>(data, data + size);
}

template <typename KeyType, typename ValueType>
UnorderedMapIterator<KeyType, ValueType> UnorderedMap<KeyType, ValueType>::end() {
    return UnorderedMapIterator<KeyType, ValueType>(data + size, data + size);
}

// ... Rest of UnorderedMapIterator methods

#endif // UNORDERED_MAP_H