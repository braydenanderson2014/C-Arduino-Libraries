#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <SimpleVector.h>
#include <Arduino.h>
// Forward declaration of KeyHash
template<typename K>
struct KeyHash {
    bool debug;
    KeyHash(bool debug = false) : debug(debug) {}
};

// Specialization for String
template<>
struct KeyHash<String> {
    bool debug;

    KeyHash(bool debug = false) : debug(debug) {}
    unsigned long operator()(const String& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = 31 * hash + c;
        }
        if (debug){
            Serial.println("[HASHTABLE]: Hash of " + key + " is " + String(hash));
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

// Specialization for boolean
template<>
struct KeyHash<bool> {
    unsigned long operator()(const bool& key) const {
        return static_cast<unsigned long>(key);
    }
};

// Specialization for float
template<>
struct KeyHash<float> {
    unsigned long operator()(const float& key) const {
        // This approach treats the binary representation of the float as an integer.
        // Caution: This is a simple method and might not be the best for all use cases.
        unsigned long hash = *reinterpret_cast<const unsigned long*>(&key);
        return hash;
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
    bool debug;
        
// Simplified hash function that delegates to the Hash functor
    int hash(const K& key) const {
        if(debug){
            Serial.println("[HASHTABLE]: Hashing key " + String(key));
        }
        return hashFunction(key) % TABLE_SIZE;
    }
 // Private function to resize the hash table
    bool resize(int newSize) {
        if(debug){
            Serial.println("[HASHTABLE]: Attempting to resize to " + String(newSize));
        }
        Entry** newTable = new Entry*[newSize]();
        if (!newTable) {
            if(debug){
                Serial.println("[HASHTABLE]: Error - Unable to allocate memory for resizing");
            }
            return false; // Memory allocation failed
        }

        for (int i = 0; i < TABLE_SIZE; ++i) {
            Entry* entry = table[i];
            while (entry) {
                Entry* next = entry->next;
                int index = hash(entry->key) % newSize; // Hash with respect to the new table size
                entry->next = newTable[index];
                newTable[index] = entry;
                entry = next;
            }
        }

        delete[] table; // Free the old table
        table = newTable; // Update the pointer to the new table
        TABLE_SIZE = newSize; // Update the size
        if(debug){
            Serial.println("[HASHTABLE]: Successfully resized to " + String(TABLE_SIZE));
        }
        return true;
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
        bool debug;

        void goToNextEntry() {
            if(debug){
                Serial.println("[HASHTABLE]: Advancing to the next entry");
            }

            if (currentEntry && currentEntry->next) {
                currentEntry = currentEntry->next;
                if(debug){
                    Serial.println("[HASHTABLE]: Moved to next entry in the same bucket");
                }
            } else {
                do {
                    currentBucket++;
                    if (currentBucket < hashtable->TABLE_SIZE) {
                        currentEntry = hashtable->table[currentBucket];
                        if(debug){
                            Serial.print("[HASHTABLE]: Checking bucket "); 
                            Serial.println("[HASHTABLE]: " + String(currentBucket));
                        }
                    }
                } while (!currentEntry && currentBucket < hashtable->TABLE_SIZE - 1);

                if (!currentEntry) {
                    if(debug){
                        Serial.println("[HASHTABLE]: Reached the end of the hashtable");
                    }
                } else {
                    if(debug){
                        Serial.println("[HASHTABLE]: Moved to the next available bucket");
                    }
                }
            }
        }

    public:
         // Define the dereference operator to return a key-value pair.
        KeyValuePair operator*() const {
            if(debug){
                Serial.println("[HASHTABLE]: Dereferencing iterator");
                Serial.println("[HASHTABLE]: Current bucket: " + String(currentBucket));
            }
            return KeyValuePair{currentEntry->key, currentEntry->value};
        }

        Iterator(const Hashtable<K, V, Hash>* ht, int bucket, Entry* entry, bool debug = false)
        : hashtable(ht), currentBucket(bucket), currentEntry(entry), debug(debug) {
            if(debug){
                Serial.println("[HASHTABLE]: Initializing iterator");
            }
            if (!currentEntry) {
                if(debug){
                    Serial.println("[HASHTABLE]: Iterator moving to next entry");
                }
                goToNextEntry();
            }
        }

        bool operator!=(const Iterator& other) const {
            if(debug){
                Serial.println("[HASHTABLE]: Comparing Buckets and Entries");
            }
            return currentEntry != other.currentEntry || currentBucket != other.currentBucket;
        }

        Iterator& operator++() {
            if(debug){
                Serial.println("[HASHTABLE]: Incrementing iterator");
            }
            goToNextEntry();
            return *this;
        }

       
    };

    Iterator begin() const {
        if(debug){
            Serial.println("[HASHTABLE]: Initializing iterator");
        }
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i]) {
                return Iterator(this, i, table[i], debug);
            }
        }
        return Iterator(this, TABLE_SIZE, nullptr, debug);
    }

