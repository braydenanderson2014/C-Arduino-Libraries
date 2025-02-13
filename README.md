# C-Arduino-Libraries
### All Libraries are contained under the `lib` folder. For Best Editing Results, Open Parent folder this README.md File is contained inside. This will make the whole Project Available to check out. The `src` folder contains the main project `main.cpp` that you can use to test the Libraries. The 'Test' folder is where you can write unit tests and test the libraries (Or your code) with the Unity library. Just enable the dependency in the 'platformio.ini' file. Follow the directions for the Libraries to import them into your project and set them up correctly. Down below is a list of all Libraries and their status's. You can use the list to go directly to each Libraries infromation. It should be identical to the readme files contained inside each library folder. Some libraries are available on Arduino Library Manager. These libraries are listed below. Please note not all Arduino library versions are up to date with the platformio versions. If the library is listed with `[ON-PAR]`, this means the versions are the same. But if it is not listed, it may not be the same version. Arduino versions have their own repository, however if you encounter issues, please report them on this repository and just mention which version you are using. Thank you!

### CONSIDER SUPPORTING ME: https://gofund.me/923e5f10

### Most Libraries have been [PARTIALLY-TESTED], or [MOSTLY-TESTED]. However even the libraries that are [STABLE] or [CONSIDERED-STABLE] may have Unknown Bugs. If you Run Across any Bugs or have a question about the library, Please raise an issue report so I can look into it.

### Please Note, some of these libraries have different names then their published counterparts. For example, (Properties) is published as -> SimpleProperties... These are Noted below in the List of available Libraries.

### Please Note (Also XD) that any library published to the [Development-Branch] is considered Pure development versions and are subject to change at any time. While most will still work with no trouble, alot of libraries are under Active Development and may not compile. For up To date working (Theoretically). Any Library published under main should be functional and [On-Par] with Platformio or Arduino Library Manager.

