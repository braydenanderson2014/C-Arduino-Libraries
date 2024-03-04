# Octree Library for Arduino

## Overview
The Octree library offers a spatial partitioning structure for efficient querying of 3D space, optimized for Arduino environments. It allows for storing and manipulating data in a three-dimensional space, using a tree structure where each node has exactly eight children. This library is particularly useful for applications involving 3D graphics, spatial analysis, collision detection, and point cloud management.

## Features
* Spatial Partitioning: Efficiently divides 3D space into smaller regions for quick querying and data retrieval.
* Dynamic Allocation: Utilizes a node pool for optimized memory management, limiting the maximum number of nodes to MAX_NODES.
* Search Operations: Supports various search operations including point search, range search, and nearest neighbor search.
* Traversal Methods: Offers multiple tree traversal methods, such as in-order, pre-order, post-order, and level-order traversals.

## Usage
To use the Octree library, include it at the top of your Arduino sketch:

```cpp

#include "Octree.h"
```
## Creating an Octree
Instantiate an Octree object specifying the type of data it will hold:

```cpp

Octree<float> myOctree;
```
## Inserting Data
Insert data into the octree. The data should be of the type specified during the octree's instantiation:

```cpp

myOctree.insert(5.0f);
```
## Searching
Perform a search to find if a specific value exists within the octree:

```cpp

bool found = myOctree.search(5.0f);
Serial.println(found ? "Found" : "Not Found");
```
## Range Search
Query all points within a specified 3D range:

```cpp

SimpleVector<float> results = myOctree.rangeSearch(x1, x2, y1, y2, z1, z2);
```
## Nearest Neighbor
Find the nearest neighbor to a given point in 3D space:

```cpp
float nearest = myOctree.nearestNeighbor(data);
```
## Traversal
Traverse the octree in various orders to process or retrieve the stored data:

```cpp

SimpleVector<float> inOrderResults = myOctree.inOrderTraversal();
```
## Memory Management
The library includes mechanisms to manage memory efficiently, reusing nodes from a preallocated pool to avoid dynamic memory allocation during runtime. This is crucial for the limited memory environment of Arduino devices.

## Application Scenarios
* 3D Graphics: Managing objects in a 3D scene for rendering or collision detection.
* Spatial Analysis: Performing queries on spatial data, such as point clouds.
* Game Development: Efficiently managing game world entities and their interactions.

** Recommended for Microprocessors/MicroControllers with more memory capablility **

## Extending the Library
The Octree library is designed to be extensible. Users can add more features such as balancing the tree, supporting dynamic resizing, and enhancing search capabilities.

## License
This library is open-source and can be used and modified according to the terms specified in the license accompanying the library source. Contributions and improvements from the community are welcome. 