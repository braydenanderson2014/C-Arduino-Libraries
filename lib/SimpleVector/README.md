# SimpleVector

## Overview
**SimpleVector** is a lightweight and efficient dynamic array implementation for **Arduino and C++ projects**. It provides **fast and flexible** storage with **easy-to-use methods** for adding, removing, and accessing elements.

### Key Features:
✅ Dynamic resizing with memory management
✅ Supports **bulk addition**, **iterators**, and **sorting**
✅ STL-like interface for ease of use
✅ **Optimized for ESP32, ESP8266, and Arduino platforms**

## Installation
To use **SimpleVector**, simply include the header file:
```cpp
#include <SimpleVector.h>
```

## Sponsor This Project ❤️
If you find this project useful, consider supporting its development:
- **Patreon:** [Support on Patreon](https://www.patreon.com/posts/122298248)
- **GoFundMe:** [Donate via GoFundMe](https://gofund.me/923e5f10)
- **GitHub Sponsors:** Click the **"Sponsor"** button on the repository

Your contributions help keep this project alive and growing! 🚀

---

## API Reference
| Function | Description |
|----------|------------|
| **Constructors** | |
| `SimpleVector()` | Creates a new vector with an initial capacity of 4. |
| `SimpleVector(unsigned int initialCapacity)` | Creates a vector with a custom initial capacity. |
| `SimpleVector(const SimpleVector& other)` | Copy constructor. |
| **Destructor** | |
| `~SimpleVector()` | Destroys the vector and releases memory. |
| **Adding Elements** | |
| `void put(const T& item)` | Adds an item to the vector (alias: `push_back`). |
| `void push_back(const T& item)` | Alias for `put()`. |
| `void bulkAdd(Args... args)` | Adds multiple elements at once. |
| `void emplace_back()` | Adds a **default-constructed** element. |
| `void emplace_back(const T& value)` | Adds an element **by value**. |
| **Removing Elements** | |
| `void remove(const T& item)` | Removes an item by value. |
| `void erase(int index)` | Removes an item by index. |
| `void clear()` | Clears the vector and resets to default capacity. |
| `void clear(size_t newCapacity)` | Clears the vector and sets a new capacity. |
| **Accessing Elements** | |
| `T& get(unsigned int index)` | Retrieves an element at a given index. |
| `T* getPtr(unsigned int index)` | Returns a pointer to an element at an index. |
| `T& back()` | Returns the last element. |
| `T& operator[](unsigned int index)` | Access element at index using bracket notation. |
| `const T& operator[](unsigned int index) const` | Read-only access to elements. |
| **Capacity & Size** | |
| `unsigned int size() const` | Returns the total capacity. |
| `unsigned int elements() const` | Returns the number of elements. |
| `bool isEmpty() const` | Checks if the vector is empty. |
| `bool shrinkToFit()` | Shrinks capacity to match the element count. |
| **Searching** | |
| `int indexOf(const T& element)` | Finds the index of an element. |
| **Memory Management** | |
| `void releaseMemory()` | Manually releases allocated memory. |
| **Iterator Support** | |
| `SimpleVectorIterator begin()` | Returns an iterator to the first element. |
| `SimpleVectorIterator end()` | Returns an iterator to the last element. |


## Changelog

### Arduino Library Manager:

#### Version 1.0.0:
- Initial Release; On par with PlatformIO Version 1.0.5

#### Version 1.0.1:
- Added Support for Range-Based For Loops.
- On par with PlatformIO Version 1.0.6

#### Version 1.0.2:
- Removed unnecessary negative number check in `operator[]` since unsigned ints cannot be negative.
- On par with PlatformIO Version 1.0.8

#### Version 1.0.3 BETA:
- Added Assignment Operator (`operator=`)
- Added const Assignment Operator (`const operator=`)
- Fixed `clear()` function to delete internal array, then reinitialize with the default size.
- Fixed Destructor to properly check if array is `nullptr`.
- Added new `clear(size_t newCapacity)` function.
- On par with PlatformIO Version 1.0.9 - BETA

#### Version 1.0.3: 
- RESOLVED ISSUE: Compile fails for Wemos D1 mini clone (Issue #103)
    - This issue stemed from the Initializer list constructor. In order to resolve the issue, Initializer list is no longer supported. Instead template args are now supported (THEY HAVE NOT BEEN TESTED)  ```cpp template<typename... Args> ```
- In case you haven't noticed, the README.md file (this file) has been rebuilt and stylized.

#### Version 1.0.4: BETA [CURRENT RELEASE]
- Due to version mismatches in the arduino library manager, progressing forward 1 version to solve issue
    - Version 1.0.3-BETA was actually the most up to date version, while 1.0.3 was the one before hand. This version is to ensure the latest version is the latest version.


### PlatformIO Registry:

#### Version 1.0.0:
- Initial Release

#### Version 1.0.1:
- Added Remove Function.
- Renamed `push_back` function to `add`.
- Updated Example.

#### Version 1.0.2:
- Added `elements()` function.
- Added Iterators to the library.
- Modified `size()` to return the total array size instead of the number of elements.

#### Version 1.0.3:
- Added `[SIMPLE VECTOR]:` prefix to `Serial.print()` statements for better debugging.

#### Version 1.0.4:
- Modified Constructor to accept a boolean parameter to control debug messages.
- Added `setDebug()` to enable or disable debug output.
- Added `getDebug()` to retrieve debug status.

#### Version 1.0.5:
- Removed `Serial.print()` statements to save memory.
- Added function comments.

#### Version 1.0.6:
- Added Support for Range-Based For Loops.
- Added Support for Adding Elements During Construction.
- Added `bulkAdd()` function to add multiple elements at once.

#### Version 1.0.7 - BETA:
- Added `emplace_back()` function for proper dependency support in new libraries.
- Added Compiler Directives for board/platform detection to ensure correct initializer list behavior.
- If no compatible directives are found, initializer list support is **automatically disabled**.

#### Version 1.0.8:
- Removed unnecessary negative number check in `operator[]` since unsigned ints cannot be negative.

#### Version 1.0.9 - BETA: 
- Added Assignment Operator (`operator=`).
- Added const Assignment Operator (`const operator=`).
- Fixed `clear()` function to delete internal array, then reinitialize with the default size.
- Fixed Destructor to call `releaseMemory()` correctly.
- Updated `releaseMemory()` to ensure it properly sets array to `nullptr` after deletion.
- Added new `clear(size_t newCapacity)` function.

#### Version 1.0.9: [CURRENT RELEASE]
- RESOLVED ISSUE: Compile fails for Wemos D1 mini clone (Issue #103)
    - This issue stemed from the Initializer list constructor. In order to resolve the issue, Initializer list is no longer supported. Instead template args are now supported (THEY HAVE NOT BEEN TESTED)  ```cpp template<typename... Args> ```
- In case you haven't noticed, the README.md file (this file) has been rebuilt and stylized.


---

## Usage Examples

### **Basic Usage**
```cpp
#include <SimpleVector.h>
SimpleVector<int> numbers;
numbers.put(10);
numbers.put(20);
numbers.put(30);
Serial.println(numbers.get(1)); // Output: 20
```

### **Bulk Addition**
```cpp
SimpleVector<int> values;
values.bulkAdd(1, 2, 3, 4, 5);
```

### **Using Iterators**
```cpp
SimpleVector<int> nums;
nums.put(10);
nums.put(20);
nums.put(30);
for (auto it = nums.begin(); it != nums.end(); ++it) {
    Serial.println(*it);
}
```

### **Memory Management**
```cpp
SimpleVector<int> list;
list.put(100);
list.put(200);
list.releaseMemory(); // Frees memory
```

---

## License
This project is licensed under the **Apache License**. Feel free to use, modify, and contribute! 🚀

## Contributing
Pull requests and feature suggestions are welcome! Open an issue or submit improvements to help enhance the library.

**Happy coding! 🎯**