## List of All Available Libraries ## [WARNING]: Tested/Partially Tested BY AUTHOR
* [Dictionary](#dictionary-class) [DEVELOPMENT-COMPLETE] (PURE VIRTUAL : No Concrete Implementation in this Library), [TESTING-NOT-NECESSARY],[IMPLEMENTED IN ANOTHER LIBRARY]
* [Hashtable](#hashtable-class) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [iostream](#iostream-class) [PASSIVE-DEVELOPMENT] (Published as : BasicIOStream), [PARTIALLY-TESTED], [STABLE]
* [iterator](#iterator-class) [DEVELOPMENT-COMPLETE], (PURE VIRTUAL : No Concrete Implementation in this Library)  [TESTING-NOT-NECESSARY] 
* [Properties](#properties-class) [PASSIVE-DEVELOPMENT] (Published as : SimpleProperties), [MOSTLY-TESTED], [STABLE]
* [SimpleVector](#simplevector-class) [PASSIVE-DEVELOPMENT] , [TESTED], [STABLE]
* [Timer](#timer-class) [PASSIVE_DEVELOPMENT] (Published as : SimpleArduinoTimer), [MOSTLY-TESTED], [SEMI-STABLE]
* [UnorderedMap](#unorderedmap-class) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [LinkedList](#linkedlist-class) [PASSIVE-DEVELOPMENT] (Published as : BasicLinkedList), [PARTIALLY-TESTED], [STABLE]
* [DoubleLinkedList](#doublelinkedlist-class) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [MyDictionary](#mydictionary-class) [ACTIVE-DEVELOPMENT],(implemented Version of the Dictionary Class), [UNTESTED]
* [SDList](#sdlist-class) [PASSIVE-DEVELOPMENT], [REIMPLEMENTED], [MOSTLY-TESTED], [UNSTABLE]
* [ArrayList](#arraylist-class) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [SimpleMutex](#simplemutex-class) [ACTIVE-DEVELOPMENT]
* [SimpleThreadManager](#simplethreadmanager-class) [ACTIVE-DEVELOPMENT]
* [Operators](#operators-class) [PASSIVE-DEVELOPMENT], [STABLE]
* [Predicates](#predicates-class) [PASSIVE-DEVELOPMENT], [STABLE]
* [TypeTraits](#typetraits-library) [PASSIVE-DEVELOPMENT], [STABLE]
* [Numeric_Limits](#numeric-limits-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Map](#map-library-for-arduino) [ACTIVE-DEVELOPMENT], [STABLE], [PARTIALLY-TESTED]
* [Queue](#queue-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Stack](#stack-library-for-arduino) [PASSIVE-DEVELOPMENT], [STABLE]
* [Predicates](#predicates-class) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [Operators](#operators-class) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [MatrixMath](#matrixmath-library-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [MathLib](#mathlib-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [ColorManager](#colormapper-library) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [InitializerList](#initializer-list-library-for-arduino) [PASSIVE-DEVELOPMENT], [UNTESTED]


Total Available Libraries: (42)
Total Libraries: (48)

## ACTIVELY WORKING ON LISTED LIBRARIES (Bug Fixing or Feature Adding):
1.  [SimpleMutex]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
2.  [SimpleThreadManager]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
3.  [OrderedMap]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
4.  [JSON]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
5.  [STRING]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
6.  [MemoryManager](#memory-management)


=======

## ACTIVELY TESTING ON LISTED LIBRARIES (Perforing Basic Tests On the Library):
1. [MyDictionary](#mydictionary-class) [TESTING]
2. [AVLTree](#avl-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
3. [BINARY Search Tree](#binary-search-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
4. [BINARY Tree](#binary-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
5. [B+ Tree](#b-plus-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
6. [B Tree](#btree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
7. [FenwickTree](#fenwick-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
8. [HeapTree](#heap-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
9. [InitializerList](#initializer-list-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
10. [IntervalTree](#interval-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
11. [KDimensionalTree](#k-dimensional-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
12. [MathLib](#mathlib-for-arduino)  [ACTIVE-DEVELOPMENT], [TESTING]
13. [MatrixMath](#matrixmath-library-for-arduino)[ACTIVE-DEVELOPMENT], [TESTING]
14. [OCTree](#octree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
15. [QuadTree](#quadtree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
16. [RedBlackTree](#red-black-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
17. [RTree](#r-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
18. [SegmentTree](#segment-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]
19. [Suffixtree](#suffix-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING] 
20. [TrieTree](#trie-tree-library-for-arduino) [ACTIVE-DEVELOPMENT], [TESTING]

=======

## UNPUBLISHED (In Development) (Not yet Documented)
========================================================== 
[WARNING]: NO DOCUMENTATION AVAILABLE YET &&|| UNTESTED BY AUTHOR
1.  [SimpleMutex]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
2.  [SimpleThreadManager]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
3.  
4.  






## ALL LIBRARIES INCLUDE A README.md ##

# PlatformIO Leaderboard for these Libraries (Out of /15,214 Libraries on the Platform) UPDATED 01/29/2025 09:08pm Mountain
## Library Name: Position Downloads-Month-To-Date, Total Downloads
* ArrayList: 1,100; 26 Downloads MTD; *174 Downloads*
* Dictionary: 14,128; 0 Download MTD; *16 Downloads*
* SimpleArduinoTimer: 3,947; 2 Downloads MTD; *16 Downloads*
* UnorderedMap: 3,952; 2 Downloads MTD; *46 Downloads*
* SimpleProperties: 3,948; 2 Downloads MTD; *46 Downloads*
* Iterator: 3949; 2 Download MTD; *25 Downloads*
* BasicIO: 1679; 12 Downloads MTD; *360 Downloads*
* SDList: 3951; 2 Downloads MTD; *12 Downloads*
* Hashtable: 1375; 17 Downloads MTD; *183 Downloads*
* SimpleVector: 1259; 21 Downloads MTD; *247 Downloads*
* MyDictionary: 14,136; 0 Downloads MTD; *3 Downloads* :(
* Operators: 14,221; 0 Download MTD; *15 Download*
* Predicates: 14,222; 0 Downloads MTD; *15 Download*
* BasicLinkedList: 2818(13,610); 4 Downloads MTD; *20 Downloads*
* DoubleLinkedList: 3,250; 3 Downloads MTD; *21 Downloads* :)
* TypeTraits: 3980; 2 Download MTD; *28 Downloads*
* AVLTree: 14,422; 0 Download MTD; *4 Downloads*
* BinarySearchTree: 14,423; 0 Download MTD; *4 Downloads*
* BinaryTree: 14,424; 0 Download MTD; *4 Downloads*
* B+ Tree: 14,425; 0 Download MTD; *4 Downloads*
* B Tree: 14,426; 0 Download MTD; *4 Downloads*
* ColorManager: 14,427; 0 Download MTD; *5 Downloads*
* FenwickTree: 14,428; 0 Download MTD; *4 Downloads*
* HeapTree: 14,429; 0 Download MTD; *4 Downloads*
* InitializerList: 1379; 18 Download MTD; *217 Downloads*
* IntervalTree: 14,430; 0 Download MTD; *4 Downloads*
* KDimensionalTree: 14,431; 0 Download MTD; *4 Downloads*
* MathLib: 3,982; 2 Download MTD; *21 Downloads*
* Map: 2726; 1 Download MTD; *38 Downloads*
* MatrixMath: 14,444; 0 Download MTD; *4 Downloads*
* Numeric_Limits: 3983;  2 Download MTD; *25 Downloads*
* OCTree: 14,432; 0 Download MTD; *4 Downloads* 
* QuadTree: 14,436; 0 Download MTD; *4 Downloads*            
* Queue: 14,437; 0 Download MTD; *5 Downloads*
* RedBlackTree: 14,438; 0 Download MTD; *4 Downloads*
* RTree: 14,439; 0 Download MTD; *4 Downloads*
* SegmentTree: 14,440; 0 Download MTD; *4 Downloads*
* Stack: 14,441; 0 Download MTD; *6 Downloads*
* SuffixTree: 14,442; 0 Download MTD; *4 Downloads*
* TrieTree: 14,443; 0 Download MTD; *4 Downloads*

# Libraries Published on Arduino Library Manager
* ArrayList [ON-PAR]
* SimpleVector [ON-PAR]
* Hashtable [ON-PAR]
* Properties (Simple Properties)
* Timer (ArduinoTimer)
* LinkedList (ArduinoLinkedList)
* UnorderedMap 
* DoubleLinkedList (ArduinoDoubleLinkedList)
* SDList (Arduino SDList) [ON-PAR]


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


```

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
- `size()`: Returns the current capacity of the hash table.
- `elements()`: Returns the number of key-value pairs in the hash table
- `isEmpty()`: Checks if the hash table is empty.
- `keys()`: Returns an iterator for iterating over the keys in the hash table.
- `merge(const Hashtable<K, V>& other)`: Merges the contents of another hash table into the current one.
- `containsKey(const K& key)`: Check if a Table contains a key.
- `containsValue(const V& value)`: Check if a Table contains a Value.

## Usage

To use the `Hashtable` class, follow these steps:

1. Include the `Hashtable.h` header in your C++ program.
2. Create an instance of the `Hashtable` class with specific data types for keys and values.
3. Use the provided methods to add, retrieve, or remove key-value pairs from the hash table.

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
    String value = dictionary.get(2);
    Serial.println("Value for key 2: " + value);

    // Remove a key-value pair
    dictionary.remove(1);

    // Check if the hash table is empty
    if (dictionary.isEmpty()) {
       Serial.println("Hash table is empty.");
    } else {
        Serial.println("Hash table is not empty.")
    }

    return 0;
}

```

# iostream Class

The `iostream` class is a C++ class designed for simple input and output operations in embedded systems using Arduino. It provides methods for printing messages to the serial monitor, LCD displays (both standard and I2C), and saving/loading data to/from an SD card. This class aims to simplify common input and output tasks when working with Arduino-based projects.

## Features

- Print messages to the serial monitor.
- Print messages to standard LCD displays (LiquidCrystal).
- Print messages to I2C-enabled LCD displays (LiquidCrystal_I2C).
- Clear LCD display lines before printing (optional).
- Save data to a file on an SD card.
- Load data from a file on an SD card.

## Usage

To use the `iostream` class, follow these steps:

1. Include the necessary libraries (`Arduino.h`, `LiquidCrystal_I2C.h`, `LiquidCrystal.h`, `Wire.h`, `SD.h`) in your Arduino sketch.
2. Create an instance of the `iostream` class to perform input and output operations.
3. Use the provided methods to perform specific input and output tasks, such as printing messages or saving/loading data.

## Example

Here's an example of how to use the `iostream` class for basic input and output operations:

```cpp
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SD.h>
#include "iostream.h"

// Create an instance of the iostream class
iostream cout;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize SD card
    if (!SD.begin(10)) {
        cout.println("SD card initialization failed!");
        return;
    }

    // Initialize LCD display
    LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address and dimensions as needed
    lcd.init();
    lcd.backlight();

    // Print messages to various outputs
    cout.print("Hello, Serial Monitor!");
    cout.printLCD("Hello, LCD!", lcd, 0, true);
    cout.printI2CLCD("This message is too long for the LCD and will scroll.", lcd, 1, true);

    // Save data to a file
    bool saved = cout.saveToFile("data.txt", "This is sample data.");
    if (saved) {
        cout.println("Data saved to file successfully.");
    } else {
        cout.println("Failed to save data to file.");
    }

    // Load data from a file
    char buffer[128];
    bool loaded = cout.loadFromFile("data.txt", buffer, sizeof(buffer));
    if (loaded) {
        cout.print("Loaded data from file: ");
        cout.println(buffer);
    } else {
        cout.println("Failed to load data from file.");
    }
}

void loop() {
    // Your main code here
}

```

# Iterator Class

The `Iterator` class is a C++ template class that provides an abstract interface for creating iterators. Iterators are commonly used in C++ for traversing and manipulating data structures like arrays, linked lists, and containers. This class defines the core methods necessary for creating custom iterators.

## Usage

To use the `Iterator` class, follow these steps:

1. Include the `Iterator` header file in your C++ program.
2. Define a class that implements the `Iterator` interface for your specific data structure or container.
3. Implement the required methods: `operator*`, `operator++`, and `operator!=`.
4. Use the custom iterator class to traverse and manipulate your data structure.

## Example

Here's an example of how to define and use an iterator class using the `Iterator` interface:

```cpp
#include <iostream>
#include "Iterator.h"

// Define a custom data structure (e.g., a linked list)
template <typename T>
class MyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    MyLinkedList() : head(nullptr) {}

    // Add elements to the linked list (not shown here)

    // Iterator class for MyLinkedList
    class LinkedListIterator : public Iterator<T> {
    private:
        Node* current;

    public:
        LinkedListIterator(Node* start) : current(start) {}

        // Implement the operator* to get the current element
        T& operator*() override {
            return current->data;
        }

        // Implement the operator++ to move to the next element
        LinkedListIterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        // Implement the operator!= to compare iterators
        bool operator!=(const Iterator<T>& other) const override {
            const LinkedListIterator& otherIterator = static_cast<const LinkedListIterator&>(other);
            return current != otherIterator.current;
        }
    };

    // Get an iterator for the linked list
    LinkedListIterator begin() {
        return LinkedListIterator(head);
    }

    LinkedListIterator end() {
        return LinkedListIterator(nullptr);
    }
};

int main() {
    MyLinkedList<int> myList;

    // Add elements to myList (not shown here)

    // Use the custom iterator to traverse the linked list
    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}


```

# Properties Class

The `Properties` class is a C++ class that provides a simple key-value store for managing properties. It allows you to store and retrieve key-value pairs, save properties to an SD card, load properties from an SD card, and perform other operations commonly used for managing configurations and settings.

## Usage

To use the `Properties` class, follow these steps:

1. Include the necessary header files (`Properties.h` and `Hashtable.h`) in your C++ program.
2. Create an instance of the `Properties` class.
3. Use the provided methods to set, get, remove, or manipulate properties as needed.

Here's an example of how to use the `Properties` class:

```cpp
#include <Arduino.h>
#include <SD.h>
#include "Properties.h"

Properties myProperties;

void setup() {
    Serial.begin(9600);
    // Initialize SD card (make sure SD card is properly connected)
    if (!SD.begin(4)) {
        Serial.println("Failed to initialize SD card.");
    }
}

void loop() {
    // Set a property
    myProperties.setProperty("name", "John");

    // Get a property
    String name = myProperties.getProperty("name");
    Serial.println("Name: " + name);

    // Check if a property exists
    bool hasAge = myProperties.containsKey("age");
    Serial.println("Property 'age' exists: " + String(hasAge ? "Yes" : "No"));

    // Remove a property
    myProperties.removeProperty("name");

    // Check if the properties are empty
    bool isEmpty = myProperties.isEmpty();
    Serial.println("Properties are empty: " + String(isEmpty ? "Yes" : "No"));

    delay(1000);
}


```

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

Here's an example of how to use the `SimpleVector` class:

```cpp
#include <iostream>
#include "SimpleVector.h"

int main() {
    // Create a SimpleVector of integers
    SimpleVector<int> intVector;

    // Add elements to the vector
    intVector.add(10);
    intVector.add(20);
    intVector.add(30);

    // Access elements using subscript operator
    int element = intVector[1];
    std::cout << "Element at index 1: " << element << std::endl;

    // Remove an element
    intVector.remove(20);

    // Get the current size of the vector
    std::cout << "Vector size: " << intVector.size() << std::endl;

    // Iterate over the elements using an iterator
    SimpleVector<int>::SimpleVectorIterator it = intVector.begin();
    while (it.hasNext()) {
        int value = it.next();
        std::cout << "Element: " << value << std::endl;
    }

    // Release memory (optional)
    intVector.releaseMemory();

    return 0;
}

```

# Timer Class

The `Timer` class is a C++ class for managing time-related operations and timers. It allows you to create, start, stop, reset, and pause timers, making it useful for various timing and scheduling tasks in your Arduino projects.

## Features

- Start, stop, reset, and pause timers.
- Set target durations and check if a timer has reached its target.
- Synchronize with an RTC (Real-Time Clock) module.
- Get the current RTC time.
- Manage both running and paused states of timers.

## Usage

To use the `Timer` class, follow these steps:

1. Include the necessary header file (`Timer.h`) in your C++ program.
2. Create an instance of the `Timer` class.

Here's an example of how to use the `Timer` class:

```cpp
#include <Arduino.h>
#include "Timer.h"

Timer myTimer;

void setup() {
    Serial.begin(9600);
    myTimer.setTargetMinutes(5);  // Set the target time to 5 minutes
    myTimer.start();  // Start the timer
}

void loop() {
    if (myTimer.hasReachedTarget()) {
        Serial.println("Timer has reached the target duration.");
        // Perform actions when the timer reaches the target.
        // You can also stop or reset the timer as needed.
    }
}

```

# MyDictionary Class

`MyDictionary` is a custom dictionary class designed for Arduino projects. It provides a simple interface for storing and retrieving key-value pairs, where keys are strings and values are integers. This Library was renamed from DictionaryImplementation to MyDictionary.

## Features

- Implements a dictionary using an unordered map. (Also An Available Library)
- Provides basic dictionary operations such as insertion, deletion, and retrieval.
- Uses custom error handling suitable for Arduino's constrained environment.

## Usage

To use `MyDictionary`, include the header file in your Arduino sketch and create an instance of `MyDictionary`.

```cpp
#include "MyDictionary.h"

MyDictionary myDict;
myDict.put("key1", 100);

```

### Retrieving a Value
```cpp
int value = myDict.get("key1");
if (value != -1) {
    // Use the value
}
```
### Removing a Value
```cpp
if (!myDict.remove("key1")) {
    // Handle the error
}
```
### Checking Dictionary Size
```cpp

int size = myDict.size();

```
### Checking if the Dictionary is Empty
```cpp
bool isEmpty = myDict.isEmpty();
```
## Error Handling
The get, put, and remove methods use return values to indicate success or failure, as throwing exceptions is not supported in the Arduino environment.

get returns -1 if the key is not found.
put returns 1 as a success indicator.
remove returns 0 if the key is not found and 1 as a success indicator.

## Dependencies
MyDictionary depends on the Dictionary and UnorderedMap classes. Make sure these classes are available in your project.

Contributing
Contributions to MyDictionary are welcome. Please adhere to the provided coding standards and include unit tests with your pull requests.

License
Specify the license under which your code is available. Common licenses for open-source projects include MIT, GPL, and Apache 2.0.

# SDList Class 

`SDList` is a dynamic array class for Arduino, with the additional capability to persist data on an SD card. It allows you to store and retrieve elements just like a regular array but can also save its contents to an SD card and load them back when needed.

## Features

- Dynamic resizing of the list when more capacity is needed.
- Persistence of list data using an SD card.
- Simple API for adding, accessing, and setting elements.
- Checking the availability of an SD card.

## Installation

To use `SDList` in your Arduino project:

1. Copy `SDList.h` to your project's directory.
2. Include the `SDList.h` file in your sketch.

## ChangeLog
### Version 1.0.0
* Initial Release
### Version 1.0.1
* Dependency Update (Issues with certain Dependencies)
### Version 1.0.2  
* Dependency Update (Issue was not resolved in 1.0.1 but is now properly resolved)
* Added [SD LIST]: in front of all Serial.prints to make it easier to debug
### Version 1.0.3 [BETA-TESTING]
* Update to README
* Completely Reimplemented this library, It now functions properly and is much more stable. (Only Lightly Tested)
* This Library now uses the ArrayList Library to store the list in memory, and then writes the list to the SD card when the list is modified (In SDLIST). This makes the library much more stable and reliable.
* This Library has not been Thoughly tested, and may not be stable. USE AT YOUR OWN RISK! If you find any bugs, please report them to the Author
* Removed the c++ file and moved implementation to the header file
    
## Usage

To create an `SDList`, specify the data type you want to store and provide the Chip Select (CS) pin and a filename for the page file on the SD card.

```cpp
#include "SDList.h"

SDList <int> myList(SDCARD, 8); // Mode is (SDCARD || MEMORY), Initial Capacity: 8
```
### Appending Elements
```cpp
myList.append(42);
```
### Accessing Elements
```cpp
int value = myList.get(0);
```
### Inserting Elements
```cpp
myList.insert(0, 100);
```
### Checking List Size
```cpp
uint16_t listSize = myList.size();
```

## Methods
* append: Add an element to the end of the list.
* get: Retrieve the element at a specified index.
* insert: inserts the value of the element at a specified index.
* size: Get the number of elements in the list.
* capacity: Get the arraylist capacity
* clear: clear list and file
* begin: REQUIRED FOR SD OPERATIONS (Please call begin function before any SD Begin Functions... also beware that setMode can override SD.begin() at any point)
* setMode: set the Mode(SDCARD || MEMORY)
* remove: Remove an element (Checks what mode you are in.)


## Notes
The list starts with an initial capacity, which will be doubled each time it runs out of space.
Ensure the SD card is formatted correctly and that the Arduino has the necessary permissions to read from and write to the SD card.

## Dependencies
* Arduino
* SD
* ArrayList (braydenanderson2014)
* SPI


## Contributing
Contributions to SDList are welcome. Please adhere to the provided coding standards and include unit tests with your pull requests.



# LinkedList Class
* Published as BasicLinkedList on PlatformIO
A simple, templated linked list implementation for Arduino projects. This class allows you to create and manage a linked list of any data type.

## Features

- Templated class: Can store any data type.
- Dynamic resizing: Automatically manages memory as elements are added or removed.
- Basic operations: `append`, `prepend`, `insert`, `remove`, `get`, `contains`, `getSize`, `isEmpty`, `clear`.

## Installation

To use `LinkedList` in your Arduino sketch:

1. Copy the `LinkedList` class code to your project's directory.
2. Include `LinkedList.h` at the top of your sketch.

## Usage

To create a linked list, simply declare an instance of `LinkedList` with the desired type:

```cpp
#include <LinkedList.h>

LinkedList<int> myList;
```
## Adding Elements
### Append an element to the end of the list:

```cpp
myList.append(1);
```
### Prepend an element to the beginning of the list:

```cpp
myList.prepend(0);
```
### Insert an element at a specific position:

```cpp
myList.insert(2, 1);  // Insert '2' at position '1'
```
## Accessing Elements
### Retrieve an element at a specific position:

```cpp
int value = myList.get(1);
```
## Removing Elements
### Remove an element by value:

```cpp
myList.remove(1);
```
## Utility Functions
### Check if the list contains a specific value:

```cpp
if (myList.contains(2)) {
    // Element is in the list
}
```
## Get the size of the list:

```cpp
size_t size = myList.getSize();
```
### Check if the list is empty:

```cpp
if (myList.isEmpty()) {
    // List is empty
}
```
### Clear the list:

```cpp
myList.clear();
```
## Example
```cpp
LinkedList<int> myList;
myList.append(1);
myList.append(2);
myList.prepend(0);
myList.insert(3, 3);

for (size_t i = 0; i < myList.getSize(); i++) {
    Serial.print("Element at position ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(myList.get(i));
}

myList.remove(2);
myList.clear();
```
# UnorderedMap Class

`UnorderedMap` is a custom hash map implementation designed for Arduino projects that require efficient key-value pair storage. The class provides basic hash map functionalities such as insertion, retrieval, and deletion of elements.

## Features

- Templated class that can handle various data types for keys and values.
- Automatic resizing when the map reaches capacity.
- Duplicate key checking to prevent overwriting without explicit intent.
- Simple and intuitive API.

## Installation

To use `UnorderedMap` in your Arduino sketch:

1. Copy the `UnorderedMap.h` file to your project's directory.
2. Include `UnorderedMap.h` at the top of your sketch.

## Usage

To create an `UnorderedMap`, specify the key and value data types when declaring an instance:

```cpp
#include "UnorderedMap.h"

UnorderedMap<String, int> myMap;
```
### Inserting Elements
```cpp
myMap.insert("temperature", 25);
```
### Retrieving Elements
```cpp
int temperature;
if (myMap.get("temperature", temperature)) {
    // Use the temperature value
}
```
### Removing Elements
```cpp
if (myMap.remove("temperature")) {
    // Temperature was successfully removed
}
```
### Map Size and Empty Check
```cpp
size_t mapSize = myMap.getSize();

if (myMap.isEmpty()) {
    // Map is empty
}
```
## Methods
* insert: Add a key-value pair to the map. If the key already exists, its value is updated.
* get: Retrieve a value associated with a given key.
* remove: Remove a key-value pair from the map.
* getSize: Get the number of key-value pairs in the map.
* isEmpty: Check if the map is empty.

## Example
```cpp
#include "UnorderedMap.h"

UnorderedMap<String, int> myMap;

void setup() {
    Serial.begin(9600);
    myMap.insert("humidity", 60);
    myMap.insert("pressure", 1013);
}

void loop() {
    int humidity;
    if (myMap.get("humidity", humidity)) {
        Serial.print("Humidity: ");
        Serial.println(humidity);
    }
    delay(1000);
}
```
## Notes
This implementation is designed to be memory-efficient, which is critical for the constrained environment of Arduino devices.
The resize method will automatically be called when the map's capacity is exceeded, doubling the map's size.
Error handling for memory allocation failures is critical on Arduino, and the resize method includes a simple error message output to the serial monitor.

## Contributing
Contributions to UnorderedMap are welcome. Please adhere to the provided coding standards and include unit tests with your pull requests.

# DoubleLinkedList Class
PENDING DOCUMENTATION

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

# SimpleMutex Class
[DOCUMENTATION PENDING]


# SimpleThreadManager Class
[DOCUMENTATION PENDING]

# Operators Class

## Overview
The ArrayListOperations library provides a comprehensive set of operations that can be performed on elements of an ArrayList. This library is designed for Arduino projects and offers a wide range of mathematical and utility functions.

## Features
* Operations on individual elements (e.g., increment, double, multiply by a factor)
* Mathematical operations (e.g., square, cube, square root, cube root)
* Conversion functions (e.g., decimal to binary, binary to decimal)
* Statistical calculations (e.g., mean, median, mode, standard deviation, variance)
* Utility operations (e.g., find minimum, find maximum, count elements)
* Customizable global multiplier for scalable operations

## Installation
* Download the Operators.h file.

* Place it in your Arduino project's directory.

* Include the library in your Arduino sketch:

```cpp
#include "Operations.h"
```
## Usage
### Basic Operations
You can perform basic operations on array list elements:

```cpp
Operators<int>::setGlobalMultiplier(5);
int result = Operators<int>::multiply(10);  // result is 50
```

## Mathematical Operations
Perform mathematical operations like square, cube, or roots:

```cpp
int squared = Operators<int>::square(4);  // squared is 16
```


## Conversion Functions
Convert between different number systems:

```cpp
int binary = Operators<int>::decimalToBinary(5);  // binary is 101
```

## Statistical Calculations
### Calculate statistical measures:

```cpp
int array[] = {1, 2, 3, 4, 5};
int mean = Operators<int>::average(array, 5);  // mean is 3
```
## Customization
You can modify the Operations class to include more operations as needed for your specific use case.

## Contribution
Feel free to fork this repository and contribute to expanding the library's functionalities.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


*** Please make sure you use the setGlobalMultiplier before utilizing the functions that need it. Otherwise its default value is 0 *** 

*** Please Make sure you add items to the Internal Array before using Mathmatical functions that have no arguments, otherwise you will end up with a 0 ***



## List of Available Functions 
* incrementByOne(T element)
* doubleValue(T element)
* setGlobalMultiplier(T multiplier)
* multiply(T value)
* multiply(T value, T value2)
* divide(T value)
* add(T value)
* add(T value, T value2)
* subtract(T value)
* subtract(T value, T value2)
* modulo(T value)
* multiplyByTwo(T value)
* divideByTwo(T value)
* addTwo(T value)
* subtractTwo(T value)
* moduloTwo(T value)
* square(T value)
* cube(T value)
* squareRoot(T value)
* cubeRoot(T value)
* powerOfTwo(T value)
* powerOfThree(T value)
* powerOf(T value)
* powerOf(T value, T power)
* absoluteValue(T value)
* factorial(T value)
* fibonacci(T value)
* fibonacciPrint(T value)
* greatestCommonDivisor(T value)
* leastCommonMultiple(T value)
* decimalToBinary(T value)
* binaryToDecimal(T value)
* decimalToOctal(T value)
* octalToDecimal(T value)
* decimalToHexadecimal(T value)
* hexadecimalToDecimal(T value)
* calculatePercentage(T value)
* calculatePercentage(T value, T percentage)
* calculatePercentageOf(T value)
* calculatePercentageOf(T value, T percentage)
* calculatePercentageChange(T value)
* calculatePercentageChange(T value, T percentage)
* calculatePercentageDifference(T value)
* calculatePercentageDifference(T value, T percentage)
* calculatePercentageError(T value)
* calculatePercentageError(T value, T percentage)
* calculatePercentageOfError(T value)
* calculatePercentageOfError(T value, T percentage)
* calculatePercentageOfChange(T value)
* calculatePercentageOfChange(T value, T percentage)
* calculatePercentageOfDifference(T value)
* calculatePercentageOfDifference(T value, T percentage)
* calculatePercentageOfTotal(T value)
* calculatePercentageOfTotal(T value, T percentage)
* calculatePercentageOfDifferenceFrom(T value)
* calculatePercentageOfDifferenceFrom(T value, T percentage)
* calculatePercentageOfDifferenceTo(T value)
* calculatePercentageOfDifferenceTo(T value, T percentage)
* calculatePercentageOfTotalFrom(T value)
* calculatePercentageOfTotalFrom(T value, T percentage)
* calculatePercentageOfTotalTo(T value)
* calculatePercentageOfTotalTo(T value, T percentage)
* calculatePercentageOfDifferenceBetween(T value)
* calculatePercentageOfDifferenceBetween(T value, T percentage)
* calculatePercentageOfTotalBetween(T value)
* calculatePercentageOfTotalBetween(T value, T percentage)
* calculatePercentageOfDifferenceFromTo(T value)
* calculatePercentageOfDifferenceFromTo(T value, T percentage)
* calculatePercentageOfTotalFromTo(T value)
* calculatePercentageOfTotalFromTo(T value, T percentage)
* calculatePercentageOfDifferenceBetweenTo(T value)
* calculatePercentageOfDifferenceBetweenTo(T value, T percentage)
* calculatePercentageOfTotalBetweenTo(T value)
* calculatePercentageOfTotalBetweenTo(T value, T percentage)
* calculatePercentageOfDifferenceBetweenFrom(T value)
* calculatePercentageOfDifferenceBetweenFrom(T value, T percentage)
* calculatePercentageOfTotalBetweenFrom(T value)
* calculatePercentageOfTotalBetweenFrom(T value, T percentage)
* calculatePercentageOfDifferenceBetweenFromTo(T value)
* calculatePercentageOfDifferenceBetweenFromTo(T value, T percentage)
* calculatePercentageOfTotalBetweenFromTo(T value)
* calculatePercentageOfTotalBetweenFromTo(T value, T percentage)
* calculateDecimalPercentage(T value)
* calculateDecimalPercentage(T value, T percentage)
* calculateDecimal(T value)
* calculateDecimal(T value, T percentage)
* divideByHundred(T value)
* calculatePercentageOfValue(T value, T percentage)
* divide(T value, T divisor)
* addItemsToArray(T value)
* removeItemsFromArray(T value)
* getItemsFromArray(T value)
* average(T value)
* median(T value)
* mode(T value)
* range(T value)
* standardDeviation(T value)
* variance(T value)
* sum()
* sum(T value)
* product()
* product(T value)
* findMin()
* minimum(T* array, size_t size)
* findMax()
* count(T value)
  
# Predicates Class

# ArrayListPredicates Library for Arduino

## Overview
The ArrayListPredicates library provides a comprehensive set of Predicates that can be performed on elements of an ArrayList or other list structure. This library is designed for Arduino projects and offers a wide range of mathematical and utility functions.

## Features
* Predicates on individual elements (e.g., isPrime, isNegative, isDivisibleBy)
* Customizable global multiplier for scalable operations

## Installation
* Download the Predicates.h && the Operators.h file.

* Place it in your Arduino project's directory.

* Include the library in your Arduino sketch:

```cpp
#include "Predicates.h"
#include "Operators.h"
```

## ChangeLog
### Version 1.0.0
* Initial Release
* Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK
* Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them.
### Version 1.0.1
* Added Function Comments to the Library
* Fixed a bug where the library would not build due to an issue with an overloaded function. It was overloaded exactly the same way.
### Version 1.0.2
* Removed the debug messages from library to save memory space
            


## Usage
### Basic Operations
You can perform basic Predicate Checks on array list elements:

```cpp
Predicates<int>::setGlobalMultiplier(5);

if(Predicates<int>::isDivisibleBy(5)){
    Serial.println("yay, The Number Is Divisible by the Multiplier.");
}

if(Predicates<int>::isDivisibleBy(5, list.getByString(i).toInt())){
    Serial.println("Yay, The Number is Divisible by the specified number");
}
```

## Customization
You can modify the Predicates class to include more operations as needed for your specific use case.

## Contribution
Feel free to fork this repository and contribute to expanding the library's functionalities.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


*** Please make sure you use the setGlobalMultiplier or sync() before utilizing the functions that need it. Otherwise you may get the wrong boolean value returned *** 

*** Please Make sure you add items to the Internal Array before using Mathmatical functions that have no arguments, otherwise you will likely end up getting the wrong boolean value ***



## List of Available Functions 
* setDebug(bool debug)
* isDebug()
* setGlobalMultiplier(int globalMultiplier)
* getGlobalMultiplier() 
* sync() //Syncs to the Operators class
* isEven(const T& value) 
* isOdd(const T& value)
* isPositive(const T& value) 
* isNegative(const T& value)
* isZero(const T& value)
* isNonZero(const T& value)
* isGreaterThan(T value) 
* isGreaterThan(T value, T value2)
* isLessThan(T value)
* isLessThan(T value, T value2)
* isEqual(T value)
* isEqual(T value, T value2)
* isGreaterThanOrEqualTo(const T& value, const T& threshold) 
* isLessThanOrEqualTo(const T& value, const T& threshold)
* isEqualTo(const T& value, const T& threshold)
* isNotEqualTo(const T& value, const T& threshold) 
* isBetween(const T& value, const T& lower, const T& upper) 
* isNotBetween(const T& value, const T& lower, const T& upper)
* isWithin(const T& value, const T& center, const T& radius) 
* isNotWithin(const T& value, const T& center, const T& radius) 
* isWithinExclusive(const T& value, const T& center, const T& radius)
* isNotWithinExclusive(const T& value, const T& center, const T& radius) 
* isWithinInclusive(const T& value, const T& center, const T& radius)
* isNotWithinInclusive(const T& value, const T& center, const T& radius) 
* isWithinLeftExclusive(const T& value, const T& center, const T& radius) 
* isNotWithinLeftExclusive(const T& value, const T& center, const T& radius) 
* isWithinRightExclusive(const T& value, const T& center, const T& radius) 
* isNotWithinRightExclusive(const T& value, const T& center, const T& radius) 
* isWithinLeftInclusive(const T& value, const T& center, const T& radius) 
* isNotWithinLeftInclusive(const T& value, const T& center, const T& radius) 
* isWithinRightInclusive(const T& value, const T& center, const T& radius) 
* isNotWithinRightInclusive(const T& value, const T& center, const T& radius) 
* isWithinLeft(const T& value, const T& center, const T& radius) 
* isNotWithinLeft(const T& value, const T& center, const T& radius) 
* isWithinRight(const T& value, const T& center, const T& radius) 
* isNotWithinRight(const T& value, const T& center, const T& radius)
* isWithinInclusiveLeft(const T& value, const T& center, const T& radius) 
* isNotWithinInclusiveLeft(const T& value, const T& center, const T& radius) 
* isWithinInclusiveRight(const T& value, const T& center, const T& radius) 
* isNotWithinInclusiveRight(const T& value, const T& center, const T& radius) 
* isWithinExclusiveLeft(const T& value, const T& center, const T& radius) 
* isNotWithinExclusiveLeft(const T& value, const T& center, const T& radius) 
* isWithinExclusiveRight(const T& value, const T& center, const T& radius) 
* isPrime(T value)
* isDivisibleBy(T value) 
* isDivisibleBy(T value, T divisor) 
* isDivisibleByTwo(T value) 


# ColorMapper Library

The ColorMapper library is designed for easy color management in Arduino and PlatformIO projects. It allows the creation, manipulation, and conversion of colors between different formats.

## Features

- Define colors using RGB values.
- Convert colors to and from hexadecimal format.
- Manage a dynamic list of colors.
- Supports up to 15 colors in the dynamic list.

## Installation

### For Arduino

1. Download the `ColorMapper` library.
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library and select the downloaded file.

### For PlatformIO

1. Add `ColorMapper` to your `platformio.ini` dependencies.

lib_deps =
  git_user/ColorMapper


```c++

  #include "ColorMapper.h"

ColorMapper colorMapper;

void setup() {
  Serial.begin(9600);
  
  // Adding a color
  Color myColor(128, 128, 128); // Grey color
  colorMapper.addColor(myColor);
  
  // Getting a color
  Color firstColor = colorMapper.getColor(0);
  Serial.print("First color RGB: ");
  Serial.print(firstColor.getR());
  Serial.print(", ");
  Serial.print(firstColor.getG());
  Serial.print(", ");
  Serial.println(firstColor.getB());
  
  // Converting color to Hex
  unsigned long hex = colorMapper.colorToHex(firstColor);
  Serial.print("Hex: #");
  Serial.println(hex, HEX);
}

void loop() {
  // Your code here
}
```

Contributing
Contributions to the ColorMapper library are welcome. Please submit pull requests or open issues to discuss proposed changes.


# Trees Library (Will Be Separated into separate Libraries.)





# TypeTraits Library 

## Overview

The TypeTraits library provides a collection of template structures to facilitate type checking and type traits identification in Arduino programming. It offers a simple, compile-time mechanism to query the characteristics of types, such as whether a type is arithmetic, a character, a string, an integral type, a floating-point type, an array, a pointer, a reference, const-qualified, volatile-qualified, or whether two types are the same.

You can use it in boolean operations, Serial statements, static_asserts(), etc...
## Features

- **Arithmetic Type Check**: Determine if a type is one of the basic arithmetic types (`int`, `float`, `double`, `long`, `byte`).
- **Character Type Check**: Check if a type is a `char`.
- **String Type Check**: Identify if a type is an Arduino `String`.
- **Integral Type Check**: Check if a type is an integral (`int`, `long`, `char`, `bool`).
- **Floating-Point Type Check**: Determine if a type is a floating-point type (`float`, `double`).
- **Array Type Check**: Identify if a type is an array.
- **Pointer Type Check**: Check if a type is a pointer, including const pointers.
- **Reference Type Check**: Determine if a type is a reference.
- **Const Qualification Check**: Identify if a type is const-qualified.
- **Volatile Qualification Check**: Check if a type is volatile-qualified.
- **Type Comparison**: Determine if two types are the same.

## Installation

1. Download the TypeTraits library.
2. Place the `TypeTraits.h` file into your Arduino project's library folder or a location accessible by your project.
3. Include the library in your sketch with `#include <TypeTraits.h>`.

## Usage Example

```cpp
#include <Arduino.h>
#include <TypeTraits.h>

void setup() {
    Serial.begin(9600);

    // Example usage of is_arithmetic
    Serial.println(is_arithmetic<int>::value); // Outputs: 1 (true)
    Serial.println(is_arithmetic<String>::value); // Outputs: 0 (false)
    static_assert(is_arithmetic<float>::value); // Throws compile error if condition is false; in this case the float is an 
    // arithmetic type .

    // Example usage of is_same
    Serial.println(is_same<int, int>::value); // Outputs: 1 (true)
    Serial.println(is_same<int, float>::value); // Outputs: 0 (false)
}

void loop() {
    // Your code here
}
```

## Contributing
We welcome contributions to improve the TypeTraits library. Please feel free to submit issues and pull requests.

## License
This library is open-source and available under the Apache license. See the LICENSE file for more information.


# AVL Tree Library for Arduino

## Overview
This library provides an implementation of an AVL Tree, a self-balancing binary search tree, for use in Arduino projects. The AVL Tree maintains a balance factor for each node to ensure that the tree remains approximately balanced at all times, offering O(log n) complexity for insertion, deletion, and lookup operations.

## Features
* Insertion: Add elements to the tree while maintaining its balanced nature.
* Deletion: Remove elements from the tree, automatically rebalancing as necessary.
* Search: Efficiently find elements within the tree.
* Traversal: Supports in-order, pre-order, and post-order traversals to iterate over elements in the tree.
* Min/Max: Quickly retrieve the minimum or maximum value contained in the tree.
* Height and Balance Information: Access the height of the tree and the balance factor of any node to understand the tree's structure.
* Printing: Utility functions for printing the tree structure to aid in debugging.

## Usage
To use this library in your Arduino project, include the AVLTree.h header file and follow the example below:

```cpp
#include "AVLTree.h"

AVLTree<int> myTree;

void setup() {
  Serial.begin(9600);
  
  // Insert elements into the AVL Tree
  myTree.insert(10);
  myTree.insert(20);
  myTree.insert(30);
  myTree.insert(5);
  myTree.insert(3);

  // Perform in-order traversal of the tree
  Serial.println("In-order Traversal:");
  myTree.inOrder();

  // Find the minimum and maximum elements
  Serial.print("Minimum: ");
  Serial.println(myTree.findMin());
  Serial.print("Maximum: ");
  Serial.println(myTree.findMax());

  // Delete an element
  myTree.deleteNode(10);
  Serial.println("After deleting 10:");
  myTree.inOrder();
}

void loop() {
  // Tree operations can be invoked here or in setup()
}
```

## Implementation Details
The AVL Tree implementation automatically handles balancing through rotations during insertions and deletions, ensuring that the tree remains as balanced as possible without requiring manual intervention.

## Functions
* ```cpp void insert(T data)```: Inserts a new element into the tree.
* ```void remove(T data)```: Removes an element from the tree.
* ```void inOrder()```, void preOrder(), void postOrder(): Perform tree traversals.
* ```T findMin(), T findMax()```: Retrieve the minimum and maximum elements of the tree.
* ```void printTree()```: Prints a visual representation of the tree.
* ```void clear()```: Clears the tree, removing all elements.
* ```bool isEmpty()```: Checks if the tree is empty.

## Customization
The library is templated to allow for the storage of any data type that supports comparison operations (i.e., has operator< and operator> defined). This makes it suitable for a wide range of applications, from storing simple integers to complex objects, as long as they provide the necessary comparison operators.

## Contribution
Contributions to the library are welcome, whether they are for bug fixes, improvements, or new features. Please follow standard coding conventions and add comments to your code to ensure readability and maintainability.

## License
This library is released under the Apache License. See the LICENSE file for more details.



# Binary Search Tree Library for Arduino

## Overview
This library provides a simple and efficient implementation of a Binary Search Tree (BST) for Arduino projects. BSTs are fundamental data structures that enable fast data storage, retrieval, and manipulation operations, all of which are essential in various computing applications.

## Features
* Insertion: Adds a new element to the BST while preserving the binary search property.
* Deletion: Removes an element from the BST and reorganizes the tree to maintain its properties.
* Search: Finds an element in the BST and returns a pointer to the node containing it.
* Clear: Removes all elements from the BST, freeing up the allocated memory.

## Usage
To use this library in your Arduino project, include the BinarySearchTree.h header file and instantiate a BinarySearchTree object with your desired data type. The following example demonstrates basic operations such as insertion, search, and deletion:

```cpp
#include "BinarySearchTree.h"

BinarySearchTree<int> bst;

void setup() {
  Serial.begin(9600);

  // Insert elements
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(2);
  bst.insert(4);

  // Search for an element
  BinarySearchNode<int>* found = bst.search(4);
  if (found != NULL) {
    Serial.print("Found: ");
    Serial.println(found->data);
  } else {
    Serial.println("Element not found.");
  }

  // Delete an element
  bst.deleteNode(3);

  // Clear the BST
  bst.clear();
}

void loop() {
  // Placeholder for the main logic
}

```
## Implementation Details
The BST is implemented using templates, allowing it to store any type of data that supports comparison operations (< and >).
Each node in the BST contains the data of type T, and pointers to the left and right child nodes.
The insert and deleteNode functions ensure that the BST properties are maintained.

## Customization
This library can be easily customized and extended to include additional functionalities such as in-order, pre-order, and post-order traversals, or balancing operations to transform it into a self-balancing binary search tree like an AVL tree or a Red-Black tree.

## Contribution
Contributions to the library are welcome. If you find a bug, have a suggestion for improvement, or want to add new features, please feel free to contribute.

## License
This library is open-sourced and licensed under the Apache License. See the LICENSE file for more information.


# Binary Tree Library for Arduino

## Overview
The Binary Tree library for Arduino provides a comprehensive implementation for managing binary trees. Binary trees are a fundamental data structure in computer science, used in various applications such as sorting, data storage, and more. This library enables the creation, manipulation, and traversal of binary trees directly on your Arduino projects.

## Key Features
* Dynamic Tree Creation: Easily create and manage binary trees with any type of data.
* CRUD Operations: Supports insert, search, and delete operations.
* Traversal Methods: Includes in-order, pre-order, post-order, and level-order traversal methods for exploring your trees.
* Utility Functions: Functions to calculate height, count leaves, find minimum and maximum values, and more.
* Mirroring: Ability to mirror the binary tree, swapping left and right children of all nodes.
* Memory Management: Efficiently manages memory to ensure the stability of your Arduino projects.

## Usage
To utilize the BinaryTree library in your project, include the BinaryTree.h header and instantiate a BinaryTree object. Here's a basic example demonstrating some of the library's functionalities:

```cpp
#include "BinaryTree.h"

BinaryTree<int> myTree;

void setup() {
  Serial.begin(9600);
  
  // Insert data into the binary tree
  myTree.insert(50);
  myTree.insert(30);
  myTree.insert(70);
  
  // Perform an in-order traversal
  Serial.println("In-order Traversal:");
  myTree.inorder();
  
  // Find the maximum value in the binary tree
  Serial.print("Maximum Value: ");
  Serial.println(myTree.findMax());
  
  // Mirror the binary tree
  myTree.mirror();
  
  // Perform a level-order traversal on the mirrored tree
  Serial.println("Level-order Traversal of Mirrored Tree:");
  myTree.levelorder();
}

void loop() {
  // Placeholder for loop logic
}
```

## Customization and Extension
The library is templated, allowing it to work with any data type that supports comparison operators.
You can extend the library to include additional tree operations as needed, such as balancing the tree or implementing specific traversal algorithms.

## Contribution
Contributions are welcome! If you have improvements or bug fixes, please feel free to contribute to the library. Your input is valuable in making this library more robust and feature-rich.

## License
This library is released under an Apache license. 

# B Plus Tree Library for Arduino

## Overview
The B+ Tree library for Arduino offers an efficient implementation of the B+ tree data structure, catering to applications that require a balanced tree for rapid data retrieval, insertion, and deletion. B+ trees are especially useful for database and filesystem implementations due to their ability to maintain sorted data and support efficient range queries.

## Key Features
* Balanced Tree Structure: Ensures all leaf nodes are at the same level, providing uniform access times.
* Efficient Storage: Optimizes space by storing keys in internal nodes and values in leaf nodes, making it ideal for large datasets.
* Dynamic Insertion and Deletion: Supports dynamic updates while maintaining balance, ensuring optimal performance.
* Range Queries: Facilitates efficient execution of range queries, returning all values within a given range quickly.
* Traversal and Search: Provides methods for tree traversal and searching specific keys, offering fast access to data.

## Usage
To use the B+ Tree library, include the B_PLUS_TREE.h header in your Arduino sketch and instantiate a BPlusTree object with the desired minimum degree.

Here is a simple example that demonstrates how to create a B+ Tree, insert keys, and perform a search:

```cpp
#include "B_PLUS_TREE.h"

// Initialize a B+ tree with a minimum degree of 3
BPlusTree<int> myTree(3);

void setup() {
  Serial.begin(9600);

  // Insert keys into the B+ tree
  myTree.insert(10);
  myTree.insert(20);
  myTree.insert(30);

  // Search for a key in the B+ tree
  BPlusTreeNode<int>* found = myTree.search(myTree.getRoot(), 20);
  if (found != nullptr) {
    Serial.println("Key found!");
  } else {
    Serial.println("Key not found.");
  }
}

void loop() {
  // Placeholder for loop logic
}
```
## Customization
The library can be customized for different data types and applications. You can adjust the minimum degree based on your dataset size and access patterns to optimize performance.

## Contribution
Contributions to the library are welcome, whether it's through adding new features, improving existing ones, or fixing bugs. Your efforts will help enhance the library's functionality and utility for the Arduino community.

## License
This library is provided under an open-source license. You are free to use, modify, and distribute it in your projects, provided that you comply with the license terms.




# BTree Library for Arduino

## Overview
This BTree library for Arduino provides an implementation of the B-Tree data structure, offering efficient mechanisms for storing, searching, and navigating through a large set of ordered data. B-Trees are widely used in databases and file systems to allow for quick data retrieval, insertions, and deletions while maintaining a balanced tree structure.

## Key Features
* Balanced Tree Structure: Ensures that the tree remains balanced, providing efficient data access times.
* Efficient Storage and Retrieval: Utilizes a node-based structure to efficiently store data and support rapid retrieval operations.
* Support for Dynamic Updates: Seamlessly handles insertions and deletions, ensuring the tree remains optimally balanced.
* Traversal and Search: Enables in-order traversal of the tree and supports effective searching for specific keys.
* Customizable Degree: Allows for customization of the tree's minimum degree, accommodating various use cases and optimizing performance.

# Usage
To utilize the BTree library, include BTREE_H in your Arduino project and instantiate a BTree object specifying the minimum degree for the tree.

# Example Usage
Below is a simple example demonstrating how to create a B-Tree, insert elements, and perform a search operation:

```cpp
#include "BTree"

// Define the minimum degree for the B-Tree
const int t = 3;

// Initialize a B-Tree with integers and a minimum degree of t
BTree<int> myBTree(t);

void setup() {
  Serial.begin(9600);

  // Insert elements into the B-Tree
  myBTree.insert(1);
  myBTree.insert(5);
  myBTree.insert(10);

  // Search for an element in the B-Tree
  BTreeNode<int>* result = myBTree.search(5);
  if (result != nullptr) {
    Serial.println("Element found in the B-Tree.");
  } else {
    Serial.println("Element not found in the B-Tree.");
  }
}

void loop() {
  // Placeholder for loop() method
}
```

## Customization
Adjust the minimum degree (t) of the B-Tree according to your dataset's size and your application's specific requirements to optimize performance.

## Contribution
We welcome contributions to the BTree library. Whether it's adding new features, optimizing existing ones, or fixing bugs, your input helps improve the library for the Arduino community.

## License
This library is made available under an Apache. 


# Fenwick Tree Library for Arduino

## Overview
The Fenwick Tree, also known as a Binary Indexed Tree (BIT), offers an efficient way to update elements and calculate prefix sums in a table of numbers. This Arduino-compatible Fenwick Tree library enables easy manipulation of sequences of numbers, providing a powerful tool for handling cumulative frequency tables and range sum queries in embedded projects.

## Key Features
* Efficient Range Updates and Queries: Perform range updates and range sum queries efficiently, ideal for dynamic cumulative frequency tables.
* Flexible Point Updates and Queries: Supports updating values at specific points and querying sums over ranges or single points.
* Scalable Operations: Efficiently scales operations on large datasets with logarithmic time complexity for updates and queries.
* Arduino-Compatible: Designed to integrate seamlessly with Arduino projects, enhancing data handling capabilities in embedded systems.


## Usage
To use the Fenwick Tree library, include FENWICKTREE_H in your Arduino project. The library requires SimpleVector.h for dynamic array management and TypeTraits.h for type checking.

## Example Usage
Below is a simple example that demonstrates how to initialize a Fenwick Tree, perform updates, and query sums:

```cpp
#include "FENWICKTREE_H"

// Initialize a Fenwick Tree with a size of 10
FenwickTree<int> ft(10);

void setup() {
  Serial.begin(9600);

  // Perform updates on the Fenwick Tree
  ft.add(0, 5); // Add 5 to index 0
  ft.add(1, 3); // Add 3 to index 1
  ft.range_update(2, 4, 2); // Add 2 to the range [2, 4]

  // Query the sum of the first 5 elements
  int sumFirstFive = ft.sum(0, 4);
  Serial.print("Sum of first 5 elements: ");
  Serial.println(sumFirstFive);

  // Query a single point
  int pointQuery = ft.point_query(3);
  Serial.print("Value at index 3: ");
  Serial.println(pointQuery);
}

void loop() {
  // Loop code here (if necessary)
}
```

## Customization
The FenwickTree class template can be instantiated with any numeric type (int, long, float, etc.), allowing for flexibility based on the requirements of your application.

## Contribution
Contributions to the Fenwick Tree library are welcome. Enhancements, bug fixes, or documentation improvements will help make the library more useful for the Arduino community.

## License
This library is distributed under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.



# Heap Tree Library for Arduino

## Overview
The Heap Tree library for Arduino provides an efficient implementation of a binary heap data structure, offering fast insertions, and extraction of the maximum (or minimum) element. This library is tailored for Arduino projects that require priority queue functionality, sorting, or managing a dynamically changing dataset with quick access to the highest or lowest element.

## Key Features
* Dynamic Resizing: Automatically resizes the underlying storage as more elements are added to maintain performance and efficiency.
* Max and Min Heap Operations: Supports extracting the maximum and minimum values, making it versatile for different use cases.
* Insertion and Removal: Allows for the insertion of new elements and removal of specific elements or based on value, maintaining the heap property.
* Utility Functions: Provides functions to check if the heap is empty, clear the heap, and get the current size of the heap.

## Usage
To use the Heap Tree library, include HEAP_TREE_h in your Arduino sketch. Ensure you have adequate memory available on your Arduino device, as dynamic memory allocation is used.

## Example Usage
Here's a simple example demonstrating how to use the Heap Tree library to manage integers:

```cpp
#include "HEAP_TREE_h"

HeapTree<int> heap;

void setup() {
  Serial.begin(9600);

  // Insert elements into the heap
  heap.insert(10);
  heap.insert(20);
  heap.insert(5);
  
  // Print the heap
  heap.print();

  // Extract the maximum element
  int max = heap.extractMax();
  Serial.print("Extracted Max: ");
  Serial.println(max);

  // Print the heap after extraction
  heap.print();
}

void loop() {
  // Your loop code here
}
```

## Customization
The HeapTree class template can be instantiated with any data type that supports comparison operations (<, >). This allows it to be used with custom data types, provided they have the necessary comparison operators defined.

## Contribution
Contributions to the Heap Tree library are welcome. Whether it's adding new features, fixing bugs, or improving documentation, your help is appreciated to make this library more useful for the Arduino community.

## License
This library is distributed under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.



# Initializer List Library for Arduino

## Overview
The Initializer List library provides a minimal and efficient implementation of initializer lists for Arduino, similar to the C++11 feature. It allows Arduino developers to initialize objects and arrays in a clean and concise way, improving code readability and maintainability.

## Key Features
Standard-Compatible Types: Offers types such as value_type, reference, const_reference, size_type, iterator, and const_iterator for compatibility with standard C++ initializer list behavior.
Size and Iteration: Supports querying the size of the initializer list and provides iterator access to its elements, enabling range-based for loops and standard iteration patterns.
No Dynamic Memory: Operates without dynamic memory allocation, ensuring predictability and efficiency, crucial for memory-constrained environments like Arduino.

## Usage
This library is intended to be used where fixed-size, compile-time initializer lists are beneficial, such as static configuration data, pin mappings, or predefined sequences of values.

## Example Usage
Here's an example demonstrating how to use the initializer_list library to initialize and process a list of integers:

```cpp

#include "INITIALIZER_LIST_H"

void processList(const initializer_list<int>& list) {
  for (auto item : list) {
    Serial.println(item);
  }
}

void setup() {
  Serial.begin(9600);
  // Wait for the serial monitor to open
  while (!Serial) {}

  // Initialize a list of integers using initializer_list
  initializer_list<int> myList = {1, 2, 3, 4, 5};

  // Process and print the list
  processList(myList);
}

void loop() {
  // Your loop code here
}
```
## Customization
The initializer_list template can be instantiated with any type, allowing it to be used with a wide range of data types beyond primitives, including custom classes and structures, provided they are copyable.

## Contribution
Contributions to enhance the functionality, extend the usage scenarios, or improve the documentation of the Initializer List library are welcome. Your input can help make this library a valuable resource for the Arduino community.

## License
This library is provided under an open-source license, permitting modification, distribution, and use in both personal and commercial projects, in line with the spirit of Arduino and its community of makers and developers.


# Interval Tree Library for Arduino

## Overview
The Interval Tree library for Arduino offers an efficient way to store intervals and perform fast search operations to find overlapping intervals. This library is particularly useful in applications requiring management of multiple time intervals or ranges, such as scheduling tasks, managing resources, or collision detection in spatial data.

## Features
* Generic Implementation: Works with any arithmetic type (e.g., int, float) for interval boundaries.
* Overlap Checking: Provides a method to check if a given interval overlaps with any interval in the tree.
* Insertion and Deletion: Supports inserting new intervals and deleting existing intervals from the tree.
* Traversal: Enables inorder traversal of the tree to access stored intervals in a sorted manner.
* Balance Maintenance: Automatically maintains the balance of the tree for optimal search performance.

## Usage
### Setup
Before using the Interval Tree library, include it in your Arduino sketch:

```cpp

#include "INTERVAL_TREE_h"
```
### Creating an Interval Tree
Instantiate an Interval Tree object with the desired type for the interval boundaries:

```cpp
IntervalTree<int> myTree;
```
### Inserting Intervals
Insert intervals into the tree using the insert() method:

```cpp
Interval<int> myInterval = {low: 10, high: 20};
myTree.insert(myInterval);
```
### Searching for Overlapping Intervals
Search for any interval overlapping with a given interval using the search() method:

```cpp

Interval<int> searchInterval = {low: 15, high: 25};
IntervalTreeNode<int>* result = myTree.search(searchInterval);
if (result != nullptr) {
    Serial.println("Overlap found.");
}
```

### Deleting Intervals
Delete an interval from the tree using the deleteNode() method:

```cpp

myTree.deleteNode(myInterval);
```
### Traversing the Tree
Perform an inorder traversal of the tree:

```cpp

myTree.inorder(); // This will print the intervals in sorted order
```
## Example
```cpp

void setup() {
  Serial.begin(9600);
  IntervalTree<int> tree;
  tree.insert({5, 10});
  tree.insert({15, 20});
  tree.insert({25, 30});
  tree.insert({10, 15}); // Overlaps with {5, 10} and {15, 20}

  Serial.println("Inorder traversal of the Interval Tree:");
  tree.inorder();

  Interval<int> searchInt = {14, 16};
  auto result = tree.search(searchInt);
  if (result != nullptr) {
    Serial.print("An overlapping interval for ");
    Serial.print("[" + String(searchInt.low) + ", " + String(searchInt.high) + "] ");
    Serial.println("was found.");
  } else {
    Serial.println("No overlapping interval found.");
  }
}

void loop() {
  // Put your main code here, to run repeatedly:
}
```

## Contribution
Contributions to the Interval Tree library are welcome. Whether it's extending functionality, improving efficiency, or enhancing documentation, your input can help make this library more useful for the Arduino community.

## License
This library is released under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.


# K-Dimensional Tree Library for Arduino

## Overview
The K-Dimensional Tree (KD-Tree) library for Arduino provides a powerful tool for organizing points in a k-dimensional space. This data structure facilitates efficient search operations, such as nearest neighbor search and range search, making it ideal for applications in robotics, spatial indexing, and more.

## Key Features
* Generic Implementation: Compatible with any numeric type (e.g., int, float) for the coordinates of points.
* Dynamic Insertion: Allows for dynamic insertion of points into the k-dimensional space.
* Search Operations: Supports efficient nearest neighbor search and range search within the k-dimensional space.
* Deletion: Enables removal of points from the tree.
* Flexibility: Works with any number of dimensions specified at tree initialization.

## Usage
### Initialization
First, include the library and create a KD-Tree instance specifying the dimension.

```cpp

#include "K_DIMENSIONAL_TREE_h"
KDimensionalTree<float> kdTree(3); // For a 3-dimensional space
```
## Inserting Points
Insert points into the KD-Tree. Points are represented as SimpleVector<T> objects.

``` cpp

SimpleVector<float> point1 = {1.0, 2.0, 3.0};
kdTree.insert(point1);
```
## Searching for a Nearest Neighbor
Find the nearest neighbor of a given point.

```cpp
SimpleVector<float> searchPoint = {1.1, 2.1, 3.1};
SimpleVector<float> nearestNeighbor = kdTree.nearestNeighbor(searchPoint);
``` 
## Range Search
Find all points within a specified range.

```cpp

SimpleVector<float> lowerBound = {0.0, 0.0, 0.0};
SimpleVector<float> upperBound = {2.0, 2.0, 2.0};
SimpleVector<SimpleVector<float>> pointsInRange = kdTree.rangeSearch(lowerBound, upperBound);
```
## Removing Points
Remove a point from the KD-Tree.

```cpp

kdTree.remove(point1);
```
## Clearing the Tree
Remove all points from the KD-Tree.

```cpp

kdTree.clear();
```
## Example Sketch
```cpp
void setup() {
    Serial.begin(9600);

    KDimensionalTree<float> kdTree(2); // 2D space
    kdTree.insert({1.0, 2.0});
    kdTree.insert({3.0, 4.0});
    kdTree.insert({5.0, 6.0});

    SimpleVector<float> searchPoint = {3.1, 4.1};
    SimpleVector<float> nearest = kdTree.nearestNeighbor(searchPoint);

    Serial.print("Nearest neighbor to (3.1, 4.1): (");
    Serial.print(nearest[0]);
    Serial.print(", ");
    Serial.print(nearest[1]);
    Serial.println(")");

    // Range search
    SimpleVector<float> lower = {2.0, 3.0};
    SimpleVector<float> upper = {4.0, 5.0};
    auto pointsInRange = kdTree.rangeSearch(lower, upper);
    Serial.println("Points in range:");
    for (auto& p : pointsInRange) {
        Serial.print("(");
        Serial.print(p[0]);
        Serial.print(", ");
        Serial.print(p[1]);
        Serial.println(")");
    }
}

void loop() {
    // Put your main code here, to run repeatedly:
}
```
## Contribution
Contributions to the KD-Tree library are welcome. Whether it's optimizing the implementation, adding new features, or improving the documentation, your input can help make this library more useful for the Arduino community.

## License
This library is released under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.


# MathLib for Arduino

## Overview
MathLib is a comprehensive mathematical library designed for the Arduino platform. It offers a wide range of mathematical functions, from basic arithmetic operations to more complex trigonometric and hyperbolic calculations. This library aims to enhance the mathematical capabilities of Arduino projects without the need for external dependencies.

## Features
* Basic Mathematical Functions: Includes factorial, power, prime checking, and prime number generation.
* Trigonometric Functions: Provides sine, cosine, tangent, cotangent, secant, and cosecant functions, along with their arc (inverse) versions.
* Hyperbolic Functions: Offers sinh, cosh, tanh, coth, sech, and csch functions, along with their arc (inverse) versions.
* Exponential and Logarithmic Functions: Supports exponential, natural logarithm, and logarithm base 10 calculations.
* Square Root and Cube Root Functions: Implements efficient algorithms for calculating square roots and cube roots.
* Utility Functions: Includes absolute value, ceiling, floor, power with floating-point exponent, log base 2, and log with arbitrary base functions.

## Usage
To use the MathLib, include the library in your Arduino sketch and call any of the available functions:

```cpp

#include "MATHLIB.h"

void setup() {
  Serial.begin(9600);

  // Using trigonometric functions
  double sineValue = Sin(30); // Calculate sine of 30 degrees
  double cosineValue = Cos(45); // Calculate cosine of 45 degrees

  // Using logarithmic functions
  double logValue = Log(10); // Natural log of 10
  double log10Value = Log10(100); // Log base 10 of 100

  // Using power functions
  double powerValue = Power(2, 8); // 2 raised to the power of 8

  // Print results
  Serial.println("Sine(30): " + String(sineValue));
  Serial.println("Cos(45): " + String(cosineValue));
  Serial.println("Log(10): " + String(logValue));
  Serial.println("Log10(100): " + String(log10Value));
  Serial.println("Power(2, 8): " + String(powerValue));
}

void loop() {
  // Your main code here, to run repeatedly:
}
```
## Customization
* MYPI: Defines the value of pi used in trigonometric calculations.
* MYMAX_ITERATIONS: Determines the number of iterations for iterative functions to ensure accuracy.

## Contribution
MathLib is open for contributions. Enhancements, optimizations, or additions to the library are welcome to make it more versatile and efficient.

## License
MathLib is released under an open-source license, allowing free use, modification, and distribution of the library in both personal and commercial projects.


# MatrixMath Library for Arduino

## Overview
MatrixMath is a versatile library designed for handling matrices on the Arduino platform. It enables users to perform a wide range of matrix operations essential for applications in robotics, signal processing, and linear algebra.

## Features
* Matrix Creation and Manipulation: Allows for the creation, copying, and manipulation of matrices.
* Element Access and Modification: Facilitates accessing and modifying individual elements within the matrix.
* Basic Operations: Supports addition, subtraction, multiplication, and division of matrices and scalars.
* Advanced Operations: Includes matrix transposition, inversion, scaling, and setting matrices to identity or zero matrices.
* Transformation Operations: Provides functions for rotating matrices around the X, Y, Z axes, or an arbitrary axis.
* Utility Functions: Offers additional functionalities like printing matrix contents to the serial monitor, clearing the matrix, and more.

## Usage
To use the MatrixMath library in your project, include it at the beginning of your Arduino sketch:

```cpp

#include "MatrixMath.h"
```
## Example: Matrix Addition
``` cpp
MatrixMath matrix1(2, 2);
MatrixMath matrix2(2, 2);

void setup() {
  Serial.begin(9600);

  // Initialize matrices
  matrix1.set(0, 0, 1); matrix1.set(0, 1, 2);
  matrix1.set(1, 0, 3); matrix1.set(1, 1, 4);

  matrix2.set(0, 0, 5); matrix2.set(0, 1, 6);
  matrix2.set(1, 0, 7); matrix2.set(1, 1, 8);

  // Perform matrix addition
  matrix1.add(matrix2);

  // Print the result
  matrix1.print();
}

void loop() {
  // Your loop code here
}
```
## Example: Matrix Scaling
```cpp
void scaleMatrix() {
  MatrixMath matrix(2, 2);
  matrix.set(0, 0, 1); matrix.set(0, 1, 2);
  matrix.set(1, 0, 3); matrix.set(1, 1, 4);

  // Scale the matrix by a factor of 2
  matrix.scale(2.0);

  // Print the scaled matrix
  matrix.print();
}
```
## Contribution
Contributions to the MatrixMath library are welcome. Whether it's adding new features, improving existing ones, or fixing bugs, your help can make MatrixMath more powerful and user-friendly.

## License
MatrixMath is released under an open-source license. Feel free to use it, modify it, and distribute it in your projects as you see fit.


# Map Library for Arduino

## Overview
The Map library provides a simple and efficient way to store and manage key-value pairs in Arduino projects. It's designed to offer easy insertion, removal, and search operations, making it suitable for various applications where associative arrays or dictionaries are needed.

## Features
* Key-Value Storage: Stores elements as key-value pairs for efficient data retrieval based on keys.
* Insertion and Deletion: Supports adding new key-value pairs and removing existing ones.
* Value Retrieval: Allows fetching values associated with specific keys.
* Key and Value Lists: Enables retrieving lists of all keys or values stored in the map.
* Size and Capacity Management: Provides functions to check the size of the map and clear its contents.
* Search and Check Operations: Includes methods to search for keys or values and check if the map contains specific entries.


## Usage
To use the Map library in your Arduino sketch, include it at the beginning of your code:

``` cpp

#include "Map.h"
```
## Example: Storing and Retrieving Data
```cpp
Map<String, int> ageMap;

void setup() {
  Serial.begin(9600);

  // Inserting data into the map
  ageMap.insert("Alice", 30);
  ageMap.insert("Bob", 25);
  ageMap.put("Charlie", 35);

  // Retrieving and printing a value
  Serial.print("Alice's age: ");
  Serial.println(ageMap.get("Alice"));

  // Checking if a key exists
  if (ageMap.containsKey("Bob")) {
    Serial.println("Bob is in the map.");
  }

  // Removing a key-value pair
  ageMap.remove("Charlie");

  // Printing all key-value pairs
  ageMap.print();
}

void loop() {
  // Loop code here
}
```
## Iteration Example
You can iterate over the map using the provided iterator class:

```cpp

for (auto it = ageMap.begin(); it != ageMap.end(); ++it) {
  Serial.print(it->first); // Access the key
  Serial.print(": ");
  Serial.println(it->second); // Access the value
}

```
## Contribution
Contributions to the Map library are welcome. Whether you're fixing bugs, adding new features, or improving the documentation, your help will enhance this library for the Arduino community.

## License
The Map library is open-source and freely available for personal and commercial use under the terms of the license provided with the library source.


# Numeric Limits Library for Arduino
## Overview
The Numeric Limits library provides a way to determine the properties and limits of arithmetic types in the Arduino environment. It covers a range of types including integers, floating-point numbers, and bytes, offering functionality similar to std::numeric_limits from the C++ Standard Library but tailored for Arduino's constraints and types.

## Features
* Type Limits: Obtain minimum and maximum values for various data types.
* Type Properties: Check whether a type is signed, integer, or floating-point.
* Compatibility: Designed with Arduino's 32-bit architecture in mind, ensuring compatibility with common Arduino boards.

## Usage
To use the Numeric Limits library in your Arduino sketch, include it at the beginning of your code:

```cpp
#include "Numeric_Limits.h"
```

## Examples
### Getting Integer Limits
```cpp

int intMin = numeric_limits<int>::Min();
int intMax = numeric_limits<int>::Max();
Serial.print("Int Min: ");
Serial.println(intMin);
Serial.print("Int Max: ");
Serial.println(intMax);
```

### Floating-Point Limits
```cpp

float floatMin = numeric_limits<float>::Min();
float floatMax = numeric_limits<float>::Max();
Serial.print("Float Min: ");
Serial.println(floatMin, 10); // '10' specifies the number of digits after the decimal point
Serial.print("Float Max: ");
Serial.println(floatMax, 10);
```
### Checking Type Properties
```cpp

bool isIntSigned = numeric_limits<int>::is_signed();
Serial.print("Is Int Signed: ");
Serial.println(isIntSigned ? "Yes" : "No");
```
## Supported Types
The library provides implementations for the following types:

* int
* long
* float
* double
* byte
* char
* bool

Note: On Arduino, double is often equivalent to float, reflected in their shared limits.

## Contribution
Contributions to the Numeric Limits library are welcome. Whether it's through adding support for additional types, enhancing the existing functionality, or improving compatibility across various Arduino platforms, your contributions are valuable to the Arduino community.

## License
This library is open-source and can be freely used and modified in both personal and commercial projects under the terms specified in the license accompanying the library source.


# Octree Library for Arduino

## Overview
The Octree library offers a spatial partitioning structure for efficient querying of 3D space, optimized for Arduino environments. It allows for storing and manipulating data in a three-dimensional space, using a tree structure where each node has exactly eight children. This library is particularly useful for applications involving 3D graphics, spatial analysis, collision detection, and point cloud management.

## Features
* Spatial Partitioning: Efficiently divides 3D space into smaller regions for quick querying and data retrieval.
* Dynamic Allocation: Utilizes a node pool for optimized memory management, limiting the maximum number of nodes to MAX_NODES.
* Search Operations: Supports various search operations including point search, range search, and nearest neighbor search.
* Traversal Methods: Offers multiple tree traversal methods, such as in-order, pre-order, post-order, and level-order traversals.

## Usage
To use the Octree library, include it at the top of your Arduino sketch:

```cpp

#include "Octree.h"
```
## Creating an Octree
Instantiate an Octree object specifying the type of data it will hold:

```cpp

Octree<float> myOctree;
```
## Inserting Data
Insert data into the octree. The data should be of the type specified during the octree's instantiation:

```cpp

myOctree.insert(5.0f);
```
## Searching
Perform a search to find if a specific value exists within the octree:

```cpp

bool found = myOctree.search(5.0f);
Serial.println(found ? "Found" : "Not Found");
```
## Range Search
Query all points within a specified 3D range:

```cpp

SimpleVector<float> results = myOctree.rangeSearch(x1, x2, y1, y2, z1, z2);
```
## Nearest Neighbor
Find the nearest neighbor to a given point in 3D space:

```cpp
float nearest = myOctree.nearestNeighbor(data);
```
## Traversal
Traverse the octree in various orders to process or retrieve the stored data:

```cpp

SimpleVector<float> inOrderResults = myOctree.inOrderTraversal();
```
## Memory Management
The library includes mechanisms to manage memory efficiently, reusing nodes from a preallocated pool to avoid dynamic memory allocation during runtime. This is crucial for the limited memory environment of Arduino devices.

## Application Scenarios
* 3D Graphics: Managing objects in a 3D scene for rendering or collision detection.
* Spatial Analysis: Performing queries on spatial data, such as point clouds.
* Game Development: Efficiently managing game world entities and their interactions.

** Recommended for Microprocessors/MicroControllers with more memory capablility **

## Extending the Library
The Octree library is designed to be extensible. Users can add more features such as balancing the tree, supporting dynamic resizing, and enhancing search capabilities.

## License
This library is open-source and can be used and modified according to the terms specified in the license accompanying the library source. Contributions and improvements from the community are welcome. 


# QuadTree Library for Arduino

## Overview
The QuadTree library is a specialized data structure for Arduino projects that need efficient querying of two-dimensional space. It's well-suited for applications requiring spatial partitioning and indexing, such as graphics rendering, collision detection, and geographic data management.

## Features
* Efficient Spatial Partitioning: Divides 2D space into four quadrants to efficiently manage spatial data.
* Dynamic Insertion and Querying: Supports dynamic insertion of points and querying of rectangular regions.
* Point and Rectangle Support: Includes structures for points and rectangles, including utility functions like containment checks and intersection tests.
* Customizable Capacity: Each node's capacity is customizable, allowing for flexibility in balancing depth versus breadth of the tree.

## Usage
To utilize the QuadTree library, include it at the beginning of your Arduino sketch:

```cpp

#include "QuadTree.h"
```
## Initialization
Create a QuadTree object by specifying the boundary and capacity:

```cpp

Rectangle<float> boundary = {{0, 0}, {100, 100}};
int capacity = 4;
QuadTree<float> quadTree(boundary, capacity);
```
## Inserting Points
Add points to the QuadTree with the insert method:

```cpp

Point<float> point = {25.0, 75.0};
quadTree.insert(point);
```
## Querying Regions
Retrieve points within a specific region by defining a query rectangle:

```cpp

Rectangle<float> queryRect = {{10, 10}, {50, 50}};
SimpleVector<Point<float>> foundPoints = quadTree.query(queryRect);
```
## Deleting Points
Remove a point from the QuadTree, if necessary:

```cpp

quadTree.deletePoint(point);
```
## Clearing the QuadTree
Reset the QuadTree, removing all points:

```cpp

quadTree.clear();
```
## Additional Operations
The library supports additional operations like checking if a point is within the QuadTree's boundary, moving points within the tree, and printing the tree structure for debugging purposes.

## Applications
* Game Development: Managing game entities and detecting collisions.
* Geographic Information Systems (GIS): Indexing geographic locations for rapid querying.
* Computer Graphics: Spatially partitioning objects for efficient rendering.

##  Extending the Library 
While the QuadTree library provides a solid foundation for 2D spatial partitioning, it can be extended with features like balancing the tree, supporting more complex shapes, and optimizing memory usage for large datasets.

## License
This library is provided under an open-source license, allowing for modification and redistribution in both personal and commercial projects. Check the license details for specific terms and conditions.


# Queue Library for Arduino

## Overview
The Queue library provides a generic, dynamic queue implementation in C++ tailored for use in Arduino projects. It's designed to manage a collection of elements in a first-in, first-out (FIFO) manner. Ideal for tasks requiring a sequential processing order, such as event handling or task scheduling.

## Features
* Generic Type Support: Can store elements of any data type.
* Dynamic Resizing: Automatically resizes to accommodate more elements than its initial capacity.
* Basic Queue Operations: Supports enqueue (add), dequeue (remove), and peek (view the front element) operations.
* Utility Functions: Includes methods to check if the queue is empty or full, clear the queue, and count the elements.

## Usage
To use the Queue library, include it at the beginning of your Arduino sketch:

```cpp

#include "Queue.h"
```
## Initialization
Create a Queue object specifying the data type of the elements it will hold:

```cpp

Queue<int> myQueue;
```
## Adding Elements
Add elements to the queue using the enqueue method:

```cpp

myQueue.enqueue(10);
myQueue.enqueue(20);
```
## Removing Elements
Remove the element at the front of the queue with dequeue:

```cpp

int frontElement = myQueue.dequeue();
```
## Viewing the Front Element
Access the element at the front without removing it using peek:

```cpp

int peekElement = myQueue.peek();
```
## Checking Queue Status
Determine if the queue is empty or full:

```cpp

bool isEmpty = myQueue.isEmpty();
bool isFull = myQueue.isFull();
```
## Clearing the Queue
Remove all elements from the queue:

```cpp

myQueue.clear();
```
## Printing Queue Elements
For debugging purposes, print all elements in the queue:

```cpp

myQueue.print();
```
## Counting Elements
Retrieve the number of elements in the queue:

```cpp

size_t elementCount = myQueue.count();
```
## Applications
The Queue library is versatile for a wide range of applications, including but not limited to:

* Event-driven programming where actions or events are queued for processing.
* Managing tasks in multitasking environments, such as scheduling tasks in a specific order.
* Buffering data streams, where data is temporarily stored before processing.
* Customization
* The initial size of the queue and the resizing behavior can be customized by modifying the library's source code to better fit specific requirements, such as memory constraints or expected workload.

## License
The Queue library is open-source and can be freely used and modified. It's recommended to check the specific license details for usage in both personal and commercial projects.


# Red-Black Tree Library for Arduino

## Overview
The Red-Black Tree library provides a balanced binary search tree implementation, ensuring O(log n) time complexity for insertions, deletions, and lookups. It is particularly well-suited for datasets that undergo frequent modifications, as it maintains balance through tree rotations and color flips.

## Key Features
* Self-Balancing: Automatically maintains its balance during insertions and deletions, ensuring optimal path lengths.
* Generic Data Storage: Capable of storing any comparable data type, making it versatile for various applications.
* Efficient Operations: Offers efficient search, insertion, and deletion operations.
* Traversal Methods: Supports in-order, pre-order, and post-order traversals for flexible data access.

## Usage
To use the Red-Black Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "RedBlackTree.h"
```
## Creating a Tree
Instantiate a Red-Black Tree object specifying the data type:

```cpp

RedBlackTree<int> tree;
```
## Inserting Elements
Add elements to the tree using the insert method:

```cpp

tree.insert(5);
tree.insert(3);
tree.insert(10);
```
## Deleting Elements
Remove elements from the tree with deleteNode:

```cpp

tree.deleteNode(3);
```
## Searching for Elements
### Search for an element in the tree:

```cpp

RedBlackNode<int>* node = tree.search(5);
if (node != NULL) {
    Serial.println("Found");
} else {
    Serial.println("Not found");
}
```
## Traversals
Perform different tree traversals:

```cpp

// In-order traversal
tree.inOrder();

// Pre-order traversal
tree.preOrder();

// Post-order traversal
tree.postOrder();
```
## Clearing the Tree
Remove all elements from the tree:

```cpp

tree.clear();
```
## Applications
Red-Black Trees are useful in many scenarios where a balanced search tree is required, such as:

* Maintaining a sorted stream of data.
* Implementing associative arrays.
* Building priority queues with efficient updates.
* Handling datasets with frequent insertions and deletions.

## Customization
While the library is designed to be generic and broadly applicable, specific use cases might require adjustments, such as custom comparison functions for complex data types. Modify the library source to accommodate such needs.

## License
This library is released for public use. Depending on the project's specifics, ensure compliance with the licensing terms if included in both open-source and proprietary projects.


# R-Tree Library for Arduino

## Overview
The R-Tree library for Arduino provides a spatial index structure for efficiently organizing and querying geometric data, such as rectangles or spatial objects. It's particularly useful for applications requiring spatial searches, like collision detection, viewport queries, or geographic information systems.

## Features
* Geometric Data Management: Efficiently stores and manages rectangles, supporting operations like insertion, deletion, and spatial queries.
* Spatial Queries: Supports querying data within a specified geometric range.
* Traversal Orders: Offers multiple tree traversal orders including pre-order, in-order, post-order, and level-order.
* Dynamic Tree Structure: Automatically adjusts its structure for optimal performance as data is added or removed.

## Usage
To use the R-Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "RTree.h"
```
## Creating an R-Tree
Instantiate an R-Tree object specifying the data type:

```cpp

RTree<int> tree;
```
## Inserting Data
Insert rectangles into the tree:

```cpp

Rectangle<int> rect1 = {0, 0, 10, 10};
Rectangle<int> rect2 = {5, 5, 15, 15};
tree.insert(rect1);
tree.insert(rect2);
```

## Removing Data
Remove a rectangle from the tree:

```cpp

tree.remove(rect1);
```
## Searching
Search for a rectangle in the tree (example function, actual search implementation might vary based on specific needs):

```cpp

Rectangle<int> searchRect = {1, 1, 9, 9};
bool found = tree.search(searchRect);
if (found) {
    Serial.println("Rectangle found.");
} else {
    Serial.println("Rectangle not found.");
}
```
## Clearing the Tree
Remove all elements from the tree:

```cpp

tree.clear();
```
## Traversal and Printing
Print the tree contents in different traversal orders:

```cpp

tree.print(PRE_ORDER);
tree.print(IN_ORDER);
tree.print(POST_ORDER);
tree.print(LEVEL_ORDER);
```
## Applications
R-Trees are ideal for a wide range of applications that require spatial data indexing and queries, including:

* Geographic Information Systems (GIS).
* Collision detection in gaming and simulations.
* Spatial data analysis and visualization.
* Database index structures for spatial querying.
``RECOMMENDED FOR MICROCONTROLLERS/MICROPROCESSORS WITH MORE MEMORY CAPABILITY. (UNO, and NANO ARE NOT GOING TO WORK :) )``
``RECOMMEND USING DEVICES LIKE THE RASPBERRY PI, ARDUINO GIGA, ARDUINO MEGA, etc...``

## Customization
While the R-Tree library is designed to be generic, specific applications might require customization, such as supporting different geometric shapes, optimizing for specific query patterns, or enhancing memory management strategies.

## License
This library is provided for public use. Depending on the project's requirements, ensure to comply with the licensing terms if used within both open-source and proprietary projects.


# Segment Tree Library for Arduino

## Overview
The Segment Tree library for Arduino provides a versatile data structure for efficient querying and updating of array segments. It's particularly useful for scenarios requiring frequent updates and queries over a range, such as finding the sum, minimum, or maximum in a segment of an array.

## Features
* Efficient Range Queries: Quickly compute sums, minimums, or maximums within a range of an array.
* Lazy Propagation: Optimizes range updates to avoid unnecessary calculations, enhancing performance for multiple updates.
* Dynamic Updates: Supports updating values or ranges within the array, reflecting changes in subsequent queries.
* Arithmetic Type Support: Designed for numeric data types, ensuring broad applicability for different numeric operations.

## Usage
To use the Segment Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "SegmentTree.h"
```

## Creating a Segment Tree
Instantiate a Segment Tree object using an array (or SimpleVector) of numeric values:

```cpp

SimpleVector<int> arr = {1, 3, 5, 7, 9, 11};
SegmentTree<int> segTree(arr);
```

## Performing Range Queries
Compute the sum, minimum, or maximum within a specified range of the array:

```cpp

int sum = segTree.getSum(1, 3); // Gets the sum of elements from index 1 to 3
int min = segTree.getMin(1, 4); // Gets the minimum value from index 1 to 4
int max = segTree.getMax(2, 5); // Gets the maximum value from index 2 to 5
```

## Updating Values
Update a single value or a range of values in the array:

```cpp

segTree.updateValue(arr, 2, 10); // Updates the value at index 2 to 10
segTree.updateRange(1, 4, 5); // Adds 5 to each element from index 1 to 4
```
## Clearing and Rebuilding
To clear or rebuild the tree after significant changes:

```cpp

segTree.clear(); // Clears the segment tree
segTree.build(arr, 0, arr.size() - 1, 0); // Rebuilds the tree with the current array
```

## Applications
Segment Trees are versatile and can be used in various applications including:

* Computational geometry for spatial queries.
* Dynamic data analysis where ranges of data are frequently queried and updated.
* Game development for efficient collision detection and world state management.

## Customization
The library is template-based, allowing for customization with different numeric types (int, float, long, etc.). Users can extend its functionality by implementing additional query and update methods based on specific needs.

## License
Ensure compliance with the library's licensing terms if used within both open-source and proprietary projects.


# Stack Library for Arduino

## Overview
The Stack library for Arduino provides a classic stack data structure implementation, enabling efficient Last-In-First-Out (LIFO) operations. It's suitable for a wide range of applications, from managing function calls to parsing expressions and beyond.

## Features
* Dynamic Resizing: Automatically adjusts its capacity based on the number of elements, ensuring efficient use of memory.
* Generic Implementation: Templated class allows for storing any data type.
* Core Operations: Supports standard stack operations such as push, pop, and peek.
* Utility Functions: Includes methods to check if the stack is empty or full, count the elements, and print the stack contents.

## Usage
To utilize the Stack library in your Arduino sketches, include it at the top of your source file:

```cpp

#include "Stack.h"
```

## Instantiating a Stack
Create a stack object capable of holding elements of any specified type:

```cpp

Stack<int> myStack;
```
## Pushing Elements
Add elements to the stack using the push method:

```cpp

myStack.push(10);
myStack.push(20);
```

## Popping Elements
Remove and retrieve the top element from the stack with pop:

```cpp

int element = myStack.pop();
```
## Peeking at the Top Element
Access the top element without removing it using peek:

```cpp

int topElement = myStack.peek();
```
## Checking Stack Status
Determine if the stack is empty or full:

```cpp

bool empty = myStack.isEmpty();
bool full = myStack.isFull();
```

## Counting Elements
Get the number of elements in the stack:

```cpp

uint16_t count = myStack.count();
```
## Clearing the Stack
Reset all elements in the stack to their default value:

```cpp

myStack.clear();
```
## Printing Stack Contents
For debugging purposes, print the contents of the stack:

```cpp

myStack.print();
```
## Applications
Stacks are fundamental in many areas of computer science and programming, including:

* Algorithmic expression evaluation and syntax parsing.
* Undo mechanisms in applications.
* Memory management and function call management in programming languages.
* Customization and Extensions
* The template-based implementation allows for easy customization to accommodate various data types. Further extensions could include implementing additional functionalities like iterator support or integrating with other data structures for complex data management.

## License
Please adhere to the licensing terms of the library when incorporating it into your projects, whether open-source or proprietary.


# Suffix Tree Library for Arduino

## Overview
The Suffix Tree library provides an efficient data structure for quick pattern matching and substring operations within a given text. It's particularly useful in applications involving text analysis, such as finding the longest repeated substring, pattern searching, and more.

## Features
* Pattern Search: Quickly search for the presence of a pattern within the text.
* Longest Repeated Substring: Find the longest repeated substring within the text, useful for data compression algorithms and detecting repeated patterns.
* Longest Common Substring: Determine the longest common substring between two texts, a common problem in text analysis and bioinformatics.
* Support for char and String Types: The library supports both char and String types for versatile applications.
* Dynamic Tree Construction: Builds the suffix tree dynamically as the text is processed, allowing efficient insertion and search operations.

## Usage
To use the Suffix Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "SuffixTree.h"
```

## Creating a Suffix Tree
Instantiate a Suffix Tree with a given text:

```cpp

String myText = "banana";
SuffixTree mySuffixTree(myText);
``` 

## Searching for a Pattern
Check if a pattern exists within the text:

```cpp

bool found = mySuffixTree.patternSearch("ana");
Serial.println(found ? "Pattern found" : "Pattern not found");
```

## Finding the Longest Repeated Substring
Get the longest repeated substring in the text:

```cpp

mySuffixTree.getLongestRepeatedSubstring();
```

## Longest Common Substring
To find the longest common substring between the original text and another string:

```cpp

String otherText = "panama";
String lcs = mySuffixTree.longestCommonSubstring(otherText);
Serial.println("Longest Common Substring: " + lcs);
```

## Palindrome Detection
Detect the longest palindrome within the text:

```cpp

String palindrome = mySuffixTree.longestPalindrome();
Serial.println("Longest Palindrome: " + palindrome);
```
## Applications
* Bioinformatics: Searching for motifs or repeated sequences in DNA or protein sequences.
* Text Editing: Implementing features like autocomplete, spell checking, and find/replace operations.
* Data Compression: Identifying repeated sequences that can be compressed.
* Forensics: Analyzing textual data for patterns or hidden messages.

## Customization
* Extend the library to include additional text analysis functionalities, such as different variations of pattern search or substring enumeration.
* Adapt the library for more complex data types or to optimize for specific applications, such as large-scale text processing or real-time analysis.

## License
Ensure to comply with the library's licensing terms when integrating it into your projects, respecting open-source contributions and proprietary restrictions where applicable.


# Trie Tree Library for Arduino

## Overview
The Trie Tree library is a specialized data structure for handling a dynamic set or associative array where the keys are strings. It excels in solving problems related to word validations, auto-completion, and prefix searching, making it ideal for applications that deal with large datasets of words or characters, such as dictionaries, word games, or text processing tools.

## Features
* Word Insertion: Efficiently inserts words into the trie for later retrieval or matching.
* Word Search: Quickly determines if a word is present in the trie.
* Prefix Search: Checks whether there is any word in the trie that starts with the given prefix, useful for auto-completion features.
* Print All Words: Traverses the trie and prints all stored words, aiding in debugging or visualization of the trie's contents.
* AutoComplete: Given a prefix, the library can suggest all words that complete the prefix, simulating an auto-complete feature.
* Clear Trie: Allows for resetting the trie, removing all inserted words to start afresh.
* Erase Word: Removes a specific word from the trie, if present.

## Usage
To use the Trie Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "TrieTree.h"
```

## Creating a Trie Tree
Instantiate a Trie Tree object:

```cpp

TrieTree myTrie;
```

## Inserting Words
Add words to the trie:

```cpp

myTrie.insert("hello");
myTrie.insert("world");
```
## Searching for Words
Check if a word exists in the trie:

```cpp

if(myTrie.search("hello")) {
    Serial.println("Found 'hello'");
}
```
## Prefix Search
Verify if any word in the trie starts with a given prefix:

```cpp

if(myTrie.startsWith("he")) {
    Serial.println("Prefix 'he' found");
}
```
## Printing All Words
Display all words stored in the trie:

```cpp

myTrie.printAllWords();
```

## Auto-Completion
Suggest words based on a prefix:

```cpp

SimpleVector<String> suggestions = myTrie.autoComplete("wo");
for(String word : suggestions) {
    Serial.println(word);
}
```

## Applications
* Text Editors: Implementing features like spell-checking and auto-completion.
* Games: Creating word games or puzzles that require fast validation of word existence or generation of possible word options.
* Educational Software: Assisting in teaching languages, vocabulary, and spelling.
* Search Engines: Quick prefix-based search suggestions.
* Data Analysis: Processing and analyzing large volumes of textual data for patterns or trends.

## Customization
The library can be extended or customized to include more complex functionalities such as handling different languages, case sensitivity options, or integrating with external storage for handling extremely large datasets.

## License
Make sure to comply with the library's license and respect the contributions of the open-source community when incorporating it into your projects.






# STATUS MEANINGS:
* UNTESTED: Simply means the Author has not Tested the code on an Arduino. (Author may have tested to make sure project builds)
* PARTIALLY-TESTED: Some Functions in the Library have been tested
* MOSTLY-TESTED: Most of the Functions in the Library have been tested. The library may be considered stable at this point.
* TESTED: The library has been basically tested. All functions have passed testing in one way or another. The library will be considered Stable at this point
* TESTING: Working on Testing the library with basic use cases.
* TESTING-NOT-NECESSARY: Exclusively for Virtual classes. (Iterator, Dictionary)
* ACTIVE-DEVELOPMENT: Actively Building the library. Things are Extremely likely to change as things get built. The library may still be considered stable if it has been tested at all. However it is advised to use at your own risk!
* PASSIVE-DEVELOPMENT: Library is usually tested at this point. May add features or modify a few things, especially if bugs are found. Mostly the readme files may be updated if they have not already been updated.
* STABLE: Means the library should be functioning as intended.
* UNSTABLE: Library May have issues.
* SEMI-STABLE: Library may have a few issues but overall should operate as intended
* UNPUBLISHED: Means the library has not been published on PlatformIO. This is reseverd for New Libraries that are Still being developed.
* ON-PAR: Arduino Library Manager Version and Platformio Versions are the same

