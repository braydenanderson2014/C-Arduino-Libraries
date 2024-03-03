/*
    Example Name: Example For RedBlackTree Library

    Basic example for the RedBlackTree library. This example demonstrates the basic functionality of the RedBlackTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <RedBlackTree.h>

// Create a RedBlackTree object
RedBlackTree<int> rbtree;

void setup(){
    // Insert some values into the RedBlackTree
    rbtree.insert(10); // This will insert the value 10 into the RedBlackTree
    rbtree.insert(20); // This will insert the value 20 into the RedBlackTree
    rbtree.insert(30); // This will insert the value 30 into the RedBlackTree
    rbtree.insert(40); // This will insert the value 40 into the RedBlackTree

    // Search for a value in the RedBlackTree
    bool found = rbtree.search(20); // This will search for the value 20 in the RedBlackTree
    if (found) {
        Serial.println("Value found in RedBlackTree!");
    } else {
        Serial.println("Value not found in RedBlackTree.");
    }

    // You can also perform other operations like traversals, etc.
    rbtree.inOrder(); // This will perform an in-order traversal of the RedBlackTree

    rbtree.clear(); // This will clear the RedBlackTree
}