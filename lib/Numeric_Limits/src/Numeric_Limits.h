#ifndef NUMERIC_LIMITS_H
#define NUMERIC_LIMITS_H

//ASSUMES 32 BIT ARCHITECTURE (Later versions may include a check for the architecture and adjust accordingly)
#define CHAR_MIN (-128)
#define CHAR_MAX 127
#define BYTE_MAX 255

#define INT_MIN (-32768)
#define INT_MAX 32767

#define LONG_MIN (-2147483648L)
#define LONG_MAX 2147483647L

#define FLT_MIN 1.175494351e-38F
#define FLT_MAX 3.402823466e+38F

#define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
#define DBL_MAX FLT_MAX

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
