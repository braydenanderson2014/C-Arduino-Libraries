/*
    Example Name: Example For SimpleVector Library

    Basic example for the SimpleVector library. This example demonstrates the basic functionality of the SimpleVector library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/18/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <SimpleVector.h>
#include <Arduino.h>

void setup() {
    Serial.begin(9600); // Initialize serial communication

    // Create a SimpleVector of integers
    SimpleVector<int> numbers;  // Create a SimpleVector of integers 

    // Add some numbers to the vector
    for(int i = 0; i < 10; i++) {
        numbers.put(i); // Add an element to the vector with value i
    }
    
    // Access and print the elements using the [] operator
    Serial.println("Elements in the vector:");
    for (unsigned int i = 0; i < numbers.size(); i++) { // Iterate through the vector using the [] operator
        Serial.print(numbers.get(i)); // Print the value
        Serial.print(" ");
    }
    Serial.println();

    // Iterate through the vector using an iterator
    SimpleVector<int>::SimpleVectorIterator iter = numbers.begin(); // Create an iterator for the vector
    Serial.println("Elements using iterator:");
    while (iter.hasNext()) { // Check if there are more elements in the vector
        int value = iter.next(); // Get the next element from the vector
        Serial.print(value); // Print the value
        Serial.print(" ");
    }
    Serial.println();

    numbers.remove(3); // Remove the element at index 3
    numbers.releaseMemory(); // Release the memory used by the vector
}

void loop() {
    // Your Arduino code that runs repeatedly goes here
}
