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
        Serial.println("[HASHTABLE]: Hash of " + key + " is " + String(hash));
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

    

    // Simplified hash function that delegates to the Hash functor
    int hash(const K& key) const {
        Serial.println("[HASHTABLE]: Hashing key " + String(key));
        return hashFunction(key) % TABLE_SIZE;
    }

    // Private function to resize the hash table
    void resize(int newSize) {
        Serial.println("[HASHTABLE]: Resizing to " + String(newSize));
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
        Serial.println("[HASHTABLE]: Resized to " + String(TABLE_SIZE));
        Serial.println("[HASHTABLE]: Deleting old table");
        delete[] oldTable;
    }
public:
    struct KeyValuePair {
        K key;
        V value;
    };
    class Iterator {
    private:
        const Hashtable<K, V, Hash>* hashtable;
        int currentBucket;
        Entry* currentEntry;

        void goToNextEntry() {
            Serial.println("[HASHTABLE]: Advancing to the next entry");

            if (currentEntry && currentEntry->next) {
                currentEntry = currentEntry->next;
                Serial.println("[HASHTABLE]: Moved to next entry in the same bucket");
            } else {
                do {
                    currentBucket++;
                    if (currentBucket < hashtable->TABLE_SIZE) {
                        currentEntry = hashtable->table[currentBucket];
                        Serial.print("[HASHTABLE]: Checking bucket "); 
                        Serial.println("[HASHTABLE]: " + String(currentBucket));
                    }
                } while (!currentEntry && currentBucket < hashtable->TABLE_SIZE - 1);

                if (!currentEntry) {
                    Serial.println("[HASHTABLE]: Reached the end of the hashtable");
                } else {
                    Serial.println("[HASHTABLE]: Moved to the next available bucket");
                }
            }
        }

    public:
         // Define the dereference operator to return a key-value pair.
        KeyValuePair operator*() const {
            Serial.println("[HASHTABLE]: Dereferencing iterator");
            Serial.println("[HASHTABLE]: Current bucket: " + String(currentBucket));
            return KeyValuePair{currentEntry->key, currentEntry->value};
        }

        Iterator(const Hashtable<K, V, Hash>* ht, int bucket, Entry* entry)
        : hashtable(ht), currentBucket(bucket), currentEntry(entry) {
            Serial.println("[HASHTABLE]: Initializing iterator");
            if (!currentEntry) {
                Serial.println("[HASHTABLE]: Iterator moving to next entry");
                goToNextEntry();
            }
        }

        bool operator!=(const Iterator& other) const {
            Serial.println("[HASHTABLE]: Comparing Buckets and Entries");
            return currentEntry != other.currentEntry || currentBucket != other.currentBucket;
        }

        Iterator& operator++() {
            Serial.println("[HASHTABLE]: Incrementing iterator");
            goToNextEntry();
            return *this;
        }

       
    };

    Iterator begin() const {
        Serial.println("[HASHTABLE]: Initializing iterator");
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i]) {
                return Iterator(this, i, table[i]);
            }
        }
        return Iterator(this, TABLE_SIZE, nullptr);
    }

    Iterator end() const {
        Serial.println("[HASHTABLE]: End of Iterator");
        return Iterator(this, TABLE_SIZE, nullptr);
    }

    Hashtable() : TABLE_SIZE(INITIAL_TABLE_SIZE), count(0) {
        Serial.println("[HASHTABLE]: Initializing Hashtable");
        table = new Entry*[TABLE_SIZE]();
    }

     Hashtable(size_t initialCapacity, float loadFactor) 
        : TABLE_SIZE(initialCapacity), count(0), loadFactorThreshold(loadFactor) {
        Serial.println("[HASHTABLE]: Initializing Hashtable with custom size and load factor");
        table = new Entry*[TABLE_SIZE]();
        // Initialize buckets to nullptr...
    }

    ~Hashtable() {
        Serial.println("[HASHTABLE]: Deleting Hashtable");
        clear();
        delete[] table;
    }

    void put(const K& key, const V& value) {
        Serial.println("[HASHTABLE]: Putting key " + String(key) + " with value " + String(value));
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
        Serial.println("[HASHTABLE]: Count is " + String(count));
        Serial.println("[HASHTABLE]: Checking load factor for possible resize");
        if (static_cast<float>(count) / TABLE_SIZE > loadFactorThreshold) {
            resize(TABLE_SIZE * 2);
        }
    }
    
    V* get(const K& key) const {
        Serial.println("[HASHTABLE]: Getting value for key " + String(key));
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

//Changed to exists instead of get
    bool exists(const K& key, V& value) const {
        Serial.println("[HASHTABLE]: Checking if key: " + String(key) + " exists");
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
        Serial.println("[HASHTABLE]: Removing key " + String(key));
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
        Serial.println("[HASHTABLE]: Clearing Hashtable");
       for (int i = 0; i < TABLE_SIZE; ++i) {
           Entry* entry = table[i];
           while (entry != nullptr) {
               Entry* toDelete = entry;
               entry = entry->next;
               delete toDelete;
               Serial.println("[HASHTABLE]: Deleted entry");
           }
           table[i] = nullptr; // Make sure to still nullify the bucket after deletion
       }
       count = 0;
        Serial.println("[HASHTABLE]: Cleared Hashtable/ Now Resizing back to defualt size");
       // Resize the table back to the initial size if it's not already
       if (TABLE_SIZE > INITIAL_TABLE_SIZE) {
           resize(INITIAL_TABLE_SIZE);
           return;
       }
    }

    // Inside Hashtable
    float loadFactor() const {
        Serial.println("[HASHTABLE]: Calculating load factor");
        return static_cast<float>(count) / TABLE_SIZE;
    }

    void checkLoadFactorAndRehash() {
        Serial.println("[HASHTABLE]: Checking load factor for possible resize");
        if (loadFactor() > loadFactorThreshold) {
            resize(TABLE_SIZE * 2);
        }
    }

    // Inside Hashtable
    size_t bucketCount() const {
        Serial.println("[HASHTABLE]: Calculating bucket count");
        return TABLE_SIZE;
    }

    size_t bucketSize(size_t index) const {
        Serial.println("[HASHTABLE]: Calculating bucket size");
        size_t size = 0;
        Entry* entry = table[index];
        while (entry != nullptr) {
            ++size;
            entry = entry->next;
        }
        return size;
    }


    int size() const {
        Serial.println("[HASHTABLE]: Calculating size");
        return TABLE_SIZE;
    }

    bool isEmpty() const {
        Serial.println("[HASHTABLE]: Checking if Hashtable is empty");
        return size() == 0;
    }
    int elements() const {
        Serial.println("[HASHTABLE]: Calculating element count");
        return count;
    }

    SimpleVector<K> keys() const {
        Serial.println("[HASHTABLE]: Getting keys");
        SimpleVector<K> keys;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                keys.put(entry->key);
            }
        }
        return keys;
    }

    bool containsKey(const K& key) const {
        Serial.println("[HASHTABLE]: Checking if key exists");
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
        Serial.println("[HASHTABLE]: Checking if value exists");
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