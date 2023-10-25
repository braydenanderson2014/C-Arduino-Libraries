#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "SimpleVector.h"
#include <Arduino.h>

template <typename K, typename V>
class Hashtable {
private:
    struct Entry {
        K key;
        V value;
        Entry *next;

        Entry(K k, V v) : key(k), value(v), next(nullptr) {} // Inline constructor
    };

    const static int INITIAL_TABLE_SIZE = 16; // Initial size, adjust based on your needs
    Entry** table;
    int TABLE_SIZE;
    int count;
    float loadFactorThreshold = 0.7; // Adjust the threshold as needed

    int stringHash(const String& str) {
        int hash = 0;
        for (unsigned int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            hash = (hash * 31) + c;
        }
        return hash;
    }

    int hash(const String& key) const {
        return stringHash(key) % TABLE_SIZE;
    }
    //Hello
    // Private function to resize the hash table
   void resize(int newSize) {
        int oldSize = TABLE_SIZE;
        Entry** oldTable = table;
        TABLE_SIZE = newSize;
        table = new Entry*[TABLE_SIZE](); // Initialize to nullptr

        // Rehash all existing elements into the new table
        for (int i = 0; i < oldSize; i++) {
            Entry* current = oldTable[i];
            while (current) {
                int index = hash(current->key);
                Entry* temp = current->next;
                current->next = table[index];
                table[index] = current;
                current = temp;
            }
        }

        delete[] oldTable;
    }

public:
    Hashtable() : count(0), TABLE_SIZE(INITIAL_TABLE_SIZE) {
        table = new Entry*[TABLE_SIZE]();
    }

   ~Hashtable() {
        clear();
        delete[] table;
    }

    class HashtableIterator {
        private:
        typename Hashtable<K, V>::Entry** table;
        int TABLE_SIZE;
        int index;
        typename Hashtable<K, V>::Entry* current;

        public:
        HashtableIterator(typename Hashtable<K, V>::Entry** t, int size) : table(t), TABLE_SIZE(size), index(0), current(nullptr) {}

        bool hasNext() {
            while (!current && index < TABLE_SIZE) {
                current = table[index++];
            }
            return current != nullptr;
        }

        typename Hashtable<K, V>::Entry* next() {
            if (!hasNext()) {
                return nullptr;
            }
            typename Hashtable<K, V>::Entry* result = current;
            current = current->next;
            return result;
        }
    };
    
    void releaseMemory() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Entry *current = table[i];
            while (current) {
                Entry *next = current->next;
                delete current;
                current = next;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    V get(K key) const {
        int index = hash(key);
        Entry *current = table[index];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return V(); // Return default value
    }
    // Move your other function implementations here
    // Example:
    void put(K key, V value);
    V get(K key);
    void remove(K key);
    void clear();
    int size();
    bool isEmpty();
    SimpleVector<K> keys() const;


    // Repeat for other functions...
};
//template <typename K, typename V>
//int Hashtable<K, V>::TABLE_SIZE = 16; // Initialize it to your desired size


template <typename K, typename V>
void Hashtable<K, V>::put(K key, V value) {
    int index = hash(key);
    Entry *newEntry = new Entry(key, value);
    if (!table[index]) {
        table[index] = newEntry;
    } else{
        Entry *prev = nullptr;
        Entry *current = table[index];
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = newEntry;
                    newEntry->next = current->next;
                    delete current;
                } else {
                    table[index] = newEntry;
                    newEntry->next = current->next;
                    delete current;
                }
                return;
            }
            prev = current;
            current = current->next;
        }
        prev->next = newEntry;
    }
    count++;
    float currentLoadFactor = static_cast<float>(count) / TABLE_SIZE;
        if (currentLoadFactor >= loadFactorThreshold) {
            // Resize to double the current size
            resize(TABLE_SIZE * 2);
        }
}

template <typename K, typename V>
V Hashtable<K, V>::get(K key) {
    int index = hash(key);
    Entry *current = table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return V(); // Return default value
}

template <typename K, typename V>
void Hashtable<K, V>::remove(K key) {
    int index = hash(key);
    Entry *current = table[index];
    Entry *prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

template <typename K, typename V>
void Hashtable<K, V>::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = table[i];
        while (current) {
            Entry *next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    count = 0;
}

template <typename K, typename V>
int Hashtable<K, V>::size() {
    return count;
}

template <typename K, typename V>
bool Hashtable<K, V>::isEmpty() {
    return count == 0;
}

template <typename K, typename V>
SimpleVector<K> Hashtable<K, V>::keys() const {
    SimpleVector<K> result;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = table[i];
        while (current) {
            result.push_back(current->key);
            current = current->next;
        }
    }
    return result;
}
#endif // HASHTABLE_H
