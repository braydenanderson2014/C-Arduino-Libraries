# AVL Tree Library for Arduino
## Overview
This library provides an implementation of an AVL Tree, a self-balancing binary search tree, for use in Arduino projects. The AVL Tree maintains a balance factor for each node to ensure that the tree remains approximately balanced at all times, offering O(log n) complexity for insertion, deletion, and lookup operations.

## Features
* Insertion: Add elements to the tree while maintaining its balanced nature.
* Deletion: Remove elements from the tree, automatically rebalancing as necessary.
* Search: Efficiently find elements within the tree.
* Traversal: Supports in-order, pre-order, and post-order traversals to iterate over elements in the tree.
* Min/Max: Quickly retrieve the minimum or maximum value contained in the tree.
* Height and Balance Information: Access the height of the tree and the balance factor of any node to understand the tree's structure.
* Printing: Utility functions for printing the tree structure to aid in debugging.

## Usage
To use this library in your Arduino project, include the AVLTree.h header file and follow the example below:

```cpp
#include "AVLTree.h"

AVLTree<int> myTree;

void setup() {
  Serial.begin(9600);
  
  // Insert elements into the AVL Tree
  myTree.insert(10);
  myTree.insert(20);
  myTree.insert(30);
  myTree.insert(5);
  myTree.insert(3);

  // Perform in-order traversal of the tree
  Serial.println("In-order Traversal:");
  myTree.inOrder();

  // Find the minimum and maximum elements
  Serial.print("Minimum: ");
  Serial.println(myTree.findMin());
  Serial.print("Maximum: ");
  Serial.println(myTree.findMax());

  // Delete an element
  myTree.deleteNode(10);
  Serial.println("After deleting 10:");
  myTree.inOrder();
}

void loop() {
  // Tree operations can be invoked here or in setup()
}
```

## Implementation Details
The AVL Tree implementation automatically handles balancing through rotations during insertions and deletions, ensuring that the tree remains as balanced as possible without requiring manual intervention.

## Functions
* ```cpp void insert(T data)```: Inserts a new element into the tree.
* ```void remove(T data)```: Removes an element from the tree.
* ```void inOrder()```, void preOrder(), void postOrder(): Perform tree traversals.
* ```T findMin(), T findMax()```: Retrieve the minimum and maximum elements of the tree.
* ```void printTree()```: Prints a visual representation of the tree.
* ```void clear()```: Clears the tree, removing all elements.
* ```bool isEmpty()```: Checks if the tree is empty.

## Customization
The library is templated to allow for the storage of any data type that supports comparison operations (i.e., has operator< and operator> defined). This makes it suitable for a wide range of applications, from storing simple integers to complex objects, as long as they provide the necessary comparison operators.

## Contribution
Contributions to the library are welcome, whether they are for bug fixes, improvements, or new features. Please follow standard coding conventions and add comments to your code to ensure readability and maintainability.

## License
This library is released under the Apache License. See the LICENSE file for more details.


# Arduino Library Manager
## ChangeLog:
### Version 1.0.0-ALPHA
* Initial Untested Release... This Release has been test compiled but not tested.

# PlatformIO Registry
## ChangeLog:
### Version 1.0.0-ALPHA
* Initial Untested Release... This Release has been test compiled but not tested.
