#ifndef NUMERIC_LIMITS_H
#define NUMERIC_LIMITS_H

// COMMENT THE FOLLOWING LINE IF YOU DON'T WANT TO USE A CUSTOM ARCHITECTURE, AND UNCOMMENT THE LINE BELOW IF YOU WANT TO USE A CUSTOM ARCHITECTURE
//#define CUSTOM_ARCHITECTURE // Uncomment this line if you want to use a custom architecture
/*

0*/


#if defined(_i386_) || defined(_M_IX86) // x86 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_x86_64_) || defined(_M_X64) // x86 64-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-2147483648)   
    #define INT_MAX 2147483647

    #define LONG_MIN (-9223372036854775808L)
    #define LONG_MAX 9223372036854775807L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_ARM_) || defined(_M_ARM) // ARM 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_MIPS_) || defined(_M_MIPS) // MIPS 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_PPC_) || defined(_M_PPC) // PowerPC 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_RISCV_) || defined(_M_RISCV) // RISC-V 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_AVR_) || defined(_M_AVR) // AVR (Arduino) - 8-bit 
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_ARC_) || defined(_M_ARC) // ARC - 32-bit
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_SAMD21G18A_) || defined(_M_SAMD21G18A) // SAMD21G18A (Arduino Zero) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_ESP8266_) || defined(_M_ESP8266) // ESP8266 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_ESP32_) || defined(_M_ESP32) // ESP32 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_STM32F1_) || defined(_M_STM32F1) // STM32F1 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_STM32F4_) || defined(_M_STM32F4) // STM32F4 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_STM32F7_) || defined(_M_STM32F7) // STM32F7 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_STM32H7_) || defined(_M_STM32H7) // STM32H7 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(_STM32L4_) || defined(_M_STM32L4) // STM32L4 (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(_M_PORTENTA_H7_M7) // Portenta H7 M7 Core (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_arc_) || defined(_M_ARC) // ARC
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_riscv_) || defined(_M_RISCV) // RISC-V
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX

#elif defined(_mips_) || defined(_M_MIPS) // MIPS
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined (_AVR_ATmega2560_)
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#elif defined (ARDUINO_GIGA) || defined(_M_GIGA) // GIGA (Arduino) - 32-bit ARM
    #define CHAR_MIN (-128)
    #define CHAR_MAX 127

    #define BYTE_MIN 0
    #define BYTE_MAX 255

    #define INT_MIN (-32768)
    #define INT_MAX 32767

    #define LONG_MIN (-2147483648L)
    #define LONG_MAX 2147483647L

    #define FLT_MIN 1.175494351e-38F
    #define FLT_MAX 3.402823466e+38F

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX
#else
    #ifndef CUSTOM_ARCHITECTURE
        static_assert(false, "Unknown architecture");
        static_assert(false, "Please define the min and max values for your architecture");
        #define CHAR_MIN 0
        #define CHAR_MAX 0

        #define BYTE_MIN 0
        #define BYTE_MAX 0

        #define INT_MIN 0
        #define INT_MAX 0

        #define LONG_MIN 0
        #define LONG_MAX 0

        #define FLT_MIN 0
        #define FLT_MAX 0

        #define DBL_MIN 0
        #define DBL_MAX 0
    #else
        #include "CustomArchitecture.h" // Include your custom architecture file
    #endif
#endif




#include <TypeTraits.h> // AssuMing this is your custom type traits library

template <typename T>
struct numeric_limits {
    static T Min() { return T(); }
    static T Max() { return T(); }
    static bool is_signed() { return false; }
    static bool is_integer() { return false; }
};

template <>
struct numeric_limits<int> {
    static int Min() { return INT_MIN; }
    static int Max() { return INT_MAX; }
    static bool is_signed() { return true; }
    static bool is_integer() { return true; }
};

template <>
struct numeric_limits<float> {
    static float Min() { return FLT_MIN; }
    static float Max() { return FLT_MAX; }
    static bool is_signed() { return true; }
    static bool is_integer() { return false; }
};

template <>
struct numeric_limits<double> {
    static double Min() { return DBL_MIN; }
    static double Max() { return DBL_MAX; }
    static bool is_signed() { return true; }
    static bool is_integer() { return false; }
};

template <>
struct numeric_limits<long> {
    static long Min() { return LONG_MIN; }
    static long Max() { return LONG_MAX; }
    static bool is_signed() { return true; }
    static bool is_integer() { return true; }
};

template <>
struct numeric_limits<byte> {
    static byte Min() { return 0; }
    static byte Max() { return 255; }
    static bool is_signed() { return false; }
    static bool is_integer() { return true; }
};

template <>
struct numeric_limits<char> {
    static char Min() { return CHAR_MIN; }
    static char Max() { return CHAR_MAX; }
    static bool is_signed() { return (char)-1 < 0; } // True if char is signed
    static bool is_integer() { return true; }
};

template <>
struct numeric_limits<bool> {
    static bool Min() { return false; }
    static bool Max() { return true; }
    static bool is_signed() { return false; }
    static bool is_integer() { return true; }
};

#endif // NUMERIC_LIMITS_H
