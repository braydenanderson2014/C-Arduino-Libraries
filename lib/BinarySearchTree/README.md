
# Binary Search Tree Library for Arduino

## Overview
This library provides a simple and efficient implementation of a Binary Search Tree (BST) for Arduino projects. BSTs are fundamental data structures that enable fast data storage, retrieval, and manipulation operations, all of which are essential in various computing applications.

## Features
* Insertion: Adds a new element to the BST while preserving the binary search property.
* Deletion: Removes an element from the BST and reorganizes the tree to maintain its properties.
* Search: Finds an element in the BST and returns a pointer to the node containing it.
* Clear: Removes all elements from the BST, freeing up the allocated memory.

## Usage
To use this library in your Arduino project, include the BinarySearchTree.h header file and instantiate a BinarySearchTree object with your desired data type. The following example demonstrates basic operations such as insertion, search, and deletion:

```cpp
#include "BinarySearchTree.h"

BinarySearchTree<int> bst;

void setup() {
  Serial.begin(9600);

  // Insert elements
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(2);
  bst.insert(4);

  // Search for an element
  BinarySearchNode<int>* found = bst.search(4);
  if (found != NULL) {
    Serial.print("Found: ");
    Serial.println(found->data);
  } else {
    Serial.println("Element not found.");
  }

  // Delete an element
  bst.deleteNode(3);

  // Clear the BST
  bst.clear();
}

void loop() {
  // Placeholder for the main logic
}

```
## Implementation Details
The BST is implemented using templates, allowing it to store any type of data that supports comparison operations (< and >).
Each node in the BST contains the data of type T, and pointers to the left and right child nodes.
The insert and deleteNode functions ensure that the BST properties are maintained.

## Customization
This library can be easily customized and extended to include additional functionalities such as in-order, pre-order, and post-order traversals, or balancing operations to transform it into a self-balancing binary search tree like an AVL tree or a Red-Black tree.

## Contribution
Contributions to the library are welcome. If you find a bug, have a suggestion for improvement, or want to add new features, please feel free to contribute.

## License
This library is open-sourced and licensed under the Apache License. See the LICENSE file for more information.