#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "iterator.h"

template <typename K, typename V>
class Dictionary {
public:
    // Default constructor
    Dictionary() {}

    // Destructor
    virtual ~Dictionary() {}

    // Abstract methods to manage dictionary
    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual V get(const K& key) const = 0;
    virtual void put(const K& key, const V& value) = 0;
    virtual V remove(const K& key) = 0;

    // Abstract method to get an iterator over keys
    virtual Iterator<K>& keys() const = 0;

    // Abstract method to get an iterator over values
    virtual Iterator<V>& values() const = 0;

    // Additional methods can be defined here as needed
};

// Example usage (uncomment and implement the following code to test the class)
/*
class MyDictionary : public Dictionary<int, String> {  // Example
    // Implementation of Dictionary's pure virtual functions
    // You need to define the data structure for storing the dictionary
    // entries and implement these methods accordingly.
    // ...
};
*/

#endif // DICTIONARY_H
