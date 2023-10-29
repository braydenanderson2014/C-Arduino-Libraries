# C-Arduino-Libraries
This repository contains C++ and Arduino Libraries that are lightweight and easy to use. All libraries were created/mimicked by me and chatgpt. All Libraries have or will be published on Platformio. If you have any issues, please file a bug report. The libraries can be found under the lib folder. A big example can be found under the src folder. This repository is now a platformio project so you can see how you can configure a project in a way that allows you to use these libraries and test them on a platformio compatible GPIO (General Purpose Input Ouput). 


## List of All Available Libraries ##
1. Dictionary
2. Hashtable
3. iostream
4. iterator
5. Properties
6. SimpleVector
7. Timer

## ALL LIBRARIES INCLUDE A MORE INDEPTH README.md ##
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
- `size()`: Returns the number of key-value pairs in the hash table.
- `isEmpty()`: Checks if the hash table is empty.
- `keys()`: Returns an iterator for iterating over the keys in the hash table.
- `merge(const Hashtable<K, V>& other)`: Merges the contents of another hash table into the current one.

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