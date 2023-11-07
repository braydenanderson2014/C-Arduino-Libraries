#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <SimpleVector.h>
#include <Arduino.h>
    // Forward declaration of KeyHash
template<typename K>
struct KeyHash;

// Specialization for String
template<>
struct KeyHash<String> {
    unsigned long operator()(const String& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = 31 * hash + c;
        }
        return hash;
    }
};

// Specialization for int
template<>
struct KeyHash<int> {
    unsigned long operator()(const int& key) const {
        return static_cast<unsigned long>(key);
    }
};

template <typename K, typename V, typename Hash = KeyHash<K>>
class Hashtable {
private:
    
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    static const int INITIAL_TABLE_SIZE = 16;
    Entry** table;
    int TABLE_SIZE;
    int count;
    float loadFactorThreshold = 0.7;
    Hash hashFunction;

    int stringHash(const String& str) {
        int hash = 0;
        for (unsigned int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            hash = (hash * 31) + str.charAt(i);
        }
        return hash % TABLE_SIZE;
    }

    // Simplified hash function that delegates to the Hash functor
    int hash(const K& key) const {
        return hashFunction(key) % TABLE_SIZE;
    }

    // Private function to resize the hash table
    void resize(int newSize) {
        Entry** oldTable = table;
        int oldSize = TABLE_SIZE;
        TABLE_SIZE = newSize;
        table = new Entry*[TABLE_SIZE]();

        for (int i = 0; i < oldSize; ++i) {
            Entry* entry = oldTable[i];
            while (entry) {
                Entry* next = entry->next;
                int index = hash(entry->key);
                entry->next = table[index];
                table[index] = entry;
                entry = next;
            }
        }

        delete[] oldTable;
    }

public:
    Hashtable() : TABLE_SIZE(INITIAL_TABLE_SIZE), count(0) {
        table = new Entry*[TABLE_SIZE]();
    }

    ~Hashtable() {
        clear();
        delete[] table;
    }

    void put(const K& key, const V& value) {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }

        Entry* newEntry = new Entry(key, value);
        newEntry->next = table[index];
        table[index] = newEntry;
        ++count;

        if (static_cast<float>(count) / TABLE_SIZE > loadFactorThreshold) {
            resize(TABLE_SIZE * 2);
        }
    }
    V* get(const K& key) const {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return &(entry->value); // Return the address of the value
            }
            entry = entry->next;
        }
        return nullptr; // Return null if the key is not found
    }


    bool get(const K& key, V& value) const {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                value = entry->value;
                return true;
            }
            entry = entry->next;
        }
        return false;
    }

    bool remove(const K& key) {
        int index = hash(key);
        Entry* current = table[index];
        Entry* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev != nullptr) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                --count;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Entry* entry = table[i];
            while (entry != nullptr) {
                Entry* toDelete = entry;
                entry = entry->next;
                delete toDelete;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    int size() const {
        return TABLE_SIZE;
    }

    bool isEmpty() const {
        return size() == 0;
    }
    int elements() const {
        return count;
    }

    SimpleVector<K> keys() const {
        SimpleVector<K> keys;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                keys.put(entry->key);
            }
        }
        return keys;
    }

    bool containsKey(const K& key) const {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;
        }
        return false;
    }

    bool containsValue(const V& value) const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Entry* entry = table[i];
            while (entry != nullptr) {
                if (entry->value == value) {
                    return true;
                }
                entry = entry->next;
            }
        }
        return false;
    }
    // More functions can be added here
};

#endif // HASHTABLE_H
