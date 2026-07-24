# ArrayList Class

The `ArrayList` class is a C++ template class that provides an implementation of an ArrayList for easy storage of values of any designated type. It allows efficient storage and retrieval using indexes, similar to Java's `ArrayList`. The library supports sorting, memory optimizations through compiler directives, and advanced list operations.

## Overview

The `ArrayList` class is designed to offer a dynamic and scalable list with the following features:

- **Dynamic Resizing**: The list dynamically resizes itself when full (default behavior).
- **Fixed Size Option**: Can be instantiated with a fixed size to prevent resizing.
- **Iterator Support**: Supports iteration over elements.
- **Sorting Algorithms**: Offers multiple sorting algorithms (Bubble Sort, Quick Sort, Merge Sort).
- **Memory Optimization**: Uses compiler directives to include or exclude functionalities based on constraints.
- **Advanced Functionalities**: Provides bulk addition, removal, cloning, and capacity management.

## Important Notes

⚠️ **This library utilizes pointers (`*`)** for dynamic memory management.
⚠️ **Compiler Directives** are available to optimize memory usage and selectively enable/disable functions.
⚠️ **Known Issue** There is a known issue where if you place ArrayList in a struct, then create an ArrayList of that struct, 
recursion can occur. A work around would be to utilize lazy loading. (turn Arraylist into a pointer *) Another possible workaround
might be to use Variant instead. 

