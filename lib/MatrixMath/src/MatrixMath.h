#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <Arduino.h>
#include <math.h>
#include "MemoryManager.h"

class MatrixMath {
private:
    float *matrix;
    size_t rows;
    size_t cols;
    MemoryManager &memManager; // Memory manager reference

    // Allocate memory using MemoryManager
    void allocate(size_t r, size_t c) {
        if (matrix) memManager.free(matrix);
        rows = r;
        cols = c;
        matrix = (float*)memManager.malloc(rows * cols * sizeof(float), __FILE__, __LINE__);
        if (matrix) memset(matrix, 0, rows * cols * sizeof(float));
    }

public:
    // Constructor
    MatrixMath(size_t r, size_t c, MemoryManager &manager) : memManager(manager), matrix(nullptr), rows(r), cols(c) {
        allocate(rows, cols);
    }

    // Destructor
    ~MatrixMath() {
        if (matrix) memManager.free(matrix);
    }

    // Set a value
    void set(size_t row, size_t col, float value) {
        matrix[row * cols + col] = value;
    }

    // Get a value
    float get(size_t row, size_t col) const {
        return matrix[row * cols + col];
    }

    // Add two matrices
    void add(const MatrixMath &m) {
        if (rows != m.rows || cols != m.cols) return;
        for (size_t i = 0; i < rows * cols; ++i) matrix[i] += m.matrix[i];
    }

    // Subtract two matrices
    void subtract(const MatrixMath &m) {
        if (rows != m.rows || cols != m.cols) return;
        for (size_t i = 0; i < rows * cols; ++i) matrix[i] -= m.matrix[i];
    }

    // Multiply two matrices
    void multiply(const MatrixMath &m) {
        if (cols != m.rows) return;
        float* result = (float*)memManager.malloc(rows * m.cols * sizeof(float), __FILE__, __LINE__);
        if (!result) return;
        memset(result, 0, rows * m.cols * sizeof(float));

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < m.cols; ++j) {
                float sum = 0;
                for (size_t k = 0; k < cols; ++k) {
                    sum += get(i, k) * m.get(k, j);
                }
                result[i * m.cols + j] = sum;
            }
        }

        memManager.free(matrix);
        matrix = result;
        cols = m.cols;
    }

    // Scale matrix by a scalar
    void scale(float scalar) {
        for (size_t i = 0; i < rows * cols; ++i) matrix[i] *= scalar;
    }

    // Transpose matrix
    void transpose() {
        float *result = (float*)memManager.malloc(cols * rows * sizeof(float), __FILE__, __LINE__);
        if (!result) return;

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[j * rows + i] = get(i, j);
            }
        }

        memManager.free(matrix);
        matrix = result;
        size_t temp = rows;
        rows = cols;
        cols = temp;
    }

    // Identity matrix
    void identity(size_t size) {
        allocate(size, size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                set(i, j, (i == j) ? 1.0f : 0.0f);
            }
        }
    }

    // Clear matrix (set all elements to zero)
    void clear() {
        if (matrix) memset(matrix, 0, rows * cols * sizeof(float));
    }

    // Print matrix
    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Serial.print(get(i, j), 3);
                Serial.print(" ");
            }
            Serial.println();
        }
    }

    // Compute magnitude (for vector interpretation)
    float magnitude() const {
        float sum = 0;
        for (size_t i = 0; i < rows; ++i) {
            sum += get(i, 0) * get(i, 0);
        }
        return sqrt(sum);
    }

    // Normalize matrix as a vector
    void normalize() {
        float mag = magnitude();
        if (mag > 0) scale(1 / mag);
    }

    // Compute dot product with a vector
    float dotProduct(const MatrixMath &vector) const {
        if (cols != 1 || vector.cols != 1 || rows != vector.rows) return 0.0f;
        float sum = 0;
        for (size_t i = 0; i < rows; ++i) {
            sum += get(i, 0) * vector.get(i, 0);
        }
        return sum;
    }

    // Compute cross product (only for 3D vectors)
    MatrixMath crossProduct(const MatrixMath &m) const {
        if (rows != 3 || cols != 1 || m.rows != 3 || m.cols != 1) return MatrixMath(0, 0, memManager);
        MatrixMath result(3, 1, memManager);
        result.set(0, 0, get(1, 0) * m.get(2, 0) - get(2, 0) * m.get(1, 0));
        result.set(1, 0, get(2, 0) * m.get(0, 0) - get(0, 0) * m.get(2, 0));
        result.set(2, 0, get(0, 0) * m.get(1, 0) - get(1, 0) * m.get(0, 0));
        return result;
    }

    //operators

    // Assignment operator
    MatrixMath &operator=(const MatrixMath &m) {
        if (this == &m) return *this;
        if (rows != m.rows || cols != m.cols) allocate(m.rows, m.cols);
        memcpy(matrix, m.matrix, rows * cols * sizeof(float));
        return *this;
    }

    // Addition operator
    MatrixMath operator+(const MatrixMath &m) const {
        MatrixMath result(*this);
        result.add(m);
        return result;
    }

    // Subtraction operator
    MatrixMath operator-(const MatrixMath &m) const {
        MatrixMath result(*this);
        result.subtract(m);
        return result;
    }

    // Multiplication operator
    MatrixMath operator*(const MatrixMath &m) const {
        MatrixMath result(rows, m.cols, memManager);
        result.multiply(m);
        return result;
    }

    // Scale operator
    MatrixMath operator*(float scalar) const {
        MatrixMath result(*this);
        result.scale(scalar);
        return result;
    }

    // Equality operator
    bool operator==(const MatrixMath &m) const {
        if (rows != m.rows || cols != m.cols) return false;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (get(i, j) != m.get(i, j)) return false;
            }
        }
        return true;
    }

    // Inequality operator
    bool operator!=(const MatrixMath &m) const {
        return !(*this == m);
    }

    // Access operator
    float operator()(size_t i, size_t j) const {
        return get(i, j);
    }

    // Access operator
    float &operator()(size_t i, size_t j) {
        return matrix[i * cols + j];
    }

    // Get number of rows
    size_t getRows() const {
        return rows;
    }

    // Get number of columns
    size_t getCols() const {
        return cols;
    }
};

#endif // MATRIXMATH_H
