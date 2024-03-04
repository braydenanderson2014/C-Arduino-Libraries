# MatrixMath Library for Arduino

## Overview
MatrixMath is a versatile library designed for handling matrices on the Arduino platform. It enables users to perform a wide range of matrix operations essential for applications in robotics, signal processing, and linear algebra.

## Features
* Matrix Creation and Manipulation: Allows for the creation, copying, and manipulation of matrices.
* Element Access and Modification: Facilitates accessing and modifying individual elements within the matrix.
* Basic Operations: Supports addition, subtraction, multiplication, and division of matrices and scalars.
* Advanced Operations: Includes matrix transposition, inversion, scaling, and setting matrices to identity or zero matrices.
* Transformation Operations: Provides functions for rotating matrices around the X, Y, Z axes, or an arbitrary axis.
* Utility Functions: Offers additional functionalities like printing matrix contents to the serial monitor, clearing the matrix, and more.

## Usage
To use the MatrixMath library in your project, include it at the beginning of your Arduino sketch:

```cpp

#include "MatrixMath.h"
```
## Example: Matrix Addition
``` cpp
MatrixMath matrix1(2, 2);
MatrixMath matrix2(2, 2);

void setup() {
  Serial.begin(9600);

  // Initialize matrices
  matrix1.set(0, 0, 1); matrix1.set(0, 1, 2);
  matrix1.set(1, 0, 3); matrix1.set(1, 1, 4);

  matrix2.set(0, 0, 5); matrix2.set(0, 1, 6);
  matrix2.set(1, 0, 7); matrix2.set(1, 1, 8);

  // Perform matrix addition
  matrix1.add(matrix2);

  // Print the result
  matrix1.print();
}

void loop() {
  // Your loop code here
}
```
## Example: Matrix Scaling
```cpp
void scaleMatrix() {
  MatrixMath matrix(2, 2);
  matrix.set(0, 0, 1); matrix.set(0, 1, 2);
  matrix.set(1, 0, 3); matrix.set(1, 1, 4);

  // Scale the matrix by a factor of 2
  matrix.scale(2.0);

  // Print the scaled matrix
  matrix.print();
}
```
## Contribution
Contributions to the MatrixMath library are welcome. Whether it's adding new features, improving existing ones, or fixing bugs, your help can make MatrixMath more powerful and user-friendly.

## License
MatrixMath is released under an open-source license. Feel free to use it, modify it, and distribute it in your projects as you see fit.