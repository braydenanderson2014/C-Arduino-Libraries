/*
    Example Name: Example For OCTree Library

    Basic example for the OCTree library. This example demonstrates the basic functionality of the OCTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <OCTree.h>

// Create an OCTree object
Octree<int> octree;

void setup(){
  // Insert some values into the octree. In a real application, these could be 3D coordinates or other spatial data.
  octree.insert(10);
  octree.insert(20);
  octree.insert(30);
  octree.insert(40);

  // Search for a value in the octree.
  bool found = octree.search(20);
  if (found) {
    Serial.println("Value found in Octree!");
  } else {
    Serial.println("Value not found in Octree.");
  }

  // You can also perform other operations like nearest neighbor search, traversals, etc.

  Serial.println(octree.nearestNeighbor(15)); // This will return the nearest neighbor to the value 15 in the octree

  octree.clear(); // This will clear the octree
}

void loop(){

}
