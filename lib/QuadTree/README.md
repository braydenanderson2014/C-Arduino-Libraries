# QuadTree Library for Arduino

## Overview
The QuadTree library is a specialized data structure for Arduino projects that need efficient querying of two-dimensional space. It's well-suited for applications requiring spatial partitioning and indexing, such as graphics rendering, collision detection, and geographic data management.

## Features
* Efficient Spatial Partitioning: Divides 2D space into four quadrants to efficiently manage spatial data.
* Dynamic Insertion and Querying: Supports dynamic insertion of points and querying of rectangular regions.
* Point and Rectangle Support: Includes structures for points and rectangles, including utility functions like containment checks and intersection tests.
* Customizable Capacity: Each node's capacity is customizable, allowing for flexibility in balancing depth versus breadth of the tree.

## Usage
To utilize the QuadTree library, include it at the beginning of your Arduino sketch:

```cpp

#include "QuadTree.h"
```
## Initialization
Create a QuadTree object by specifying the boundary and capacity:

```cpp

Rectangle<float> boundary = {{0, 0}, {100, 100}};
int capacity = 4;
QuadTree<float> quadTree(boundary, capacity);
```
## Inserting Points
Add points to the QuadTree with the insert method:

```cpp

Point<float> point = {25.0, 75.0};
quadTree.insert(point);
```
## Querying Regions
Retrieve points within a specific region by defining a query rectangle:

```cpp

Rectangle<float> queryRect = {{10, 10}, {50, 50}};
SimpleVector<Point<float>> foundPoints = quadTree.query(queryRect);
```
## Deleting Points
Remove a point from the QuadTree, if necessary:

```cpp

quadTree.deletePoint(point);
```
## Clearing the QuadTree
Reset the QuadTree, removing all points:

```cpp

quadTree.clear();
```
## Additional Operations
The library supports additional operations like checking if a point is within the QuadTree's boundary, moving points within the tree, and printing the tree structure for debugging purposes.

## Applications
* Game Development: Managing game entities and detecting collisions.
* Geographic Information Systems (GIS): Indexing geographic locations for rapid querying.
* Computer Graphics: Spatially partitioning objects for efficient rendering.

##  Extending the Library 
While the QuadTree library provides a solid foundation for 2D spatial partitioning, it can be extended with features like balancing the tree, supporting more complex shapes, and optimizing memory usage for large datasets.

## License
This library is provided under an open-source license, allowing for modification and redistribution in both personal and commercial projects. Check the license details for specific terms and conditions.