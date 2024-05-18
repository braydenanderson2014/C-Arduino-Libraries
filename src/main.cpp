// #include <Arduino.h>
// // #include <CustomString.h>
// // #include <MemoryManager.h>
// // #include <MathLib.h>
// // #include <Numeric_Limits.h>
// // #include <LittleFSProperties.h>
// #include "SimpleArduinoTimer.h"
// #include "Map.h"
// #include "MathLib.h"
// #include "ArrayList.h"
// Map <String, int> ThisMap;
// Timer timer = new Timer(false);
// // SDList <String> strings(10, "pagefile.dat");
// ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
// void setup()
// {
//     /*
//     Custom_String::String str = "Hello, World!"; // Create a string object with the value "Hello, World!"
//     Serial.begin(9600); // Start the serial communication
//     Serial.println(str.C_STR()); // Hello, World!

//     str.append(" This is a test!"); // Append the string " This is a test!" to the end of the string
//     Serial.println(str.C_STR()); // Hello, World! This is a test!

//     Serial.println(str.Capacity()); // Strings capacity

//     int i = 0;
//     Serial.println(str.indexOf('o', i) + i); // 4

//     Custom_String::String str2 = "9999";
//     Serial.println(Random(str.toInt(), 5));

//     float f = 3.14159*str.toFloat(); // Converts String to float
//     Serial.println(f); // 9.42477

//     Serial.println(Round(f)*str.toInt()*str.toFloat()/2.25); // 18

//     */
//     Serial.begin(9600);
//     while (!Serial)
//         ;
//     /*
//     timer.setTargetMinutes(5); // Set the target duration to 5 minutes
//     ThisMap.put("Timer started at: ", millis()); // Put the start time of the timer in the map
//     unsigned long x = 1000;

    
//     timer.start(); // Start the timer
//     while(!timer.hasReachedTarget()){
//         if(timer.checkTimer(x)){ // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
//             Serial.println(String(timer.remainingTime())); //1 side effect of remainingTime() is that it will auto adjust based off of the time remaining. all without you knowing.
//             x += 10000;
//             ThisMap.put("Time remaining: " + x, timer.remainingTime()); // Put the remaining time in the map
//         }
//     }
//     Serial.println("Timer has reached target duration!");
//     ThisMap.put("Timer ended at: ", millis()); // Put the end time of the timer in the map
//     timer.reset(); // Reset the timer
//     x = 1000; //sets the variable x back to 1 second (THIS IS NOT NECESSARY, JUST FOR DEMONSTRATION PURPOSES)

//     Serial.println("Printing map: ");
//     ThisMap.print(); // Print the map

//     ThisMap.put("New Timer started at: ", millis()); // Put the start time of the timer in the map
//     timer.start();
//     while (!timer.hasReachedTarget())
//     {
//         if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
//         {
//             timer.printTimeRemaining(); // Prints the remaining time on the timer
//             x += 1000;
//         }
//     }

//     timer.setTargetSeconds(10); // Set the target duration to 10 seconds
//     timer.start(); // Start the timer
//     while (!timer.hasReachedTarget())
//     {
//         if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
//         {
//             timer.printTimeRemaining(); // Prints the remaining time on the timer
//             x += 1000;
//             ThisMap.put("Time remaining: " + x, timer.remainingTime()); // Put the remaining time in the map
//         }
//     }
//     Serial.println("Timer has reached target duration!");
//     ThisMap.put("New Timer ended at: ", millis()); // Put the end time of the timer in the map

//     Serial.println("Printing map: ");
//     ThisMap.print(); // Print the map


//     timer.setTargetHours(2); // Set the target duration to 1 hour
//     timer.start(); // Start the timer
//     while (!timer.hasReachedTarget())
//     {
//         if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
//         {
//             timer.printTimeRemaining(); // Prints the remaining time on the timer
//             x += 1000;
//         }
//     }
//     Serial.println("Timer has reached target duration!");

//     */
//     ThisMap["Hello"] = 1;
//     ThisMap["World"] = 2;
//     ThisMap["This"] = 3;
//     ThisMap["Is"] = 4;
//     ThisMap["A"] = 5;
//     ThisMap["Test"] = 6;
//     ThisMap["Of"] = 7;
//     ThisMap["The"] = 8;
//     ThisMap["Map"] = 9;
//     ThisMap["Class"] = 10;
//     delay(1000); // Delay for 1 second (1000 milliseconds
//     Serial.println("Printing map: ");
//     ThisMap.print(); // Print the map

//     ThisMap.put("Bye", 11);
//     ThisMap.put("Earth", 12);
//     ThisMap.put("I", 13);
//     ThisMap.put("Will", 14);
//     ThisMap.put("Miss", 15);
//     ThisMap.put("You", 16);
//     ThisMap.put("very", 17);
//     ThisMap.put("much", 18);
//     ThisMap.put("!", 19);
//     ThisMap.put("!!", 20);
//     delay(1000); // Delay for 1 second (1000 milliseconds
//     Serial.println("Printing map: ");
//     ThisMap.print(); // Print the map    

//     if(ThisMap.containsKey("Hello")){
//         Serial.println("Hello is in the map!");
//     } else {
//         Serial.println("Hello is not in the map!");
//     }

//     if(ThisMap.containsValue(25)){
//         Serial.println("25 is in the map!");
//     } else {
//         Serial.println("25 is not in the map!");
//     }


//     ThisMap.remove("Hello");
//     ThisMap.remove("World");

//     if(ThisMap.containsKey("Hello")){
//         Serial.println("Hello is in the map!");
//     } else {
//         Serial.println("Hello is not in the map!");
//     }

//     Serial.println("Printing map: ");
//     ThisMap.print(); // Print the map




//     Serial.println(Factorial(5)); // 120
//     Serial.println(Power(2, 3)); // 8
//     Serial.println(Sinh(1)); // 1.1752
//     Serial.println(Cosh(1)); // 1.54308
//     Serial.println(Tanh(1)); // 0.761594
//     Serial.println(Coth(1)); // 1.31304
//     Serial.println(Sech(1)); // 0.648054
//     Serial.println(Csch(1)); // 0.850918
//     Serial.println(Arcsinh(1)); // 0.881374
//     Serial.println(Log(1)); // 0
//     Serial.println(EXP(1)); // 2.71828
//     Serial.println(Log10(1)); // 0
//     Serial.println(Sqrt(4)); // 2
//     Serial.println(Cbrt(8)); // 2
//     Serial.println("Done!");
// }

// void loop()
// {
// }
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
#include <Arduino.h>
// Custom class for values
class Person { // Create a custom class for the values in the hashtable
public:
    String name; // Name of the person
    int age; // Age of the person

    Person(const String& n, int a) : name(n), age(a) {} // Constructor for the Person class
    Person(){}
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
