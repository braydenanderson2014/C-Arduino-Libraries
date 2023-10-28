#include <SimpleVector.h>
#include <Arduino.h>

void setup() {
    Serial.begin(9600);

    // Create a SimpleVector of integers
    SimpleVector<int> numbers;

    // Add some numbers to the vector
    numbers.add(1);
    numbers.add(2);
    numbers.add(3);
    numbers.add(4);
    numbers.add(5);

    // Access and print the elements using the [] operator
    Serial.println("Elements in the vector:");
    for (unsigned int i = 0; i < numbers.size(); i++) {
        Serial.print(numbers[i]);
        Serial.print(" ");
    }
    Serial.println();

    // Iterate through the vector using an iterator
    SimpleVector<int>::SimpleVectorIterator iter = numbers.begin();
    Serial.println("Elements using iterator:");
    while (iter.hasNext()) {
        int value = iter.next();
        Serial.print(value);
        Serial.print(" ");
    }
    Serial.println();

    numbers.remove(3); //NEW in Version 1.0.1, remove an element from the vector
    // Release the memory used by the vector
    numbers.releaseMemory();
}

void loop() {
    // Your Arduino code that runs repeatedly goes here
}
