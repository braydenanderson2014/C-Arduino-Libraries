#ifndef OPERATORS_H
#define OPERATORS_H

#include <Arduino.h>
template <typename T>
class Operators {

private:
    static bool debug;
    
public:

    static T globalMultiplier = 0;
    static T array = new T[100];
    // Example operation: Increment each element (assuming T supports the ++ operator)
    static T incrementByOne(T element) {
        if(debug){
            Serial.println("[OPERATORS]: Incrementing each element by one: " + (element + 1));
        }
        return element + 1;
    }

    // Example operation: static T each element (assuming T supports the * operator)
    static T doubleValue(T element) {
        if(debug){
            Serial.println("[OPERATORS]: Doubling each element: " + (element * 2));
        }
        return element * 2;
    }

    // Function to set the global multiplier
    void setGlobalMultiplier(T multiplier) {
        if(debug){
            Serial.println("[OPERATORS]: Setting global multiplier to: " + multiplier);
        }
        globalMultiplier = multiplier;
    }

    // Function to multiply a value by the global multiplier
    static T multiply(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Multiplying value by global multiplier: " + (value * globalMultiplier));
        }
        return value * globalMultiplier;
    }

    // Function to Divide a value by the global multiplier
    static T divide(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Dividing value by global multiplier: " + (value / globalMultiplier));
        }
        return value / globalMultiplier;
    }

    static T add(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Adding value to global multiplier: " + (value + globalMultiplier));
        }
        return value + globalMultiplier;
    }
    
    static T add(T value, T value2){
        if(debug){
            Serial.println("[OPERATORS]: Adding value to value2: " + (value + value2));
        }
        return value + value2;
    }

    static T subtract(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Subtracting value from global multiplier: " + (value - globalMultiplier));
        }
        return value - globalMultiplier;
    }

    static T subtract(T value, T value2){
        if(debug){
            Serial.println("[OPERATORS]: Subtracting value from value2: " + (value - value2));
        }
        return value - value2;
    }

    static T modulo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the modulous of value and global multiplier: " + (value % globalMultiplier));
        }
        return value % globalMultiplier;
    }

    static T multiplyByTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Multiplying value by two: " + (value * 2));
        }
        return value * 2;
    }

    static T divideByTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Dividing value by two: " + (value / 2));
        }
        return value / 2;
    }

    static T addTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Adding two to value: " + (value + 2));
        }
        return value + 2;
    }

    static T subtractTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Subtracting two from value: " + (value - 2));
        }
        return value - 2;
    }

    static T moduloTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the modulous of value and two: " + (value % 2));
        }
        return value % 2;
    }

    static T square(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Squaring value: " + (value * value));
        }
        return value * value;
    }

    static T cube(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Cubing value: " + (value * value * value));
        }
        return value * value * value;
    }

    static T squareRoot(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the square root of value: " + sqrt(value));
        }
        return sqrt(value);
    }

    static T cubeRoot(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the cube root of value: " + cbrt(value));
        }
        return cbrt(value);
    }

    static T powerOfTwo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the power of two of value: " + pow(value, 2));
        }
        return pow(value, 2);
    }

    static T powerOfThree(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the power of three of value: " + pow(value, 3));
        }
        return pow(value, 3);
    }

    static T powerOf(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the power of value: " + pow(value, globalMultiplier));
        }
        return pow(value, globalMultiplier);
    }
    static T powerOf(T value, T power) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the power of value: " + pow(value, power));
        }
        return pow(value, power);
    }

    static T absoluteValue(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the absolute value of value: " + abs(value));
        }
        return abs(value);
    }
    // More operations can be added as needed...
    static T factorial(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the factorial of value: " + value);
        }
        static T factorial = 1;
        for (static T i = 1; i <= value; i++) {
            factorial *= i;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the factorial of value; Factorial: " + factorial);
        }
        return factorial;
    }

    static T fibonacci(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the fibonacci of value: " + value);
        }
        if (value <= 1) {
            if(debug){
                Serial.println("[OPERATORS]: value is less than or equal to one: " + value);
            }
            return value;
        }
        if(debug){
            Serial.println("[OPERATORS]: Due to the complexity of this function, it may take a while to complete. Please wait...");
            Serial.println("[OPERATORS]: Will not be printing the fibonacci sequence as it will take up too much space and time");
        }
        return fibonacci(value - 1) + fibonacci(value - 2);
    }

    static void fibonacciPrint(T value){
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the fibonacci of value: " + value);
            Serial.println("[OPERATORS]: Due to the complexity of this function, it may take a while to complete. Please wait...");
        }
        if (value <= 1) {
            if(debug){
                Serial.println("[OPERATORS]: value is less than or equal to one: " + value);
            }
            return;
        }
        Serial.println("[OPERATORS]: Obtaining the fibonacci sequence of value: " + fibonacciPrint(value - 1) + fibonacciPrint(value - 2));
    }

    static T greatestCommonDivisor(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the greatest common divisor of value: " + value);
        }
        static T gcd = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                gcd = i;
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the greatest common divisor of value; GCD: " + gcd);
        }
        return gcd;
    }

    static T leastCommonMultiple(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the least common multiple of value: " + value);
        }
        static T lcm = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                lcm = i;
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the least common multiple of value; LCM: " + lcm);
        }
        return (value * globalMultiplier) / lcm;
    }

    static T decimalToBinary(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the binary of value: " + value);
        }
        static T binary = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 2;
            value /= 2;
            binary += remainder * i;
            i *= 10;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the binary of value: Binary:" + binary);
        }
        return binary;
    }

    static T binaryToDecimal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Binary value: " + value);
        }
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(2, i);
            ++i;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Binary value: Decimal:" + decimal);
        }
        return decimal;
    }

    static T decimalToOctal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the octal of value: " + value);
        }
        static T octal = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 8;
            value /= 8;
            octal += remainder * i;
            i *= 10;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the octal of value: Octal:" + octal);
        }
        return octal;
    }

    static T octalToDecimal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Octal value: " + value);
        }
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(8, i);
            ++i;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Octal value: Decimal:" + decimal);
        }
        return decimal;
    }

    static T decimalToHexadecimal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the hexadecimal of value: " + value);
        }
        static T hexadecimal = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 16;
            value /= 16;
            hexadecimal += remainder * i;
            i *= 10;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the hexadecimal of value: Hexadecimal:" + hexadecimal);
        }
        return hexadecimal;
    }

    static T hexadecimalToDecimal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Hexadecimal value: " + value);
        }
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(16, i);
            ++i;
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of Hexadecimal value: Decimal:" + decimal);
        }
        return decimal;
    }

    static T calculatePercentage(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of value: " + (value * globalMultiplier) / 100);
        }
        return (value * globalMultiplier) / 100;
    }

    static T calculatePercentage(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of value: " + (value * percentage) / 100);
        }
        return (value * percentage) / 100;
    }

    static T calculatePercentageOf(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of global multiplier: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOf(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageChange(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage change of value: " + (globalMultiplier - value) / value);
        }
        return (globalMultiplier - value) / value;
    }

    static T calculatePercentageChange(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage change of value: " + (percentage - value) / value);
        }
        return (percentage - value) / value;
    }

    static T calculatePercentageDifference(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage difference of value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageDifference(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage difference of value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageError(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage error of value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageError(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage error of value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfError(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of error of value: " + (globalMultiplier - value) / value);
        }
        return (globalMultiplier - value) / value;
    }

    static T calculatePercentageOfError(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of error of value: " + (percentage - value) / value);
        }
        return (percentage - value) / value;
    }

    static T calculatePercentageOfChange(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of change of value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfChange(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of change of value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfDifference(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference of value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifference(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference of value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total of value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotal(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total of value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceFrom(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference from value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceFrom(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference from value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfDifferenceTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference to value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference to value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalFrom(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total from value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalFrom(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total from value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfTotalTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total to value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total to value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetween(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }   

    static T calculatePercentageOfDifferenceBetween(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetween(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetween(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceFromTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference from to value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceFromTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference from to value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalFromTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total from to value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalFromTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total from to value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between to value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between to value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between to value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between to value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenFrom(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between from value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenFrom(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between from value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenFrom(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between from value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenFrom(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between from value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenFromTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between from to value: " + (globalMultiplier - value) / globalMultiplier);
        }
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenFromTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of difference between from to value: " + (percentage - value) / percentage);
        }
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenFromTo(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between from to value: " + (globalMultiplier * 100) / value);
        }
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenFromTo(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of total between from to value: " + (percentage * 100) / value);
        }
        return (percentage * 100) / value;
    }

    static T calculateDecimalPercentage(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal percentage of value: " + (value * globalMultiplier) / 100.0);
        }
        return (value * globalMultiplier) / 100.0;
    }

    static T calculateDecimalPercentage(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal percentage of value: " + (value * percentage) / 100.0);
        }
        return (value * percentage) / 100.0;
    }

    static T calculateDecimal(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of value: " + value / 100.0);
        }
        return value / 100.0;
    }

    static T calculateDecimal(T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the decimal of value: " + value / percentage);
        }
        return value / percentage;
    }

    static T divideByHundred (T value) {
        if(debug){
            Serial.println("[OPERATORS]: Dividing value by 100: " + value / 100.0);
        }
        return value / 100.0;
    }

    static T calculatePercentageOfValue (T value, T percentage) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the percentage of value: " + value / percentage);
        }
        return value / percentage;
    }

    static T divide(T value, T divisor) {
        if(debug){
            Serial.println("[OPERATORS]: Dividing value by divisor: " + value / divisor);
        }
        return value / divisor;
    }

    static void addItemsToArray(T value){
        if(debug){
            Serial.println("[OPERATORS]: Adding value to array: " + value);
        }
        for(static T i = 0; i < 100; i++){
            if(array[i] != 0){
                array[i] = value;
                if(debug){
                    Serial.println("[OPERATORS]: Adding value to array: " + value);
                }
                return;
            }else {
                continue;
            }
        }
    }

    static void removeItemsFromArray(T value){
        if(debug){
            Serial.println("[OPERATORS]: Removing value from array: " + value);
        }
        for(static T i = 0; i < 100; i++){
            if(array[i] == value){
                array[i] = 0;
                if(debug){
                    Serial.println("[OPERATORS]: Removing value from array: " + value);
                }
                return;
            }else {
                continue;
            }
        }
    }

    static T getItemsFromArray(T value){
        for(static T i = 0; i < 100; i++){
            if(array[i] == value){
                if(debug){
                    Serial.println("[OPERATORS]: Getting value from array: " + value);
                }
                return array[i];
            }else {
                continue;
            }
        }
    }

    static T average(T value) {
        static T sum = 0;
        static T average = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        average = sum / value;
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the average of value: Average" + sum(value) / value);
        }
        return average;
    }

    static T median(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the median of value: " + median);
        }
        static T median = 0;
        if (value % 2 == 0) {
            median = (array[(value - 1) / 2] + array[value / 2]) / 2;
        } else {
            median = array[value / 2];
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the median of value: Median: " + median);
        }
        return median;
    }

    static T mode(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the mode of value: " + mode);
        }
        static T mode = 0;
        static T maxCount = 0;
        for (static T i = 0; i < value; ++i) {
            static T count = 0;
            for (static T j = 0; j < value; ++j) {
                if (array[j] == array[i]) {
                    ++count;
                }
            }
            if (count > maxCount) {
                maxCount = count;
                mode = array[i];
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the mode of value: Mode: " + mode);
        }
        return mode;
    }

    static T range(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the range of value: " + value);
        }
        static T range = 0;
        range = array[value - 1] - array[0];
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the range of value: Range: " + range);
        }
        return range;
    }

    static T standardDeviation(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the standard deviation of value: " + value);
        }
        static T sum = 0;
        static T mean = 0;
        static T standardDeviation = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        mean = sum / value;
        for (static T i = 0; i < value; i++) {
            standardDeviation += pow(array[i] - mean, 2);
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the standard deviation of value: Standard Deviation: " + sqrt(standardDeviation / value));
        }
        return sqrt(standardDeviation / value);
    }

    static T variance(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the variance of value: " + value);
        }
        static T sum = 0;
        static T mean = 0;
        static T variance = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        mean = sum / value;
        for (static T i = 0; i < value; i++) {
            variance += pow(array[i] - mean, 2);
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the variance of value: Variance: " + variance / value);
        }
        return variance / value;
    }

    static T sum(){
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the sum of the array");
        }
        static T sum = 0;
        for (static T i = 0; i < 100; i++) {
            sum += array[i];
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the sum of the array: Sum: " + sum);
        }
        return sum;
    }

    static T sum(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the sum of value: " + value + " and the array");
        }
        static T sum = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the sum of value: Sum: " + sum);
        }
        return sum;
    }

    static T product(){
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the product of the array");
        }
        static T product = 1;
        for (static T i = 0; i < 100; i++) {
            product *= array[i];
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the product of the array: Product: " + product);
        }
        return product;
    }

    static T product(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the product of value: " + value + " and the array");
        }
        static T product = 1;
        for (static T i = 0; i < value; i++) {
            product *= array[i];
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the product of value: Product: " + product);
        }
        return product;
    }

    static T findMin() {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the minimum of the array");
        }
        static T minValue = array[0];
        for (static T i = 1; i < 100; i++) {
            if (array[i] < minValue) {
                minValue = array[i];
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the minimum of the array: Minimum: " + minValue);
        }
        return minValue;
    }

    static T minimum(T* array, size_t size) {
        if (size == 0) {
            if(debug){
                Serial.println("[OPERATORS]: Obtaining the minimum of the array: Minimum:  0 (Array is empty)");
            }
            return 0;
        }

        T minValue = array[0];
        for (size_t i = 1; i < size; i++) {
            if (array[i] < minValue) {
                minValue = array[i];
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the minimum of the array: Minimum: " + minValue);
        }
        return minValue;
    }

    static T findMax() {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the maximum of the array");
        }
        static T maxValue = array[0];
        for (static T i = 1; i < 100; i++) {
            if (array[i] > maxValue) {
                maxValue = array[i];
            }
        }
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the maximum of the array: Maximum: " + maxValue);
        }
        return maxValue;
    }

    static T count(T value) {
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the count of value: " + value);
        }
        static T count = 0;
        for (static T i = 0; i < value; i++) {
            count++;
        }
        return count;
    }

    

    static T multiply(T value, T value2){
        if(debug){
            Serial.println("[OPERATORS]: Obtaining the product of value: " + value + " and value: " + value2);
        }
        return value * value2;
    }
};



#endif // OPERATORS_H