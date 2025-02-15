# Variant Class

## Overview
The Variant class is a template-based utility that can store either a single value of type T or a list of values using ArrayList<T>. This allows flexible storage of both individual elements and collections dynamically.

## Features
Supports both single-value storage and list storage.
Can be converted from a single value to a list dynamically.
Methods to set, retrieve, and modify values.
Provides a size() method to determine how many elements are stored.
Uses ArrayList<T> for list storage.

## Usage
Including the Header
Ensure that you include the required dependencies:

```cpp
#include "Variant.h"
#include "ArrayList.h"
```

## Creating a Variant
### Storing a Single Value
```cpp
Variant<int> singleVariant(42);
if (singleVariant.isSingle()) {
    Serial.println(singleVariant.getSingle()); // Output: 42
}
```

### Storing a List of Values
```cpp
ArrayList<int> numbers;
numbers.add(10);
numbers.add(20);
numbers.add(30);

Variant<int> listVariant(numbers);
if (listVariant.isArrayList()) {
    Serial.println(listVariant.getList().size()); // Output: 3
}
```
## Switching Between Single and List Mode
### Convert Single to List
```cpp
Variant<int> v(5);
v.getList().add(10); // Automatically converts to list
Serial.println(v.size()); // Output: 2
```
### Convert Back to Single
```cpp
v.setSingle(100);
Serial.println(v.getSingle()); // Output: 100
```
### Adding Elements to a List
```cpp
v.addValue(25);
v.addValue(50);
Serial.println(v.getList().size()); // Output: 3
```
### Clearing a Variant
```cpp
v.clear();
Serial.println(v.size()); // Output: 1 (Default initialized value)
```

## API Reference
### Method Description
| Method                  | Description                                                                 |
|-------------------------|-----------------------------------------------------------------------------|
| `//#define useSimpleVector` | When enabled, Variant switches from ArrayList to SimpleVector            |
| `Variant()`             | Default constructor (sets to single value mode).                            |
| `Variant(const T& value)` | Initializes with a single value.                                           |
| `Variant(const ArrayList<T>& values)` | Initializes with a list.                                       |
| `Variant(const SimpleVector<T>& values)` | Initializes with a list.                                    |
| `bool isSingle() const` | Returns true if storing a single value.                                     |
| `bool isArrayList() const` | Returns true if storing a list.                                           |
| `bool isSimpleVector() const` | Returns true if storing a list.                                        |
| `void setSingle(const T& value)` | Sets a single value, overriding list mode.                          |
| `void setList(const ArrayList<T>& values)` | Sets a list, overriding single value mode.                |
| `void setList(const SimpleVector<T>& values)` | Sets a list, overriding single value mode.             |
| `T getSingle() const`   | Returns the single value. If in list mode, returns the first element.       |
| `ArrayList<T>& getList()` | Returns the list. Converts to list mode if needed.                        |
| `const ArrayList<T>& getList() const` | Returns the list in a constant context.                       |
| `SimpleVector<T>& getList()` | Returns the list. Converts to list mode if needed.                     |
| `const SimpleVector<T>& getList() const` | Returns the list in a constant context.                    |
| `void addValue(const T& value)` | Adds a value to the list. Converts to list mode if needed.           |
| `int size() const`      | Returns the number of elements.                                             |
| `void clear()`          | Resets the Variant to a single default value.                               |


## Notes
When retrieving a single value while in list mode, getSingle() returns the first element.
getList() automatically converts a single value into a list if needed.
The class uses ArrayList<T> or SimpleVector<T> for internal list storage.

Uncomment the #define useSimpleVector to use the SimpleVectorLibrary instead of ArrayList. SimpleVector may use less memory overall but thats because its simpler and not as feature rich.

## 📜 **PlatformIO Changelog**
### Latest Version:
- **v1.0.0** [BETA] (2025-02-15)
             - Initial Release

### Previous Versions:


## 📜 **Arduino Changelog**
### Latest Version:
- **v1.0.0** [BETA] (2025-02-15) [ON-PAR] -> Platformio v1.0.0 [BETA]
             - Initial Release

### Previous Versions:




## License
This project follows the license specified in the repository.