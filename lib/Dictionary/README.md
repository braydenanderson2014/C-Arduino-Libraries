# Dictionary Class

The `Dictionary` class is a C++ template class that provides a basic interface for implementing dictionaries (associative arrays) in your code. It allows you to store and manipulate key-value pairs, similar to a dictionary or a map in other programming languages.

## Overview

The `Dictionary` class is designed to be a base class for implementing dictionaries with different data types for keys and values. It defines a set of abstract methods that must be implemented by derived classes to provide specific functionality for storing and retrieving key-value pairs.

### Abstract Methods

The following abstract methods must be implemented in derived classes:

- `size()`: Returns the number of key-value pairs in the dictionary.
- `isEmpty()`: Checks if the dictionary is empty.
- `keys()`: Returns an iterator for iterating over the keys in the dictionary.
- `elements()`: Returns an iterator for iterating over the values in the dictionary.
- `get(const K& key)`: Retrieves the value associated with the given key.
- `put(const K& key, const V& value)`: Associates the given key with the given value.
- `remove(const K& key)`: Removes the key-value pair associated with the given key.

### Iterators

The choice of iterator (e.g., `input_iterator_tag`) can be adapted to your specific requirements and usage of the `Dictionary`. You may choose to use other iterators like `forward_iterator`, `bidirectional_iterator`, or `random_access_iterator` based on your needs.

## Usage

To use the `Dictionary` class, follow these steps:

1. Create a derived class from `Dictionary` with specific data types for keys and values.
2. Implement the abstract methods defined in the `Dictionary` class in your derived class.
3. Use your derived class to create dictionary objects and perform key-value operations.

## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Dictionary.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <Dictionary.h> 
```

## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Dependency Update. Issues with a certain Dependency.
### Version 1.0.2:
* Fixed STD namespace issue -> made a mistake where i used std namespace instead of returning an error code
* Verified the Iterator and IOStream dependencies should be working now
### Version 1.0.3:
* Update to this Readme

        
## Example

Here's an example of how to create a derived dictionary class and use it:

```cpp
#include <iostream.h>
#include <iterator.h>

// Define a derived class from Dictionary with specific key and value types
class MyDictionary : public Dictionary<int, std::string> {
public:
    // Implement the abstract methods of the Dictionary class
    // ...

    // Example custom methods for your dictionary
    // ...

    // Constructor and Destructor
    MyDictionary() {}
    virtual ~MyDictionary() {}
};

int main() {
    // Create an instance of your derived dictionary class
    MyDictionary dict;

    // Use dictionary methods to add, retrieve, and remove key-value pairs
    // ...

    return 0;
}


