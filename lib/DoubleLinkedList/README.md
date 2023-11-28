# DoubleLinkedList for Arduino

A simple, templated linked list implementation for Arduino projects. This class allows you to create and manage a linked list of any data type.

## Features

- Templated class: Can store any data type.
- Dynamic resizing: Automatically manages memory as elements are added or removed.
- Basic operations: `append`, `prepend`, `insert`, `remove`, `get`, `contains`, `getSize`, `isEmpty`, `clear`.

## WARNING: This Library Utilizes POINTERS *. This is due to the Libraries ability to utilize any return type. (Bool, String, int, float, etc)


## Installation

To use `DoubleLinkedList` in your Arduino sketch:

1. Copy the `DoubleLinkedList` class code to your project's directory.
2. Include `DoubleLinkedList.h` at the top of your sketch.

## Change Log
### Version 1.0.0
* Initial Release
* Added Library name in the Serial Outputs for debugging purposes
* This particular version is not tested and may not work as intended. USE AT YOUR OWN RISK!",
* This Library is extremely similar to the LinkedList Library, but with a few changes to the code to make it a double linked list instead of a single linked list.
### Version 1.0.1
* "Added a boolean to the Constructor that determines if the Library should print the Debug Messages to the Serial Monitor or not. This is useful for when you want to use the Serial Monitor for other things and don't want the Debug Messages to interfere with the other things you are doing.
* Added a new function called setDebugMode() that allows you to change the Debug Mode after the Library has been initialized. This is useful for when you want to use the Serial Monitor for other things and don't want the Debug Messages to interfere with the other things you are doing.
* Added a new function called getDebugMode() that allows you to get the current Debug Mode of the Library. This is useful for when you want to use the Serial Monitor for other things and don't want the Debug Messages to interfere with the other things you are doing.
* By default, the Debug Mode is set to false. This means that the Debug Messages will not be printed to the Serial Monitor if you do not pass the boolean in. This is useful for when you want to use the Serial Monitor for other things and don't want the Debug Messages to interfere with the other things you are doing.
            


## Usage

To create a linked list, simply declare an instance of `DoubleLinkedList` with the desired type:

```cpp
#include <DoubleLinkedList.h>

DoubleLinkedList<int> myList;
DoubleLinkedList<int> debugList(true);
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
