# Hashtable Class

The `Hashtable` class is a C++ template class that provides an implementation of a hash table for storing key-value pairs. It allows you to efficiently store and retrieve data using keys, similar to a dictionary or map in other programming languages.

## Overview

The `Hashtable` class is designed to offer a dynamic and scalable hash table with the following features:

- **Dynamic Sizing**: The hash table dynamically resizes itself when the load factor exceeds a specified threshold, ensuring efficient memory usage.
- **Collision Handling**: It uses separate chaining to handle collisions, allowing multiple key-value pairs with the same hash value to be stored and retrieved correctly.
- **Iterator Support**: You can iterate through the keys in the hash table using iterators, making it easy to perform operations on the keys and their associated values.

## WARNING: This Library Utilizes POINTERS *. This is due to the Libraries ability to utilize any return type. (Bool, String, int, float, etc)

### Key Features

- `put(K key, V value)`: Associates the given key with the specified value in the hash table.
- `get(K key)`: Retrieves the pointer to the value associated with the given key.
- `getElement(K key)` : Retrieves the value associated with the given key
- `remove(K key)`: Removes the key-value pair associated with the given key.
- `clear()`: Clears all key-value pairs from the hash table.
- `size()`: Returns the size of the table.
- `elements()`: Returns the number of elements in the table.
- `isEmpty()`: Checks if the hash table is empty.
- `keys()`: Returns an iterator for iterating over the keys in the hash table.
- `merge(const Hashtable<K, V>& other)`: Merges the contents of another hash table into the current one.
- `containsKey(const K& key)`: Check if a Table contains a key.
- `containsValue(const V& value)`: Check if a Table contains a Value.
- `begin()` : Begins Iteration
- `end()` : ends Iteration
- `loadFactor()` : Returns calculated load factor
- `checkLoadFactorAndRehash()`: checks the load factor and rehashes the table if needed
- `bucketCount()` : returns the number of buckets in the table
- `bucketSize()` : returns the size of a bucket in the table
- `values()` : Returns an iterator for iterating over the values in the hash table.
- `exists()` : Checks to see if a item exists on the table.


#### Iterator Functions

- `Iterator const Hashtable<K, V, Hash>* ht, int bucket, Entry* entry): hashtable(ht), currentBucket(bucket), currentEntr (entry) ` : Constructor
- `bool operator!=(const Iterator& other)` : Return currentEntry != other.currentEntry || currentBucket != other.currentBucket
- `Iterator& operator++()` : Moves iteration to the next element, the returns that element
- `Entry* operator*()` : returns currentEntry
- `find()` : Returns a value associated with the key
- `getKey()` Returns a Vector of Keys
- `getValues()` Returns a Vector of Values



## Usage

To use the `Hashtable` class, follow these steps:

1. Include the `Hashtable.h` header in your C++ program.
2. Create an instance of the `Hashtable` class with specific data types for keys and values.
3. Use the provided methods to add, retrieve, or remove key-value pairs from the hash table.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Hashtable.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <Hashtable.h> 
```


# Arduino:
## ChangeLog
### Version 1.0.0:
* Initial Release. Based off of PlatformIO version v1.1.2 
### Version 1.0.1- BETA:
* Added New getKey() Function for Iterator
* Added New getValue() Function for Iterator
* Added New find() Function for Iterator.
* NOTE: All 3 functions are not tested and should be used with Caution. 
### Version 1.0.1: 
* RESOLVED ISSUE: Iterator Iterates out of bounds (Issue #85)
* RESOLVED ISSUE: Resizing corrupts data in hashtable (Issue #81)
* RESOLVED ISSUE: get() and getElement() returning incorrectly (Issue #82)
* Added new operator[] function that allows you to access elements by index in the Hashtable
* Added new getBucket() function that returns the bucket at a specific index in the Hashtable
* Added new getBucketSize() function that returns the size of a bucket at a specific index in the Hashtable
* Added new debugIterator() function that iterates the table and prints out the [key, value] pairs.
* Adjusted Iterator operator* function. 
[FROM THIS]
```cpp
    KeyValuePair operator*() const {
        
        return KeyValuePair{currentEntry->key, currentEntry->value};
    }
```
[TO THIS]
```cpp
    KeyValuePair operator*() const {
        if (!currentEntry) {
            return KeyValuePair{"", ""}; // Return an empty key-value pair if invalid
        }
        return KeyValuePair{currentEntry->key, currentEntry->value};
    }
