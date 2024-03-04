# MathLib for Arduino

## Overview
MathLib is a comprehensive mathematical library designed for the Arduino platform. It offers a wide range of mathematical functions, from basic arithmetic operations to more complex trigonometric and hyperbolic calculations. This library aims to enhance the mathematical capabilities of Arduino projects without the need for external dependencies.

## Features
* Basic Mathematical Functions: Includes factorial, power, prime checking, and prime number generation.
* Trigonometric Functions: Provides sine, cosine, tangent, cotangent, secant, and cosecant functions, along with their arc (inverse) versions.
* Hyperbolic Functions: Offers sinh, cosh, tanh, coth, sech, and csch functions, along with their arc (inverse) versions.
* Exponential and Logarithmic Functions: Supports exponential, natural logarithm, and logarithm base 10 calculations.
* Square Root and Cube Root Functions: Implements efficient algorithms for calculating square roots and cube roots.
* Utility Functions: Includes absolute value, ceiling, floor, power with floating-point exponent, log base 2, and log with arbitrary base functions.

## Usage
To use the MathLib, include the library in your Arduino sketch and call any of the available functions:

```cpp

#include "MATHLIB.h"

void setup() {
  Serial.begin(9600);

  // Using trigonometric functions
  double sineValue = Sin(30); // Calculate sine of 30 degrees
  double cosineValue = Cos(45); // Calculate cosine of 45 degrees

  // Using logarithmic functions
  double logValue = Log(10); // Natural log of 10
  double log10Value = Log10(100); // Log base 10 of 100

  // Using power functions
  double powerValue = Power(2, 8); // 2 raised to the power of 8

  // Print results
  Serial.println("Sine(30): " + String(sineValue));
  Serial.println("Cos(45): " + String(cosineValue));
  Serial.println("Log(10): " + String(logValue));
  Serial.println("Log10(100): " + String(log10Value));
  Serial.println("Power(2, 8): " + String(powerValue));
}

void loop() {
  // Your main code here, to run repeatedly:
}
```
## Customization
* MYPI: Defines the value of pi used in trigonometric calculations.
* MYMAX_ITERATIONS: Determines the number of iterations for iterative functions to ensure accuracy.

## Contribution
MathLib is open for contributions. Enhancements, optimizations, or additions to the library are welcome to make it more versatile and efficient.

## License
MathLib is released under an open-source license, allowing free use, modification, and distribution of the library in both personal and commercial projects.