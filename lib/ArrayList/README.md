# ArrayList Class

The `ArrayList` class is a C++ template class that provides an implementation of a ArrayList for easy storing of a Value of any designated type. It allows you to efficiently store and retrieve data using indexes, similar to an ArrayList in Java. In more recent versions, you can sort the list, remove functions to cut down on memory, or use this library as a building block for your own code. 

## Overview

The `ArrayList` class is designed to offer a dynamic and scalable ArrayList with the following features:

- **Dynamic Sizing**: The List can dynamically resizes itself when the ArrayList becomes full. (This is the default behavior)
- **Fixed Size**: The List can remain a fixed size when specified in the constructor. (Must Be Passed into the Constructor, otherwise the list will be dynamic)
- **Iterator Support**: You can iterate through the Values in the List.

## WARNING: This Library Utilizes POINTERS * to Memory. This is due to the Libraries ability to utilize any return type. (Bool, String, int, float, etc)

## WARNING: This Library Utilizes Compiler Directives. This is to cut down on Memory usage if you need to do so. The compiler directives can remove groups of functions. This is not necessary on all platforms. By default the Arraylist will utilize its full configuration. You may also use build flags if you know what you are doing.

### GENERAL INFO: 
This library is heavily based around the Java ArrayList Class. Almost all functions contained inside Java's ArrayList class have been implemented in this library. This does ***NOT*** mean it behaves exactly the same way. Some functions, due to the fact that they were designed for Arduino, are super simplistic in design. This means that sometimes, instead of returning a value, the library may just display a message instead. (ONLY IN DEBUG MODE). Please be Mindful of this behavior. If you are having issues with the library, please pass the debug boolean into the constructor, and keep track of the libraries output. This will be useful when reporting a bug. Thank you!
- Brayden Anderson (Developer)

### Key Features
`Constructor(SizeType type, size_t initialSize, bool debug): ` - Creates a new Instance of the ArrayList object (Debug enables the Serial Print statements. This is set to false by default so if you do not pass a true value in, no Print statements from this library should happen).

`Destructor()` : Destructs the Instance of the library/ library object

`add(T item)` : Adds an Item to a List

`addAll(const ArrayList<T>& other)` : Add all elements from a different ArrayList into the current ArrayList

`addAll(const T* other, size_t length)` : Add all elements from a different List type. Utilizes a pointer to that particular List.

`insert(size_t index, T item) ` : Insert an item at a certain index.

`insertAll(size_t index, const ArrayList<T>& other)` : Inserts all elements of a different ArrayList into a certain start index, then moves to the next index until all items have been added.

`insertAll(size_t index, const T* other, size_t length)` : Inserts all elements of a different list type (Utilizing a pointer) into a certain start index, then moves to the next index until all items have been added.

`removeItem(T item)` : Removes an element by utilizing the actual element you want to remove.

`removeIf(bool (*predicate)(T))`  : Remove an item if the condition is true

`removeRange(size_t fromIndex, size_t toIndex)` : Removes items from a specified start index to a specified end index.

`retainAll(const ArrayList<T>& other) `: Removes all Elements in the ArrayList that do not match the ArrayList in the argument.

`remove(size_t index)`: Removes the value stored at the specified index.

`clear()`: Clears all values in the list.

`contains(T item)` : Returns true if an element is contained inside the ArrayList or false if it is not.

`indexOf(T item)` : Returns the index of the specified item.

`capacity()`: Returns the capacity of the list.

`size()`: Returns the number of elements in the list.

`isEmpty()`: Checks if the list is empty.

`set(size_t index, T item) `: Replaces the item stored at a certain index. Otherwise, it adds it in.

`replaceAll(T (*operatorFunction)(T))`  : Replaces All items in the ArrayList with the result of applying a specified function to each element.

`forEach(void (*consumer)(T)) `: Type of Iterator that allows you to perform certain Functions as it Iterates.

`toArray(T* outputArray)` : Transforms an ArrayList to a normal Array.

`sublist(size_t fromIndex, size_t toIndex) `: Creates a new List out of the specified index range.

`clone() `: Clones the ArrayList.

`clone(int initialCapacity)` : Clones the ArrayList... The clone will have the specified default capacity.

`ensureCapacity(size_t minCapacity) `: Resizes the ArrayList to the specified capacity.

`trimToSize()` : This method is used to trim the capacity of an ArrayList instance to the list's current size (capacity of the ArrayList will be set to the number of elements currently stored)

`sort(bool (*comparator)(T, T))` : Chooses which sort algorithm to use based on the current set Algorithm

