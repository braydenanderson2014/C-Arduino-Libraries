/*
    Example Name: Example2 For Hashtable Library

    Basic example for the Hashtable library. This example demonstrates the basic functionality of the Hashtable library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/18/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <Hashtable.h>

// Custom class for values
class Person { // Create a custom class for the values in the hashtable
public:
    String name; // Name of the person
    int age; // Age of the person

    Person(const String& n, int a) : name(n), age(a) {} // Constructor for the Person class
};

void setup() {
    Serial.begin(9600);

    Hashtable<String, Person> people; // Create a hashtable with keys of type String and values of type Person

    // Adding custom objects to the hashtable
    people.put("Alice", Person("Alice", 30)); // Add a Person object with name "Alice" and age 30 to the hashtable
    people.put("Bob", Person("Bob", 25)); // Add a Person object with name "Bob" and age 25 to the hashtable
    people.put("Charlie", Person("Charlie", 40)); // Add a Person object with name "Charlie" and age 40 to the hashtable

    // Retrieving custom objects by key
    Person alice = people.getElement("Alice"); // Get the Person object associated with the key "Alice"
    Person bob = people.getElement("Bob"); // Get the Person object associated with the key "Bob"

    Serial.print("Alice: ");
    Serial.print(alice.name); // Print the name of the person
    Serial.print(", Age: ");
    Serial.println(alice.age); // Print the age of the person

    Serial.print("Bob: ");
    Serial.print(bob.name); // Print the name of the person
    Serial.print(", Age: ");
    Serial.println(bob.age); // Print the age of the person

    // Iterating through keys
    SimpleVector<String> keys = people.keys(); // Get a vector of all the keys in the hashtable
    for (const String& key : keys) { // Iterate through the keys, using a range-based for loop... String& is used to avoid copying the key
        Person person = people.getElement(key); // Get the Person object associated with the key
        Serial.print("Key: ");
        Serial.print(key); // Print the key
        Serial.print(", Name: ");
        Serial.print(person.name); // Print the name of the person
        Serial.print(", Age: ");
        Serial.println(person.age);     // Print the age of the person
    }
}

void loop() {
    // Your Arduino code that runs repeatedly goes here
}
