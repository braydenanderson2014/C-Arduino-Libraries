# ArrayListOperations Library for Arduino

## Overview
The ArrayListOperations library provides a comprehensive set of operations that can be performed on elements of an ArrayList. This library is designed for Arduino projects and offers a wide range of mathematical and utility functions.

## Features
* Operations on individual elements (e.g., increment, double, multiply by a factor)
* Mathematical operations (e.g., square, cube, square root, cube root)
* Conversion functions (e.g., decimal to binary, binary to decimal)
* Statistical calculations (e.g., mean, median, mode, standard deviation, variance)
* Utility operations (e.g., find minimum, find maximum, count elements)
* Customizable global multiplier for scalable operations

## Installation
* Download the Operators.h file.

* Place it in your Arduino project's directory.

* Include the library in your Arduino sketch:

```cpp
#include "Operations.h"
```
## Usage
### Basic Operations
You can perform basic operations on array list elements:

```cpp
Operators<int>::setGlobalMultiplier(5);
int result = Operators<int>::multiply(10);  // result is 50
```

## Mathematical Operations
Perform mathematical operations like square, cube, or roots:

```cpp
int squared = Operators<int>::square(4);  // squared is 16
```


## Conversion Functions
Convert between different number systems:

```cpp
int binary = Operators<int>::decimalToBinary(5);  // binary is 101
```

## Statistical Calculations
### Calculate statistical measures:

```cpp
int array[] = {1, 2, 3, 4, 5};
int mean = Operators<int>::average(array, 5);  // mean is 3
```
## Customization
You can modify the Operations class to include more operations as needed for your specific use case.

## Contribution
Feel free to fork this repository and contribute to expanding the library's functionalities.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


*** Please make sure you use the setGlobalMultiplier before utilizing the functions that need it. Otherwise its default value is 0 *** 

*** Please Make sure you add items to the Internal Array before using Mathmatical functions that have no arguments, otherwise you will end up with a 0 ***



## List of Available Functions 
* incrementByOne(T element)
* doubleValue(T element)
* setGlobalMultiplier(T multiplier)
* multiply(T value)
* multiply(T value, T value2)
* divide(T value)
* add(T value)
* add(T value, T value2)
* subtract(T value)
* subtract(T value, T value2)
* modulo(T value)
* multiplyByTwo(T value)
* divideByTwo(T value)
* addTwo(T value)
* subtractTwo(T value)
* moduloTwo(T value)
* square(T value)
* cube(T value)
* squareRoot(T value)
* cubeRoot(T value)
* powerOfTwo(T value)
* powerOfThree(T value)
* powerOf(T value)
* powerOf(T value, T power)
* absoluteValue(T value)
* factorial(T value)
* fibonacci(T value)
* fibonacciPrint(T value)
* greatestCommonDivisor(T value)
* leastCommonMultiple(T value)
* decimalToBinary(T value)
* binaryToDecimal(T value)
* decimalToOctal(T value)
* octalToDecimal(T value)
* decimalToHexadecimal(T value)
* hexadecimalToDecimal(T value)
* calculatePercentage(T value)
* calculatePercentage(T value, T percentage)
* calculatePercentageOf(T value)
* calculatePercentageOf(T value, T percentage)
* calculatePercentageChange(T value)
* calculatePercentageChange(T value, T percentage)
* calculatePercentageDifference(T value)
* calculatePercentageDifference(T value, T percentage)
* calculatePercentageError(T value)
* calculatePercentageError(T value, T percentage)
* calculatePercentageOfError(T value)
* calculatePercentageOfError(T value, T percentage)
* calculatePercentageOfChange(T value)
* calculatePercentageOfChange(T value, T percentage)
* calculatePercentageOfDifference(T value)
* calculatePercentageOfDifference(T value, T percentage)
* calculatePercentageOfTotal(T value)
* calculatePercentageOfTotal(T value, T percentage)
* calculatePercentageOfDifferenceFrom(T value)
* calculatePercentageOfDifferenceFrom(T value, T percentage)
* calculatePercentageOfDifferenceTo(T value)
* calculatePercentageOfDifferenceTo(T value, T percentage)
* calculatePercentageOfTotalFrom(T value)
* calculatePercentageOfTotalFrom(T value, T percentage)
* calculatePercentageOfTotalTo(T value)
* calculatePercentageOfTotalTo(T value, T percentage)
* calculatePercentageOfDifferenceBetween(T value)
* calculatePercentageOfDifferenceBetween(T value, T percentage)
* calculatePercentageOfTotalBetween(T value)
* calculatePercentageOfTotalBetween(T value, T percentage)
* calculatePercentageOfDifferenceFromTo(T value)
* calculatePercentageOfDifferenceFromTo(T value, T percentage)
* calculatePercentageOfTotalFromTo(T value)
* calculatePercentageOfTotalFromTo(T value, T percentage)
* calculatePercentageOfDifferenceBetweenTo(T value)
* calculatePercentageOfDifferenceBetweenTo(T value, T percentage)
* calculatePercentageOfTotalBetweenTo(T value)
* calculatePercentageOfTotalBetweenTo(T value, T percentage)
* calculatePercentageOfDifferenceBetweenFrom(T value)
* calculatePercentageOfDifferenceBetweenFrom(T value, T percentage)
* calculatePercentageOfTotalBetweenFrom(T value)
* calculatePercentageOfTotalBetweenFrom(T value, T percentage)
* calculatePercentageOfDifferenceBetweenFromTo(T value)
* calculatePercentageOfDifferenceBetweenFromTo(T value, T percentage)
* calculatePercentageOfTotalBetweenFromTo(T value)
* calculatePercentageOfTotalBetweenFromTo(T value, T percentage)
* calculateDecimalPercentage(T value)
* calculateDecimalPercentage(T value, T percentage)
* calculateDecimal(T value)
* calculateDecimal(T value, T percentage)
* divideByHundred(T value)
* calculatePercentageOfValue(T value, T percentage)
* divide(T value, T divisor)
* addItemsToArray(T value)
* removeItemsFromArray(T value)
* getItemsFromArray(T value)
* average(T value)
* median(T value)
* mode(T value)
* range(T value)
* standardDeviation(T value)
* variance(T value)
* sum()
* sum(T value)
* product()
* product(T value)
* findMin()
* minimum(T* array, size_t size)
* findMax()
* count(T value)