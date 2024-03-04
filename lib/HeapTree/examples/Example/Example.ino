/*
    Example Name: Example For HeapTree Library

    Basic example for the HeapTree library. This example demonstrates the basic functionality of the HeapTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <HeapTree.h>

HeapTree<int> heap; // Create a Heap Tree


void setup() {
    Serial.begin(9600);
  
  // Inserting values into the heap
  heap.insert(10);
  heap.insert(20);
  heap.insert(30);
  heap.insert(40);
  heap.insert(50);
  
  // Extracting the maximum value
  Serial.print("Maximum value: ");
  Serial.println(heap.extractMax());

  // Extracting the next maximum value
  Serial.print("Next maximum value: ");
  Serial.println(heap.extractMax());

    // Extracting the minimum value
  Serial.print("Minimum value: ");
  Serial.println(heap.extractMin());

  heap.removeItem(10); // Remove 10 from the heap
  heap.remove(1); // Remove the element at index 1

  heap.print(); // Print the heap
}
void loop() {
    // put your main code here, to run repeatedly:
}

