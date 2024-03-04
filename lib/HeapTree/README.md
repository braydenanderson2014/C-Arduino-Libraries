
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