#ifndef NUMERIC_LIMITS_H
#define NUMERIC_LIMITS_H

#include <stddef.h>

// COMMENT THE FOLLOWING LINE IF YOU DON'T WANT TO USE A CUSTOM ARCHITECTURE, AND UNCOMMENT THE LINE BELOW IF YOU WANT TO USE A CUSTOM ARCHITECTURE
//#define CUSTOM_ARCHITECTURE // Uncomment this line if you want to use a custom architecture
/*

0*/


#define NUMERIC_LIMITS_BOARD_NAME_UNKNOWN "unknown"
#define NUMERIC_LIMITS_SRAM_UNKNOWN 0UL
#define NUMERIC_LIMITS_PROGRAM_SPACE_UNKNOWN 0UL

#if defined(ARDUINO_AVR_UNO) || defined(__AVR_ATmega328P__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Uno"
    #define NUMERIC_LIMITS_SRAM_BYTES 2048UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 32256UL
#elif defined(ARDUINO_AVR_NANO) || defined(__AVR_ATmega328P__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Nano"
    #define NUMERIC_LIMITS_SRAM_BYTES 2048UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 30720UL
#elif defined(ARDUINO_AVR_MINI) || defined(__AVR_ATmega328P__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Pro Mini"
    #define NUMERIC_LIMITS_SRAM_BYTES 2048UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 30720UL
#elif defined(ARDUINO_AVR_LEONARDO) || defined(__AVR_ATmega32U4__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Leonardo"
    #define NUMERIC_LIMITS_SRAM_BYTES 2560UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 28672UL
#elif defined(ARDUINO_AVR_MICRO) || defined(__AVR_ATmega32U4__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Micro"
    #define NUMERIC_LIMITS_SRAM_BYTES 2560UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 28672UL
#elif defined(_AVR_ATmega2560_) || defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Mega 2560"
    #define NUMERIC_LIMITS_SRAM_BYTES 8192UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 258048UL
#elif defined(ARDUINO_AVR_MEGA) || defined(__AVR_ATmega1280__)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Mega"
    #define NUMERIC_LIMITS_SRAM_BYTES 8192UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 126976UL
#elif defined(ARDUINO_ARCH_RENESAS) || defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_ARCH_RENESAS_UNO)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Uno R4"
    #define NUMERIC_LIMITS_SRAM_BYTES 32768UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 262144UL
#elif defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_MBED_NANO)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Nano RP2040 Connect"
    #define NUMERIC_LIMITS_SRAM_BYTES 270336UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 16777216UL
#elif defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ARCH_RP2040)
    #define NUMERIC_LIMITS_BOARD_NAME "RP2040"
    #define NUMERIC_LIMITS_SRAM_BYTES 270336UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 2097152UL
#elif defined(ARDUINO_GIGA) || defined(ARDUINO_GIGA_R1) || defined(ARDUINO_GIGA_R1_WIFI)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Giga R1 WiFi"
    #define NUMERIC_LIMITS_SRAM_BYTES 1048576UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 2097152UL
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
    #define NUMERIC_LIMITS_BOARD_NAME "Arduino Portenta H7"
    #define NUMERIC_LIMITS_SRAM_BYTES 1048576UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 16777216UL
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266) || defined(_ESP8266_) || defined(_M_ESP8266)
    #define NUMERIC_LIMITS_BOARD_NAME "ESP8266"
    #define NUMERIC_LIMITS_SRAM_BYTES 81920UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 4194304UL
#elif defined(ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    #define NUMERIC_LIMITS_BOARD_NAME "ESP32-S3"
    #define NUMERIC_LIMITS_SRAM_BYTES 524288UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 4194304UL
#elif defined(ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C3)
    #define NUMERIC_LIMITS_BOARD_NAME "ESP32-C3"
    #define NUMERIC_LIMITS_SRAM_BYTES 400000UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 4194304UL
#elif defined(ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S2)
    #define NUMERIC_LIMITS_BOARD_NAME "ESP32-S2"
    #define NUMERIC_LIMITS_SRAM_BYTES 327680UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 4194304UL
#elif defined(ESP32) || defined(ESPRESSIF32) || defined(ARDUINO_ARCH_ESP32) || defined(_ESP32_) || defined(_M_ESP32)
    #define NUMERIC_LIMITS_BOARD_NAME "ESP32"
    #define NUMERIC_LIMITS_SRAM_BYTES 520192UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 4194304UL
#elif defined(ARDUINO_SAMD_MKRZERO) || defined(ARDUINO_SAMD_ZERO) || defined(_SAMD21G18A_) || defined(_M_SAMD21G18A)
    #define NUMERIC_LIMITS_BOARD_NAME "SAMD21"
    #define NUMERIC_LIMITS_SRAM_BYTES 32768UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 262144UL
#elif defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(__SAMD51__)
    #define NUMERIC_LIMITS_BOARD_NAME "SAMD51"
    #define NUMERIC_LIMITS_SRAM_BYTES 196608UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 524288UL
#elif defined(ARDUINO_ARCH_STM32) || defined(_STM32F1_) || defined(_M_STM32F1) || defined(_STM32F4_) || defined(_M_STM32F4) || defined(_STM32F7_) || defined(_M_STM32F7) || defined(_STM32H7_) || defined(_M_STM32H7) || defined(_STM32L4_) || defined(_M_STM32L4)
    #define NUMERIC_LIMITS_BOARD_NAME "STM32"
    #define NUMERIC_LIMITS_SRAM_BYTES 65536UL
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES 524288UL
#else
    #define NUMERIC_LIMITS_BOARD_NAME NUMERIC_LIMITS_BOARD_NAME_UNKNOWN
    #define NUMERIC_LIMITS_SRAM_BYTES NUMERIC_LIMITS_SRAM_UNKNOWN
    #define NUMERIC_LIMITS_PROGRAM_SPACE_BYTES NUMERIC_LIMITS_PROGRAM_SPACE_UNKNOWN
#endif

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
#elif defined(_AVR_ATmega2560_) || defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
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
    static const char* board_name() { return NUMERIC_LIMITS_BOARD_NAME; }
    static size_t sram_bytes() { return static_cast<size_t>(NUMERIC_LIMITS_SRAM_BYTES); }
    static size_t program_space_bytes() { return static_cast<size_t>(NUMERIC_LIMITS_PROGRAM_SPACE_BYTES); }
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
