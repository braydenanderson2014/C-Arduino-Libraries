/*
    Example Name: Example For kdimensionalTree Library

    Basic example for the kdimensionalTree library. This example demonstrates the basic functionality of the kdimensionalTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <kDimensionalTree.h>

// Create a kdimensionalTree object
KDimensionalTree<int> kdtree;

void setup() {
    // Create a point
    SimpleVector<int> point;
    point.push_back(1);
    point.push_back(2);
    point.push_back(3);
    point.push_back(4);
    point.push_back(5);

    // Insert the point into the kdimensionalTree
    kdtree.insert(point); // This will insert the point (SimpleVector<int> point) into the kdimensionalTree
    kdtree.insert(SimpleVector<int>({51, 62, 23, 84, 15})); // This will insert the point (SimpleVector<int>({51,62,23,84,15})) into the kdimensionalTree
    // Search for the point in the kdimensionalTree
    if(kdtree.search(point)){
        Serial.println("Point found");
    } else {
        Serial.println("Point not found");
    }

    bool found = kdtree.search(SimpleVector<int>({1, 2, 3, 4, 5})); // This will search for the point (SimpleVector<int>({1, 2, 3, 4, 5})) in the kdimensionalTree
    if(found){
        Serial.println("Point found");
    } else {
        Serial.println("Point not found");
    }
    // Remove the point from the kdimensionalTree
    kdtree.remove(point);

    kdtree.clear(); // This will clear the kdimensionalTree
}
