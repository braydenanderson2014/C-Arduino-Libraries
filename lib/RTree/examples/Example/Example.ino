/*
    Example Name: Example For RTree Library

    Basic example for the RTree library. This example demonstrates the basic functionality of the RTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <RTree.h>

// Instantiate RTree for rectangles with integer coordinates
RTree<int> myRTree;

void setup() {
    Serial.begin(9600); // Start the serial communication

    // Wait for the serial port to connect. Needed for native USB port only
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // Define and insert rectangles into the RTree
    Rectangle<int> rect1 = {1, 1, 2, 2}; // Rectangle with coordinates (1, 1) and (2, 2)
    Rectangle<int> rect2 = {3, 3, 4, 4}; // Rectangle with coordinates (3, 3) and (4, 4)
    Rectangle<int> rect3 = {5, 5, 6, 6}; // Rectangle with coordinates (5, 5) and (6, 6)
    Rectangle<int> rect4 = {1, 3, 2, 4}; // Rectangle with coordinates (1, 3) and (2, 4)

    myRTree.insert(rect1); // Insert the rectangles into the RTree
    myRTree.insert(rect2); // Insert the rectangles into the RTree
    myRTree.insert(rect3); // Insert the rectangles into the RTree 
    myRTree.insert(rect4); // Insert the rectangles into the RTree

    // Print the RTree in different traversal orders
    Serial.println("RTree in Pre-Order Traversal:");
    myRTree.print(PRE_ORDER);

    Serial.println("RTree in In-Order Traversal:");
    myRTree.print(IN_ORDER);

    Serial.println("RTree in Post-Order Traversal:");
    myRTree.print(POST_ORDER);

    Serial.println("RTree in Level-Order Traversal:");
    myRTree.print(LEVEL_ORDER);

    // Search for a rectangle in the RTree
    bool found = myRTree.search(rect2);

    if (found) {
        Serial.println("Rectangle found in RTree!");
    } else {
        Serial.println("Rectangle not found in RTree.");
    }

    myRTree.remove(rect2); // Remove a rectangle from the RTree


    // Clear the RTree
    myRTree.clear();
}

void loop() {
    // Not used in this example
}