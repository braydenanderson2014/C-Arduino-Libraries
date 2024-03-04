# Red-Black Tree Library for Arduino

## Overview
The Red-Black Tree library provides a balanced binary search tree implementation, ensuring O(log n) time complexity for insertions, deletions, and lookups. It is particularly well-suited for datasets that undergo frequent modifications, as it maintains balance through tree rotations and color flips.

## Key Features
* Self-Balancing: Automatically maintains its balance during insertions and deletions, ensuring optimal path lengths.
* Generic Data Storage: Capable of storing any comparable data type, making it versatile for various applications.
* Efficient Operations: Offers efficient search, insertion, and deletion operations.
* Traversal Methods: Supports in-order, pre-order, and post-order traversals for flexible data access.

## Usage
To use the Red-Black Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "RedBlackTree.h"
```
## Creating a Tree
Instantiate a Red-Black Tree object specifying the data type:

```cpp

RedBlackTree<int> tree;
```
## Inserting Elements
Add elements to the tree using the insert method:

```cpp

tree.insert(5);
tree.insert(3);
tree.insert(10);
```
## Deleting Elements
Remove elements from the tree with deleteNode:

```cpp

tree.deleteNode(3);
```
## Searching for Elements
### Search for an element in the tree:

```cpp

RedBlackNode<int>* node = tree.search(5);
if (node != NULL) {
    Serial.println("Found");
} else {
    Serial.println("Not found");
}
```
## Traversals
Perform different tree traversals:

```cpp

// In-order traversal
tree.inOrder();

// Pre-order traversal
tree.preOrder();

// Post-order traversal
tree.postOrder();
```
## Clearing the Tree
Remove all elements from the tree:

```cpp

tree.clear();
```
## Applications
Red-Black Trees are useful in many scenarios where a balanced search tree is required, such as:

* Maintaining a sorted stream of data.
* Implementing associative arrays.
* Building priority queues with efficient updates.
* Handling datasets with frequent insertions and deletions.

## Customization
While the library is designed to be generic and broadly applicable, specific use cases might require adjustments, such as custom comparison functions for complex data types. Modify the library source to accommodate such needs.

## License
This library is released for public use. Depending on the project's specifics, ensure compliance with the licensing terms if included in both open-source and proprietary projects.