#include <SimpleVector.h>
#include <Arduino.h>

void setup() {
    Serial.begin(9600);

    // Create a SimpleVector of integers
    SimpleVector<int> numbers;

    // Add some numbers to the vector
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(3);
    numbers.push_back(4);
    numbers.push_back(5);

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

    // Release the memory used by the vector
    numbers.releaseMemory();
}

void loop() {
    // Your Arduino code that runs repeatedly goes here
}
