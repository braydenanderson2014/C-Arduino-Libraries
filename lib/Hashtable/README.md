# **Hashtable Library**
🚀 **A lightweight and efficient Hashtable implementation for Arduino**

## 📖 **Overview**
The `Hashtable` library provides an efficient way to store key-value pairs in an Arduino environment. It supports various key types, handles collisions, and allows dynamic resizing for optimal performance.

---
## 📚 **API Reference**

### 🔹 **Hashtable Methods**

| Function | Description |
|----------|-------------|
| `Hashtable()` | Constructs a new hashtable with the default size. |
| `Hashtable(size_t initialCapacity, float loadFactor)` | Constructs a hashtable with a specified size and load factor. |
| `void put(const K& key, const V& value)` | Inserts a key-value pair into the hashtable. Overwrites if the key exists. |
| `V* get(const K& key) const` | Retrieves the value associated with a key. Returns `nullptr` if key is not found. |
| `V getElement(const K& key) const` | Retrieves the value associated with a key or returns a default-constructed value if not found. |
| `bool getElement(const K& key, V* value) const` | Retrieves the value and stores it in `value` if key exists. Returns `true` if found. |
| `bool exists(const K& key) const` | Checks if a key exists in the hashtable. |
| `bool remove(const K& key)` | Removes a key-value pair from the hashtable. Returns `true` if removed. |
| `void clear()` | Clears the hashtable and frees memory. |
| `float loadFactor() const` | Returns the current load factor. |
| `bool checkLoadFactorAndRehash()` | Checks and rehashes the table if the load factor threshold is exceeded. |
| `size_t bucketCount() const` | Returns the number of buckets. |
| `size_t bucketSize(size_t index) const` | Returns the number of elements in a given bucket. |
| `bool isEmpty() const` | Returns `true` if the hashtable is empty. |
| `int elements() const` | Returns the total number of elements stored. |
| `SimpleVector<K> keys() const` | Returns a vector of all keys. |
| `SimpleVector<V> values() const` | Returns a vector of all values. |
| `bool containsKey(const K& key) const` | Checks if the given key exists in the table. |
| `bool containsValue(const V& value) const` | Checks if the given value exists in the table. |
| `V& operator[](const K& key)` | Accesses elements by key. If key does not exist, inserts a new default value. |
| `void debugIterator()` | Prints all key-value pairs in the hashtable using an iterator. |

---
## 🛠 **Usage Examples**

### 🔹 **Basic Usage**
```cpp
#include <Arduino.h>
#include "Hashtable.h"

Hashtable<String, int> hashTable;

void setup() {
    Serial.begin(9600);
    hashTable.put("Temperature", 25);
    hashTable.put("Humidity", 60);

    Serial.print("Temperature: ");
    Serial.println(*hashTable.get("Temperature"));
}

void loop() {
    // Your logic here
}
```

### 🔹 **Checking and Removing Elements**
```cpp
if (hashTable.exists("Humidity")) {
    Serial.println("Humidity data exists!");
    hashTable.remove("Humidity");
}
```

### 🔹 **Iterating Over Entries**
```cpp
for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
    auto kv = *it;
    Serial.print("Key: ");
    Serial.print(kv.key);
    Serial.print(" | Value: ");
    Serial.println(kv.value);
}
```

