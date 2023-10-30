#include <iostream.h>
#include <LinkedList.h>
#include "SimpleVector.h"

template <typename KeyType, typename ValueType>
class UnorderedMap {
private:
    // Define a structure to hold key-value pairs
    struct KeyValuePair {
        KeyType key;
        ValueType value;
    };

    // Define the hash table as a SimpleVector of lists of KeyValuePair
    SimpleVector<std::list<KeyValuePair>> table;
    size_t size;

    // Hash function to map a key to an index in the table
    size_t hash(const KeyType& key) const {
        // You can replace this simple hash function with a more sophisticated one
        return std::hash<KeyType>{}(key) % table.size();
    }

public:
    // Constructor
    UnorderedMap(size_t initialSize = 100) : table(initialSize), size(0) {}

    // Destructor
    ~UnorderedMap() {}

    // Insert a key-value pair into the map
    void insert(const KeyType& key, const ValueType& value) {
        size_t index = hash(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                // Key already exists, update the value
                pair.value = value;
                return;
            }
        }
        // Key not found, add a new key-value pair
        KeyValuePair newPair{key, value};
        table[index].put(newPair);
        size++;
    }

    // Retrieve a value by key
    bool get(const KeyType& key, ValueType& value) const {
        size_t index = hash(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                value = pair.value;
                return true; // Key found
            }
        }
        return false; // Key not found
    }

    // Remove a key-value pair by key
    bool remove(const KeyType& key) {
        size_t index = hash(key);
        auto& list = table[index];
        for (size_t i = 0; i < list.size(); ++i) {
            if (list[i].key == key) {
                list.remove(list[i]);
                size--;
                return true; // Key found and removed
            }
        }
        return false; // Key not found
    }

    // Check if the map is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Get the number of key-value pairs in the map
    size_t getSize() const {
        return size;
    }
};


