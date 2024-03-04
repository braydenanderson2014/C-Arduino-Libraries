/*
    Example Name: Example For ColorManager Library

    Basic example for the ColorManager library. This example demonstrates the basic functionality of the ColorManager library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <AVLTree.h>

// Create a new instance of the AVLTree class
AVLTree<int> tree;

void setup() {
  Serial.begin(9600); // Start the serial communication
  
  // Inserting values into the tree
  tree.insert(10); // Inserting the root node
  tree.insert(20); 
  tree.insert(30); 
  tree.insert(40); 
  tree.insert(50);
  
  // Printing the tree in-order
  Serial.println("In-order traversal:");
  tree.inOrder(); // In-order traversal
  
  // Printing the tree pre-order
  Serial.println("Pre-order traversal:");
  tree.preOrder(); // Pre-order traversal
  
  // Printing the tree post-order
  Serial.println("Post-order traversal:");
  tree.postOrder(); // Post-order traversal
  
  // Finding minimum and maximum
  Serial.print("Minimum: ");
  Serial.println(tree.findMin()); // Finding the minimum value
  Serial.print("Maximum: ");
  Serial.println(tree.findMax()); // Finding the maximum value
  
  // Deleting a node
  tree.deleteNode(30); // Deleting the node with value 30
  
  // Printing the tree in-order after deletion
  Serial.println("In-order traversal after deletion:");
  tree.inOrder(); // In-order traversal after deletion
  
  Serial.println(tree.getBalance()); // Getting the balance of the tree

  tree.clear(); // Clearing the tree
}

void loop() {
  // put your main code here, to run repeatedly:
}