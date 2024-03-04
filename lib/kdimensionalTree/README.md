# K-Dimensional Tree Library for Arduino

## Overview
The K-Dimensional Tree (KD-Tree) library for Arduino provides a powerful tool for organizing points in a k-dimensional space. This data structure facilitates efficient search operations, such as nearest neighbor search and range search, making it ideal for applications in robotics, spatial indexing, and more.

## Key Features
* Generic Implementation: Compatible with any numeric type (e.g., int, float) for the coordinates of points.
* Dynamic Insertion: Allows for dynamic insertion of points into the k-dimensional space.
* Search Operations: Supports efficient nearest neighbor search and range search within the k-dimensional space.
* Deletion: Enables removal of points from the tree.
* Flexibility: Works with any number of dimensions specified at tree initialization.

## Usage
### Initialization
First, include the library and create a KD-Tree instance specifying the dimension.

```cpp

#include "K_DIMENSIONAL_TREE_h"
KDimensionalTree<float> kdTree(3); // For a 3-dimensional space
```
## Inserting Points
Insert points into the KD-Tree. Points are represented as SimpleVector<T> objects.

``` cpp

SimpleVector<float> point1 = {1.0, 2.0, 3.0};
kdTree.insert(point1);
```
## Searching for a Nearest Neighbor
Find the nearest neighbor of a given point.

```cpp
SimpleVector<float> searchPoint = {1.1, 2.1, 3.1};
SimpleVector<float> nearestNeighbor = kdTree.nearestNeighbor(searchPoint);
``` 
## Range Search
Find all points within a specified range.

```cpp

SimpleVector<float> lowerBound = {0.0, 0.0, 0.0};
SimpleVector<float> upperBound = {2.0, 2.0, 2.0};
SimpleVector<SimpleVector<float>> pointsInRange = kdTree.rangeSearch(lowerBound, upperBound);
```
## Removing Points
Remove a point from the KD-Tree.

```cpp

kdTree.remove(point1);
```
## Clearing the Tree
Remove all points from the KD-Tree.

```cpp

kdTree.clear();
```
## Example Sketch
```cpp
void setup() {
    Serial.begin(9600);

    KDimensionalTree<float> kdTree(2); // 2D space
    kdTree.insert({1.0, 2.0});
    kdTree.insert({3.0, 4.0});
    kdTree.insert({5.0, 6.0});

    SimpleVector<float> searchPoint = {3.1, 4.1};
    SimpleVector<float> nearest = kdTree.nearestNeighbor(searchPoint);

    Serial.print("Nearest neighbor to (3.1, 4.1): (");
    Serial.print(nearest[0]);
    Serial.print(", ");
    Serial.print(nearest[1]);
    Serial.println(")");

    // Range search
    SimpleVector<float> lower = {2.0, 3.0};
    SimpleVector<float> upper = {4.0, 5.0};
    auto pointsInRange = kdTree.rangeSearch(lower, upper);
    Serial.println("Points in range:");
    for (auto& p : pointsInRange) {
        Serial.print("(");
        Serial.print(p[0]);
        Serial.print(", ");
        Serial.print(p[1]);
        Serial.println(")");
    }
}

void loop() {
    // Put your main code here, to run repeatedly:
}
```
## Contribution
Contributions to the KD-Tree library are welcome. Whether it's optimizing the implementation, adding new features, or improving the documentation, your input can help make this library more useful for the Arduino community.

## License
This library is released under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.