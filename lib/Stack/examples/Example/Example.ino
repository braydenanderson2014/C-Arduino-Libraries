/*
    Example Name: Example For Stack Library

    Basic example for the Stack library. This example demonstrates the basic functionality of the Stack library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <Arduino.h>
#include <Stack.h>

// Create a Stack object
Stack<int> stack; // This will create a stack of integers (First in Last Out)

void setup() {
    Serial.begin(9600);
    // Push some values onto the stack
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.push(40);
    Serial.println(stack.peek()); // This will print the value at the top of the stack
    Serial.println(stack.isFull()); // This will print "false" because the stack is not full
    Serial.println(stack.isEmpty()); // This will print "false" because the stack is not empty
    Serial.println(stack.count()); // This will print "4" because there are 4 elements in the stack

    // Pop a value from the stack
    int value = stack.pop();
    Serial.println(value); // This will print the value that was popped

    // You can also perform other operations like peek, clear, etc.
    Serial.println(stack.peek()); // This will print the value at the top of the stack
    stack.clear(); // This will clear the stack
}