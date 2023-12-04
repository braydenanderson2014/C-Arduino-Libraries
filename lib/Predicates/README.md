# ArrayListPredicates Library for Arduino

## Overview
The ArrayListPredicates library provides a comprehensive set of Predicates that can be performed on elements of an ArrayList or other list structure. This library is designed for Arduino projects and offers a wide range of mathematical and utility functions.

## Features
* Predicates on individual elements (e.g., isPrime, isNegative, isDivisibleBy)
* Customizable global multiplier for scalable operations

## Installation
* Download the Predicates.h && the Operators.h file.

* Place it in your Arduino project's directory.

* Include the library in your Arduino sketch:

```cpp
#include "Predicates.h"
#include "Operators.h"
```

## ChangeLog
### Version 1.0.0
* Initial Release
* Please Note, This Library Has not been Tested in any Shape or form, USE AT YOUR OWN RISK
* Although this library has not been tested, We do encourage you to use this library so we can find bugs and fix them.
### Version 1.0.1
* Added Function Comments to the Library
* Fixed a bug where the library would not build due to an issue with an overloaded function. It was overloaded exactly the same way.
            


## Usage
### Basic Operations
You can perform basic Predicate Checks on array list elements:

```cpp
Predicates<int>::setGlobalMultiplier(5);

if(Predicates<int>::isDivisibleBy(5)){
    Serial.println("yay, The Number Is Divisible by the Multiplier.");
}

if(Predicates<int>::isDivisibleBy(5, list.getByString(i).toInt())){
    Serial.println("Yay, The Number is Divisible by the specified number");
}
```

## Customization
You can modify the Predicates class to include more operations as needed for your specific use case.

## Contribution
Feel free to fork this repository and contribute to expanding the library's functionalities.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


*** Please make sure you use the setGlobalMultiplier or sync() before utilizing the functions that need it. Otherwise you may get the wrong boolean value returned *** 

*** Please Make sure you add items to the Internal Array before using Mathmatical functions that have no arguments, otherwise you will likely end up getting the wrong boolean value ***



## List of Available Functions 
* setDebug(bool debug)
* isDebug()
* setGlobalMultiplier(int globalMultiplier)
* getGlobalMultiplier() 
* sync() //Syncs to the Operators class
* isEven(const T& value) 
* isOdd(const T& value)
* isPositive(const T& value) 
* isNegative(const T& value)
* isZero(const T& value)
* isNonZero(const T& value)
* isGreaterThan(T value) 
* isGreaterThan(T value, T value2)
* isLessThan(T value)
* isLessThan(T value, T value2)
* isEqual(T value)
* isEqual(T value, T value2)
* isGreaterThanOrEqualTo(const T& value, const T& threshold) 
* isLessThanOrEqualTo(const T& value, const T& threshold)
* isEqualTo(const T& value, const T& threshold)
* isNotEqualTo(const T& value, const T& threshold) 
* isBetween(const T& value, const T& lower, const T& upper) 
* isNotBetween(const T& value, const T& lower, const T& upper)
* isWithin(const T& value, const T& center, const T& radius) 
* isNotWithin(const T& value, const T& center, const T& radius) 
* isWithinExclusive(const T& value, const T& center, const T& radius)
* isNotWithinExclusive(const T& value, const T& center, const T& radius) 
* isWithinInclusive(const T& value, const T& center, const T& radius)
* isNotWithinInclusive(const T& value, const T& center, const T& radius) 
* isWithinLeftExclusive(const T& value, const T& center, const T& radius) 
* isNotWithinLeftExclusive(const T& value, const T& center, const T& radius) 
* isWithinRightExclusive(const T& value, const T& center, const T& radius) 
* isNotWithinRightExclusive(const T& value, const T& center, const T& radius) 
* isWithinLeftInclusive(const T& value, const T& center, const T& radius) 
* isNotWithinLeftInclusive(const T& value, const T& center, const T& radius) 
* isWithinRightInclusive(const T& value, const T& center, const T& radius) 
* isNotWithinRightInclusive(const T& value, const T& center, const T& radius) 
* isWithinLeft(const T& value, const T& center, const T& radius) 
* isNotWithinLeft(const T& value, const T& center, const T& radius) 
* isWithinRight(const T& value, const T& center, const T& radius) 
* isNotWithinRight(const T& value, const T& center, const T& radius)
* isWithinInclusiveLeft(const T& value, const T& center, const T& radius) 
* isNotWithinInclusiveLeft(const T& value, const T& center, const T& radius) 
* isWithinInclusiveRight(const T& value, const T& center, const T& radius) 
* isNotWithinInclusiveRight(const T& value, const T& center, const T& radius) 
* isWithinExclusiveLeft(const T& value, const T& center, const T& radius) 
* isNotWithinExclusiveLeft(const T& value, const T& center, const T& radius) 
* isWithinExclusiveRight(const T& value, const T& center, const T& radius) 
* isPrime(T value)
* isDivisibleBy(T value) 
* isDivisibleBy(T value, T divisor) 
* isDivisibleByTwo(T value) 
   