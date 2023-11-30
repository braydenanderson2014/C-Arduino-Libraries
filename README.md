# C-Arduino-Libraries

## Please Ensure you start the Serial interface before using these libraries!!!
```cpp
    Serial.begin(9600);
```
```cpp
    Serial.begin(115400);
```
=======
## List of All Available Libraries ## [WARNING]: UNTESTED/Partially Tested BY AUTHOR
* [Dictionary](#dictionary-class) [PASSIVE-DEVELOPMENT] (PURE VIRTUAL : No Concrete Implementation in this Library), [TESTING-NOT-NECESSARY],[IMPLEMENTED IN ANOTHER LIBRARY]
* [Hashtable](#hashtable-class) [PASSIVE-DEVELOPMENT], [MOSTLY-TESTED], [STABLE]
* [iostream](#iostream-class) [PASSIVE-DEVELOPMENT] (Published as : BasicIOStream), [PARTIALLY-TESTED], [STABLE]
* [iterator](#iterator-class) [PASSIVE-DEVELOPMENT], (PURE VIRTUAL : No Concrete Implementation in this Library)  [TESTING-NOT-NECESSARY] 
* [Properties](#properties-class) [PASSIVE-DEVELOPMENT] (Published as : SimpleProperties), [MOSTLY-TESTED], [STABLE]
* [SimpleVector](#simplevector-class) [PASSIVE-DEVELOPMENT] , [MOSTLY-TESTED], [STABLE]
* [Timer](#timer-class) [PASSIVE_DEVELOPMENT] (Published as : SimpleArduinoTimer), [UNTESTED]
* [UnorderedMap](#unorderedmap-class) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [LinkedList](#linkedlist-class) [PASSIVE-DEVELOPMENT] (Published as : BasicLinkedList), [PARTIALLY-TESTED], [STABLE]
* [DoubleLinkedList](#doublelinkedlist-class) [ACTIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [MyDictionary](#mydictionary-class) [ACTIVE-DEVELOPMENT],(implemented Version of the Dictionary Class), [UNTESTED]
* [SDList](#sdlist-class) [ACTIVE-DEVELOPMENT], [FAILED-TESTING]
* [ArrayList](#arraylist-class) [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]
* [SimpleMutex]() [ACTIVE-DEVELOPMENT]
* [SimpleThreadManager]() [ACTIVE-DEVELOPMENT]
* [ArrayListOperations]() [ACTIVE-DEVELOPMENT], [SUBLIBRARY](SubLibrary of ArrayList)
SubLibraries: (1)
Total Available Libraries: (14)
Total Libraries: (16)

## ACTIVELY WORKING ON LISTED LIBRARIES (Bug Fixing or Feature Adding):
1. UnorderedMap [TESTING]
2. LinkedList [TESTING]
3. MyDictionary [TESTING]
4. SDList [ACTIVE-DEVELOPMENT], [BUG-FIXING], [TESTING]
5. ArrayList [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED], 
6. DoubleLinkedList [TESTING], [DOCUMENTATION-PENDING] (DOCUMENTATION AVAILABLE IN LIBRARY, JUST NOT THIS README FILE)
7.  [SimpleMutex]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
8.  [SimpleThreadManager]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]

=======
1. SDList [BUG-FIXING, ACTIVE-DEVELOPMENT, KNOWN-ISSUES], [FAILED-TESTING]


## UNPUBLISHED (In Development) (Not yet Documented)
========================================================== 
[WARNING]: NO DOCUMENTATION AVAILABLE YET &&|| UNTESTED BY AUTHOR
1.  [SimpleMutex]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]
2.  [SimpleThreadManager]() [ACTIVE-DEVELOPMENT], [UNPUBLISHED]

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

1. Copy `SDList.h` and `SDList.cpp` to your project's directory.
2. Include the `SDList.h` file in your sketch.

## Usage

To create an `SDList`, specify the data type you want to store and provide the Chip Select (CS) pin and a filename for the page file on the SD card.

```cpp
#include "SDList.h"

SDList<int> myList(10, "mydata.dat"); // CS pin is 10, page file name is "mydata.dat"
```
### Appending Elements
```cpp
myList.append(42);
```
### Accessing Elements
```cpp
int value = myList.get(0);
```
### Setting Elements
```cpp
myList.set(0, 100);
```
### Checking List Size
```cpp
uint16_t listSize = myList.size();
```
### SD Card Availability
```cpp
if (myList.sdAvailable()) {
    // SD card is ready for use
}
```
## Methods
* append: Add an element to the end of the list.
* get: Retrieve the element at a specified index.
* set: Set the value of the element at a specified index.
* size: Get the number of elements in the list.
* sdAvailable: Check if the SD card is available and ready for use.
* expandCapacity: (private) Expand the list's capacity when needed.
## Notes
The list starts with an initial capacity, which will be doubled each time it runs out of space.
If the SD card is not available, the list will function as a normal in-memory list without persistence.
Ensure the SD card is formatted correctly and that the Arduino has the necessary permissions to read from and write to the SD card.

## Dependencies
SDList depends on the SD library. Ensure you have included the SD library in your project.

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
* UNPUBLISHED: Means the library has not been published on PlatformIO. This is reseverd for New Libraries that are Still being developed.
