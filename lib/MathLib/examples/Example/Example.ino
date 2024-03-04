/*
    Example Name: Example For MathLib Library

    Basic example for the MathLib library. This example demonstrates the basic functionality of the MathLib library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <MathLib.h>
#include <Arduino.h>    

void setup(){
    Serial.begin(9600);
    Serial.println(EXP(2)); // This will print the value of e^2 where e is the base of the natural logarithm
    Serial.println(Log10(100)); // This will print the value of log10(100) Log10 is the log base 10
    Serial.println(Sqrt(100)); // This will print the value of sqrt(100) Sqrt is the square root
    Serial.println(Cbrt(100)); // This will print the value of cbrt(100) cbrt is the cube root
    Serial.println(Pow(2, 3)); // This will print the value of 2^3 where 2 is the base and 3 is the exponent
    Serial.println(Log2(100)); // This will print the value of log2(100) Log2 is the log base 2
    Serial.println(Logn(100, 10)); // This will print the value of log10(100) Logn is the log base n
    Serial.println(ABS(-100)); // This will print the value of abs(-100) ABS is the absolute value
    Serial.println(Ceil(100.5)); // This will print the value of ceil(100.5) Ceil rounds up to the nearest integer
    Serial.println(Floor(100.5)); // This will print the value of floor(100.5) Floor rounds down to the nearest integer

    int y = Factorial(5); // This will return the factorial of 5
    int z;
    if(isPrime(y)){ //checks if y is a prime number
        z = Power(y, y); //y is the factorial of 5, This will return the value of y^y
        Serial.println(z);
    } else 
        z = Power(y, y); //y is the factorial of 5, This will return the value of y^y
        Serial.println(z);
        z = Prime(y); //y is the factorial of 5, This will return the next prime number after y
        Serial.println(z);
    }




void loop(){

}