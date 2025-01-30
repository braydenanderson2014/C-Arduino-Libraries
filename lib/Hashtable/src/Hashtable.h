#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <SimpleVector.h>
#include <Arduino.h>

// Forward declaration of KeyHash
/**
 * @brief A functor that hashes a key of type K
*/
template<typename K>
struct KeyHash {
    KeyHash() {}
};

// Specialization for String
/**
 * @brief A functor that hashes a String key
 * @details This is a specialization of the KeyHash functor for String keys.
*/
template<>
struct KeyHash<String> {
    KeyHash() {}
    unsigned long operator()(const String& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = 31 * hash + c;
        }     
        return hash;
    }
};

// Specialization for int
/**
 * @brief A functor that hashes an int key
 * @details This is a specialization of the KeyHash functor for int keys.
*/
template<>
struct KeyHash<int> {
    unsigned long operator()(const int& key) const {
        return static_cast<unsigned long>(key);
    }
};

// Specialization for boolean
/**
 * @brief A functor that hashes a boolean key
 * @details This is a specialization of the KeyHash functor for boolean keys.
*/
template<>
struct KeyHash<bool> {
    unsigned long operator()(const bool& key) const {
        return static_cast<unsigned long>(key);
    }
};

// Specialization for float
/**
 * @brief A functor that hashes a float key
 * @details This is a specialization of the KeyHash functor for float keys.
*/
template<>
struct KeyHash<float> {
    unsigned long operator()(const float& key) const {
        // This approach treats the binary representation of the float as an integer.
        // Caution: This is a simple method and might not be the best for all use cases.
        unsigned long hash = *reinterpret_cast<const unsigned long*>(&key);
        return hash;
    }
};

// Specialization for double
/**
 * @brief A functor that hashes a double key
 * @details This is a specialization of the KeyHash functor for double keys.
*/
template<>
struct KeyHash<double> {
    unsigned long operator()(const double& key) const {
        // This approach treats the binary representation of the double as an integer.
        // Caution: This is a simple method and might not be the best for all use cases.
        unsigned long hash = *reinterpret_cast<const unsigned long*>(&key);
        return hash;
    }
};
template <typename K, typename V, typename Hash = KeyHash<K>>
class Hashtable {
private:
    /**
     * @brief A struct representing a key-value pair
     * @details This struct represents a key-value pair in the hash table.
     * 
     * @note This struct is private because it is only used internally.
     * @param key The key of the pair
     * @param value The value of the pair
     * @param next A pointer to the next entry in the bucket
     * 
    */
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    static const int INITIAL_TABLE_SIZE = 16; // The initial size of the table
    Entry** table; // The table itself
    int TABLE_SIZE; // The current size of the table
    int count;  // The number of elements in the table
    float loadFactorThreshold = 0.7; // The load factor threshold for resizing
    Hash hashFunction; // The hash function to use
        
// Simplified hash function that delegates to the Hash functor
    /**
     * @brief Hashes a key
     * @details This function hashes a key of type K using the Hash functor.
     * @param key The key to hash
     * @return The hash of the key
    */
    int hash(const K& key) const {
        return hashFunction(key) % TABLE_SIZE;
    }

    /**
    * @brief Calculates the index of a key in the hash table
    * @details This function calculates the index of a key in the hash table.
    * @param key The key to calculate the index for
    * @param tableSize The size of the hash table
    * @return The index of the key in the hash table
    */
    int calculateIndex(const K& key, int tableSize) const {
        return hashFunction(key) % tableSize;
    }
 // Private function to resize the hash table
    /**
     * @brief Resize the hash table
     * @details This function resizes the hash table to the specified size.
     * @param newSize The new size of the hash table
     * 
     * @note This function is private because it is only used internally.
     * @note This function will rehash all elements in the table.
     * @note This function will delete the old table.
     * @note This function will update the TABLE_SIZE member variable.
     * @note This function will return false if the memory allocation fails.
     * @note This function will return true if the resize is successful.
     * 
     * @return Whether or not the resize was successful
    */
    bool resize(int newSize) {
        Entry** newTable = new Entry*[newSize]();
        if (!newTable) {
            return false; // Memory allocation failed
        }
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Entry* entry = table[i];
            while (entry) {
                Entry* next = entry->next;
                int index = calculateIndex(entry->key, newSize); // Hash with respect to the new table size
                entry->next = newTable[index];
                newTable[index] = entry;
                entry = next;
            }
        }

