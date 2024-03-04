# TypeTraits Library for Arduino

## Overview

The TypeTraits library provides a collection of template structures to facilitate type checking and type traits identification in Arduino programming. It offers a simple, compile-time mechanism to query the characteristics of types, such as whether a type is arithmetic, a character, a string, an integral type, a floating-point type, an array, a pointer, a reference, const-qualified, volatile-qualified, or whether two types are the same.

You can use it in boolean operations, Serial statements, static_asserts(), etc...
## Features

- **Arithmetic Type Check**: Determine if a type is one of the basic arithmetic types (`int`, `float`, `double`, `long`, `byte`).
- **Character Type Check**: Check if a type is a `char`.
- **String Type Check**: Identify if a type is an Arduino `String`.
- **Integral Type Check**: Check if a type is an integral (`int`, `long`, `char`, `bool`).
- **Floating-Point Type Check**: Determine if a type is a floating-point type (`float`, `double`).
- **Array Type Check**: Identify if a type is an array.
- **Pointer Type Check**: Check if a type is a pointer, including const pointers.
- **Reference Type Check**: Determine if a type is a reference.
- **Const Qualification Check**: Identify if a type is const-qualified.
- **Volatile Qualification Check**: Check if a type is volatile-qualified.
- **Type Comparison**: Determine if two types are the same.

## Installation

1. Download the TypeTraits library.
2. Place the `TypeTraits.h` file into your Arduino project's library folder or a location accessible by your project.
3. Include the library in your sketch with `#include <TypeTraits.h>`.

## Usage Example

```cpp
#include <Arduino.h>
#include <TypeTraits.h>

void setup() {
    Serial.begin(9600);

    // Example usage of is_arithmetic
    Serial.println(is_arithmetic<int>::value); // Outputs: 1 (true)
    Serial.println(is_arithmetic<String>::value); // Outputs: 0 (false)
    static_assert(is_arithmetic<float>::value); // Throws compile error if condition is false; in this case the float is an 
    // arithmetic type .

    // Example usage of is_same
    Serial.println(is_same<int, int>::value); // Outputs: 1 (true)
    Serial.println(is_same<int, float>::value); // Outputs: 0 (false)
}

void loop() {
    // Your code here
}
```

## Contributing
We welcome contributions to improve the TypeTraits library. Please feel free to submit issues and pull requests.

## License
This library is open-source and available under the Apache license. See the LICENSE file for more information.


This README provides a basic introduction, installation instructions, usage examples, and contribution guidelines. Adjust the content as necessary to match the specifics of your library and its usage.


# Arduino Library Manager
## ChangeLog:
### Version 1.0.0-ALPHA:
* Initial ALPHA Release.


# PlatformIO Registry
## ChangeLog:
### Version 1.0.0-ALPHA:
* Initial ALPHA Release.