# SimpleVector Class

The `SimpleVector` class is a C++ template class that provides a dynamic array-like data structure. It allows you to create, manage, and manipulate sequences of elements of any data type.

## Features

- Dynamic resizing: Automatically grows to accommodate more elements.
- Adding elements: Easily add elements to the end of the vector.
- Removing elements: Remove specific elements by value.

## Usage

To use the `SimpleVector` class, follow these steps:

1. Include the necessary header file (`SimpleVector.h`) in your C++ program.
2. Create an instance of the `SimpleVector` class, specifying the data type of the elements you want to store.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/SimpleVector.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <SimpleVector.h> 
```

# Arduino Library Manager:
## Changelog: 
### Version 1.0.0:
* Initial Release; On par with Platformio Version 1.0.5
### Version 1.0.1:
* Added Support for Range Based for Loops.
* On par with Platformio Version 1.0.6
### Version 1.0.2:
* Removed Uneccessary negative number check in function operator[] since its not possible for unsigned int's to be negative.
* On par with Platformio Version 1.0.7 - BETA


# PlatformIO Registry:
## ChangeLog:
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Added Remove Function.
* Renamed push_back function to add
* Updated Example.
### Version 1.0.2:
* Added Elements Function.
* Added Iterators to the library
* Modified the size Function to return size of array and not how many elements are on the array
### Version 1.0.3:
* Added [SIMPLE VECTOR]: to the start of the Serial.print() statements to make it easier to debug
### Version 1.0.4:
* Modified Constructor to take in a boolean value to determine if the Library should print Debug Messages
* Added a new function called setDebug() to set the debug value
* Added a new function called getDebug() to get the debug value"
### Version 1.0.5:
* Removed the Serial.print() Statements from the library to save memory. A version of this library that contains these debug statements may be available at some point.
* Added Function Comments
### Version 1.0.6: 
* Added Support for Range Based For loops
* Added Support for Adding Elements during Construction.
* Added BulkAdd Function() to add multiple elements at once.
### Version 1.0.7:- ALPHA 
* Added EmplaceBack Function so new libraries can depend on it correctly.
* Added Compiler Directives to check if a certain board/Platform is being used. This is to ensure proper functionality with initializer lists.
* Everything is handled Automatically, but if none of the preprocesor directives are met, then the Initializer_Lists will not be included which means you will not be able to bulk add directly into the constructor as that constructor will be removed by the compiler. However, if the compiler directives are met, then that constructor will remain and be available.
### Version 1.0.7:- BETA [CURRENT-RELEASE]
* Removed Uneccessary negative number check in function operator[] since its not possible for unsigned int's to be negative.


Here's an example of how to use the `SimpleVector` class:

```cpp
#include <Arduino.h>
#include "SimpleVector.h"

void setup() {
    Serial.begin(9600);
    // Create a SimpleVector of integers
    SimpleVector<int> intVector;

    // Add elements to the vector
    intVector.add(10);
    intVector.add(20);
    intVector.add(30);

    // Access elements using subscript operator
    int element = intVector[1];
    Serial.println("Element at index 1: " + element);
    // Remove an element
    intVector.remove(20);

    // Get the current size of the vector
    Serial.println("Vector size: " + intVector.size());
    // Iterate over the elements using an iterator
    SimpleVector<int>::SimpleVectorIterator it = intVector.begin();
    while (it.hasNext()) {
        int value = it.next();
        Serial.println("Element: " + value);
    }

    // Release memory (optional)
    intVector.releaseMemory();

    return 0;
}

```
