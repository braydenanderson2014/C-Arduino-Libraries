# ArrayList Class

The `ArrayList` class is a C++ template class that provides an implementation of a ArrayList for easy storing of a Value of any designated type. It allows you to efficiently store and retrieve data using indexes, similar to an ArrayList in Java.

## Overview

The `ArrayList` class is designed to offer a dynamic and scalable ArrayList with the following features:

- **Dynamic Sizing**: The List can dynamically resizes itself when the ArrayList becomes full. (Only if you specify "DYNAMIC" in the constructor)
- **Fixed Size**: The List can remain a fixed size when specified in the constructor.
- **Iterator Support**: You can iterate through the Values in the List.

## WARNING: This Library Utilizes POINTERS * to Memory. This is due to the Libraries ability to utilize any return type. (Bool, String, int, float, etc)

## ADDITIONAL WARNING: Please Ensure you have initialized the Serial Interface (Serial.begin()) before using debug mode. 

## Please Ensure you have Added the Arduino.h Header in your project. This is required to utilize this library!
### GENERAL INFO: 
This library is heavily based around the Java ArrayList Class. Almost all functions contained inside Java's ArrayList class have been implemented in this library. This does ***NOT*** mean it behaves exactly the same way. Some functions, due to the fact that they were designed for Arduino, are super simplistic in design. This means that sometimes, instead of returning a value, the library may just display a message instead. (ONLY IN DEBUG MODE). Please be Mindful of this behavior. If you are having issues with the library, please pass the debug boolean into the constructor, and keep track of the libraries output. This will be useful when reporting a bug. Thank you!
- Brayden Anderson (Developer)

### Key Features

- `Constructor(SizeType type, size_t initialSize, bool debug)`: Creates a new Instance of the ArrayList object (Debug enables the Serial Print statements. This is set to false by default so if you do not pass a true value in, no Print statements from this library should happen).
- `Destructor()` : Destructs the Instance of the library/ library object
- `add(T item)`: Adds an Item to a List
- `addAll(const ArrayList<T>& other)` : Add all elements from a different ArrayList into the current ArrayList
- `addAll(const T* other, size_t length)` : Add all elements from a different List type. Utilizes a pointer to that particular List.
- `insert(size_t index, T item)` : Insert an item at a certain index.
- `insertAll(size_t index, const ArrayList<T>& other)` : Inserts all elements of a different ArrayList into a certain start index, then moves to the next index until all items have been added.
- `insertAll(size_t index, const T* other, size_t length)` : Inserts all elements of a different list type (Utilizing a pointer) into a certain start index, then moves to the next index until all items have been added.
- `removeItem(T item)` : Removes an element by utilizing the actual element you want to remove.
- `removeIf(bool (*predicate)(T)) ` : Remove an item if the condition is true
- `removeRange(size_t fromIndex, size_t toIndex)` : Removes items from a specified start index to a specified end index.
- `retainAll(const ArrayList<T>& other)` : Removes all Elements in the ArrayList that do not match the ArrayList in the argument.
- `remove(size_t index)`: Removes the value stored at the specified index.
- `clear()`: Clears all values in the list.
- `contains(T item)` : Returns `true` if an element is contained inside the ArrayList or `false` if it is not.
- `indexOf(T item)` : Returns the index of the specified item.
- `capacity()`: Returns the capacity of the list.
- `size()`: Returns the number of elements in the list.
- `isEmpty()`: Checks if the list is empty.
- `set(size_t index, T item)` : Replaces the item stored at a certain index. Otherwise it adds it in.
- `replaceAll(T (*operatorFunction)(T)) ` : Replaces All items in the ArrayList with the result of applying a specified function to each element.
- `forEach(void (*consumer)(T))` : Type of Iterator that allows you to perform certain Functions as it Iterates.
- `toArray(T* outputArray)` : Transforms an ArrayList to a normal Array.
- `sublist(size_t fromIndex, size_t toIndex)` : Creates a new List out of the specified index range.
- `clone()` : Clones the ArrayList.
- `ensureCapacity(size_t minCapacity)` : resizes the ArrayList to the specified capacity.
- `trimToSize()` : This method is used to trim the capacity of an ArrayList instance to the list's current size (capacity of the ArrayList will be set to the number of elements currently stored)
- `sort(bool (*comparator)(T, T)) ` : Sorts the ArrayList according to the argument parameters
- `resize()` : Private function: resizes the List
- `removeAt(T item)` : Private Function: Removes an item at the index specified (remove function utilizes this function to work)



#### Specific Iterator Functions
- `begin()` : Defines the Iterator Start Position
- `end()` : Defines the Iterator Stop Position