---
## 📜 **PlatformIO Changelog**
### Latest Version:
- **v1.1.3** (2025-01)
  - RESOLVED ISSUE: Iterator Iterates out of bounds (Issue #85)
  - RESOLVED ISSUE: Resizing corrupts data in hashtable (Issue #81)
  - RESOLVED ISSUE: get() and getElement() returning incorrectly (Issue #82)
  - Added new operator[] function that allows you to access elements by index in the Hashtable
  - Added new getBucket() function that returns the bucket at a specific index in the Hashtable
  - Added new getBucketSize() function that returns the size of a bucket at a specific index in the Hashtable
  - Added new debugIterator() function that iterates the table and prints out the [key, value] pairs.
  - Adjusted Iterator operator* function. 

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
 - Adjusted begin() function to return end() instead of 
```cpp 
return Iterator(this, TABLE_SIZE, nullptr);
```
which is exactly what end() already has inside. This is just to be neater.


### Previous Versions:
- **v1.0.0** - Initial Release
- **v1.0.1** - Added Examples for how the library works (Example.cpp)(Example2.cpp)
- **v1.0.2** - Added Dynamic Downsize for the Vector to save memory (Useful for Memory Constrained Environments Such As Arduino)

- **v1.0.3** - Updated Included Libraries to Latest Version
             - Added merge Function that allows you to merge two Hashtables together

- **v1.0.4** - Fixed Issue with get function: Conversion from 'int' to 'const String' is ambiguous. -> Issue appears to happen when you use the get function with a int as the key.
             - Fixed Hashing Issue: Hashing was not working correctly, causing the get function to not work.
             - Added containsKey() Function: This function allows you to check if a key exists in the Hashtable
             - Added containsValue() Function: This function allows you to check if a value exists in the Hashtable

- **v1.0.5** - Added new Iteration Feature to the library [UNTESTED]
- **v1.0.6** - Repaired Iterator so it now works properly [Tested-Using-Properties-Library]
             - Added elements() function to return the number of elements currently on the table.
             - Adjusted size() function to return current capacity
             - Adjusted clear() function to properly clear the table [Tested-Using-Properties-Library]

- **v1.0.7** - New Overloaded Constructor to allow you to set the initial capacity and load factor
             - Added new loadFactor() function to return the current calculated load factor
             - Added new checkLoadFactorAndRehash() which checks the load factor and rehashes the table if needed.
             - Added new bucketCount() function to return the number of buckets in the table
             - Added new bucketSize() function to return the size of a bucket.
             - All new functions are untested, please report any bugs you find!
             - These functions are desinged to open up the library to allow you to use it in more ways, such as using it as a hashset.

- **v1.0.8** - Added [HASHTABLE]: to the front of each Serial.println() statement to make it easier to debug.

- **v1.0.9** - Modified the Constructor to accept a boolean value to allow you to disable the Serial.print() statements.
             - Added support for 3 new types: float, double, boolean
             - Please note you can add your own specializations for other types if you wish, please see the README.md for more information.

- **v1.1.0** - Added new exists() Function that only takes in 1 parameter, the key, this function returns a boolean value
             - Added new values() Function that returns a SimpleVector of all the values in the Hashtable
             - Added function comments.

- **v1.1.1** - Emergency Patch: Fixed an issue where the Constructor wasnt properly overloaded.
             - Adjusted Library Keywords

- **v1.1.2** - Added new getElement() Function that returns an element at a specific index in the Hashtable (Please do not get confused between getElement and get... get returns a pointer)

- **v1.1.3** [BETA]
             - Added New getKey() Function for Iterator
             - Added New getValue() Function for Iterator
             - Added New find() Function for Iterator.



---
## 📜 **Arduino Changelog**
### Latest Version:
- **v1.0.3** (2025-01) [ON-PAR] -> Platformio v1.1.3
             - RESOLVED ISSUE: Iterator Iterates out of bounds (Issue #85)
             - RESOLVED ISSUE: Resizing corrupts data in hashtable (Issue #81)
             - RESOLVED ISSUE: get() and getElement() returning incorrectly (Issue #82)
             - Added new operator[] function that allows you to access elements by index in the Hashtable
             - Added new getBucket() function that returns the bucket at a specific index in the Hashtable
             - Added new getBucketSize() function that returns the size of a bucket at a specific index in the Hashtable
             - Added new debugIterator() function that iterates the table and prints out the [key, value] pairs.
             - Adjusted Iterator operator* function. 

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
 - Adjusted begin() function to return end() instead of 
```cpp 
return Iterator(this, TABLE_SIZE, nullptr);
```
which is exactly what end() already has inside. This is just to be neater.


### Previous Versions:
- **v1.0.0** - Initial release [ON-PAR] -> Platformio v1.1.2
- **v1.0.1** [BETA]
             - Added New getKey() Function for Iterator
             - Added New getValue() Function for Iterator
             - Added New find() Function for Iterator.
             - [NOTE]: All 3 functions are not tested and should be used with Caution. 

---
## 💖 **Support My Work**  
If you find this library useful, consider supporting me:

🔗 **[Donate via GoFundMe](https://gofund.me/923e5f10)**  
💰 **Click the Sponsor Button** at the top of this repository!  

---
## 📜 **Final Notes**
- This hashtable is optimized for Arduino but can be adapted for other environments.
- It uses **chaining for collision handling** and supports **dynamic resizing**.
- **Supports int, float, String, and custom key types** with specialized hash functions.

🚀 **Enjoy coding with Hashtable!**
