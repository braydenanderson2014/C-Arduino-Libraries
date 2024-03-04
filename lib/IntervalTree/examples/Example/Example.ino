/*
    Example Name: Example For IntervalTree Library

    Basic example for the IntervalTree library. This example demonstrates the basic functionality of the IntervalTree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
/* Interval: An interval is a set of real numbers with the property that any number that lies between two numbers in the set is 
also included in the set. For the purposes of interval trees, an interval is typically represented as a pair of numbers,
where the first number is the lower bound of the interval and the second number is the upper bound. 
For example, the interval [5, 10] represents all numbers x such that 5 <= x <= 10.

Interval Tree: An Interval Tree is a type of self-balancing binary search tree where every node stores an interval and can be 
used for efficiently finding all intervals that overlap with any given interval or point. It's commonly used for windowing 
queries, for instance, finding all roads on a computer graphics screen, finding all visible buildings in a skyline or similar 
problems in computational geometry
*/

#include <IntervalTree.h>
#include <Arduino.h>

IntervalTree<int> tree; // Create an Interval Tree
IntervalTree<String> tree2; // Create an Interval Tree of type String... THIS IS NOT ALLOWED and will throw a compile time error
void setup(){
    // Inserting values into the tree
    tree.insert(Interval<int>{15, 20}); // Insert an interval
    tree.insert(Interval<int>{10, 30});
    tree.insert(Interval<int>{17, 19});
    tree.insert(Interval<int>{5, 20});
    tree.insert(Interval<int>{12, 15});
    tree.insert(Interval<int>{30, 40});
    tree.insert(Interval<int>{40, 50});
    tree.insert(Interval<int>{6, 7});
    tree.insert(Interval<int>{7, 8});
    tree.insert(Interval<int>{8, 9});
    tree.insert(Interval<int>{9, 10});

    tree.insert({55, 60}); // Insert an interval
    tree.insert({50, 70});
    tree.insert({57, 59});
    tree.insert({45, 60});

    Serial.println("The intervals are: ");
    tree.inorder(); // Print the tree

    Interval<int> searchInterval = {14, 16}; // create a search interval with a low of 14 and a high of 16
    IntervalTreeNode<int>* node = tree.search(searchInterval); // Search for the interval using the search interval
    if (node != nullptr) { // If the interval is found
        Serial.println("Found interval: [" + String(node->i.low) + ", " + String(node->i.high) + "]");
    } else { // If the interval is not found
        Serial.println("Interval not found");
    }

    
}

void loop(){

}