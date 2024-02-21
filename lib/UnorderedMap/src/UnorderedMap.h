#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "Hashtable.h"

template <typename KeyType, typename ValueType, typename Hash = KeyHash<KeyType>>
class UnorderedMap {
private:
    Hashtable<KeyType, ValueType, Hash> hashtable = Hashtable<KeyType, ValueType, Hash>();

public:
    // Forward the constructor arguments
    // Forward the constructor arguments
    UnorderedMap(size_t initialCapacity = 16, float loadFactor = 0.7) : hashtable(initialCapacity, loadFactor) {}


    // Forward the destructor
    ~UnorderedMap() = default;

    // Forward the put operation
    /**
     * @brief Put a key-value pair into the map
     * @param key The key to insert
     * @param value The value to insert
     * 
     * @implements Hashtable::put
     * @note If the key already exists, the value will be overwritten.
     * 
     */
    void put(const KeyType& key, const ValueType& value) {
        hashtable.put(key, value);
    }

    // Forward the get operation (by reference and by value)
    /**
     * @brief Get the value associated with a key
     * @param key The key to get the value for
     * @return The value associated with the key
     * 
     * @implements Hashtable::get
     * @note If the key does not exist, a default-constructed value will be returned.
     * 
     */
    ValueType getValue(const KeyType& key) const {
        return *hashtable.get(key);
    }

    KeyType getKey(const KeyType& key) const {
        return hashtable.getKey(key);
    }
    // Forward the containsKey operation
    /**
     * @brief Check if a key exists in the map
     * @param key The key to check for
     * @return Whether or not the key exists in the map
     * 
     * @implements Hashtable::containsKey
     * 
     */
    bool contains(const KeyType& key) const {
        return hashtable.containsKey(key);
    }

    // Forward the remove operation
    /**
     * @brief Remove a key-value pair from the map
     * @param key The key to remove
     * @return Whether or not the key was removed
     * 
     * @implements Hashtable::remove
     * 
     */
    bool remove(const KeyType& key) {
        hashtable.remove(key);
        if(contains(key)){
            return false;
        } else {
            return true;
        }
    }

    // Forward the clear operation
    /**
     * @brief Clear the map
     * 
     * @implements Hashtable::clear
     * 
     */
    void clear() {
        return hashtable.clear();
    }

    // Forward the size operation
    /**
     * @brief Get the number of elements in the map
     * @return The number of elements in the map
     * 
     * @implements Hashtable::elements
    */
    size_t getSize() const {
        return hashtable.elements();
    }

    // Forward the capacity operation
    /**
     * @brief Get the capacity of the map
     * @return The capacity of the map
     * 
     * @implements Hashtable::size
     * 
     */
    size_t getCapacity() const {
        return hashtable.size();
    }

    // Forward the isEmpty operation
    /**
     * @brief Check if the map is empty
     * @return Whether or not the map is empty
     * 
     * @implements Hashtable::isEmpty
     * 
     */
    bool isEmpty() const {
        return hashtable.isEmpty();
    }

    // Provide a begin() iterator operation
    /**
     * @brief Get an iterator to the beginning of the map
     * @return An iterator to the beginning of the map
    */
    typename Hashtable<KeyType, ValueType, Hash>::Iterator begin() const {
        return hashtable.begin();
    }

    // Provide an end() iterator operation
    /**
     * @brief Get an iterator to the end of the map
     * @return An iterator to the end of the map
    */
    typename Hashtable<KeyType, ValueType, Hash>::Iterator end() const {
        return hashtable.end();
    }

    // Forward other operations as needed...
    // Inside UnorderedMap
    /**
     * @brief Get the hash function used by the map
     * @return The hash function used by the map
     * 
    */
    Hash hashFunction() const {
        return hashtable.hashFunction;
    }

    // Inside UnorderedMap
    /**
     * @overload operator[] 
     * Overloads the subscript operator
     * 
     * This function is used to access or modify elements in the map.
     * If the key does not exist in the map, a new element with the key and a default-constructed value is inserted.
     * 
     * @param key The key of the element to be accessed or modified.
     * @return Reference to the value associated with the key.
    */
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
    /**
     * Finds an element with a specific key in the hashtable.
     * 
     * This function is used to find an element in the map by its key.
     * It returns an iterator pointing to the element if the key is found, 
     * otherwise it returns an iterator pointing to the end of the map.
     * 
     * @param key The key of the element to be found.
     * @return An iterator pointing to the element if found, otherwise an iterator pointing to the end of the map.
    */
    typename Hashtable<KeyType, ValueType, Hash>::Iterator find(const KeyType& key) const {
        return hashtable.find(key); // Assuming Hashtable has a find method
    }

   /**
     * @brief Returns whether a specific key exists in the hashtable.
     * 
     * This function is used to check if a key is present in the map.
     * If the key exists in the map, it returns 1, otherwise it returns 0.
     * 
     * @param key The key to be checked in the map.
     * @return 1 if the key exists in the map, 0 otherwise.
    */
    size_t count(const KeyType& key) const {
        return hashtable.containsKey(key) ? 1 : 0;
    }

// Implement equal_range if needed, returning a pair of iterators

// Key equality is implicit in your design, so you can provide a placeholder if needed.

};

#endif // UNORDERED_MAP_H
