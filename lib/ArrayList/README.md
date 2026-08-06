# ArrayList

Reference version: `1.0.8-BETA`


<!-- HEALTH_BADGES_START -->
[![Health: Potentially Unhealthy](https://img.shields.io/badge/Health-Potentially%20Unhealthy-d73a49?style=flat-square)](../../reports/library-health-report.md)
[![Testing: Unmanaged](https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square)](../../reports/library-health-report.md)
<!-- HEALTH_BADGES_END -->

`ArrayList` is a templated container for Arduino projects with fixed-size and dynamically growing modes, bulk operations, iterators, sorting, cloning, and manual capacity controls for advanced use cases.

## Overview

The goal of this library is to provide a Java-style `ArrayList` for Arduino code without forcing you into a single memory strategy. It supports fixed-capacity lists for predictable storage, dynamic growth for convenience, and several utility functions that make it practical as a general-purpose container.

## Quick Start

```cpp
#include <ArrayList.h>

ArrayList<int> dynamicList(ArrayList<int>::DYNAMIC2, 10);
ArrayList<String> fixedList(ArrayList<String>::FIXED, 5);
```

## Key Features

- Fixed or dynamically growing storage.
- Bulk add and insert operations from another `ArrayList` or a raw array.
- Multiple sorting algorithms with a shared comparator contract.
- Range-based for loop support through `begin()` and `end()`.
- Cloning, sublists, conversion helpers, and capacity controls.
- Compile-time directives for reducing memory usage in constrained builds.

## Behavior Notes

- `DYNAMIC` doubles capacity when growth is needed.
- `DYNAMIC2` grows by 1.5x when growth is needed.
- `FIXED` never resizes automatically.
- The comparator used by `sort()` should return `true` when the first value should come after the second value.
- `begin()` and `end()` return raw pointers, so range-based for loops work even when the backing buffer is empty.

## Installation

### Arduino Library Manager

Search for `ArduinoArrayList` in Arduino IDE's Library Manager and install the published release that matches the version listed above.

### PlatformIO

Add the library to `lib_deps` in your `platformio.ini`:

```ini
lib_deps =
    ArrayList
```

Pinning a version is recommended when you want reproducible builds:

```ini
lib_deps =
    ArrayList@1.0.8-BETA
```

### Updating

- Arduino IDE: open Library Manager, search for `ArrayList`, and install the newer release over the existing one.
- PlatformIO: update the `lib_deps` entry to the newer version and run a clean rebuild.
- Development repo: this repository is the issue tracker and working area; published package updates should be made through the remote library manager repositories.

### Include the header

```cpp
#include <ArrayList.h>
```

If you are using the local development copy, make sure the library is included from the `lib/` folder or added through your normal dependency workflow.

## API Reference

### Constructors and lifetime

| Function | Description |
|---|---|
| `ArrayList(SizeType type = DYNAMIC2, size_t initialSize = 8)` | Creates a list with the requested size policy and initial capacity. |
| `ArrayList(const ArrayList<T>& list)` | Deep-copy constructor. |
| `~ArrayList()` | Frees the backing array. |

### Core mutation

| Function | Description |
|---|---|
| `void add(T item)` | Appends one item. |
| `bool addAll(const ArrayList<T>& other)` | Appends all items from another `ArrayList`. |
| `bool addAll(const T* other, size_t length)` | Appends all items from a raw array. |
| `bool insert(size_t index, T item)` | Inserts one item at `index`. |
| `bool insertAll(size_t index, const ArrayList<T>& other)` | Inserts another `ArrayList` at `index`. |
| `bool insertAll(size_t index, const T* other, size_t length)` | Inserts a raw array at `index`. |
| `bool removeItem(T item)` | Removes the first matching value. |
| `void remove(size_t index)` | Removes the item at `index`. |
| `bool removeIf(bool (*predicate)(T))` | Removes items matching a predicate. |
| `void removeRange(size_t fromIndex, size_t toIndex)` | Removes the half-open range `[fromIndex, toIndex)`. |
| `bool retainAll(const ArrayList<T>& other)` | Keeps only items also found in `other`. |
| `void clear()` | Removes all items without releasing capacity. |

### Lookup and access

| Function | Description |
|---|---|
| `T get(size_t index) const` | Returns the value at `index`, or `T()` if out of bounds. |
| `T& getReference(size_t index)` | Mutable reference to the item at `index`, or a reset static default if out of bounds. |
| `const T& getReference(size_t index) const` | Const reference to the item at `index`, or a const default if out of bounds. |
| `T& operator[](size_t index)` | Indexing operator with the same out-of-bounds fallback as `getReference()`. |
| `String getAsString(size_t index) const` | Returns the item at `index` as an Arduino `String`. |
| `String toString(const T& value)` | Converts a value to `String`. |
| `bool contains(T item) const` | Returns whether `item` exists in the list. |
| `size_t indexOf(T item) const` | Returns the first matching index, or `INVALID_INDEX`. |

### Capacity and size

| Function | Description |
|---|---|
| `size_t capacity() const` | Returns current backing capacity. |
| `size_t size() const` | Returns the number of stored items. |
| `bool isEmpty() const` | Returns `true` when the list has no items. |
| `void ensureCapacity(size_t minCapacity)` | Expands capacity to at least `minCapacity`. |
| `void trimToSize()` | Shrinks capacity to match `size()`. |
| `void setSizeType(SizeType type)` | Changes the resize policy. |
| `SizeType getSizeType()` | Returns the current resize policy. |
| `size_t getInitialSize()` | Returns the constructor's initial capacity. |
| `void setInitialSize(size_t size)` | Updates the stored initial capacity value. |

### Sorting

| Function | Description |
|---|---|
| `void setSortAlgorithm(SortAlgorithm algorithm = MERGE_SORT)` | Sets the default algorithm used by the single-argument `sort()`. |
| `SortAlgorithm getSortAlgorithm()` | Returns the currently selected default algorithm. |
| `void sort(bool (*comparator)(T, T))` | Sorts using the selected default algorithm. |
| `void sort(bool (*comparator)(T, T), SortAlgorithm algorithm)` | Sorts using the chosen algorithm. |
| `void bubbleSort(bool (*comparator)(T, T))` | Bubble sort implementation. |
| `void quickSort(bool (*comparator)(T, T))` | Quick sort implementation. |
| `void mergeSort(ArrayList<T>& list, bool (*comparator)(T, T), int l, int r)` | Merge sort implementation. |

### Iteration and utility

| Function | Description |
|---|---|
| `T* begin() const` | Returns a pointer to the first element for range-based loops. |
| `T* end() const` | Returns a pointer one past the last element for range-based loops. |
| `void replaceAll(T (*operatorFunction)(T))` | Replaces each element with the result of a function. |
| `void forEach(void (*consumer)(T)) const` | Calls a function for each element. |
| `T* toArray(T* outputArray) const` | Copies all items into a caller-supplied array. |
| `ArrayList<T>* sublist(size_t fromIndex, size_t toIndex) const` | Returns a heap-allocated sublist. |
| `ArrayList<T>* clone() const` | Returns a deep copy with the same capacity. |
| `ArrayList<T>* clone(int InitialCapacity) const` | Returns a deep copy with a custom capacity. |

### Advanced memory controls

| Function | Description |
|---|---|
| `void setCapacity(size_t capacity)` | Directly overwrites the stored capacity. Requires `IKnowWhatIAmDoing`. |
| `void setCount(size_t count)` | Directly overwrites the stored count. Requires `IKnowWhatIAmDoing`. |

## Enumerations

### `SizeType`

| Value | Description |
|---|---|
| `FIXED` | Never auto-resizes. |
| `DYNAMIC` | Doubles capacity when growth is needed. |
| `DYNAMIC2` | Grows by 1.5x when growth is needed. |

### `SortAlgorithm`

| Value | Description |
|---|---|
| `BUBBLE_SORT` | Simple comparison-based sort. |
| `QUICK_SORT` | In-place quick sort. |
| `MERGE_SORT` | Stable merge sort. |

## Compile-Time Directives

| Directive | Effect |
|---|---|
| `SkinnyArray` | Removes memory-heavy features unless restored by overrides. |
| `AL_NO_SERIAL` | Suppresses `Serial.println()` error output from the library. |
| `IKnowWhatIAmDoing` | Enables manual capacity/count setters. |
| `OverrideCopyConstructor` | Restores the copy constructor when `SkinnyArray` is enabled. |
| `OverrideAssignmentOperator` | Restores copy assignment when `SkinnyArray` is enabled. |
| `OverrideSort` | Restores sorting APIs when `SkinnyArray` is enabled. |
| `OverrideAddAll` | Restores `addAll()` overloads when `SkinnyArray` is enabled. |
| `OverrideInsertAll` | Restores `insertAll()` overloads when `SkinnyArray` is enabled. |
| `OverrideSpecialtyRemove` | Restores `removeIf`, `removeRange`, and `retainAll` when `SkinnyArray` is enabled. |
| `OverrideUtilityFunctions` | Restores utility functions such as `clone`, `sublist`, `ensureCapacity`, and `trimToSize` when `SkinnyArray` is enabled. |

## Example

```cpp
#include <ArrayList.h>

bool descending(int a, int b) {
    return a < b;
}

ArrayList<int> values(ArrayList<int>::DYNAMIC2, 4);
values.add(4);
values.add(1);
values.add(3);
values.sort(descending, ArrayList<int>::MERGE_SORT);
```

## Support and Links

- Report bugs or request features in [GitHub Issues](https://github.com/braydenanderson2014/C-Arduino-Libraries/issues).
- Support development through [GitHub Sponsors](https://github.com/sponsors/braydenanderson2014).
- Alternate support links: [Patreon](https://www.patreon.com/posts/122298248) and [GoFundMe](https://gofund.me/923e5f10).

## Changelog

### 1.0.8-BETA

- Fixed `operator[]` undefined behavior by returning a reset static default reference for out-of-bounds access.
- Fixed `getReference()` shared static corruption by resetting the fallback value on each out-of-bounds call.
- Fixed `sort()` so the selected algorithm path now receives the comparator correctly.
- Initialized `sortAlgorithm` in the constructor.
- Copied `initialSize` and `sortAlgorithm` in the copy constructor and assignment operator.
- Replaced bulk-copy and growth paths with element-by-element assignment where needed to preserve non-trivial copy semantics.
- Changed `clear()` to preserve capacity instead of reallocating.
- Added null checks to the active growth and capacity-management paths.
- Restored `sort()` to respect the user comparator for merge sort.
- Guarded iterator accessors against dereferencing empty backing buffers.
- Hardened growth so empty or tiny capacities can still expand.
- Fixed bulk add/insert paths so they continue resizing until enough capacity exists.

## Notes

- The library uses dynamic allocation and pointer-based iteration.
- Bulk operations preserve non-trivial copy semantics by assigning elements one by one.
- If you enable `SkinnyArray`, re-enable only the features you need with the matching override directives.