#### **Please Note, You can use a Simple for-loop instead of the iterator. It will work the same. The iterator is only there for those who like to utilize them**

## Usage

To use the `ArrayList` class, follow these steps:

1. Include the `ArrayList.h` header in your C++ program.
2. Create an instance of the `ArrayList` class with specific data types for keys and values.
3. Use the provided methods to add, retrieve, or remove values from the list.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/Hashtable.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <ArrayList.h> 
```

## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1
* Updated the constructor to accept a boolean value to determine if the Serial.println should be used or not
* Added a bunch new functions to the library which have not been tested yet... These functions are based off of Java's ArrayList Class" 
* MASSIVE update to the Readme 
## Example

Here's an example of how to use the `ArrayList` class:

```cpp
#include <ArrayList.h>
#include <Arduino.h>
ArrayList<int> List(ArrayList<int>::DYNAMIC, 10);
ArrayList<String> myList(ArrayList<String>::FIXED, 10); // Fixed size, with a size of 10
ArrayList<double> doubleList(ArrayList<double>::Dynamic, 10, true); //Enables Debug mode
ArrayList<String> stringList(ArrayList<String>::FIXED, 10, false); //Keeps Debug mode Disabled.
ArrayList<bool> boolList;

int main() {
    List.add("Hello");
    List.add("World");
    List.add("!");

    for(int i = 0; i < 25; i ++){
        myList.add(i);
    }

    boolList.add(true);
    boolList.add(false);
    boolList.add(true);
    boolList.add(true);
    boolList.add(false);

    for(int i = 0; i < List.size(); i++){
        Serial.println(List.get(i));
    }

    for(int i = 0; i < myList.size(); i++){
        Serial.println(myList.get(i));
    }

    for(int i = 0; i < boolList.size(); i++){
        Serial.println(boolList.get(i));
    }



}

```


# ArrayListOperations Class: (To Be used with the ArrayList Library)

# ArrayListOperations Library for Arduino
## Overview
The ArrayListOperations library provides a comprehensive set of operations that can be performed on elements of an ArrayList. This library is designed for Arduino projects and offers a wide range of mathematical and utility functions.

## Features
* Operations on individual elements (e.g., increment, double, multiply by a factor)
* Mathematical operations (e.g., square, cube, square root, cube root)
* Boolean checks (e.g., isPrime, isEven, isOdd)
* Conversion functions (e.g., decimal to binary, binary to decimal)
* Statistical calculations (e.g., mean, median, mode, standard deviation, variance)
* Utility operations (e.g., find minimum, find maximum, count elements)
* Customizable global multiplier for scalable operations

## Installation
* Download the ArrayListOperations.h file.

* Place it in your Arduino project's directory.

* Include the library in your Arduino sketch:

```cpp
#include "ArrayListOperations.h"
```
## Usage
### Basic Operations
You can perform basic operations on array list elements:

```cpp
ArrayListOperations<int>::setGlobalMultiplier(5);
int result = ArrayListOperations<int>::multiply(10);  // result is 50
```

## Mathematical Operations
Perform mathematical operations like square, cube, or roots:

```cpp
int squared = ArrayListOperations<int>::square(4);  // squared is 16
```

## Boolean Checks
Check if a number meets certain criteria:

```cpp
bool isPrime = ArrayListOperations<int>::isPrime(7);  // isPrime is true
```

## Conversion Functions
Convert between different number systems:

```cpp
int binary = ArrayListOperations<int>::decimalToBinary(5);  // binary is 101
```

## Statistical Calculations
### Calculate statistical measures:

```cpp
int array[] = {1, 2, 3, 4, 5};
int mean = ArrayListOperations<int>::average(array, 5);  // mean is 3
```
## Customization
You can modify the ArrayListOperations class to include more operations as needed for your specific use case.

## Contribution
Feel free to fork this repository and contribute to expanding the library's functionalities.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


## List of Available Functions 
***(Please make sure you use the setGlobalMultiplier before utilizing the functions that need it. Otherwise its default value is 0)*** 
* incrementByOne(T element)
* doubleValue(T element)
* setGlobalMultiplier(T multiplier)
* multiply(T value)
* divide(T value)
* add(T value)
* subtract(T value)
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
* greatestCommonDivisor(T value)
* leastCommonMultiple(T value)
* isPrime(T value)
* isEven(T value)
* isOdd(T value)
* isPositive(T value)
* isNegative(T value)
* isZero(T value)
* isDivisibleBy(T value)
* isDivisibleBy(T value, T divisor)
* isDivisibleByTwo(T value)
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
* sum(T value)
* product(T value)
* findMin(T value)
* minimum(T* array, size_t size)
* findMax(T value)
* count(T value)