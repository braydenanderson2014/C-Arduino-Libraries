# MatrixMath

## Overview
The `MatrixMath` class provides an efficient and lightweight matrix manipulation library for Arduino. It supports operations like addition, subtraction, multiplication, transposition, scaling, and more while integrating with a custom `MemoryManager` for optimized dynamic memory allocation.

## Features
- **Dynamic memory allocation** using `MemoryManager`.
- **Matrix operations**: Addition, subtraction, multiplication, and scaling.
- **Vector operations**: Dot product, cross product, and normalization.
- **Special matrices**: Identity and zero matrices.
- **Optimized for embedded systems**: Uses minimal memory and does not depend on `std`.
- **Arduino-compatible**: Works without standard C++ libraries.

## API Reference

| Function | Description |
|----------|-------------|
| `MatrixMath(size_t rows, size_t cols, MemoryManager &manager)` | Constructor that initializes a matrix with the given dimensions using the provided `MemoryManager`. |
| `~MatrixMath()` | Destructor that frees allocated memory. |
| `void set(size_t row, size_t col, float value)` | Sets a specific element in the matrix. |
| `float get(size_t row, size_t col) const` | Gets the value of a specific matrix element. |
| `void add(const MatrixMath &m)` | Adds another matrix to this matrix. |
| `void subtract(const MatrixMath &m)` | Subtracts another matrix from this matrix. |
| `void multiply(const MatrixMath &m)` | Multiplies this matrix by another matrix. |
| `void scale(float scalar)` | Scales the matrix by a given scalar. |
| `void transpose()` | Transposes the matrix. |
| `void identity(size_t size)` | Creates an identity matrix of the given size. |
| `void clear()` | Sets all elements of the matrix to zero. |
| `void print() const` | Prints the matrix to the serial monitor. |
| `float magnitude() const` | Computes the magnitude of a vector. |
| `void normalize()` | Normalizes a vector. |
| `float dotProduct(const MatrixMath &vector) const` | Computes the dot product with another vector. |
| `MatrixMath crossProduct(const MatrixMath &m) const` | Computes the cross product with another vector (only for 3D vectors). |

## Example Usage

### **Basic Matrix Operations**
```cpp
#include <Arduino.h>
#include "MatrixMath.h"
#include "MemoryManager.h"

MemoryManager memoryManager;

void setup() {
    Serial.begin(9600);
    MatrixMath mat1(2, 2, memoryManager);
    MatrixMath mat2(2, 2, memoryManager);

    mat1.set(0, 0, 1); mat1.set(0, 1, 2);
    mat1.set(1, 0, 3); mat1.set(1, 1, 4);

    mat2.set(0, 0, 5); mat2.set(0, 1, 6);
    mat2.set(1, 0, 7); mat2.set(1, 1, 8);

    Serial.println("Matrix 1:");
    mat1.print();

    Serial.println("Matrix 2:");
    mat2.print();

    mat1.add(mat2);
    Serial.println("After Addition:");
    mat1.print();
}

void loop() {}
```

### **Matrix Multiplication**
```cpp
MatrixMath matA(2, 3, memoryManager);
MatrixMath matB(3, 2, memoryManager);

matA.set(0, 0, 1); matA.set(0, 1, 2); matA.set(0, 2, 3);
matA.set(1, 0, 4); matA.set(1, 1, 5); matA.set(1, 2, 6);

matB.set(0, 0, 7); matB.set(0, 1, 8);
matB.set(1, 0, 9); matB.set(1, 1, 10);
matB.set(2, 0, 11); matB.set(2, 1, 12);

Serial.println("Matrix A:");
matA.print();

Serial.println("Matrix B:");
matB.print();

matA.multiply(matB);
Serial.println("Result of A * B:");
matA.print();
```

### **Transpose and Scaling**
```cpp
MatrixMath mat(3, 3, memoryManager);
mat.set(0, 0, 1); mat.set(0, 1, 2); mat.set(0, 2, 3);
mat.set(1, 0, 4); mat.set(1, 1, 5); mat.set(1, 2, 6);
mat.set(2, 0, 7); mat.set(2, 1, 8); mat.set(2, 2, 9);

Serial.println("Original Matrix:");
mat.print();

mat.transpose();
Serial.println("Transposed Matrix:");
mat.print();

mat.scale(2.0);
Serial.println("Scaled by 2:");
mat.print();
```

## License
This project is open-source and available for modification and distribution.

