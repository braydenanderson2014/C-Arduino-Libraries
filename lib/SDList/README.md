# SDList

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
### Version 1.0.3 
* Update to README
* Completely Reimplemented this library, It now functions properly and is much more stable. (Only Lightly Tested)
* This Library now uses the ArrayList Library to store the list in memory, and then writes the list to the SD card when the list is modified (In SDLIST). This makes the library much more stable and reliable.
* This Library has not been Thoughly tested, and may not be stable. USE AT YOUR OWN RISK! If you find any bugs, please report them to the Author
* Removed the c++ file and moved implementation to the header file
### Version 1.0.4
* This Version was removed from PlatformIO due to a missing feature that was supposed to be added.
### Version 1.0.5 [BETA-TESTING]
* Library src Directory was not included in the previous release. This has been fixed.
* Re-Added Examples
* Added Overloaded begin() function to allow for custom file names. begin(csPin) was the original, and begin(csPin, Filename) is the new overloaded function.
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