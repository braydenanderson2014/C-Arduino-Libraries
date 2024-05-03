# Numeric Limits Library for Arduino
## Overview
The Numeric Limits library provides a way to determine the properties and limits of arithmetic types in the Arduino environment. It covers a range of types including integers, floating-point numbers, and bytes, offering functionality similar to std::numeric_limits from the C++ Standard Library but tailored for Arduino's constraints and types.

## Features
* Type Limits: Obtain minimum and maximum values for various data types.
* Type Properties: Check whether a type is signed, integer, or floating-point.
* Compatibility: Designed with Arduino's 32-bit architecture in mind, ensuring compatibility with common Arduino boards.

## Usage
To use the Numeric Limits library in your Arduino sketch, include it at the beginning of your code:

```cpp
#include "Numeric_Limits.h"
```

## Examples
### Getting Integer Limits
```cpp

int intMin = numeric_limits<int>::Min();
int intMax = numeric_limits<int>::Max();
Serial.print("Int Min: ");
Serial.println(intMin);
Serial.print("Int Max: ");
Serial.println(intMax);
```

### Floating-Point Limits
```cpp

float floatMin = numeric_limits<float>::Min();
float floatMax = numeric_limits<float>::Max();
Serial.print("Float Min: ");
Serial.println(floatMin, 10); // '10' specifies the number of digits after the decimal point
Serial.print("Float Max: ");
Serial.println(floatMax, 10);
```
### Checking Type Properties
```cpp

bool isIntSigned = numeric_limits<int>::is_signed();
Serial.print("Is Int Signed: ");
Serial.println(isIntSigned ? "Yes" : "No");
```
## Supported Types
The library provides implementations for the following types:

* int
* long
* float
* double
* byte
* char
* bool

Note: On Arduino, double is often equivalent to float, reflected in their shared limits.

## Supported Boards
The library supports the following boards/computers by default: (This library should auto detect the board if it is able to match it up...)

* Arduino Giga (Must have the following build flag -> -D ARDUINO_GIGA... (This is based off of a custom board definition))
* x86 32-bit (_i386_, _M_IX86): Commonly used in desktops, laptops, and servers with 32-bit Intel or AMD processors.
* x86 64-bit (_x86_64_, _M_X64): Commonly used in modern desktops, laptops, and servers with 64-bit Intel or AMD processors.
* ARM 32-bit (_ARM_, _M_ARM): Supports a wide range of microcontrollers and microprocessors, including those used in many embedded systems, smartphones, tablets, and some lower-end laptops like Chromebooks.
* MIPS 32-bit (_MIPS_, _M_MIPS): Used in various embedded systems, including routers, IoT devices, and some older computing systems.
* PowerPC 32-bit (_PPC_, _M_PPC): Found in older Apple Macintosh computers, some game consoles, and embedded systems.
* RISC-V 32-bit (_RISCV_, _M_RISCV): An open standard ISA used in various microcontrollers and processors for embedded systems, IoT devices, and some specific computing systems.
* AVR 8-bit (_AVR_, _M_AVR): Primarily used in Arduino boards like Arduino Uno, Mega, and Nano, which are popular in hobbyist and educational electronics.
* ARC 32-bit (_ARC_, _M_ARC): Used in some embedded systems, particularly those requiring customization of the core.
* SAMD21G18A 32-bit ARM (_SAMD21G18A_, _M_SAMD21G18A): Used in Arduino Zero and similar boards, suitable for more complex applications requiring low power consumption.
* ESP8266 32-bit ARM (_ESP8266_, _M_ESP8266): Popular in IoT devices, known for Wi-Fi capabilities.
* ESP32 32-bit ARM (_ESP32_, _M_ESP32): Successor to the ESP8266, known for its Wi-Fi and Bluetooth capabilities, used in a wide range of IoT and smart home devices.
* STM32F1, STM32F4, STM32F7, STM32H7, STM32L4 (_STM32F1_, _M_STM32F1, etc.): A wide range of STM32 microcontrollers from STMicroelectronics, used in professional and hobbyist projects for their balance of power and cost.
* Portenta H7 M7 Core (ARDUINO_PORTENTA_H7_M7, _M_PORTENTA_H7_M7): High-performance boards designed for industrial applications, featuring dual-core processors.
* Custom Architecture (CUSTOM_ARCHITECTURE): The code provides an option to define a custom architecture, indicating flexibility to support virtually any board or computer by customizing the architecture-specific definitions.

# ARDUINO:
## ChangeLog
### Version 1.0.0-ALPHA:
* Initial Release
## Version 1.0.0-BETA:
* Added Support for many different Architectures including Arduino Giga.

# PLATFORMIO:
## ChangeLog
### Version 1.0.0-ALPHA:
* Initial Release
## Version 1.0.0-BETA:
* Added Support for many different Architectures including Arduino Giga.


## Instructions for use..
1. Include the Numeric_Limits.h header in your code.
2. This library is designed to auto detect your board based off of pre defined macros... if it is able to find the board, the compilation will fail (Unless its in Custom Architecture Mode...); Now you can put the library in Custom Architecture mode by uncommenting "//#define CUSTOM_ARCHITECTURE // Uncomment this line if you want to use a custom architecture"... this will put the library into CUSTOM_ARCHITECTURE mode...

* Auto Mode
The library will automatically select your board for you if it is able to match it up. If it is not and the library is not in Custom Architecture mode... the library will not compile due to Static_Assertions. This is to make sure the library works correctly.

* Custom_Architecture Mode
If you are using this mode please make sure you uncomment the define custom architecture line... Once the library is in the Custom Architecture mdoe, you can define your architecture limits in the CustomArchitecture.h file. The template is already filled out you just need to fill it out. 
Once done, the library is ready for use.

3. Using the Library...
* Examples: 
``` cpp
float floatMin = numeric_limits<float>::Min();
float floatMax = numeric_limits<float>::Max();
```

## Contribution
Contributions to the Numeric Limits library are welcome. Whether it's through adding support for additional types, enhancing the existing functionality, or improving compatibility across various Arduino platforms, your contributions are valuable to the Arduino community.

## License
This library is open-source and can be freely used and modified in both personal and commercial projects under the terms specified in the license accompanying the library source.