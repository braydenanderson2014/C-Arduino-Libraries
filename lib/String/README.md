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