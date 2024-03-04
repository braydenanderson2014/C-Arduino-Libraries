# Interval Tree Library for Arduino

## Overview
The Interval Tree library for Arduino offers an efficient way to store intervals and perform fast search operations to find overlapping intervals. This library is particularly useful in applications requiring management of multiple time intervals or ranges, such as scheduling tasks, managing resources, or collision detection in spatial data.

## Features
* Generic Implementation: Works with any arithmetic type (e.g., int, float) for interval boundaries.
* Overlap Checking: Provides a method to check if a given interval overlaps with any interval in the tree.
* Insertion and Deletion: Supports inserting new intervals and deleting existing intervals from the tree.
* Traversal: Enables inorder traversal of the tree to access stored intervals in a sorted manner.
* Balance Maintenance: Automatically maintains the balance of the tree for optimal search performance.

## Usage
### Setup
Before using the Interval Tree library, include it in your Arduino sketch:

```cpp

#include "INTERVAL_TREE_h"
```
### Creating an Interval Tree
Instantiate an Interval Tree object with the desired type for the interval boundaries:

```cpp
IntervalTree<int> myTree;
```
### Inserting Intervals
Insert intervals into the tree using the insert() method:

```cpp
Interval<int> myInterval = {low: 10, high: 20};
myTree.insert(myInterval);
```
### Searching for Overlapping Intervals
Search for any interval overlapping with a given interval using the search() method:

```cpp

Interval<int> searchInterval = {low: 15, high: 25};
IntervalTreeNode<int>* result = myTree.search(searchInterval);
if (result != nullptr) {
    Serial.println("Overlap found.");
}
```

### Deleting Intervals
Delete an interval from the tree using the deleteNode() method:

```cpp

myTree.deleteNode(myInterval);
```
### Traversing the Tree
Perform an inorder traversal of the tree:

```cpp

myTree.inorder(); // This will print the intervals in sorted order
```
## Example
```cpp

void setup() {
  Serial.begin(9600);
  IntervalTree<int> tree;
  tree.insert({5, 10});
  tree.insert({15, 20});
  tree.insert({25, 30});
  tree.insert({10, 15}); // Overlaps with {5, 10} and {15, 20}

  Serial.println("Inorder traversal of the Interval Tree:");
  tree.inorder();

  Interval<int> searchInt = {14, 16};
  auto result = tree.search(searchInt);
  if (result != nullptr) {
    Serial.print("An overlapping interval for ");
    Serial.print("[" + String(searchInt.low) + ", " + String(searchInt.high) + "] ");
    Serial.println("was found.");
  } else {
    Serial.println("No overlapping interval found.");
  }
}

void loop() {
  // Put your main code here, to run repeatedly:
}
```

## Contribution
Contributions to the Interval Tree library are welcome. Whether it's extending functionality, improving efficiency, or enhancing documentation, your input can help make this library more useful for the Arduino community.

## License
This library is released under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.