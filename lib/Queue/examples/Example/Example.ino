/*
    Example Name: Example For Queue Library

    Basic example for the Queue library. This example demonstrates the basic functionality of the Queue library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <Arduino.h>
#include <Queue.h>

// Create a Queue object
Queue<int> queue;

void setup() {
    Serial.begin(9600);
    // Enqueue some values into the queue
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    queue.enqueue(40);
    Serial.println(queue.peek()); // This will print the value at the front of the queue
    Serial.println(queue.isFull()); // This will print "false" because the queue is not full
    Serial.println(queue.isEmpty()); // This will print "false" because the queue is not empty
    Serial.println(queue.count()); // This will print "4" because there are 4 elements in the queue

    // Dequeue a value from the queue
    int value = queue.dequeue();
    Serial.println(value); // This will print the value that was dequeued

    // You can also perform other operations like peek, clear, etc.
    Serial.println(queue.peek()); // This will print the value at the front of the queue
    queue.clear(); // This will clear the queue
}