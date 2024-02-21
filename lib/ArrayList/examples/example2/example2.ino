/*
    Example Name: Example2 For ArrayList Library

    Basic example for the ArrayList library. This example demonstrates the basic functionality of the ArrayList library.

    The circuit:
    This Library does not Require any Circuit.

    Created  month day year   // 02/17/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/17/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <ArrayList.h>

// Create an ArrayList of integers
ArrayList<String> myList(ArrayList<String>::FIXED); // Create an ArrayList of String with List being a Fixed Size (default size is 10, can be changed in the constructor), or use ArrayList<int> myList(10) to specify the size
/*
ArrayList<int> myList(ArrayList<int>::FIXED); // Create an ArrayList of integers with a fixed size (default size is 10, can be changed in the constructor), or use ArrayList<int> myList(ArrayList<int>::FIXED, 10) to specify the size
ArrayList<int> myList(ArrayList<int>::DYNAMIC); // Create an ArrayList of integers with a dynamic size (default size is 10, can be changed in the constructor), or use ArrayList<int> myList(ArrayList<int>::DYNAMIC, 10) to specify the size

*/

void setup() {
    Serial.begin(9600);

    // Add elements to the list
    Serial.println("Adding elements...");
    for (int i = 0; i < 5; i++) {
        myList.add(String(i * 10)); // Add an element to the list with value i * 10 (i x 10)
    }

    // Insert an element
    Serial.println("Inserting element at index 2...");
    myList.insert(2, String(25)); // Insert an element with value 25 at index 2 (Converting int to String using String() constructor since the ArrayList is of type String)

    // Remove an element
    Serial.println("Removing element at index 1...");
    myList.remove(1); // Remove the element at index 1

    // Check if list contains an element
    if (myList.contains(String(20))) { // Check if the list contains the element with value 20 (Converting int to String using String() constructor since the ArrayList is of type String) Could also use myList.contains("20") 
        Serial.println("List contains 20");
    } else {
        Serial.println("List does not contain 20");
    }

    // Print all elements in the list
    Serial.println("Current List:");
    for (int i = 0; i < myList.size(); i++) {
        Serial.println(String(myList.get(i))); // Converting int to String using String() constructor since the ArrayList is of type String
        // Serial.println(myList.get(i)); // No need to convert to String if the ArrayList is of type int
        //myList.get(i) returns the element at index i
    }

    // Clear the list
    Serial.println("Clearing the list...");
    myList.clear(); // Clear the list

    // Check if the list is empty
    if (myList.isEmpty()) {
        Serial.println("List is empty");
    }
}

void loop() {
    // Your loop code here
}
