/*
    Example Name: Example For TypeTraits Library

    Basic example for the TypeTraits library. This example demonstrates the basic functionality of the TypeTraits library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <TypeTraits.h>
#include <Arduino.h>

template <typename T>
struct Example{
    static_assert(is_same<T, int>::value, "T is not the same as int"); //This assertion will pass based off of the Example declaration in the Setup function
    static_assert(is_same<T, float>::value, "T is not the same as float"); //This assertion will fail based off of the Example declaration in the Setup function
    static_assert(is_same<T, double>::value, "T is not the same as double"); //This assertion will fail based off of the Example declaration in the Setup function
    static_assert(is_same<T, char>::value, "T is not the same as char"); //This assertion will fail based off of the Example declaration in the Setup function
    static_assert(is_same<T, String>::value, "T is not the same as String"); //This assertion will fail based off of the Example declaration in the Setup function
    T value;

    Example(T val){
        value = val;
    }
};

void setup(){ 
    static_assert(is_same<int, int>::value, "int is not the same as int"); //This assertion will pass
    static_assert(is_same<int, float>::value, "int is not the same as float"); //This assertion will fail
    static_assert(is_same<int, double>::value, "int is not the same as double"); //This assertion will fail

    static_assert(is_arithmetic<int>::value, "int is not an arithmetic type"); //This assertion will pass
    static_assert(is_arithmetic<float>::value, "float is not an arithmetic type"); //This assertion will pass
    static_assert(is_arithmetic<double>::value, "double is not an arithmetic type"); //This assertion will pass
    static_assert(is_arithmetic<char>::value, "char is not an arithmetic type"); //This assertion will fail

    static_assert(is_Integral<int>::value, "int is not an integral type"); //This assertion will pass
    static_assert(is_Integral<float>::value, "float is not an integral type"); //This assertion will fail
    static_assert(is_Integral<double>::value, "double is not an integral type"); //This assertion will fail
    static_assert(is_Integral<char>::value, "char is not an integral type"); //This assertion will pass

    Example<int> example(10);

    bool isChar = is_Char<char>::value; //This will return true
    bool isString = is_Char<String>::value; //This will return false

    Serial.println(is_reference<int&>::value); //This will return true
    Serial.println(is_reference<int>::value); //This will return false

    //MOSTLY USED IN LIBRARY DEVELOPMENT... THIS LIBRARY WILL ALLOW YOU TO CHECK THE TYPE OF THE VARIABLE AT COMPILE TIME. 
    //THIS WILL HELP YOU TO AVOID THE RUNTIME ERRORS, AND WILL HELP YOU TO WRITE A MORE EFFICIENT CODE.

    //THIS EXAMPLE IS MEANT TO HAVE ERRORS AT COMPILE TIME!!! 
}