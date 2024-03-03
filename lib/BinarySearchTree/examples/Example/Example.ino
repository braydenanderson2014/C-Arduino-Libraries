/*
    Example Name: Example For BinarySearchTree Library

    Basic example for the BinarySearchTree library. This example demonstrates the basic functionality of the BinarySearchTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <Arduino.h>
#include <binarySearchTree.h>

// Create a new instance of the BinarySearchTree class
BinarySearchTree<int> tree;

void setup(){
    Serial.begin(9600); // Start the serial communication

// Insert some values
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);

    // Search for a value
    BinarySearchNode<int>* node = tree.search(30);
    if (node != NULL) {
        Serial.println("Found node with value: " + node->data);
    } else {
        Serial.println("Node not found");
    }

    // Delete a value
    tree.deleteNode(30);

    // Search for the value again
    node = tree.search(30);
    if (node != NULL) {
        Serial.println("Found node with value: " + node->data);
    } else {
        Serial.println("Node not found");
    }

    tree.clear(); // Clear the tree
}

void loop(){

}