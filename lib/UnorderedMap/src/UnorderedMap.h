#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "Hashtable.h"

template <typename KeyType, typename ValueType, typename Hash = KeyHash<KeyType>>
class UnorderedMap {
private:
    Hashtable<KeyType, ValueType, Hash> hashtable;

public:
    // Forward the constructor arguments
    // Forward the constructor arguments
    UnorderedMap(size_t initialCapacity = 16, float loadFactor = 0.7) : hashtable(initialCapacity, loadFactor) {}


    // Forward the destructor
    ~UnorderedMap() = default;

    // Forward the put operation
    void put(const KeyType& key, const ValueType& value) {
        hashtable.put(key, value);
    }

    // Forward the get operation (by reference and by value)
    ValueType get(const KeyType& key) const {
        return *hashtable.get(key);
    }

    // Forward the containsKey operation
    bool contains(const KeyType& key) const {
        return hashtable.containsKey(key);
    }

    // Forward the remove operation
    bool remove(const KeyType& key) {
        return hashtable.remove(key);
    }

    // Forward the size operation
    size_t getSize() const {
        return hashtable.elements();
    }

    size_t getCapacity() const {
        return hashtable.size();
    }

    // Forward the isEmpty operation
    bool isEmpty() const {
        return hashtable.isEmpty();
    }

    // Provide a begin() iterator operation
    typename Hashtable<KeyType, ValueType, Hash>::Iterator begin() const {
        return hashtable.begin();
    }

    // Provide an end() iterator operation
    typename Hashtable<KeyType, ValueType, Hash>::Iterator end() const {
        return hashtable.end();
    }

    // Forward other operations as needed...
    // Inside UnorderedMap
    Hash hashFunction() const {
        return hashtable.hashFunction;
    }

    // Inside UnorderedMap
    ValueType& operator[](const KeyType& key) {
        // Attempt to get the value, and if it doesn't exist, insert a new one
        ValueType* value = hashtable.get(key);
        if (!value) {
            hashtable.put(key, ValueType());
            value = hashtable.get(key);
        }
        return *value;
    }

    // Inside UnorderedMap
    typename Hashtable<KeyType, ValueType, Hash>::Iterator find(const KeyType& key) const {
        return hashtable.find(key); // Assuming Hashtable has a find method
    }

    size_t count(const KeyType& key) const {
        return hashtable.containsKey(key) ? 1 : 0;
    }

// Implement equal_range if needed, returning a pair of iterators

// Key equality is implicit in your design, so you can provide a placeholder if needed.

};

#endif // UNORDERED_MAP_H
