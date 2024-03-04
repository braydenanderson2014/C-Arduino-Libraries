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