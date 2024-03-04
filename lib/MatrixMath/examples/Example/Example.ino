/*
    Example Name: Example For MatrixMath Library

    Basic example for the MatrixMath library. This example demonstrates the basic functionality of the MatrixMath library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <MatrixMath.h>

MatrixMath matrix = MatrixMath(3,3); // Create a 3x3 matrix
void setup(){
    matrix.set(0, 0, 1); // Add 1 to the 0,0 position
    matrix.set(0, 1, 2); // Add 2 to the 0,1 position
    matrix.set(0, 2, 3); // Add 3 to the 0,2 position
    matrix.set(1, 0, 4); // Add 4 to the 1,0 position
    matrix.set(1, 1, 5); // Add 5 to the 1,1 position
    matrix.set(1, 2, 6); // Add 6 to the 1,2 position
    matrix.set(2, 0, 7); // Add 7 to the 2,0 position
    matrix.set(2, 1, 8); // Add 8 to the 2,1 position
    matrix.set(2, 2, 9); // Add 9 to the 2,2 position

    matrix.print(); // Print the matrix
    matrix.add(1); // Add 1 to every element in the matrix
    matrix.print(); // Print the matrix
    matrix.subtract(1); // Subtract 1 from every element in the matrix
    matrix.print(); // Print the matrix
    matrix.multiply(2); // Multiply every element in the matrix by 2
    matrix.print(); // Print the matrix
    matrix.divide(2); // Divide every element in the matrix by 2
    matrix.print(); // Print the matrix
    matrix.transpose(); // Transpose the matrix
    matrix.print(); // Print the matrix
    matrix.invert(); // Invert the matrix
    matrix.print(); // Print the matrix

    Serial.println(matrix.magnitude()); // Calculate the magnitude of the matrix

    matrix.clear(); // Clear the matrix (sets everything to 0)
}

void loop(){

}
