# Hashtable Class

The `Hashtable` class is a C++ template class that provides an implementation of a hash table for storing key-value pairs. It allows you to efficiently store and retrieve data using keys, similar to a dictionary or map in other programming languages.

## Overview

The `Hashtable` class is designed to offer a dynamic and scalable hash table with the following features:

- **Dynamic Sizing**: The hash table dynamically resizes itself when the load factor exceeds a specified threshold, ensuring efficient memory usage.
- **Collision Handling**: It uses separate chaining to handle collisions, allowing multiple key-value pairs with the same hash value to be stored and retrieved correctly.
- **Iterator Support**: You can iterate through the keys in the hash table using iterators, making it easy to perform operations on the keys and their associated values.

### Key Features

- `put(K key, V value)`: Associates the given key with the specified value in the hash table.
- `get(K key)`: Retrieves the value associated with the given key.
- `remove(K key)`: Removes the key-value pair associated with the given key.
- `clear()`: Clears all key-value pairs from the hash table.
- `size()`: Returns the number of key-value pairs in the hash table.
- `isEmpty()`: Checks if the hash table is empty.
- `keys()`: Returns an iterator for iterating over the keys in the hash table.
- `merge(const Hashtable<K, V>& other)`: Merges the contents of another hash table into the current one.
- `containsKey()`: Check if the Hashtable contains a certain key
- `containsValue()`: Check if the Hashtable contains a certain value



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


## Example

Here's an example of how to use the `Hashtable` class:

```cpp
#include <Hashtable.h>
#include <Arduino.h>
int main() {
    Hashtable<int, String> dictionary;

    // Add key-value pairs to the hash table
    dictionary.put(1, "One");
    dictionary.put(2, "Two");
    dictionary.put(3, "Three");

    // Retrieve values using keys
    String value = dictionary.get(2);
    Serial.println("Value for key 2: ");

    // Remove a key-value pair
    dictionary.remove(1);

    // Check if the hash table is empty
    if (dictionary.isEmpty()) {
        Serial.println("Hash table is empty.");
    } else {
        Serial.println("Hash table is not empty." );
    }

    // Iterate through the keys
    Hashtable<int, String>::HashtableIterator it = dictionary.keys();
    while (it.hasNext()) {
        int key = it.next();
        Serial.println("Key" + key);
    }

    return 0;
}

```
