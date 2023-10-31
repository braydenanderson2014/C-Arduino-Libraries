# SDList

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