# Segment Tree Library for Arduino

## Overview
The Segment Tree library for Arduino provides a versatile data structure for efficient querying and updating of array segments. It's particularly useful for scenarios requiring frequent updates and queries over a range, such as finding the sum, minimum, or maximum in a segment of an array.

## Features
* Efficient Range Queries: Quickly compute sums, minimums, or maximums within a range of an array.
* Lazy Propagation: Optimizes range updates to avoid unnecessary calculations, enhancing performance for multiple updates.
* Dynamic Updates: Supports updating values or ranges within the array, reflecting changes in subsequent queries.
* Arithmetic Type Support: Designed for numeric data types, ensuring broad applicability for different numeric operations.

## Usage
To use the Segment Tree library in your Arduino project, include it at the beginning of your sketch:

```cpp

#include "SegmentTree.h"
```

## Creating a Segment Tree
Instantiate a Segment Tree object using an array (or SimpleVector) of numeric values:

```cpp

SimpleVector<int> arr = {1, 3, 5, 7, 9, 11};
SegmentTree<int> segTree(arr);
```

## Performing Range Queries
Compute the sum, minimum, or maximum within a specified range of the array:

```cpp

int sum = segTree.getSum(1, 3); // Gets the sum of elements from index 1 to 3
int min = segTree.getMin(1, 4); // Gets the minimum value from index 1 to 4
int max = segTree.getMax(2, 5); // Gets the maximum value from index 2 to 5
```

## Updating Values
Update a single value or a range of values in the array:

```cpp

segTree.updateValue(arr, 2, 10); // Updates the value at index 2 to 10
segTree.updateRange(1, 4, 5); // Adds 5 to each element from index 1 to 4
```
## Clearing and Rebuilding
To clear or rebuild the tree after significant changes:

```cpp

segTree.clear(); // Clears the segment tree
segTree.build(arr, 0, arr.size() - 1, 0); // Rebuilds the tree with the current array
```

## Applications
Segment Trees are versatile and can be used in various applications including:

* Computational geometry for spatial queries.
* Dynamic data analysis where ranges of data are frequently queried and updated.
* Game development for efficient collision detection and world state management.

## Customization
The library is template-based, allowing for customization with different numeric types (int, float, long, etc.). Users can extend its functionality by implementing additional query and update methods based on specific needs.

## License
Ensure compliance with the library's licensing terms if used within both open-source and proprietary projects.