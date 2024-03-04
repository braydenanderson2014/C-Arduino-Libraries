# Fenwick Tree Library for Arduino

## Overview
The Fenwick Tree, also known as a Binary Indexed Tree (BIT), offers an efficient way to update elements and calculate prefix sums in a table of numbers. This Arduino-compatible Fenwick Tree library enables easy manipulation of sequences of numbers, providing a powerful tool for handling cumulative frequency tables and range sum queries in embedded projects.

## Key Features
* Efficient Range Updates and Queries: Perform range updates and range sum queries efficiently, ideal for dynamic cumulative frequency tables.
* Flexible Point Updates and Queries: Supports updating values at specific points and querying sums over ranges or single points.
* Scalable Operations: Efficiently scales operations on large datasets with logarithmic time complexity for updates and queries.
* Arduino-Compatible: Designed to integrate seamlessly with Arduino projects, enhancing data handling capabilities in embedded systems.


## Usage
To use the Fenwick Tree library, include FENWICKTREE_H in your Arduino project. The library requires SimpleVector.h for dynamic array management and TypeTraits.h for type checking.

## Example Usage
Below is a simple example that demonstrates how to initialize a Fenwick Tree, perform updates, and query sums:

```cpp
#include "FENWICKTREE_H"

// Initialize a Fenwick Tree with a size of 10
FenwickTree<int> ft(10);

void setup() {
  Serial.begin(9600);

  // Perform updates on the Fenwick Tree
  ft.add(0, 5); // Add 5 to index 0
  ft.add(1, 3); // Add 3 to index 1
  ft.range_update(2, 4, 2); // Add 2 to the range [2, 4]

  // Query the sum of the first 5 elements
  int sumFirstFive = ft.sum(0, 4);
  Serial.print("Sum of first 5 elements: ");
  Serial.println(sumFirstFive);

  // Query a single point
  int pointQuery = ft.point_query(3);
  Serial.print("Value at index 3: ");
  Serial.println(pointQuery);
}

void loop() {
  // Loop code here (if necessary)
}
```

## Customization
The FenwickTree class template can be instantiated with any numeric type (int, long, float, etc.), allowing for flexibility based on the requirements of your application.

## Contribution
Contributions to the Fenwick Tree library are welcome. Enhancements, bug fixes, or documentation improvements will help make the library more useful for the Arduino community.

## License
This library is distributed under an open-source license, allowing for modification, distribution, and use in both personal and commercial projects.