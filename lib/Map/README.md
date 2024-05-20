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
# Arduino Library Manager
## ChangeLog
### Version 1.0.0-ALPHA:
* Initial Release
### Version 1.0.0-BETA:
* Fixed an issue with the [] override operator. It would not properly count the amount of items.
* Fixed the Insert Function. The original version was no different then the add and put function. It now properly inserts an item at the specified index.
* Added new removeAtIndex() function to allow you to remove an item at an index.
* Added new getIndex() function that allows you to get an index of a certain key value pair by feeding it the key.
* Added Function Comments

# PlatformIO Library Manager
## ChangeLog
### Version 1.0.0-ALPHA:
* Initial Release
### Version 1.0.0-BETA:
* Fixed an issue with the [] override operator. It would not properly count the amount of items.
* Fixed the Insert Function. The original version was no different then the add and put function. It now properly inserts an item at the specified index.
* Added new removeAtIndex() function to allow you to remove an item at an index.
* Added new getIndex() function that allows you to get an index of a certain key value pair by feeding it the key.
* Added Function Comments


## Contribution
Contributions to the Map library are welcome. Whether you're fixing bugs, adding new features, or improving the documentation, your help will enhance this library for the Arduino community.

## License
The Map library is open-source and freely available for personal and commercial use under the terms of the license provided with the library source.