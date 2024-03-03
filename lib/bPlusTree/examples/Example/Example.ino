/*
    Example Name: Example For BinaryTree Library

    Basic example for the BinaryTree library. This example demonstrates the basic functionality of the Binary library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <bPlusTree.h>

// Create a new instance of the bPlusTree class
BPlusTree<int> tree(3); // A B+ Tree with a minimum degree of 3

//Insert the keys
void setup(){
    tree.insert(10); 
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);
    tree.insert(7);
    tree.insert(17);

    //Traverse the Tree
    tree.traverse(tree.getRoot());
    
    //Search for a key
    BPlusTreeNode<int>* result = tree.search(tree.getRoot(), 6); //Search for the key 6 in the tree. If the key is found, the function will return the node containing the key. If the key is not found, the function will return nullptr.
    if (result != nullptr) {
        Serial.println("Key found");
    } else {
        Serial.println("Key not found");
    }

    // Remove a key
    tree.remove(6);
    tree.traverse(tree.getRoot()); //Traverse the Tree (getRoot() returns the root of the tree
    result = tree.search(tree.getRoot(), 6);
    if (result != nullptr) {
        Serial.println("Key found");
    } else {
        Serial.println("Key not found");
    }
}

void loop(){

}
