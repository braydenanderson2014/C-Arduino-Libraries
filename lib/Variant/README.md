# Variant Class


<!-- HEALTH_BADGES_START -->
[![Health: Stable](https://img.shields.io/badge/Health-Stable-2ea44f?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Tested](https://img.shields.io/badge/Testing-Tested-2ea44f?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

## Overview
The Variant class is a template-based utility that can store either a single value of type T or a list of values using ArrayList<T>. This allows flexible storage of both individual elements and collections dynamically.

## Features
Supports both single-value storage and list storage.
Can be converted from a single value to a list dynamically.
Methods to set, retrieve, and modify values.
Provides a size() method to determine how many elements are stored.
Uses ArrayList<T> for list storage.

## What's new in 1.0.2
- Refined the Variant API documentation and comments for the single-value and list-mode flows.
- Clarified the behavior of `setSingle()`, `setList()`, `getSingle()`, `getList()`, and `addValue()` in the reference docs.
- Added broader regression-style host simulation coverage around constructor, conversion, and mode-transition behavior.

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
| Method | Availability | Description |
|--------|--------------|-------------|
| `//#define useSimpleVector` | Compile-time option | Switches internal list storage from `ArrayList<T>` to `SimpleVector<T>`. |
| `Variant()` | Always | Default constructor. Initializes in single-value mode with `T()`. |
| `Variant(const T& value)` | Always | Initializes in single-value mode with `value`. |
| `Variant(const ArrayList<T>& values)` | When `useSimpleVector` is **not** defined | Initializes in list mode with ArrayList data. |
| `Variant(const SimpleVector<T>& values)` | When `useSimpleVector` **is** defined | Initializes in list mode with SimpleVector data. |
| `bool isSingle() const` | Always | Returns `true` when storing a single value, `false` when in list mode. |
| `bool isArrayList() const` | When `useSimpleVector` is **not** defined | Returns `true` when in list mode. |
| `bool isSimpleVector() const` | When `useSimpleVector` **is** defined | Returns `true` when in list mode. |
| `void setSingle(const T& value)` | Always | Switches to single-value mode, stores `value`, and clears backing list state. |
| `void setList(const ArrayList<T>& values)` | When `useSimpleVector` is **not** defined | Switches to list mode and assigns ArrayList contents. |
| `void setList(const SimpleVector<T>& values)` | When `useSimpleVector` **is** defined | Switches to list mode and assigns SimpleVector contents. |
| `T getSingle() const` | Always | Returns stored single value. If in list mode, returns first list element (`get(0)`). |
| `ArrayList<T>& getList()` | When `useSimpleVector` is **not** defined | Returns mutable list. If currently single, converts to list and seeds with current single value. |
| `const ArrayList<T>& getList() const` | When `useSimpleVector` is **not** defined | Returns list reference in const context (does not trigger conversion). |
| `SimpleVector<T>& getList()` | When `useSimpleVector` **is** defined | Returns mutable list. If currently single, converts to list and seeds with current single value. |
| `const SimpleVector<T>& getList() const` | When `useSimpleVector` **is** defined | Returns list reference in const context (does not trigger conversion). |
| `void addValue(const T& value)` | Always | If currently single, converts to list by seeding with single value first, then appends `value`. |
| `int size() const` | Always | Returns `1` in single mode, otherwise list element count. |
| `void clear()` | Always | Resets to single mode with default `T()` and clears backing list state. |


## Notes
When retrieving a single value while in list mode, getSingle() returns the first element.
getList() automatically converts a single value into a list if needed.
The class uses ArrayList<T> or SimpleVector<T> for internal list storage.

Uncomment the #define useSimpleVector to use the SimpleVectorLibrary instead of ArrayList. SimpleVector may use less memory overall but thats because its simpler and not as feature rich.

## 📜 **PlatformIO Changelog**
### Latest Version:
- **v1.0.2**  (2026-07-29)
             - Refined the Variant API documentation and comments for single-value and list-mode flows.
             - Clarified `setSingle()`, `setList()`, `getSingle()`, `getList()`, and `addValue()` behavior in the reference docs.
             - Added broader regression-style host simulation coverage around constructor, conversion, and mode-transition behavior.

### Previous Versions:
- **v1.0.1**  (2026-07-25)
             - Fixed mode-switch behavior in `setSingle()` so stale list data is cleared when returning to single-value mode.
             - Fixed `addValue()` single-to-list conversion to reset prior backing list state before seeding with the single value.
             - Restored `setList()` overload compatibility for `useSimpleVector` mode.
             - Added broader regression test coverage for constructor, conversion, and mode-transition flows.
- **v1.0.0**  (2025-02-15)
             - Initial Release


## 📜 **Arduino Changelog**
### Latest Version:
- **v1.0.2** (2026-07-29) [ON-PAR] -> Platformio v1.0.2 [BETA]
             - Refined the Variant API documentation and comments for single-value and list-mode flows.
             - Clarified `setSingle()`, `setList()`, `getSingle()`, `getList()`, and `addValue()` behavior in the reference docs.
             - Added broader regression-style host simulation coverage around constructor, conversion, and mode-transition behavior.

### Previous Versions:
- **v1.0.1** (2026-07-25) [ON-PAR] -> Platformio v1.0.1 [BETA]
             - Fixed mode-switch behavior in `setSingle()` so stale list data is cleared when returning to single-value mode.
             - Fixed `addValue()` single-to-list conversion to reset prior backing list state before seeding with the single value.
             - Restored `setList()` overload compatibility for `useSimpleVector` mode.
             - Added broader regression test coverage for constructor, conversion, and mode-transition flows.
- **v1.0.0** (2025-02-15) [ON-PAR] -> Platformio v1.0.0 [BETA]
             - Initial Release




## License
This project follows the license specified in the repository.
