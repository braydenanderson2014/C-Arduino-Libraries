# LinkedList for Arduino

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