- **Patreon:** [Support on Patreon](https://www.patreon.com/posts/122298248)
- **GoFundMe:** [Donate via GoFundMe](https://gofund.me/923e5f10)
- **GitHub Sponsors:** Click the **"Sponsor"** button on the repository

# 📚 ArrayList API Reference

## 🛠️ Usage

To use the `ArrayList` class, include the library header and instantiate the list:

```cpp
#include <ArrayList.h>
ArrayList<int> myList(ArrayList<int>::DYNAMIC, 10); //This is a faster resize, but less reliable. Can cause problems.
ArrayList<int> myotherlist(ArrayList<int>::DYNAMIC2, 10); //This is a slower resize, but more reliable. 
ArrayList<String> strList(ArrayList<String>::FIXED, 5); //This can never be reized (while in fixed mode)
```
|Function	| Description |
|-----------|-------------|
| ArrayList(SizeType type = DYNAMIC2, size_t initialSize = 8);	| Creates an ArrayList with an optional initial size and size type (DYNAMIC, DYNAMIC2, FIXED). Default is DYNAMIC2. |
| ~ArrayList();	| Destructor releases allocated memory. |
| void add(T item);	| Adds an item to the list. |
| bool insert(size_t index, T item);	| Inserts an item at a given index. |
| bool removeItem(T item);	| Removes an item by value. |
| void remove(size_t index);	| Removes an item by index. |
| void clear();	| Clears all elements from the list. | 
| bool addAll(const ArrayList<T>& other);	| Adds all elements from another list. |
| bool insertAll(size_t index, const ArrayList<T>& other);	| Inserts all elements of another list at a given index. |
| bool removeIf(bool (*predicate)(T));	| Removes elements that satisfy a given condition. |
| void removeRange(size_t fromIndex, size_t toIndex);	| Removes elements within a specified range. |
| bool retainAll(const ArrayList<T>& other);	| Keeps only elements that exist in another list. |
| T get(size_t index) const;	| Retrieves an item at the given index. |
| String getAsString(size_t index) const; |	Retrieves an item as a string. |
| bool contains(T item) const;	| Checks if an item exists in the list. |
| size_t indexOf(T item) const;	| Returns the index of an item, or -1 if not found. |
| size_t size() const;	| Returns the number of elements in the list. |
| size_t capacity() const;	| Returns the maximum number of elements before resizing. | 
| bool isEmpty() const;	| Checks if the list is empty. |
| void sort(bool (*comparator)(T, T));	| Sorts the list using a comparator function. |
| void setSortAlgorithm(SortAlgorithm algorithm); |	Selects the sorting algorithm. |
| SortAlgorithm getSortAlgorithm();	| Retrieves the current sorting method. |
| void forEach(void (*consumer)(T)) const;	| Iterates over elements and applies a function. |
|-----------|-------------|
| Algorithm	| Description |
|-----------|-------------|
| BUBBLE_SORT	| Simple but inefficient sorting algorithm. |
| QUICK_SORT |	Fast sorting algorithm, good for large datasets. |
| MERGE_SORT |	Stable and reliable sorting method. |
|-----------|-------------|
| Function	| Description |
|-----------|-------------|
| void ensureCapacity(size_t minCapacity); |	Ensures the list has enough capacity. |
| void trimToSize();	| Reduces capacity to match the number of elements. |
| void setSizeType(SizeType type);	| Changes the list resizing mode. |
| SizeType getSizeType();	| Retrieves the current resizing mode. |
| void setInitialSize(size_t size);	| Sets the initial size of the list. | 
| size_t getInitialSize(); |	Retrieves the initial size.|
| ArrayList<T>* clone() const;	| Creates an identical copy of the list. |
| ArrayList<T>* clone(int InitialCapacity) const; |	Clones the list with a specified capacity. |
| ArrayList<T>* sublist(size_t fromIndex, size_t toIndex) const;	| Extracts a portion of the list. |
| ⚠️ Advanced Memory Control | (Use with Caution) |
|----------------------------|---------------------|
| void setCapacity(size_t capacity);	| Manually sets list capacity. ⚠️ Use with caution! |
| void setCount(size_t count);	| Manually sets the number of elements. ⚠️ Can cause unexpected behavior! |
| ⚙️ Compiler Directives for Optimization | ⚠️ Use with caution! — can also be set via PlatformIO `build_flags` |
|-----------|-------------|
| Directive | Description |
|-----------|-------------|
| `#define SkinnyArray` | Removes memory-intensive features to save flash/RAM. |
| `#define AL_NO_SERIAL` | Suppresses all Serial output from the library. |
| `#define OverrideSort` | Restores sorting functions when SkinnyArray is active. |
| `#define OverrideAddAll` | Restores bulk-add functions when SkinnyArray is active. |
| `#define OverrideInsertAll` | Restores bulk-insert functions when SkinnyArray is active. |
| `#define OverrideSpecialtyRemove` | Restores removeIf/removeRange/retainAll when SkinnyArray is active. |
| `#define OverrideUtilityFunctions` | Restores utility functions (clone, toArray, forEach…) when SkinnyArray is active. |
| `#define IKnowWhatIAmDoing` | Grants access to manual memory controls. ⚠️ Handle with extreme care! |

## Installation

Clone the repository and include the header file:
```sh
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/ArrayList.git"
```
Include in your code:
```cpp
#include <ArrayList.h>
```

---
This documentation provides a full API reference with updated function descriptions, usage examples, sorting algorithms, and compiler directive details for memory optimization. Let me know if you'd like further modifications! 🚀

# **Arduino & PlatformIO ArrayList Library**

## **Changelog**

### 🚀 **Version 1.0.0** - Initial Release

---

## **🔹 Arduino Version**

### 🏷 **v1.0.1-BETA**
- ➕ Added **MergeSort** function  
- 🔧 Added **Merge Helper** function *(PRIVATE)*  
- 🔄 Added new **Sort** option: `MergeSort`  
- 🛠 Introduced **Sort function** that doesn't require a sort parameter  
- ⚙️ Added `setSortAlgorithm()` and `getSortAlgorithm()` functions  

### 🏷 **v1.0.1**
- ➕ Added **resize2()** function for better handling of `String` data  
- 🔄 Introduced `DYNAMIC2` mode *(default mode for ArrayList now)*  
- 🛠 Added `clone()` function with **custom initial capacity**  

### 🏷 **v1.0.2-BETA**
- 🐛 **Fixed**: `set()` function was not returning a `boolean` as expected  

### 🏷 **v1.0.2** 
- 🔄 **Refactored** add, insert, and resize logic into **separate functions**  
- 🛠 Renamed `resize()` → `resize1()` *(to allow for a new `resize()` function)*  
- ✅ New `resize()` function now properly checks `ArrayList` type before resizing  
- 🔍 Introduced `verifyResizeNeeded()` to check item limits and load factor  
- ⚙️ Added `setSizeType()` and `getSizeType()` functions to modify `ArrayList` mode  

#### ✨ **Bug Fixes & Improvements**
- 🐛 **Fixed Copy Constructor Bug**:  
  - **Before**: `this -> type = list.type;` *(incorrect variable reference)*  
  - **Now**: `this -> sizeType = list.sizeType;` *(correct variable reference)*  

#### ⚙️ **New Compiler Directives**
- 🔥 `#define IKnowWhatIAmDoing` *(Unlocks advanced functions—use at your own risk!)*  
- 🏗 `#define SkinnyArray` *(Removes advanced functions to save memory)*  
- ✅ **Override Directives** *(Restore specific functions in SkinnyArray mode)*:  
  - `#define OverrideCopyConstructor` *(Brings back the copy constructor)*  
  - `#define OverrideAssignmentOperator` *(Brings back the assignment operator)*  
  - `#define OverrideSort` *(Restores all sorting functionality)*  
  - `#define OverrideAddAll` *(Restores `addAll()` functions)*  
  - `#define OverrideInsertAll` *(Restores `insertAll()` functions)*  
  - `#define OverrideSpecialtyRemove` *(Restores advanced remove functions)*  
  - `#define OverrideUtilityFunctions` *(Restores utility functions like `forEach()` and `toArray()`)*  

🔗 **Resolved Issue:** [#96](https://github.com/braydenanderson2014/C-Arduino-Libraries/issues/96)  
✅ **New Feature**: `getSizeType()` and `setSizeType()` *(Available only if `SkinnyArray` is disabled or `OverrideUtilityFunctions` is enabled)*  

**v1.0.3-BETA** *(Latest Stable)*
- **Resolved Issue** :  ArrayList(const ArrayList<T>& list) was improperly set up causing compiling issues.
- **Added Import** TypeTraits Library is now packaged. TBH: Not entirely sure why I added that import but there you go  
---

## **🔹 PlatformIO Version**

### 🏷 **v1.0.1**
- 🛠 Updated constructor to allow enabling/disabling `Serial.println`  
- 📝 **Major ReadMe Update**  
- ➕ Added multiple **new functions** *(inspired by Java's `ArrayList` class, but untested at the time)*  

### 🏷 **v1.0.2**
- ➕ Introduced **ArrayListOperations** *(Sub-library for mathematical operations on numeric ArrayLists)*  
- 📚 **Added Documentation** for the new sub-library  
- ⚠️ Some functions are duplicates but with different names—use with caution  

### 🏷 **v1.0.3**
- ❌ **Removed** `ArrayListOperations` *(Moved to its own library: **Operators**)*  
- 🛠 Added `toString()` and `getAsString()` functions  
- 📉 **Deprecated** `setDebug()` and `getDebug()` *(Removed debug messages to save memory)*  
- 📝 Added **detailed comments** to all functions  
- 🔄 **Improved Memory Handling**:  
  - The `add()` function now uses **load factor calculation** instead of waiting until full capacity  
- 🛠 Added **Quick Sort Algorithm** *(Selectable between Bubble Sort & Quick Sort)*  
- 🛠 Fixed **Changelog inconsistencies**  

### 🏷 **v1.0.4-BETA**
- ➕ Added **MergeSort** function  
- 🛠 Added **Merge Helper** function *(PRIVATE)*  
- 🔄 Added `MergeSort` as a sorting option  
- ✅ Introduced a **default Sort function** that doesn’t require a parameter  
- 🛠 Added `setSortAlgorithm()` and `getSortAlgorithm()`  

### 🏷 **v1.0.4**
- ➕ Added `resize2()` for better handling of `String` data  
- 🔄 Introduced `DYNAMIC2` mode *(default mode for `ArrayList` now)*  
- 🛠 Added `clone()` function with **custom initial capacity**  

### 🏷 **v1.0.5-BETA**
- 🐛 **Fixed:** `set()` function was incorrectly returning void instead of `boolean`  

### 🏷 **v1.0.5** 
- 🔄 **Refactored** add, insert, and resize logic into **separate functions**  
- 🛠 Renamed `resize()` → `resize1()` *(to allow for a new `resize()` function)*  
- ✅ New `resize()` function now properly checks `ArrayList` type before resizing  
- 🔍 Introduced `verifyResizeNeeded()` to check item limits and load factor  
- ⚙️ Added `setSizeType()` and `getSizeType()` functions to modify `ArrayList` mode  

🔗 **Resolved Issue:** [#96](https://github.com/braydenanderson2014/C-Arduino-Libraries/issues/96)  
✅ **New Feature**: `getSizeType()` and `setSizeType()` *(Available only if `SkinnyArray` is disabled or `OverrideUtilityFunctions` is enabled)*  

### **v1.0.7-BETA** *(Latest)*

#### 🐛 Bug Fixes
- **Fixed `operator[]` undefined behaviour** — previously returned a reference to a stack temporary (dangling ref). Now returns a reference to a reset static default value, preventing garbled Serial output.
- **Fixed `getReference()` shared static corruption** — the mutable static default value is now reset to `T()` on every out-of-bounds call, preventing corruption across calls.
- **Fixed `sort()` compile error for QUICK_SORT** — `sort()` was calling `quickSort` with the wrong number of arguments, causing a compile error whenever the Quick Sort path was reached. Now correctly calls `quickSort(comparator)`.
- **Fixed uninitialised `sortAlgorithm`** — the sort-algorithm member was not initialised in the constructor; it is now initialised to `MERGE_SORT`.
- **Fixed copy constructor not copying `initialSize` and `sortAlgorithm`**.

#### ✅ Memory Safety Improvements
- **Replaced all `memcpy` calls with element-by-element assignment** in `addAll`, `insertAll`, `toArray`, `ensureCapacity`, `trimToSize`, `resize1`. `memcpy` bypassed copy constructors and was unsafe for any `T` with non-trivial copy semantics (e.g. Arduino `String`, RAII types). Element-wise assignment calls the proper copy-assignment operator.
- **`clear()` no longer deallocates and reallocates** — it simply resets `count = 0`, preserving capacity and avoiding unnecessary heap churn that contributes to fragmentation.
- **Null checks added after every `new`** in `resize1`, `resize2`, `ensureCapacity`, `trimToSize`, and `merge`. If an allocation fails the list is left in its previous valid state.

#### ⚙️ New Compiler Directive
- **`AL_NO_SERIAL`** — define this to suppress all `Serial.println` calls emitted by the library. Useful when `Serial` has not been initialised in the sketch.  Can be set via PlatformIO `build_flags = -DAL_NO_SERIAL`.

#### 🔧 Code Restructuring (no behaviour change)
- **Compiler-directive blocks refactored** — eliminated ~800 lines of duplicated function bodies. Each `#ifndef SkinnyArray … #elif defined(OverrideXxx) … #endif` pattern has been replaced by the equivalent `#if !defined(SkinnyArray) || defined(OverrideXxx)` block containing the function body exactly once. Semantics are identical.
- **`operator=` now guarded by `OverrideAssignmentOperator`** — previously the assignment operator was compiled unconditionally even when `SkinnyArray` was defined, inconsistent with all other advanced features.
- **Renamed internal `swap` → `swapElements`** to avoid shadowing `std::swap` on platforms that import the standard library.

---

### **v1.0.6-BETA**
- **Resolved Issue** :  ArrayList(const ArrayList<T>& list) was improperly set up causing compiling issues.
- **Added Import** TypeTraits Library is now packaged. TBH: Not entirely sure why I added that import but there you go :) 
---

🚀 Enjoy using the **Arduino & PlatformIO ArrayList Library!** Let us know if you find any issues! 🎯



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

All directives can be either uncommented in `ArrayList.h` **or** passed as build flags so
you never need to edit the library source.

### Always-available directives

| Directive | Description |
|---|---|
| `#define SkinnyArray` | Removes memory-intensive features (sorting, bulk ops, cloning) to save flash/RAM on constrained devices. **Recommended** for very memory-limited hardware. |
| `#define AL_NO_SERIAL` | Suppresses all `Serial.println` calls emitted by the library. Use this when `Serial` has not been initialized in your sketch. |
| `#define IKnowWhatIAmDoing` | Unlocks `setCapacity()` and `setCount()` for low-level memory control. ⚠️ Use with extreme caution. |

### Override directives (contingent on `SkinnyArray` being defined)

These selectively restore feature groups that `SkinnyArray` would otherwise strip:

| Directive | Restores |
|---|---|
| `#define OverrideCopyConstructor` | Copy constructor |
| `#define OverrideAssignmentOperator` | Assignment `operator=` |
| `#define OverrideSort` | All sorting functions (Bubble, Quick, Merge Sort) |
| `#define OverrideAddAll` | `addAll()` functions |
| `#define OverrideInsertAll` | `insertAll()` functions |
| `#define OverrideSpecialtyRemove` | `removeIf()`, `removeRange()`, `retainAll()` |
| `#define OverrideUtilityFunctions` | `forEach()`, `toArray()`, `clone()`, `sublist()`, `ensureCapacity()`, `trimToSize()`, `setSizeType()`, `getSizeType()` |

### Using directives from PlatformIO (`platformio.ini`)

You do **not** need to uncomment anything in the library header.  Add `-D` flags to your
environment's `build_flags` instead:

```ini
[env:myboard]
platform = ...
board    = ...
framework = arduino
build_flags =
    -DSkinnyArray
    -DOverrideSort          ; bring back sorting even in SkinnyArray mode
    -DAL_NO_SERIAL          ; suppress Serial output
```

Any combination of the directives above is valid as a `-D` flag.


