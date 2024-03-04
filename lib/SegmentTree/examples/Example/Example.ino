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

#include <SegmentTree.h>

// Create a SegmentTree object
void setup() {
    Serial.begin(9600); // Initialize serial communication

    // Wait for serial port to open, necessary for native USB
    while (!Serial) {}

    // Initialize your array of data
    SimpleVector<int> data;
    int initialValues[] = {1, 3, 5, 7, 9, 11};
    int sizeOfArray = sizeof(initialValues) / sizeof(initialValues[0]);

    for(int i = 0; i < sizeOfArray; i++) {
        data.push_back(initialValues[i]);
    }

    // Create a SegmentTree from the data
    SegmentTree<int> segTree(data);

    // Example query: Get sum in range [1, 3]
    int sum = segTree.getSum(1, 3);
    Serial.print("Sum of range [1, 3]: ");
    Serial.println(sum);

    // Update a single value: data[2] = 6
    segTree.updateValue(data, 2, 6);

    // Re-query after the update
    sum = segTree.getSum(1, 3);
    Serial.print("Sum of range [1, 3] after update: ");
    Serial.println(sum);

    // Range update: Increase elements in range [1, 4] by 5
    segTree.updateRange(1, 4, 5);

    // Query after range update
    sum = segTree.getSum(1, 4);
    Serial.print("Sum of range [1, 4] after range update: ");
    Serial.println(sum);

    // Get minimum and maximum in a range
    int min = segTree.getMin(1, 4);
    int max = segTree.getMax(1, 4);
    Serial.print("Min of range [1, 4]: ");
    Serial.println(min);
    Serial.print("Max of range [1, 4]: ");
    Serial.println(max);
}

void loop() {
    // Not used in this example
}