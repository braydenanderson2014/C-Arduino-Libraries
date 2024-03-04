# Numeric Limits Library for Arduino
## Overview
The Numeric Limits library provides a way to determine the properties and limits of arithmetic types in the Arduino environment. It covers a range of types including integers, floating-point numbers, and bytes, offering functionality similar to std::numeric_limits from the C++ Standard Library but tailored for Arduino's constraints and types.

## Features
* Type Limits: Obtain minimum and maximum values for various data types.
* Type Properties: Check whether a type is signed, integer, or floating-point.
* Compatibility: Designed with Arduino's 32-bit architecture in mind, ensuring compatibility with common Arduino boards.

## Usage
To use the Numeric Limits library in your Arduino sketch, include it at the beginning of your code:

```cpp
#include "Numeric_Limits.h"
```

## Examples
### Getting Integer Limits
```cpp

int intMin = numeric_limits<int>::Min();
int intMax = numeric_limits<int>::Max();
Serial.print("Int Min: ");
Serial.println(intMin);
Serial.print("Int Max: ");
Serial.println(intMax);
```

### Floating-Point Limits
```cpp

float floatMin = numeric_limits<float>::Min();
float floatMax = numeric_limits<float>::Max();
Serial.print("Float Min: ");
Serial.println(floatMin, 10); // '10' specifies the number of digits after the decimal point
Serial.print("Float Max: ");
Serial.println(floatMax, 10);
```
### Checking Type Properties
```cpp

bool isIntSigned = numeric_limits<int>::is_signed();
Serial.print("Is Int Signed: ");
Serial.println(isIntSigned ? "Yes" : "No");
```
## Supported Types
The library provides implementations for the following types:

* int
* long
* float
* double
* byte
* char
* bool

Note: On Arduino, double is often equivalent to float, reflected in their shared limits.

## Contribution
Contributions to the Numeric Limits library are welcome. Whether it's through adding support for additional types, enhancing the existing functionality, or improving compatibility across various Arduino platforms, your contributions are valuable to the Arduino community.

## License
This library is open-source and can be freely used and modified in both personal and commercial projects under the terms specified in the license accompanying the library source.