/*
    Example Name: Example For Hashtable Library

    Basic example for the Hashtable library. This example demonstrates the basic functionality of the Hashtable library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 01/07/2025
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <Arduino.h>
#include <Hashtable.h>

int main() {
    Hashtable<String, int> myHashtable; // Create a Hashtable with keys of type String and values of type int. You can declare Hashtable with any combination...<AnyType, AnyType>

    // Adding key-value pairs to the hashtable
    myHashtable.put("apple", 5);
    myHashtable.put("banana", 3);
    myHashtable.put("cherry", 8);

    // Retrieving values by key
    int appleCount = myHashtable.getElement("apple");
    int* bananaCount = myHashtable.get("banana");

    Serial.println("Apple count: " + appleCount);
    Serial.println("Banana count: " + *bananaCount);

    // Removing a key-value pair
    myHashtable.remove("cherry");

    // Checking if a key exists
    if (myHashtable.containsKey("cherry")) {
        Serial.println("Cherry exists in the hashtable.");
    } else {
        Serial.println("Cherry does not exist in the hashtable.");
    }

    // Iterating through keys
    SimpleVector<String> keys = myHashtable.keys(); // Get a vector of all the keys in the hashtable
    for (const String& key : keys) { // Iterate through the keys, using a range-based for loop... String& is used to avoid copying the key
        Serial.print("Key: ");
        Serial.print(key.c_str()); // c_str() returns a pointer to the underlying char array of the String
        Serial.print(", Value: ");
        Serial.println(myHashtable.getElement(key)); // Get the value associated with the key
        Serial.println(*myHashtable.get(key)); // Get the value associated with the key using a pointer
    }

    // Iterating through values
    SimpleVector<int> values = myHashtable.values(); // Get a vector of all the values in the hashtable
    for (int value : values) { // Iterate through the values, using a range-based for loop
        Serial.println("Value: " + value);
    }

    //Alternative way to iterate through the hashtable
    for (auto it = myHashtable.begin(); it != myHashtable.end(); ++it) {
        auto kv = *it;
        Serial.print("Key: ");
        Serial.print(kv.key);
        Serial.print(", Value: ");
        Serial.println(kv.value);
    }

    //debugging the hashtable iterator
    myHashtable.debugIterator();

    // Clearing the hashtable
    myHashtable.clear();


    return 0;
}
