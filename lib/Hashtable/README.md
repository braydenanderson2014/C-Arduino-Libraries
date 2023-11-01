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
* "Added Example for how the library works (Example.cpp)",
* "Added Example for how the library works (Example2.cpp)",
* "Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK",
* "Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them."
### Version 1.0.2:
* "Added Dynamic Downsize for the Vector to save memory (Useful for Memory Constrained Environments Such As Arduino)",
* "Added This ChangeLog :) ",   
* "Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK",
* "Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them."
### Version 1.0.3:
* "Updated Included Libraries to Latest Version",
* "Added merge Function that allows you to merge two Hashtables together"


## Example

Here's an example of how to use the `Hashtable` class:

```cpp
#include <Hashtable.h>

int main() {
    Hashtable<int, std::string> dictionary;

    // Add key-value pairs to the hash table
    dictionary.put(1, "One");
    dictionary.put(2, "Two");
    dictionary.put(3, "Three");

    // Retrieve values using keys
    std::string value = dictionary.get(2);
    std::cout << "Value for key 2: " << value << std::endl;

    // Remove a key-value pair
    dictionary.remove(1);

    // Check if the hash table is empty
    if (dictionary.isEmpty()) {
        std::cout << "Hash table is empty." << std::endl;
    } else {
        std::cout << "Hash table is not empty." << std::endl;
    }

    // Iterate through the keys
    Hashtable<int, std::string>::HashtableIterator it = dictionary.keys();
    while (it.hasNext()) {
        int key = it.next();
        std::cout << "Key: " << key << std::endl;
    }

    return 0;
}

```