        delete[] table; // Free the old table
        table = newTable; // Update the pointer to the new table
        TABLE_SIZE = newSize; // Update the size
        return true;
    }
public:
    /**
     * @brief A struct representing a key-value pair
     * @details This struct represents a key-value pair in the hash table.
     * 
     * @param key The key of the pair
     * @param value The value of the pair
    */
    struct KeyValuePair {
        K key;
        V value;
    };
    class Iterator {
    private:
        const Hashtable<K, V, Hash>* hashtable; // Pointer to the hash table
        int currentBucket; // The current bucket
        Entry* currentEntry; // The current entry

        /**
         * @brief Move to the next entry in the hash table
         * @details This function moves the iterator to the next entry in the hash table.
         * 
        */
        void goToNextEntry() {
            if (currentEntry && currentEntry->next) {
                // Move to the next entry in the current bucket
                currentEntry = currentEntry->next;
            } else {
                // Move to the next bucket with entries
                currentEntry = nullptr;  // Reset currentEntry to avoid stale data
                while (++currentBucket < hashtable->TABLE_SIZE) {
                    if (hashtable->table[currentBucket]) {
                        currentEntry = hashtable->table[currentBucket];
                        break;
                    }
                }
                // Stop incrementing if beyond the last bucket
                if (currentBucket >= hashtable->TABLE_SIZE) {
                    currentBucket = hashtable->TABLE_SIZE; // Cap to table size
                }
            }
        }



    public:
         // Define the dereference operator to return a key-value pair.
        /**
         * @brief Dereference operator
         * @details This operator returns a key-value pair.
         * 
         * @note This operator is defined inside the Iterator class.
         * @note This operator is used to dereference the iterator.
         * @note This operator returns a key-value pair.
        **/
        KeyValuePair operator*() const {
            if (!currentEntry) {
                return KeyValuePair{"", ""}; // Return an empty key-value pair if invalid
            }
            return KeyValuePair{currentEntry->key, currentEntry->value};
        }

        /**
         * @brief Constructor
         * @details This constructor creates an iterator for the hash table.
         * 
         * @note This constructor is defined inside the Iterator class.
         * @note This constructor is used to create an iterator for the hash table.
         * @note This constructor takes a pointer to the hash table, the current bucket, and the current entry.
         * 
         * @param ht A pointer to the hash table
         * @param bucket The current bucket
         * @param entry The current entry
         * 
        */
        Iterator(const Hashtable<K, V, Hash>* ht, int bucket, Entry* entry)
        : hashtable(ht), currentBucket(bucket), currentEntry(entry) {
            if (!currentEntry) {
                goToNextEntry();
            }
        }

        /**
         * @brief Inequality operator
         * @details This operator checks if two iterators are not equal.
         * 
         * @note This operator is defined inside the Iterator class.
         * @note This operator is used to check if two iterators are not equal.
         * @note This operator returns true if the iterators are not equal, otherwise it returns false.
         * 
         * @param other The other iterator to compare to
         * 
         * @return Whether or not the iterators are not equal
        */
        bool operator!=(const Iterator& other) const {
           // Ensure both currentEntry and currentBucket match
           return currentEntry != other.currentEntry || currentBucket != other.currentBucket;
        }



        /**
         * @brief Prefix increment operator
         * @details This operator increments the iterator to the next entry in the hash table.
         * 
         * @note This operator is defined inside the Iterator class.
         * @note This operator is used to increment the iterator to the next entry in the hash table.
         * @note This operator returns a reference to the iterator.
         * 
         * @return A reference to the iterator
        */
        Iterator& operator++() {
            goToNextEntry();
            return *this;
        }

        Iterator& find(const K& key) {
            int index = hashtable->hash(key);
            currentBucket = index;
            currentEntry = hashtable->table[index];
            while (currentEntry != nullptr) {
                if (currentEntry->key == key) {
                    return *this;
                }
                currentEntry = currentEntry->next;
            }
            goToNextEntry();
            return *this;
        }

        SimpleVector<K> getKeys() {
            SimpleVector<K> keys;
            for (int i = 0; i < hashtable->TABLE_SIZE; ++i) {
                for (Entry* entry = hashtable->table[i]; entry != nullptr; entry = entry->next) {
                    keys.put(entry->key);
                }
            }
            return keys;
        }

        SimpleVector<V> getValues() {
            SimpleVector<V> values;
            for (int i = 0; i < hashtable->TABLE_SIZE; ++i) {
                for (Entry* entry = hashtable->table[i]; entry != nullptr; entry = entry->next) {
                    values.put(entry->value);
                }
            }
            return values;
        }
    };

    /**
     * @brief Get an iterator pointing to the first element in the hash table
     * @details This function returns an iterator pointing to the first element in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get an iterator pointing to the first element in the hash table.
     * @note This function returns an iterator pointing to the first element in the hash table.
     * 
     * @return An iterator pointing to the first element in the hash table
    */
    Iterator begin() const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i]) {
                return Iterator(this, i, table[i]);
            }
        }
        return end();
    }

    /**
     * @brief Get an iterator pointing to the end of the hash table
     * @details This function returns an iterator pointing to the end of the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get an iterator pointing to the end of the hash table.
     * @note This function returns an iterator pointing to the end of the hash table.
     * 
     * @return An iterator pointing to the end of the hash table
    */
    Iterator end() const {
        return Iterator(this, TABLE_SIZE, nullptr);
    }


    /**
     * @brief Constructor
     * @details This constructor creates a hash table with the specified initial capacity and load factor.
     * 
     * @param INITIAL_TABLE_SIZE The initial capacity of the hash table
     * 
    */
    Hashtable() : TABLE_SIZE(INITIAL_TABLE_SIZE), count(0), hashFunction() {
        table = new Entry*[TABLE_SIZE]();
    }

    /**
     * @brief Constructor
     * @details This constructor creates a hash table with the specified initial capacity and load factor.
     * 
     * @param initialCapacity The initial capacity of the hash table
     * @param loadFactor The load factor of the hash table
     * 
     * @note This constructor is defined inside the Hashtable class.
     * @note This constructor is used to create a hash table with the specified initial capacity and load factor.
     * @note This constructor takes the initial capacity and load factor as parameters.
     * 
    */
    Hashtable(size_t initialCapacity, float loadFactor) 
        : TABLE_SIZE(initialCapacity), count(0), loadFactorThreshold(loadFactor), hashFunction() {
        table = new Entry*[TABLE_SIZE]();
        // Initialize buckets to nullptr...
    }

    /**
     * @brief Destructor
     * @details This destructor destroys the hash table.
     * 
     * @note This destructor is defined inside the Hashtable class.
     * @note This destructor is used to destroy the hash table.
     * 
    */
    ~Hashtable() {
        clear();
        delete[] table;
    }

    /**
     * @brief Put a key-value pair in the hash table
     * @details This function puts a key-value pair in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to put a key-value pair in the hash table.
     * @note This function takes the key and value as parameters.
     * 
     * @param key The key of the pair
     * @param value The value of the pair
     * 
     * @note If the key already exists, the value will be overwritten.
    */
    void put(const K& key, const V& value) {
        int index = calculateIndex(key, TABLE_SIZE);
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
            if (!resize(TABLE_SIZE * 2)) {
                // Handle the error as necessary, such as by not adding the new element
                return;
            }
        }
    }
    
    /**
     * @brief Get the value associated with a key
     * @details This function gets the value associated with a key.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the value associated with a key.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to get the value for
     * 
     * @note If the key does not exist, a nullptr will be returned.
     * 
     * @return The value associated with the key
    */
    V* get(const K& key) const {
        int index = calculateIndex(key, TABLE_SIZE);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return &(entry->value); // Return the address of the value
            }
            entry = entry->next;
        }
        return nullptr; // Return null if the key is not found
    }

    /**
     * @brief Returns the element stored with the specified key
     * 
     * @note if a key does not exits, return the Default constructor for the type specified.
     * 
     * @return The value associated with the key, or a default constructed object if key not found.
    */
    V getElement(const K& key) const {
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }
        return V();
    }

    /**
    * @brief Returns the element stored with the specified key
    * 
    * @param key The key to search for
    * @param value Pointer to a variable where the found value will be stored
    * 
    * @return true if the key was found and the value was stored, false if the key was not found
    * 
    * @note If the key is found, the function stores the associated value in the variable pointed to by the value parameter.
    *       If the key is not found, the function returns false and does not modify the variable pointed to by the value parameter.
    */
    bool getElement(const K& key, V* value) const{
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                *value = entry->value;
                return true;
            }
            entry = entry->next;
        }
        return false; // Return false if the key is not found
    }
    
    /**
     * @brief Check if a key exists in the hash table
     * @details This function checks if a key exists in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to check if a key exists in the hash table.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to check for
    */
    bool exists(const K& key) const {
        int index = calculateIndex(key, TABLE_SIZE);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;
        }
        return false;
    }

    /**
     * @brief Check if a key exists in the hash table
     * @details This function checks if a key exists in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to check if a key exists in the hash table.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to check for
     * @param value The value to check for
     * 
     * @return Whether or not the key exists in the hash table
    */
    bool exists(const K& key, V& value) const {
        int index = calculateIndex(key, TABLE_SIZE);
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

    /**
     * @brief Remove a key-value pair from the hash table
     * @details This function removes a key-value pair from the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to remove a key-value pair from the hash table.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to remove
     * 
     * @return Whether or not the key was removed
    */
    bool remove(const K& key) {
        int index = calculateIndex(key, TABLE_SIZE);
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

    /**
     * @brief Clear the hash table
     * @details This function clears the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to clear the hash table.
    */
    void clear() {
       for (int i = 0; i < TABLE_SIZE; ++i) {
           Entry* entry = table[i];
           while (entry != nullptr) {
               Entry* toDelete = entry;
               entry = entry->next;
               delete toDelete;
           }
           table[i] = nullptr; // Make sure to still nullify the bucket after deletion
       }
       count = 0;
       // Resize the table back to the initial size if it's not already
       if (TABLE_SIZE > INITIAL_TABLE_SIZE) {
           resize(INITIAL_TABLE_SIZE);
           return;
       }
    }

    // Inside Hashtable
    /**
     * @brief Get the load factor of the hash table
     * @details This function gets the load factor of the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the load factor of the hash table.
     * 
     * @return The load factor of the hash table
    */
    float loadFactor() const {
        return static_cast<float>(count) / TABLE_SIZE;
    }

    /**
     * @brief Checks to see if the load factor is greater than or equal to the load factor threshold and resizes, and rehashes if it is
     * @details This function checks to see if the load factor is greater than or equal to the load factor threshold and resizes, and rehashes if it is.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to check to see if the load factor is greater than or equal to the load factor threshold and resizes, and rehashes if it is.
     * 
    */
    bool checkLoadFactorAndRehash() {
        bool success = false;
        if (loadFactor() >= loadFactorThreshold) {
            success = resize(TABLE_SIZE * 2);
        }
        return success;
    }

    // Inside Hashtable
    /**
     * @brief Get the size of the hash table (the number of buckets)
     * @details This function gets the size of the hash table (the number of buckets).
     * @return The size of the hash table
     * 
    */
    size_t bucketCount() const {
        return TABLE_SIZE;
    }

    /**
     * @brief Get the size of a bucket
     * @details This function gets the size of a bucket.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the size of a bucket.
     * @note This function takes the index of the bucket as a parameter.
     * 
     * @param index The index of the bucket
     * 
     * @return The size of the bucket
    */
    size_t bucketSize(size_t index) const {
        size_t size = 0;
        Entry* entry = table[index];
        while (entry != nullptr) {
            ++size;
            entry = entry->next;
        }
        return size;
    }

    /**
     * @brief Get the size of the hash table (the Capacity)
     * @details This function gets the size of the hash table (the Capacity).
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the size of the hash table (the Capacity).
     * 
     * @return The Capacity of the hash table
    */
    int size() const {
        return TABLE_SIZE;
    }

    /**
     * @brief Check if the hash table is empty
     * @details This function checks if the hash table is empty.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to check if the hash table is empty.
     * 
     * @return Whether or not the hash table is empty
    */
    bool isEmpty() const {
        return size() == 0;
    }

    /**
     * @brief Get the number of elements in the hash table
     * @details This function gets the number of elements in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the number of elements in the hash table.
     * 
     * @return The number of elements in the hash table
    */
    int elements() const {
        return count;
    }

    /**
     * @brief Get the keys in the hash table
     * @details This function gets the keys in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the keys in the hash table.
     * 
     * @return The keys in the hash table
    */
    SimpleVector<K> keys() const {
        SimpleVector<K> keys;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                keys.put(entry->key);
            }
        }
        return keys;
    }

    /**
     * @brief Checks to see if the key exists in the hash table
     * @details This function checks to see if the key exists in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to check to see if the key exists in the hash table.
     * 
     * @param key The key to check for
     * 
     * @return Whether or not the key exists in the hash table
    */
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

     /**
      * @brief Checks to see if the value exists in the hash table
      * @details This function checks to see if the value exists in the hash table.
      * 
      * @note This function is defined inside the Hashtable class.
      * @note This function is used to check to see if the value exists in the hash table.
      * 
      * @param value The value to check for
      * 
      * @return Whether or not the value exists in the hash table
    */
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

    /**
     * @brief Get the values in the hash table
     * @details This function gets the values in the hash table.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the values in the hash table.
     * 
     * @return The values in the hash table
    */
    SimpleVector<V> values() const {
        SimpleVector<V> values;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (Entry* entry = table[i]; entry != nullptr; entry = entry->next) {
                values.put(entry->value);
            }
        }
        return values;
    }

    //operator[]
    /**
     * @brief Get the value associated with a key
     * @details This function gets the value associated with a key.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the value associated with a key.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to get the value for
     * 
     * @return The value associated with the key
    */
    V& operator[](const K& key) {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }
        // If the key does not exist, create a new entry with the default value
        V defaultValue = V();
        put(key, defaultValue);
        return table[index]->value;
    }

    /**
     * @brief Get the value associated with a key
     * @details This function gets the value associated with a key.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the value associated with a key.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to get the value for
     * 
     * @return The value associated with the key
    */
    const V& operator[](const K& key) const {
        int index = hash(key);
        Entry* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                return entry->value;
            }
            entry = entry->next;
        }
        // If the key does not exist, return the default value
        return V();
    }

    //get bucket

    /**
     * @brief Get the bucket associated with a key
     * @details This function gets the bucket associated with a key.
     * 
     * @note This function is defined inside the Hashtable class.
     * @note This function is used to get the bucket associated with a key.
     * @note This function takes the key as a parameter.
     * 
     * @param key The key to get the bucket for
     * 
     * @return The bucket associated with the key
    */
    Entry* getBucket(int index) const {
        if (index >= 0 && index < TABLE_SIZE) {
            return table[index];
        }
        return nullptr;
    }

    //getbucketsize
    size_t getBucketSize(int index) const {
        if (index >= 0 && index < TABLE_SIZE) {
            return bucketSize(index);
        }
        return 0;
    }

    /**
     * @brief Inbuilt function to print the hash table.
     * @details This function prints the hash table by iterating over it using the Iterator class.
     * 
     * @note Function can be used as a normal Iterator or an example of an iterator if you dont feel
     * like trying to set it up yourself. :)
     */
    void debugIterator() {
        Serial.println("[HASHTABLE]: Debugging Iterator:");
        auto it = begin();
        while (it != end()) {
            auto kv = *it;
            Serial.print("[HASHTABLE ITERATOR]: Key: ");
            Serial.print(kv.key);
            Serial.print(", Value: ");
            Serial.println(kv.value);
            ++it;
        }
        Serial.println("[HASHTABLE]: Iterator completed successfully.");
    }
};

#endif // HASHTABLE_H