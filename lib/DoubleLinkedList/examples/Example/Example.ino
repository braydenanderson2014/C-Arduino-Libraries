/*
    Example Name: Example For DoubleLinkedList Library

    Basic example for the DoubleLinkedList library. This example demonstrates the basic functionality of the DoubleLinkedList library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/18/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <DoubleLinkedList.h>

// Create a DoubleLinkedList of integers
DoubleLinkedList<int> myList; // Create a DoubleLinkedList of integers (default size is 10, can be changed in the constructor), or use DoubleLinkedList<int> myList(10) to specify the size

void setup() {
    Serial.begin(9600); // Initialize serial communication

    // Add elements to the list
    Serial.println("Adding elements...");
    for (int i = 0; i < 5; i++) {
        myList.append(i * 10); // Add an element to the list with value i * 10 (i x 10)
    }

    // Insert an element
    Serial.println("Inserting element at index 2...");
    myList.insert(2, 25); // Insert an element with value 25 at index 2

    // Remove an element
    Serial.println("Removing element at index 3...");
    myList.remove(3); // Remove the element at index 3

    // Check if list contains an element
    if (myList.contains(20)) { // Check if the list contains the element with value 20
        Serial.println("List contains 20");
    } else {
        Serial.println("List does not contain 20");
    }

    // Print all elements in the list
    Serial.println("Current List:");
    for (int i = 0; i < myList.size(); i++) {
        Serial.println(myList.getElement(i)); // GetElement Returns the element at index i
        Serial.println(*myList.get(i)); // Get Returns a pointer to the element at index i
    }
    // Clear the list
    Serial.println("Clearing the list..."); 
    myList.clear();

    // Check if the list is empty
    if (myList.isEmpty()) {
        Serial.println("List is empty");
    }
}