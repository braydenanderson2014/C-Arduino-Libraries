# Optional Library for Arduino & PlatformIO

## Overview
The `Optional<T>` class is a lightweight implementation for handling optional values in **Arduino** and **PlatformIO** projects. This class provides a way to represent values that may or may not be present without using pointers or complex error-handling logic.

## Features
- ✅ **Lightweight**: No dependency on `std::optional`, making it ideal for embedded systems.
- ✅ **Safe Access**: Provides methods to check if a value exists before accessing it.
- ✅ **Works with Any Type**: Supports both primitive and custom types.
- ✅ **Compatible with Arduino & PlatformIO**.
- ✅ **Enhanced functionality**: Includes additional helper functions and operator overloads for better usability.

## Installation
### Arduino IDE
1. Download or clone this repository.
2. Copy `Optional.h` into your Arduino project's directory.
3. Use `#include "Optional.h"` in your sketch.

### PlatformIO
1. Create a `lib` folder in your PlatformIO project (if not present).
2. Copy `Optional.h` into `lib/Optional/Optional.h`.
3. Include the file in your source code: `#include "Optional.h"`.

## Usage Examples

### **Basic Usage**
```cpp
#include "Optional.h"

Optional<int> maybeValue(42);
if (maybeValue.hasValue()) {
    Serial.println(maybeValue.getValue());  // Output: 42
}

Optional<int> emptyValue;
if (!emptyValue.hasValue()) {
    Serial.println("No value present");
}
```

### **Advanced Usage**
#### **Using with Custom Types**
```cpp
struct Data {
    int x;
    float y;
};

Optional<Data> maybeData(Data{10, 5.5});
if (maybeData.hasValue()) {
    Data d = maybeData.getValue();
    Serial.print("x: ");
    Serial.println(d.x);
    Serial.print("y: ");
    Serial.println(d.y);
}
```

#### **Overriding Default Values**
```cpp
Optional<String> maybeString;
Serial.println(maybeString.orElse("Default String"));
```

#### **Using `ifPresent()` to Apply a Function**
```cpp
Optional<int> num(20);
num.ifPresent([](int x) { Serial.print("Value: "); Serial.println(x); });
// Output: Value: 20
```

#### **Swapping Two Optionals**
```cpp
Optional<int> a(5), b(10);
a.swap(b);
Serial.println(a.getValue()); // Output: 10
Serial.println(b.getValue()); // Output: 5
```

#### **Emplacing New Values**
```cpp
Optional<String> text;
text.emplace("Hello, Optional!");
Serial.println(text.getValue()); // Output: Hello, Optional!
```

## Changelog
### **v1.0.0**
- 🚀 Initial release.
- ✅ Supports `hasValue()` for checking availability.
- ✅ Provides `getValue()` with a safe fallback default.
- ✅ Works with **Arduino IDE** and **PlatformIO**.
- ✅ Supports **primitive** and **custom data types**.
- ✅ Added `reset()`, assignment operator, copy assignment, explicit `bool` conversion, `==` and `!=` operators.
- ✅ Added `orElse()`, `orElseGet()`, `ifPresent()`, `swap()`, `isDefaultValue()`, and `emplace()` functions.

## License
This project is open-source and licensed under the Apache License.

---

Enjoy using `Optional<T>` in your Arduino and PlatformIO projects! 🚀

