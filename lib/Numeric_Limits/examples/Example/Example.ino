/*
    Example Name: Example For RTree Library

    Basic example for the RTree library. This example demonstrates the basic functionality of the RTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <numeric_limits.h>

void setup() {
    // Print the Minimum and Maximum values for various data types
    Serial.println("Minimum and Maximum Values for Various Data Types:");
    Serial.println("---------------------------------------------------");
    Serial.println("Data Type\t\tMinimum Value\t\tMaximum Value");
    Serial.println("---------------------------------------------------");
    Serial.println("char\t\t\t" + String(numeric_limits<char>::Min()) + "\t\t\t" + String(numeric_limits<char>::Max())); // This will print the minimum and maximum values for the char data type

    Serial.println("unsigned char\t\t" + String(numeric_limits<unsigned char>::Min()) + "\t\t\t" + String(numeric_limits<unsigned char>::Max())); // This will print the minimum and maximum values for the unsigned char data type
    
    Serial.println("short\t\t\t" + String(numeric_limits<short>::Min()) + "\t\t\t" + String(numeric_limits<short>::Max())); // This will print the minimum and maximum values for the short data type
    
    Serial.println("unsigned short\t\t" + String(numeric_limits<unsigned short>::Min()) + "\t\t\t" + String(numeric_limits<unsigned short>::Max())); // This will print the minimum and maximum values for the unsigned short data type
    
    Serial.println("int\t\t\t" + String(numeric_limits<int>::Min()) + "\t\t\t" + String(numeric_limits<int>::Max())); // This will print the minimum and maximum values for the int data type
    
    Serial.println("unsigned int\t\t" + String(numeric_limits<unsigned int>::Min()) + "\t\t\t" + String(numeric_limits<unsigned int>::Max())); // This will print the minimum and maximum values for the unsigned int data type
    
    Serial.println("long\t\t\t" + String(numeric_limits<long>::Min()) + "\t\t" + String(numeric_limits<long>::Max())); // This will print the minimum and maximum values for the long data type
    
    Serial.println("unsigned long\t\t" + String(numeric_limits<unsigned long>::Min()) + "\t\t" + String(numeric_limits<unsigned long>::Max())); // This will print the minimum and maximum values for the unsigned long data type
}