```
This is to make sure that empty data is printed as empty and not garbage data.
* Adjusted begin() function to return end() instead of 
```cpp 
return Iterator(this, TABLE_SIZE, nullptr);
```
which is exactly what end() already has inside. This is just to be neater.

=============================================================================

# PlatformIO:
## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Added Example for how the library works (Example.cpp)
* Added Example for how the library works (Example2.cpp)
* Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK
* Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them.
### Version 1.0.2:
* Added Dynamic Downsize for the Vector to save memory (Useful for Memory Constrained Environments Such As Arduino)
* Added This ChangeLog :)   
* Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK
* Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them.
### Version 1.0.3:
* Updated Included Libraries to Latest Version
* Added merge Function that allows you to merge two Hashtables together
### Version 1.0.4:
* Fixed Issue with get function: Conversion from 'int' to 'const String' is ambiguous. -> Issue appears to happen when you use the get function with a int as the key
* Fixed Hashing Issue: Hashing was not working correctly, causing the get function to not work correctly
* Added containsKey() Function: This function allows you to check if a key exists in the Hashtable
* Added containsValue() Function: This function allows you to check if a value exists in the Hashtable
### Version 1.0.5:
* Added new Iteration Feature to the library
* Feature is untested
### Version 1.0.6:
* Repaired Iterator so it now works properly [Tested-Using-Properties-Library]
* Added elements() function to return the number of elements currently on the table.
* Adjusted size() function to return current capacity
* Adjusted clear() function to properly clear the table [Tested-Using-Properties-Library]
### Version 1.0.7:
* New Overloaded Constructor to allow you to set the initial capacity and load factor
* Added new loadFactor() function to return the current calculated load factor
* Added new checkLoadFactorAndRehash() which checks the load factor and rehashes the table if needed
* Added new bucketCount() function to return the number of buckets in the table
* Added new bucketSize() function to return the size of a bucket
* All new functions are untested, please report any bugs you find!
* These functions are desinged to open up the library to allow you to use it in more ways, such as using it as a hashset
### Version 1.0.8: 
* Added [HASHTABLE]: to the front of each Serial.println() statement to make it easier to debug.
### Version 1.0.9: 
* Modified the Constructor to accept a boolean value to allow you to disable the Serial.print() statements
* Added support for 3 new types: float, double, boolean
* Please note you can add your own specializations for other types if you wish, please see the README.md for more information          
### Version 1.1.0: 
* Added new exists() Function that only takes in 1 parameter, the key, this function returns a boolean value
* Added new values() Function that returns a SimpleVector of all the values in the Hashtable
* Added function comments.
### Version 1.1.1: 
* Emergency Patch: Fixed an issue where the Constructor wasnt properly overloaded.
* Adjusted Library Keywords
### Version 1.1.2: 
* Added new getElement() Function that returns an element at a specific index in the Hashtable
### Version 1.1.3-BETA: 
* Added New getKey() Function for Iterator
* Added New getValue() Function for Iterator
* Added New find() Function for Iterator.
### Version 1.1.3: [Latest-Release]
* RESOLVED ISSUE: Iterator Iterates out of bounds (Issue #85)
* RESOLVED ISSUE: Resizing corrupts data in hashtable (Issue #81)
* RESOLVED ISSUE: get() and getElement() returning incorrectly (Issue #82)
* Added new operator[] function that allows you to access elements by index in the Hashtable
* Added new getBucket() function that returns the bucket at a specific index in the Hashtable
* Added new getBucketSize() function that returns the size of a bucket at a specific index in the Hashtable
* Added new debugIterator() function that iterates the table and prints out the [key, value] pairs.
* Adjusted Iterator operator* function. 
[FROM THIS]
```cpp
    KeyValuePair operator*() const {
        
        return KeyValuePair{currentEntry->key, currentEntry->value};
    }
```
[TO THIS]
```cpp
    KeyValuePair operator*() const {
        if (!currentEntry) {
            return KeyValuePair{"", ""}; // Return an empty key-value pair if invalid
        }
        return KeyValuePair{currentEntry->key, currentEntry->value};
    }
```
This is to make sure that empty data is printed as empty and not garbage data.
* Adjusted begin() function to return end() instead of 
```cpp 
return Iterator(this, TABLE_SIZE, nullptr);
```
which is exactly what end() already has inside. This is just to be neater.




## Example

Here's an example of how to use the `Hashtable` class:

```cpp
#include <Hashtable.h>
#include <Arduino.h> // IF YOU ARE USING ARDUNINO's IDE, IGNORE THIS LINE
int main() {
    Hashtable<int, String> dictionary;

    // Add key-value pairs to the hash table
    dictionary.put(1, "One");
    dictionary.put(2, "Two");
    dictionary.put(3, "Three");

    // Retrieve values using keys
    String value = dictionary.getElement(2);
    Serial.println("Value for key 2: " + value);

    //Another way of retrieving values using keys
    String* value = dictionary.get(3);
    if (value != nullptr) {
        Serial.println("Value of key1: " + *value);
    } else {
        Serial.println("Key1 not found");
    }

    // Remove a key-value pair
    dictionary.remove(1);

    // Check if the hash table is empty
    if (dictionary.isEmpty()) {
        Serial.println("Hash table is empty.");
    } else {
        Serial.println("Hash table is not empty." );
    }

    // Iterate through the keys
    for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
        auto kv = *it;
        Serial.print("Iterator output: Key: ");
        Serial.print(kv.key);
        Serial.print(", Value: ");
        Serial.println(kv.value);
    }

    //inbuilt iterator/debug tool
    dictionary.debugIterator();

    //Verify Bucket Data
    void debugTable() {
        for (int i = 0; i < dictionary.bucketCount(); ++i) {
            Serial.print("Bucket ");
            Serial.print(i);
            Serial.print(": ");
            auto entry = dictionary.getBucket(i); // Use a method to get the bucket by index
            while (entry != nullptr) {
                Serial.print(entry->key + " -> " + entry->value + ", ");
                entry = entry->next;
            }
            Serial.println();
        }
    }

}

```
