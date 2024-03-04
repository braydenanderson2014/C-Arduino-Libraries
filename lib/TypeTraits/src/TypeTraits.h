#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <Arduino.h>

template <typename T>
struct is_arithmetic {
    static const bool value = false;
};

template <>
struct is_arithmetic<int> {
    static const bool value = true;
};

template <>
struct is_arithmetic<float> {
    static const bool value = true;
};

template <>
struct is_arithmetic<double> {
    static const bool value = true;
};

template <>
struct is_arithmetic<long> {
    static const bool value = true;
};

template <>
struct is_arithmetic<byte> {
    static const bool value = true;
};

template <typename T>
struct is_Char {
    static const bool value = false;
};

template <>
struct is_Char<char> {
    static const bool value = true;
};

template <typename T>
struct is_String {
    static const bool value = false;
};

template <>
struct is_String<String> {
    static const bool value = true;
};

template <typename T>
struct is_Integral {
    static const bool value = false;
};

template <>
struct is_Integral<int> {
    static const bool value = true;
};

template <>
struct is_Integral<long> {
    static const bool value = true;
};

template <>
struct is_Integral<char> {
    static const bool value = true;
};

template <>
struct is_Integral<bool> {
    static const bool value = true;
};

template <typename T>
struct is_floating_point {
    static const bool value = false;
};

template <>
struct is_floating_point<float> {
    static const bool value = true;
};

template <>
struct is_floating_point<double> {
    static const bool value = true;
};

template <typename T>
struct is_Array {
    static const bool value = false;
};

template <typename T>
struct is_Array<T[]> {
    static const bool value = true;
};

template <typename T>
struct is_Array<T[1]> {
    static const bool value = true;
};

template <typename T>
struct is_pointer {
    static const bool value = false;
};

template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
};

template <typename T>
struct is_pointer<T* const> {
    static const bool value = true;
};

template <typename T>
struct is_reference {
    static const bool value = false;
};

template <typename T>
struct is_reference<T&> {
    static const bool value = true;
};

template <typename T>
struct is_const {
    static const bool value = false;
};

template <typename T>
struct is_const<const T> {
    static const bool value = true;
};

template <typename T>
struct is_volatile {
    static const bool value = false;
};

template <typename T>
struct is_volatile<volatile T> {
    static const bool value = true;
};

template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};

template <typename T>
struct is_void {
    static const bool value = false;
};



#endif // TYPETRAITS_H