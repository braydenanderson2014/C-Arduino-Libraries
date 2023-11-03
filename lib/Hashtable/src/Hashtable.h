#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <SimpleVector.h>
#include <Arduino.h>

template <typename K, typename V>
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

    int stringHash(const String& str) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < str.length(); i++) {
            hash = (hash * 31) + str.charAt(i);
        }
        return hash % TABLE_SIZE;
    }

    int hash(const K& key) const {
        // Assuming that the key K has a proper hash function for non-String types
        if constexpr (std::is_same<K, String>::value) {
            return stringHash(key);
        } else {
            return std::hash<K>{}(key) % TABLE_SIZE;
        }
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
        return count;
    }

    bool isEmpty() const {
        return size() == 0;
    }

    SimpleVector<K> keys() const {
        SimpleVector<K> keys;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                keys.push_back(entry->key);
            }
        }
        return keys;
    }

    // More functions can be added here
};

#endif // HASHTABLE_H
