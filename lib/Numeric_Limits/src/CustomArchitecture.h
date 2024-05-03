#ifndef CUSTOMARCHITECTURE_H
#define CUSTOMARCHITECTURE_H

    /*
    
    */

    #define CHAR_MIN //(-128) //Define the minimum value of char
    #define CHAR_MAX //127 // Define the maximum value of char

    #define BYTE_MIN //0 // Define the minimum value of byte
    #define BYTE_MAX //255 // Define the maximum value of byte

    #define INT_MIN //(-32768) // Define the minimum value of int
    #define INT_MAX //32767 // Define the maximum value of int

    #define LONG_MIN //(-2147483648L) // Define the minimum value of long
    #define LONG_MAX //2147483647L // Define the maximum value of long

    #define FLT_MIN //1.175494351e-38F // Define the minimum value of float
    #define FLT_MAX //3.402823466e+38F // Define the maximum value of float

    #define DBL_MIN FLT_MIN // On Arduino, double is often the same as float
    #define DBL_MAX FLT_MAX




#endif // CUSTOMARCHITECTURE_H
