# ArrayList Class

The `ArrayList` class is a C++ template class that provides an implementation of a ArrayList for easy storing of a Value of any designated type. It allows you to efficiently store and retrieve data using indexes, similar to an ArrayList in Java.

## Overview

The `ArrayList` class is designed to offer a dynamic and scalable ArrayList with the following features:

- **Dynamic Sizing**: The List can dynamically resizes itself when the ArrayList becomes full. (Only if you specify "DYNAMIC" in the constructor)
- **Fixed Size**: The List can remain a fixed size when specified in the constructor.
- **Iterator Support**: You can iterate through the Values in the List.

## WARNING: This Library Utilizes POINTERS *. This is due to the Libraries ability to utilize any return type. (Bool, String, int, float, etc)

### Key Features

- `Constructor(SizeType type, size_t initialSize)`: Creates a new Instance of the ArrayList object
- `add(T item)`: Adds an Item to a List
- `remove(size_t index)`: Removes the value stored at a certain index.
- `clear()`: Clears all values in the list.
- `capacity()`: Returns the capacity of the list.
- `size()`: Returns the number of elements in the list.
- `isEmpty()`: Checks if the list is empty.
- `contains(T item)`: Checks if the list contains the value specified
- `resize()` : Private function: resizes the List
- `removeAt(T item)` : Private Function: Removes an item at the index specified (remove function utilizes this function to work)


#### Iterator Functions
- `begin()` : Defines the Iterator Start Position
- `end()` : Defines the Iterator Stop Position

#### **Please Note, You can use a Simple for-loop instead of the iterator. It will work the same. The iterator is only there for those who like to utilize them**

## Usage

To use the `ArrayList` class, follow these steps:

1. Include the `ArrayList.h` header in your C++ program.
2. Create an instance of the `ArrayList` class with specific data types for keys and values.
3. Use the provided methods to add, retrieve, or remove values from the list.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Hashtable.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <ArrayList.h> 
```

## ChangeLog
### Version 1.0.0:
* Initial Release 

## Example

Here's an example of how to use the `ArrayList` class:

```cpp
#include <ArrayList.h>
#include <Arduino.h>
ArrayList<String> List(ArrayList<String>::DYNAMIC, 10);
ArrayList<int> myList(ArrayList<int>::FIXED, 10); // Fixed size, with a size of 10
ArrayList<bool> boolList;

int main() {
    List.add("Hello");
    List.add("World");
    List.add("!");

    for(int i = 0; i < 25; i ++){
        myList.add(i);
    }

    boolList.add(true);
    boolList.add(false);
    boolList.add(true);
    boolList.add(true);
    boolList.add(false);

    for(int i = 0; i < List.size(); i++){
        Serial.println(List.get(i));
    }

    for(int i = 0; i < myList.size(); i++){
        Serial.println(myList.get(i));
    }

    for(int i = 0; i < boolList.size(); i++){
        Serial.println(boolList.get(i));
    }



}

```