`sort(bool (*comparator)(T, T), SortAlgorithm algorithm)`  : Chooses which sort algorithm to use based on the specified sort algorithm.

`setSortAlgorithm(SortAlgorithm algorithm = MERGE_SORT)` : Set the global Sort Algorithm

`getSortAlgorithm() `: Gets the Global Sort Algorithm

`mergeSort(ArrayList<T>& list, int l, int r)`: Recursively sorts an ArrayList using Merge Sort.

`quickSort(bool (*comparator)(T, T))`: Sorts an ArrayList using Quick Sort.

`bubbleSort(bool (*comparator)(T, T))` : Sorts an ArrayList using Bubble Sort.

`resize()` : Private function: determines if array can be resized based on mode, if it can, it will call the proper resize function.

`resize1()` : Private function: resizes the List (faster but less reliable).

`resize2()` : Private function that resizes the List (slower but more accurate).

`removeAt(T item) `: Private Function: Removes an item at the index specified (remove function utilizes this function to work).

`swap(T* a, T* b)` : Swaps two elements in the ArrayList.

`merge(ArrayList<T>& list, int l, int m, int r)` : Merges two halves of an ArrayList during Merge Sort.

`quickSortHelper(bool (*comparator)(T, T), int low, int high) `: Helper function for Quick Sort.

`partition(bool (*comparator)(T, T), int low, int high)`: Partitions the ArrayList for Quick Sort.

Specific Iterator Functions

`begin()` : Defines the Iterator Start Position.

`end()` : Defines the Iterator Stop Position.

Size and Capacity Management Functions

`setSizeType(SizeType type)`: Sets the size type of the ArrayList.

`getSizeType()` : Gets the size type of the ArrayList.

`setInitialSize(size_t size)`: Sets the initial size of the ArrayList.

`getInitialSize()`: Gets the initial size of the ArrayList.

`setCapacity(size_t capacity)` : (Advanced use only) Manually sets the capacity of the ArrayList.

`setCount(size_t count)` : (Advanced use only) Manually sets the count of items in the ArrayList.



#### Sort Options:
- BUBBLE_SORT : Uses the Bubble Sort Algorithm to sort the list
- QUICK_SORT : Uses the Quick Sort Alogrithm to sort the list
- MERGE_SORT : Uses the Merge Sort Alogrithm to sort the list

### Size Types:
- FIXED: Fixed size list that cannot be resized
- DYNAMIC: Dynamically resizes the list based off of the capacity and size factors.
- DYNAMIC2: More Reliable then DYNAMIC, but it is technically slower. [DEFAULT]


#### **Please Note, You can use a Simple for-loop instead of the iterator. It will work the same. The iterator is only there for those who like to utilize them**

## Usage

To use the `ArrayList` class, follow these steps:

1. Include the `ArrayList.h` header in your C++ program.
2. Create an instance of the `ArrayList` class with specific data types for keys and values.
3. Use the provided methods to add, retrieve, or remove values from the list.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/ArrayList.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <ArrayList.h> 
```

# ARDUINO
## ChangeLog
### Version 1.0.0:
* Initial Release
### Version 1.0.1-BETA:
* Added new MergeSort Function 
* Added new Merge Helper Function (PRIVATE)
* New Sort option (MergeSort) added to the Sort Functions.
* Added new Sort function that does not take in a Sort Paremeter. 
* Added new Set and Get Functions for sort Algorithm
### Version 1.0.1: 
* Added new resize2() function that is more reliable with Strings when copying data from 1 array to the other.
* Added new DYNAMIC2 option to the mode enum. This is now the default option for the Arraylist.
* Added new clone() function that allows you to specify the initial capacity of the clone.
### Version 1.0.2 - BETA:
* Fixed: [MERGED-PULL-REQUEST]: Fixed a bug in the set() function that wouldnt return a type boolean even when the function was supposed to return a type boolean.
### Version 1.0.2: [LATEST]
* Adjusted all add, inserts, etc. All resize calculation logic has been moved to separate functions.
* Renamed original resize() function to resize1() so a new resize() function can be enacted.
* Added new resize() function that runs the check for what type of ArrayList (DYNAMIC, FIXED, DYNAMIC2) then calls the appropriate resize function if applicable (FIXED does not resize)
* Added new verifyResizeNeeded() that checks the amount of items you want to add to see if the array can deal with it. It also checks the load factor. 
* Added New setSizeType() and getSizeType() functions so you can change the list mode.
* Fixed a newly discovered bug/Syntax error in the copy constructor. Calling variables that dont exist...
Due to an oversight, the copy constructor was calling wrong variables.
Example:
```cpp
this -> type = list.type;
```
instead of
```cpp
this -> sizeType = list.sizeType;
```
* Added new Compiler Directives. 
* Directive #define IKnowWhatIAmDoing (Commented Out): This will unlock 2 functions that have uses but can break your data in a real hurry if you dont know what you are doing.
* Directive #define SkinnyArray (Commented Out): If you uncomment directive #define SkinnyArray; a bunch of functions will disappear. 
* Directive #define OverrideCopyConstructor (Commented out, only available once SkinnyArray is present), This directive places the CopyConstructor back into existence.
* Directive #define OverrideAssignmentOperator (Commented out, only available once SkinnyArray is present), This directive places the Assignment operator back into existence.
* Directive #define OverrideSort (Commented out, only available once SkinnyArray is present), This directive places all sort functionalility back into existence.
* Directive #define OverrideAddAll (Commented out, only available once SkinnyArray is present), This directive places all AddAll Functions back into existence.
* Directive #define OverrideInsertAll (Commented out, only available once SkinnyArray is present), This directive places all insertAll Functions back into existence.
* Directive #define OverrideSpecialtyRemove (Commented out, only available once SkinnyArray is present), This directive places all predicate removal Functions back into existence.
* Directive #define OverrideUtilityFunctions (Commented out, only available once SkinnyArray is present), This directive places all Utility Functions back into existence. See below for details on this.
* If you use the SkinnyArray directive but you still want to use certain types of functions such as the AddAll functions or the Sort Functions, You can then uncomment the Override Directives.
* Resolved Issue [#96](https://github.com/braydenanderson2014/C-Arduino-Libraries/issues/96)
* Added the ability to get and set SizeType (Must have SkinnyArray Directive Disabled, or the Utility Functions Override Directive Enabled)


# PLATFORMIO 
## ChangeLog
### Version 1.0.0:
* Initial Release 
### Version 1.0.1:
* Updated the constructor to accept a boolean value to determine if the Serial.println should be used or not
* Added a bunch new functions to the library which have not been tested yet... These functions are based off of Java's ArrayList Class" 
* MASSIVE update to the Readme 
### Version 1.0.2:
* Added new SubLibrary ArrayListOperations which allows you to run mathmatics on your ArrayList. (Note: ArrayList *MUST* be an Integer or other number type ArrayList. Strings may not behave correctly);
* Added Documentation for new SubLibrary in this Readme
* Some functions in the ArrayListOperations are exactly the same, just with a different function name. Just be aware of this when utilizing the library.
### Version 1.0.3:
* Removed the ArrayListOperations SubLibrary. It has been moved to its own library called Operators
* Added new toString() function to the ArrayList Class
* Added new getAsString() function to return a String instead of a type T
* Deprecated the setDebug() and getDebug() functions. Library Messages will no longer be supported to save on memory
* Added Function Comments to all functions. This will help with the documentation of the library
* Adjusted the add function to utilize a load factor instead of it waiting to fill to capacity. This will help to ensure the array can be resized.
* Added new Sort Algorithm. (Quick Sort) 
* Modified the sort function so the user can select between bubble sort and quick sort.
* Fixed Library Changelog to reflect current State of Library Changes
### Version 1.0.4-BETA:
* Added new MergeSort Function 
* Added new Merge Helper Function (PRIVATE)
* New Sort option (MergeSort) added to the Sort Functions.
* Added new Sort function that does not take in a Sort Paremeter. 
* Added new Set and Get Functions for sort Algorithm
* Documentation Fix... Marked the library as 1.0.4 even though it was only on version 1.0.3.
### Version 1.0.4: 
* Added new resize2() function that is more reliable with Strings when copying data from 1 array to the other.
* Added new DYNAMIC2 option to the mode enum. This is now the default option for the Arraylist.
* Added new clone() function that allows you to specify the initial capacity of the clone.
### Version 1.0.5 - BETA: 
* Fixed: [MERGED-PULL-REQUEST]: Fixed a bug in the set() function that wouldnt return a type boolean even when the function was supposed to return a type boolean.
### Version 1.0.5: [LATEST]
* Adjusted all add, inserts, etc. All resize calculation logic has been moved to separate functions.
* Renamed original resize() function to resize1() so a new resize() function can be enacted.
* Added new resize() function that runs the check for what type of ArrayList (DYNAMIC, FIXED, DYNAMIC2) then calls the appropriate resize function if applicable (FIXED does not resize)
* Added new verifyResizeNeeded() that checks the amount of items you want to add to see if the array can deal with it. It also checks the load factor. 
* Added New setSizeType() and getSizeType() functions so you can change the list mode.
* Fixed a newly discovered bug/Syntax error in the copy constructor. Calling variables that dont exist...
Due to an oversight, the copy constructor was calling wrong variables.
Example:
```cpp
this -> type = list.type;
```
instead of
```cpp
this -> sizeType = list.sizeType;
```
* Added new Compiler Directives. 
* Directive #define IKnowWhatIAmDoing (Commented Out): This will unlock 2 functions that have uses but can break your data in a real hurry if you dont know what you are doing.
* Directive #define SkinnyArray (Commented Out): If you uncomment directive #define SkinnyArray; a bunch of functions will disappear. 
* Directive #define OverrideCopyConstructor (Commented out, only available once SkinnyArray is present), This directive places the CopyConstructor back into existence.
* Directive #define OverrideAssignmentOperator (Commented out, only available once SkinnyArray is present), This directive places the Assignment operator back into existence.
* Directive #define OverrideSort (Commented out, only available once SkinnyArray is present), This directive places all sort functionalility back into existence.
* Directive #define OverrideAddAll (Commented out, only available once SkinnyArray is present), This directive places all AddAll Functions back into existence.
* Directive #define OverrideInsertAll (Commented out, only available once SkinnyArray is present), This directive places all insertAll Functions back into existence.
* Directive #define OverrideSpecialtyRemove (Commented out, only available once SkinnyArray is present), This directive places all predicate removal Functions back into existence.
* Directive #define OverrideUtilityFunctions (Commented out, only available once SkinnyArray is present), This directive places all Utility Functions back into existence. See below for details on this.
* If you use the SkinnyArray directive but you still want to use certain types of functions such as the AddAll functions or the Sort Functions, You can then uncomment the Override Directives.
* Resolved Issue [#96](https://github.com/braydenanderson2014/C-Arduino-Libraries/issues/96)
* Added the ability to get and set SizeType (Must have SkinnyArray Directive Disabled, or the Utility Functions Override Directive Enabled)


** PLEASE NOTE THAT NEW FUNCTIONS HAVE NOT BEEN TESTED **


### This Library Pairs well with the following libraries
```#include <Predicates>```cpp (Boolean Predicates)
```#include <Operators>```cpp (Mathmatical Operators)



