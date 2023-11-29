#ifndef ARRAYLISTOPERATIONS_H
#define ARRAYLISTOPERATIONS_H

#include <Arduino.h>
template <typename T>
class ArrayListOperations {
    
public:
    static T globalMultiplier = 0;
    static T array = new T[100];
    // Example operation: Increment each element (assuming T supports the ++ operator)
    static T incrementByOne(T element) {
        return element + 1;
    }

    // Example operation: static T each element (assuming T supports the * operator)
    static T doubleValue(T element) {
        return element * 2;
    }

    // Function to set the global multiplier
    void setGlobalMultiplier(T multiplier) {
        globalMultiplier = multiplier;
    }

    // Function to multiply a value by the global multiplier
    static T multiply(T value) {
        return value * globalMultiplier;
    }

    // Function to Divide a value by the global multiplier
    static T divide(T value) {
        return value / globalMultiplier;
    }

    static T add(T value) {
        return value + globalMultiplier;
    }

    static T subtract(T value) {
        return value - globalMultiplier;
    }

    static T modulo(T value) {
        return value % globalMultiplier;
    }

    static T multiplyByTwo(T value) {
        return value * 2;
    }

    static T divideByTwo(T value) {
        return value / 2;
    }

    static T addTwo(T value) {
        return value + 2;
    }

    static T subtractTwo(T value) {
        return value - 2;
    }

    static T moduloTwo(T value) {
        return value % 2;
    }

    static T square(T value) {
        return value * value;
    }

    static T cube(T value) {
        return value * value * value;
    }

    static T squareRoot(T value) {
        return sqrt(value);
    }

    static T cubeRoot(T value) {
        return cbrt(value);
    }

    static T powerOfTwo(T value) {
        return pow(value, 2);
    }

    static T powerOfThree(T value) {
        return pow(value, 3);
    }

    static T powerOf(T value) {
        return pow(value, globalMultiplier);
    }
    static T powerOf(T value, T power) {
        return pow(value, power);
    }

    static T absoluteValue(T value) {
        return abs(value);
    }
    // More operations can be added as needed...
    static T factorial(T value) {
        static T factorial = 1;
        for (static T i = 1; i <= value; i++) {
            factorial *= i;
        }
        return factorial;
    }

    static T fibonacci(T value) {
        if (value <= 1) {
            return value;
        }
        return fibonacci(value - 1) + fibonacci(value - 2);
    }

