#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "Hashtable.h"

template <typename KeyType, typename ValueType, typename Hash = KeyHash<KeyType>>
class UnorderedMap {
private:
    Hashtable<KeyType, ValueType, Hash> hashtable;

public:
    // Forward the constructor arguments
    UnorderedMap(size_t initialCapacity = 16, float loadFactor = 0.7) 
        : hashtable(initialCapacity, loadFactor) {}

    // Forward the destructor
    ~UnorderedMap() = default;

    // Forward the put operation
    void insert(const KeyType& key, const ValueType& value) {
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
    bool erase(const KeyType& key) {
        return hashtable.remove(key);
    }

    // Forward the size operation
    size_t size() const {
        return hashtable.elements();
    }

    // Forward the isEmpty operation
    bool empty() const {
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
};

#endif // UNORDERED_MAP_H