## Example

Here's an example of how to use the `ArrayList` class:

```cpp
#include <ArrayList.h>
#include <Arduino.h>
ArrayList<int> List(ArrayList<int>::DYNAMIC, 10); // ORIGINAL Default option, this uses memcpy to copy data from 1 array to another while resizing.
ArrayList<String> myList(ArrayList<String>::FIXED, 10); // Fixed size, with a size of 10 [CANNOT RESIZE AT ALL!!!!]
ArrayList<double> doubleList(ArrayList<double>::DYNAMIC2, 10); //DEFAULT OPTION: Specifies the use of resize2() function which is more reliable {BUT SLOWER!!!} 
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

## Available Compiler Directives
* //#define IKnowWhatIAmDoing //Uncomment this line if you know what you are doing; USE THE FUNCTIONS UNCOVERED BY THIS DIRECTIVE AT YOUR OWN RISK!!! [NOT-SUGGESTED]: Please only use the functions protected by this Directive being disabled if you know what you are doing.
* //#define SkinnyArray // Uncomment this line to remove more advanced functions to save memory or define it in your code [SUGGESTED] : If you are using super memory constrained devices and want to add a bunch of data to the arraylist.
=== Next Directives are Contigent on SkinnyArray Directive
* //#define OverrideCopyConstructor //Uncomment this line to override the copy constructor when in SkinnyArray Mode
* //#define OverrideAssignmentOperator //Uncomment this line to override the assignment operator when in SkinnyArray Mode
* //#define OverrideSort //Uncomment this line to override the sort function when in SkinnyArray Mode
* //#define OverrideAddAll //Uncomment this line to override the addAll function when in SkinnyArray Mode
* //#define OverrideInsertAll //Uncomment this line to override the insertAll function when in SkinnyArray Mode
* //#define OverrideSpecialtyRemove //Uncomment this line to override the remove function when in SkinnyArray Mode
* //#define OverrideUtilityFunctions //Uncomment this line to override the utility functions when in SkinnyArray Mode


