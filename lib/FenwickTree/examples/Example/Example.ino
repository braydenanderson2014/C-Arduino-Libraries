/*
    Example Name: Example For Fenwick Library

    Basic example for the Fenwick library. This example demonstrates the basic functionality of the Fenwick library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <FenwickTree.h>

FenwickTree<int> tree(5); // Create a Fenwick Tree with 5 elements

void setup()
{
    Serial.begin(9600); // initialize serial communication at 9600 bits per second
    tree.add(0, 1); // Add 1 to the 0th index
    tree.add(1, 2); // Add 2 to the 1st index
    tree.add(2, 3); // Add 3 to the 2nd index
    tree.add(3, 4); // Add 4 to the 3rd index
    tree.add(4, 5); // Add 5 to the 4th index
    Serial.println(tree.sum(0, 4)); // 15 (sum of all elements in the index range [0, 4])
    tree.scale(2); // Multiply all elements by 2
    Serial.println(tree.sum(0, 4)); // 30 (sum of all elements in the index range [0, 4])
    tree.add(3, 1);             // Add 1 to the 3rd index
    Serial.println(tree.sum(0, 4)); // 31 (sum of all elements in the index range [0, 4])
    Serial.println(tree.find(10));  // 4 (index of the element that has a cumulative sum of 10)

    tree.remove(3, 1); // Remove 1 from the 3rd index
    Serial.println(tree.sum(0, 4)); // 30 (sum of all elements in the index range [0, 4])
    Serial.println(tree.find(10));  // 4 (index of the element that has a cumulative sum of 10)
    tree.clear(); // Clear the Fenwick Tree
    Serial.println(tree.empty()); // 1 (Fenwick Tree is empty
}

void loop() {
    // put your main code here, to run repeatedly:
}

