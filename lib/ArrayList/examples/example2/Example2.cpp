#include "ArrayList.h"

// Create an ArrayList of integers
ArrayList<String> myList(ArrayList<String>::FIXED);

void setup() {
    Serial.begin(9600);

    // Add elements to the list
    Serial.println("Adding elements...");
    for (int i = 0; i < 5; i++) {
        myList.add(String(i * 10));
    }

    // Insert an element
    Serial.println("Inserting element at index 2...");
    myList.insert(2, String(25));

    // Remove an element
    Serial.println("Removing element at index 3...");
    myList.remove(3);

    // Check if list contains an element
    if (myList.contains(String(20))) {
        Serial.println("List contains 20");
    } else {
        Serial.println("List does not contain 20");
    }

    // Print all elements in the list
    Serial.println("Current List:");
    for (int i = 0; i < myList.size(); i++) {
        Serial.println(String(myList.get(i)));
    }

    // Clear the list
    Serial.println("Clearing the list...");
    myList.clear();

    // Check if the list is empty
    if (myList.isEmpty()) {
        Serial.println("List is empty");
    }
}

void loop() {
    // Your loop code here
}
