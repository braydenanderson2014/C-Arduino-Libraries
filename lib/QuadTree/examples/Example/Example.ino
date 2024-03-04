/*
    Example Name: Example For QuadTree Library

    Basic example for the QuadTree library. This example demonstrates the basic functionality of the QuadTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <QuadTree.h>
// Define the boundaries of the QuadTree
Rectangle<float> boundary = {{0, 0}, {100, 100}}; // The boundary of the QuadTree (0,0) = bottom left, (100,100) = top right

// Define the capacity of each quad in the QuadTree
int capacity = 4;

// Create a QuadTree with the specified boundary and capacity
QuadTree<float> qt(boundary, capacity); // This will create a QuadTree with the specified boundary and capacity

void setup() {
    Serial.begin(9600);
    // Wait for the serial monitor to open, necessary for boards that reprogram upon serial connection
    while (!Serial);

    // Insert some points into the QuadTree
    qt.insert({25, 75}); // This will insert the point (25,75) into the QuadTree
    qt.insert({50, 50}); // This will insert the point (50,50) into the QuadTree
    qt.insert({75, 25}); // This will insert the point (75,25) into the QuadTree
    qt.insert({10, 90}); // This will insert the point (10,90) into the QuadTree
    qt.insert({90, 10}); // This will insert the point (90,10) into the QuadTree

    // Define a query range (a smaller rectangle within the QuadTree boundary)
    Rectangle<float> queryRange = {{20, 20}, {80, 80}}; // The query range (20,20) = bottom left, (80,80) = top right

    // Query the QuadTree for points within the range
    SimpleVector<Point<float>> foundPoints = qt.query(queryRange); // This will query the QuadTree for points within the range

    // Print the points found within the query range
    Serial.println("Points found within the query range:");
    for (int i = 0; i < foundPoints.size(); i++) { // This will print the points found within the query range
        Serial.print("Point: "); // This will print "Point: "
        Serial.print(foundPoints[i].x); // This will print the x value of the point
        Serial.print(", "); // This will print ", "
        Serial.println(foundPoints[i].y); // This will print the y value of the point
    }

    // Optionally, clear the QuadTree to remove all points
    qt.clear(); // This will clear the QuadTree
}

void loop() {
    // Place your repeated code here
}