    Iterator end() const {
        if(debug){
            Serial.println("[HASHTABLE]: End of Iterator");
        }
        return Iterator(this, TABLE_SIZE, nullptr, debug);
    }

    Hashtable() : TABLE_SIZE(INITIAL_TABLE_SIZE), count(0) {
        if(debug){
            Serial.println("[HASHTABLE]: Initializing Hashtable");
        }
        table = new Entry*[TABLE_SIZE]();
    }

    Hashtable(size_t initialCapacity, float loadFactor, bool debug = false) 
        : TABLE_SIZE(initialCapacity), count(0), loadFactorThreshold(loadFactor), debug(debug), hashFunction(debug) {
        if(debug){
            Serial.println("[HASHTABLE]: Initializing Hashtable with custom size and load factor");
        }
        table = new Entry*[TABLE_SIZE]();
        // Initialize buckets to nullptr...
    }

    ~Hashtable() {
        Serial.println("[HASHTABLE]: Deleting Hashtable");
        clear();
        delete[] table;
    }

    void setDebug(bool debug) {
        if(this -> debug == debug){
            Serial.println("[HASHTABLE]: Setting debug mode");
        }
        this->debug = debug;
    }

    bool getDebug() const {
        return this->debug;
    }

    void put(const K& key, const V& value) {
        if(debug){
            Serial.println("[HASHTABLE]: Putting key " + String(key) + " with value " + String(value));
        }
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
        //Serial.println("[HASHTABLE]: Count is " + String(count));
        if(debug){
            Serial.println("[HASHTABLE]: Checking load factor for possible resize");
        }
        if (static_cast<float>(count) / TABLE_SIZE > loadFactorThreshold) {
            if (!resize(TABLE_SIZE * 2)) {
                if(debug){
                    Serial.println("[HASHTABLE]: Resize failed. Ignoring the new element");
                }
                // Handle the error as necessary, such as by not adding the new element
                return;
            }
        }
    }
    
    V* get(const K& key) const {
        if(debug){
            Serial.println("[HASHTABLE]: Getting value for key " + String(key));
        }
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
        if(debug){
            Serial.println("[HASHTABLE]: Checking if key: " + String(key) + " exists");
        }
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
        if(debug){
            Serial.println("[HASHTABLE]: Removing key " + String(key));
        }
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
        if(debug){
            Serial.println("[HASHTABLE]: Clearing Hashtable");
        }
       for (int i = 0; i < TABLE_SIZE; ++i) {
           Entry* entry = table[i];
           while (entry != nullptr) {
               Entry* toDelete = entry;
               entry = entry->next;
               delete toDelete;
                if(debug){
                    Serial.println("[HASHTABLE]: Deleted entry");
                }
           }
           table[i] = nullptr; // Make sure to still nullify the bucket after deletion
       }
       count = 0;
        if(debug){
            Serial.println("[HASHTABLE]: Cleared Hashtable/ Now Resizing back to defualt size");
        }
       // Resize the table back to the initial size if it's not already
       if (TABLE_SIZE > INITIAL_TABLE_SIZE) {
           resize(INITIAL_TABLE_SIZE);
           return;
       }
    }

    // Inside Hashtable
    float loadFactor() const {
        if(debug){
            Serial.println("[HASHTABLE]: Calculating load factor");
        }
        return static_cast<float>(count) / TABLE_SIZE;
    }

    void checkLoadFactorAndRehash() {
        if(debug){
            Serial.println("[HASHTABLE]: Checking load factor for possible resize");
        }
        if (loadFactor() >= loadFactorThreshold) {
            resize(TABLE_SIZE * 2);
        }
    }

    // Inside Hashtable
    size_t bucketCount() const {
        if(debug){
            Serial.println("[HASHTABLE]: Calculating bucket count");
        }
        return TABLE_SIZE;
    }

    size_t bucketSize(size_t index) const {
        if(debug){
            Serial.println("[HASHTABLE]: Calculating bucket size");
        }
        size_t size = 0;
        Entry* entry = table[index];
        while (entry != nullptr) {
            ++size;
            entry = entry->next;
        }
        return size;
    }


    int size() const {
        if(debug){
            Serial.println("[HASHTABLE]: Calculating size");
        }
        return TABLE_SIZE;
    }

    bool isEmpty() const {
        if(debug){
            Serial.println("[HASHTABLE]: Checking if Hashtable is empty");
        }
        return size() == 0;
    }
    int elements() const {
        if(debug){
            Serial.println("[HASHTABLE]: Calculating element count");
        }
        return count;
    }

    SimpleVector<K> keys() const {
        if(debug){
            Serial.println("[HASHTABLE]: Getting keys");
        }
        SimpleVector<K> keys;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                keys.put(entry->key);
            }
        }
        return keys;
    }

    bool containsKey(const K& key) const {
        if(debug){
            Serial.println("[HASHTABLE]: Checking if key exists");
        }
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
        if(debug){
            Serial.println("[HASHTABLE]: Checking if value exists");
        }
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