#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <Arduino.h>
#include <stddef.h>

template <typename KeyType, typename ValueType>
class UnorderedMap {
private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* next;
    };

    Node** table;  // Array of pointers to linked lists (buckets)
    size_t capacity; // Size of the table
    size_t count; // Number of key-value pairs in the map
    float loadFactor; // Maximum load factor before resizing

    size_t hash(const KeyType& key) const; // Hash function
    void resize(); // Resize the hashtable

public:
    UnorderedMap(size_t initialCapacity = 16, float loadFactor = 0.75); // Constructor
    ~UnorderedMap(); // Destructor

    void put(const KeyType& key, const ValueType& value); // Add or update a key-value pair
    ValueType getValue(const KeyType& key) const; // Get the value associated with the given key
    bool contains(const KeyType& key) const; // Check if the given key is present in the map
    bool remove(const KeyType& key); // Remove the key-value pair with the given key
    void clear(); // Remove all key-value pairs from the map
    size_t size() const; // Get the number of key-value pairs in the map
    bool isEmpty() const; // Check if the map is empty

    // Indexing support
    Node& operator[](size_t index) const; // Get the node at the given index

    // Iterator support (nested class)
    class Iterator {
    private:
        Node** table;
        size_t tableSize;
        size_t currentIndex;
        Node* currentNode;

    public:
        Iterator(Node** table, size_t tableSize, size_t startIndex)
            : table(table), tableSize(tableSize), currentIndex(startIndex), currentNode(nullptr) {
            // Move to the first non-empty bucket
            while (currentIndex < tableSize && table[currentIndex] == nullptr) {
                currentIndex++;
            }
            if (currentIndex < tableSize) {
                currentNode = table[currentIndex];
            }
        }

        bool operator!=(const Iterator& other) const {
            return currentIndex != other.currentIndex || currentNode != other.currentNode;
        }

        void operator++() {
            if (currentNode) {
                currentNode = currentNode->next;
            }
            while (!currentNode && currentIndex < tableSize) {
                currentIndex++;
                if (currentIndex < tableSize) {
                    currentNode = table[currentIndex];
                }
            }
        }

        Node& operator*() { return *currentNode; }
        Node* operator->() { return currentNode; }
    };

    Iterator begin() const {
        return Iterator(table, capacity, 0);
    }

    Iterator end() const {
        return Iterator(table, capacity, capacity);
    }
};

template <typename KeyType, typename ValueType>
UnorderedMap<KeyType, ValueType>::UnorderedMap(size_t initialCapacity, float loadFactor)
    : capacity(initialCapacity), count(0), loadFactor(loadFactor) {
    table = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

template <typename KeyType, typename ValueType>
UnorderedMap<KeyType, ValueType>::~UnorderedMap() {
    clear();
    delete[] table;
}

template <typename KeyType, typename ValueType>
size_t UnorderedMap<KeyType, ValueType>::hash(const KeyType& key) const {
    return static_cast<size_t>(key) % capacity; // Replace with an appropriate hash function
}

template <typename KeyType, typename ValueType>
void UnorderedMap<KeyType, ValueType>::resize() {
    size_t newCapacity = capacity * 2;
    Node** newTable = new Node*[newCapacity];
    for (size_t i = 0; i < newCapacity; ++i) {
        newTable[i] = nullptr;
    }

    for (size_t i = 0; i < capacity; ++i) {
        Node* node = table[i];
        while (node) {
            Node* next = node->next;
            size_t newIndex = static_cast<size_t>(node->key) % newCapacity;
            node->next = newTable[newIndex];
            newTable[newIndex] = node;
            node = next;
        }
    }

    delete[] table;
    table = newTable;
    capacity = newCapacity;
}

template <typename KeyType, typename ValueType>
void UnorderedMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    if ((float)count / capacity >= loadFactor) {
        resize();
    }

    size_t index = hash(key);
    Node* node = table[index];
    while (node) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    Node* newNode = new Node{key, value, table[index]};
    table[index] = newNode;
    count++;
}

template <typename KeyType, typename ValueType>
ValueType UnorderedMap<KeyType, ValueType>::getValue(const KeyType& key) const {
    size_t index = hash(key);
    Node* node = table[index];
    while (node) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return ValueType();
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::contains(const KeyType& key) const {
    size_t index = hash(key);
    Node* node = table[index];
    while (node) {
        if (node->key == key) {
            return true;
        }
        node = node->next;
    }
    return false;
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::remove(const KeyType& key) {
    size_t index = hash(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template <typename KeyType, typename ValueType>
void UnorderedMap<KeyType, ValueType>::clear() {
    for (size_t i = 0; i < capacity; ++i) {
        Node* node = table[i];
        while (node) {
            Node* temp = node;
            node = node->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    count = 0;
}

template <typename KeyType, typename ValueType>
size_t UnorderedMap<KeyType, ValueType>::size() const {
    return count;
}

template <typename KeyType, typename ValueType>
bool UnorderedMap<KeyType, ValueType>::isEmpty() const {
    return count == 0;
}

template <typename KeyType, typename ValueType>
typename UnorderedMap<KeyType, ValueType>::Node& UnorderedMap<KeyType, ValueType>::operator[](size_t index) const {
    size_t current = 0;
    for (size_t i = 0; i < capacity; ++i) {
        Node* node = table[i];
        while (node) {
            if (current == index) {
                return *node;
            }
            node = node->next;
            current++;
        }
    }
    return *table[0]; // Return the first element if index is out of bounds
}

#endif // UNORDERED_MAP_H
