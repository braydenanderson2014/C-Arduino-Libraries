/*
    Example Name: Example For LinkedList Library

    Basic example for the LinkedList library. This example demonstrates the basic functionality of the LinkedList library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/18/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <BasicLinkedList.h>
// Instantiate the list globally
LinkedList<int> myList; // Create a LinkedList of integers 

void setup() {
    Serial.begin(9600); // Initialize serial communication
    
    // Use the list
    myList.append(1); // Add an element to the list with value 1 (Append adds an element to the end of the list)
    myList.append(2); // Add an element to the list with value 2
    myList.prepend(0); // Add an element to the list with value 0 (Prepend adds an element to the beginning of the list)
    myList.insert(3, 3); // Insert an element with value 3 at index 3

    Serial.println("Size: " + String(myList.size())); // Print the size of the list

    for (size_t i = 0; i < myList.size(); i++) {
        Serial.println("Element at position " + String(i) + ": " + String(*myList.get(i))); // Print the element at index i using the get method (Get Returns a pointer to the element at index i)
        Serial.println("Element at position " + String(i) + ": " + String(myList.getElement(i))); // Print the element at index i using the getElement method (GetElement Returns the element at index i)
        Serial.println("Element at position " + String(i) + ": " + String(myList.getAsString(i))); // Print the element at index i as a string
    }

    if (myList.contains(2)) { // Check if the list contains the element with value 2
        Serial.println("List contains 2.");
    }

    myList.remove(2); // Remove the element at index 2
    myList.removeElement(3); // Remove the element with value 3
    myList.clear(); // Clear the list
}