    static T greatestCommonDivisor(T value) {
        static T gcd = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                gcd = i;
            }
        }
        return gcd;
    }

    static T leastCommonMultiple(T value) {
        static T lcm = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                lcm = i;
            }
        }
        return (value * globalMultiplier) / lcm;
    }

    static T isPrime(T value) {
        if (value <= 1) {
            return 0;
        }
        for (static T i = 2; i < value; i++) {
            if (value % i == 0) {
                return 0;
            }
        }
        return 1;
    }

    static T isEven(T value) {
        if (value % 2 == 0) {
            return 1;
        }
        return 0;
    }

    static T isOdd(T value) {
        if (value % 2 == 0) {
            return 0;
        }
        return 1;
    }

    static T isPositive(T value) {
        if (value > 0) {
            return 1;
        }
        return 0;
    }

    static T isNegative(T value) {
        if (value < 0) {
            return 1;
        }
        return 0;
    }

    static T isZero(T value) {
        if (value == 0) {
            return 1;
        }
        return 0;
    }

    static T isDivisibleBy(T value) {
        if (value % globalMultiplier == 0) {
            return 1;
        }
        return 0;
    }

    static T isDivisibleBy(T value, T divisor) {
        if (value % divisor == 0) {
            return 1;
        }
        return 0;
    }

    static T isDivisibleByTwo(T value) {
        if (value % 2 == 0) {
            return 1;
        }
        return 0;
    }

    static T decimalToBinary(T value) {
        static T binary = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 2;
            value /= 2;
            binary += remainder * i;
            i *= 10;
        }
        return binary;
    }

    static T binaryToDecimal(T value) {
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(2, i);
            ++i;
        }
        return decimal;
    }

    static T decimalToOctal(T value) {
        static T octal = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 8;
            value /= 8;
            octal += remainder * i;
            i *= 10;
        }
        return octal;
    }

    static T octalToDecimal(T value) {
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(8, i);
            ++i;
        }
        return decimal;
    }

    static T decimalToHexadecimal(T value) {
        static T hexadecimal = 0;
        static T remainder, i = 1;
        while (value != 0) {
            remainder = value % 16;
            value /= 16;
            hexadecimal += remainder * i;
            i *= 10;
        }
        return hexadecimal;
    }

    static T hexadecimalToDecimal(T value) {
        static T decimal = 0;
        static T remainder, i = 0;
        while (value != 0) {
            remainder = value % 10;
            value /= 10;
            decimal += remainder * pow(16, i);
            ++i;
        }
        return decimal;
    }

    static T calculatePercentage(T value) {
        return (value * globalMultiplier) / 100;
    }

    static T calculatePercentage(T value, T percentage) {
        return (value * percentage) / 100;
    }

    static T calculatePercentageOf(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOf(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageChange(T value) {
        return (globalMultiplier - value) / value;
    }

    static T calculatePercentageChange(T value, T percentage) {
        return (percentage - value) / value;
    }

    static T calculatePercentageDifference(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageDifference(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageError(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageError(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfError(T value) {
        return (globalMultiplier - value) / value;
    }

    static T calculatePercentageOfError(T value, T percentage) {
        return (percentage - value) / value;
    }

    static T calculatePercentageOfChange(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfChange(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfDifference(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifference(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotal(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotal(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceFrom(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceFrom(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfDifferenceTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalFrom(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalFrom(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfTotalTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetween(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }   

    static T calculatePercentageOfDifferenceBetween(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetween(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetween(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceFromTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceFromTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalFromTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalFromTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenFrom(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenFrom(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenFrom(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenFrom(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculatePercentageOfDifferenceBetweenFromTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    static T calculatePercentageOfDifferenceBetweenFromTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    static T calculatePercentageOfTotalBetweenFromTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    static T calculatePercentageOfTotalBetweenFromTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    static T calculateDecimalPercentage(T value) {
        return (value * globalMultiplier) / 100.0;
    }

    static T calculateDecimalPercentage(T value, T percentage) {
        return (value * percentage) / 100.0;
    }

    static T calculateDecimal(T value) {
        return value / 100.0;
    }

    static T calculateDecimal(T value, T percentage) {
        return value / percentage;
    }

    static T divideByHundred (T value) {
        return value / 100.0;
    }

    static T calculatePercentageOfValue (T value, T percentage) {
        return value / percentage;
    }

    static T divide(T value, T divisor) {
        return value / divisor;
    }

    static void addItemsToArray(T value){
        for(static T i = 0; i < 100; i++){
            if(array[i] != 0){
                array[i] = value;
                return;
            }else {
                continue;
            }
        }
    }

    static void removeItemsFromArray(T value){
        for(static T i = 0; i < 100; i++){
            if(array[i] == value){
                array[i] = 0;
                return;
            }else {
                continue;
            }
        }
    }

    static T getItemsFromArray(T value){
        for(static T i = 0; i < 100; i++){
            if(array[i] == value){
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
        return average;
    }

    static T median(T value) {
        static T median = 0;
        if (value % 2 == 0) {
            median = (array[(value - 1) / 2] + array[value / 2]) / 2;
        } else {
            median = array[value / 2];
        }
        return median;
    }

    static T mode(T value) {
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
        return mode;
    }

    static T range(T value) {
        static T range = 0;
        range = array[value - 1] - array[0];
        return range;
    }

    static T standardDeviation(T value) {
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
        return sqrt(standardDeviation / value);
    }

    static T variance(T value) {
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
        return variance / value;
    }

    static T sum(T value) {
        static T sum = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        return sum;
    }

    static T product(T value) {
        static T product = 1;
        for (static T i = 0; i < value; i++) {
            product *= array[i];
        }
        return product;
    }

    static T findMin(T value) {
        T minValue = array[0];
        for (T i = 0; i < value; i++) {
            if (array[i] < minValue) {
                minValue = array[i];
            }
        }
        return minValue;
    }

    static T minimum(T* array, size_t size) {
        if (size == 0) {
            Serial.println("[ARRAYLIST_OPERATIONS]: Array is empty");
            return 0;
        }

        T minValue = array[0];
        for (size_t i = 1; i < size; i++) {
            if (array[i] < minValue) {
                minValue = array[i];
            }
        }
        return minValue;
    }

    static T findMax(T value) {
        T maxValue = array[0];
        for (T i = 0; i < value; i++) {
            if (array[i] > maxValue) {
                maxValue = array[i];
            }
        }
        return maxValue;
    }

    static T count(T value) {
        static T count = 0;
        for (static T i = 0; i < value; i++) {
            count++;
        }
        return count;
    }
};



#endif // ARRAYLISTOPERATIONS_H