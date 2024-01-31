#ifndef OPERATORS_H
#define OPERATORS_H

#include <Arduino.h>
template <typename T>
class Operators {

private:
    
public:

    static T globalMultiplier = 0;
    static T array = new T[100];
    /**
     * @brief Increment element by 1 (assuming T supports being incremented)
     * @param element The element to increment
     * 
     * @return T The incremented element
     *
    */
    static T incrementByOne(T element) {
        return element + 1;
    }

    /**
     * @brief Multiply by 2 (assuming T supports being multiplied)
     * @param element The element to multiply
     * 
     * @return T The multiplied element
    */
    static T doubleValue(T element) {
        return element * 2;
    }

    // Function to set the global multiplier
    /**
     * @brief Set the global multiplier
     * @param multiplier The value to set the global multiplier to
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return void
    */
    void setGlobalMultiplier(T multiplier) {
        globalMultiplier = multiplier;
    }

    // Function to multiply a value by the global multiplier
    /**
     * @brief Multiply a value by the global multiplier
     * @param value The value to multiply
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return T The multiplied value
    */
    static T multiply(T value) {
        return value * globalMultiplier;
    }

    // Function to Divide a value by the global multiplier
    /**
     * @brief Divide a value by the global multiplier
     * @param value The value to divide
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return T The divided value
    */
    static T divide(T value) {
        return value / globalMultiplier;
    }

    // Function to add a value to the global multiplier
    /**
     * @brief Add a value to the global multiplier
     * @param value The value to add
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return T The added value
    */
    static T add(T value) {
        return value + globalMultiplier;
    }
    
    /**
     * @brief Add @param value to @param value2
     * @param value The value to add
     * @param value2 The value to add to value
     * 
     * @return T The added value
    */
    static T add(T value, T value2){
        return value + value2;
    }

    // Function to subtract a value from the global multiplier
    /**
     * @brief Subtract a value from the global multiplier
     * @param value The value to subtract
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return T The subtracted value
    */
    static T subtract(T value) {
        return value - globalMultiplier;
    }

    /**
     * @brief Subtract @param value2 from @param value
     * @param value The value to subtract from
     * @param value2 The value to subtract from value
     * 
     * @return T The subtracted value
    */
    static T subtract(T value, T value2){
        return value - value2;
    }

    /**
     * @brief Modulo @param value by Global Multiplier
     * @param value The value to modulo
     * @details Global multiplier is used in the multiply and divide functions that only take in a single parameter but would normally take in 2)
     * 
     * @return T The moduloed value
    */
    static T modulo(T value) {
        return value % globalMultiplier;
    }

    /**
     * @brief Multiply @param value by two
     * @param value The value to multiply
     * 
     * @return T The moduloed value
    */
    static T multiplyByTwo(T value) {
        return value * 2;
    }

    /**
     * @brief Divide @param value by two
     * @param value The value to divide
     * 
     * @return T The divided value
    */
    static T divideByTwo(T value) {
        return value / 2;
    }

    /**
     * @brief Add two to @param value
     * @param value The value to add two to
     * 
     * @return T The added value
    */
    static T addTwo(T value) {
        return value + 2;
    }

    /**
     * @brief Subtract two from @param value
     * @param value The value to subtract two from
     * 
     * @return T The subtracted value
    */
    static T subtractTwo(T value) {
        return value - 2;
    }

    /**
     * @brief Modulo @param value by two
     * @param value The value to modulo by two
     * 
     * @return T The moduloed value
    */
    static T moduloTwo(T value) {
        return value % 2;
    }

    /**
     * @brief Returns the Square of @param value (value * value)
     * @param value The value to square
     * 
     * @return T The squared value
    */
    static T square(T value) {
        return value * value;
    }

    /**
     * @brief Returns the Cube of @param value (value * value * value)
     * @param value The value to cube
     * 
     * @return T The cubed value
    */
    static T cube(T value) {
        return value * value * value;
    }

    /**
     * @brief Returns the Square Root of @param value
     * @param value The value to square root
     * 
     * @return T The square rooted value
    */
    static T squareRoot(T value) {
        return sqrt(value);
    }

    /**
     * @brief Returns the Cube Root of @param value
     * @param value The value to cube root
     * 
     * @return T The cube rooted value
    */
    static T cubeRoot(T value) {
        return cbrt(value);
    }

    /**
     * @brief Returns the Square of @param value (value * value)
     * @param value The value to square
     * 
     * @details This function is the same as the square function, but it uses the power function instead of the multiply function
     * @return T The squared value
    */
    static T powerOfTwo(T value) {
        return pow(value, 2);
    }

    /**
     * @brief Returns the Cube of @param value (value * value * value)
     * @param value The value to cube
     * 
     * @details This function is the same as the cube function, but it uses the power function instead of the multiply function
     * @return T The cubed value
    */
    static T powerOfThree(T value) {
        return pow(value, 3);
    }

    /**
     * @brief Returns the Power of @param value (value^globalMultiplier)
     * @param value The value to power
     * 
     * @return T The powered value
    */
    static T powerOf(T value) {
        return pow(value, globalMultiplier);
    }

    /**
     * @brief Returns the Power of @param value (value^@param power)
     * @param value The value to power
     * @param power The power to raise value to
     * 
     * @return T The powered value
    */
    static T powerOf(T value, T power) {
        return pow(value, power);
    }

    /**
     * @brief Returns the absolute value of @param value
     * @param value The value to absolute
     * 
     * @return T The absolute value
    */
    static T absoluteValue(T value) {
        return abs(value);
    }

    /**
     * @brief Returns the Factorial of @param value
     * @param value The value to factorial
     * 
     * @return T The factorial value
    */
    static T factorial(T value) {
        static T factorial = 1;
        for (static T i = 1; i <= value; i++) {
            factorial *= i;
        }
        return factorial;
    }

    /**
     * @brief Returns the fibonacci of @param value
     * @param value The value to fibonacci
     * 
     * @return T The fibonacci value
    */
    static T fibonacci(T value) {
        if (value <= 1) {
            return value;
        }
        return fibonacci(value - 1) + fibonacci(value - 2);
    }

    /**
     * @brief Calculates the greatest common divisor of @param value
     * @param value The value to calculate the greatest common divisor of
     * 
     * @return T The greatest common divisor value
    */
    static T greatestCommonDivisor(T value) {
        static T gcd = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                gcd = i;
            }
        }
        return gcd;
    }

    /**
     * @brief Calculates the least common multiple of @param value
     * @param value The value to calculate the least common multiple of
     * 
     * @return T The least common multiple value
    */
    static T leastCommonMultiple(T value) {
        static T lcm = 1;
        for (static T i = 1; i <= value; i++) {
            if (value % i == 0 && globalMultiplier % i == 0) {
                lcm = i;
            }
        }
        return (value * globalMultiplier) / lcm;
    }

    /**
     * @brief Converts @param value to binary
     * @param value The value to convert to binary
     * 
     * @return T The binary value
    */
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

    /**
     * @brief Converts @param value to decimal from binary
     * @param value The value to convert to decimal
     * 
     * @return T The decimal value
    */
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

    /**
     * @brief Converts @param value to octal from decimal
     * @param value The value to convert to octal
     * 
     * @return T The octal value
    */
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

    /**
     * @brief Converts @param value to decimal from octal
     * @param value The value to convert to decimal
     * 
     * @return T The decimal value
    */
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

    /**
     * @brief Converts @param value to hexadecimal from decimal
     * @param value The value to convert to hexadecimal
     * 
     * @return T The hexadecimal value
    */
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

    /**
     * @brief Converts @param value to decimal from hexadecimal
     * @param value The value to convert to decimal
     * 
     * @return T The decimal value
    */
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

    /**
     * @brief Calculates the Percentage of @param value (value * globalMultiplier) / 100
     * @param value The value to calculate the percentage of
     * 
     * @return T The percentage value
    */
    static T calculatePercentage(T value) {
        return (value * globalMultiplier) / 100;
    }

    /**
     * @brief Calculates the Percentage of @param value (value * @param percentage) / 100
     * @param value The value to calculate the percentage of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage value
    */
    static T calculatePercentage(T value, T percentage) {
        return (value * percentage) / 100;
    }

    /**
     * @brief Calculates the Percentage of @param value (value * globalMultiplier) / 100
     * @param value The value to calculate the percentage of
     * 
     * @return T The percentage value
    */
    static T calculatePercentageOf(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of @param value (value * @param percentage) / 100
     * @param value The value to calculate the percentage of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage value
    */
    static T calculatePercentageOf(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage Change of @param value (globalMultiplier - value) / value
     * @param value The value to calculate the percentage change of
     * 
     * @return T The percentage change value
    */
    static T calculatePercentageChange(T value) {
        return (globalMultiplier - value) / value;
    }

    /**
     * @brief Calculates the Percentage Change of @param value (percentage - value) / value
     * @param value The value to calculate the percentage change of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage change value
    */
    static T calculatePercentageChange(T value, T percentage) {
        return (percentage - value) / value;
    }

    /**
     * @brief Calculates the Percentage Difference of @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage difference of
     * 
     * @return T The percentage difference value
    */
    static T calculatePercentageDifference(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage Difference of @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage difference of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage difference value
    */
    static T calculatePercentageDifference(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage Error of @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage error of
     * 
     * @return T The percentage error value
    */
    static T calculatePercentageError(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage Error of @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage error of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage error value
    */
    static T calculatePercentageError(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfError(T value) {
        return (globalMultiplier - value) / value;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfError(T value, T percentage) {
        return (percentage - value) / value;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfChange(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfChange(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifference(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifference(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotal(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total of @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotal(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of difference from @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceFrom(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of difference from @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceFrom(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total from @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalFrom(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total from @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalFrom(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total to @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total to @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetween(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }   

    /**
     * @brief Calculates the Percentage of Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetween(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetween(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetween(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceFromTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceFromTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalFromTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalFromTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenFrom(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenFrom(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenFrom(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenFrom(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (globalMultiplier - value) / globalMultiplier
     * @param value The value to calculate the percentage of total of
     * 
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenFromTo(T value) {
        return (globalMultiplier - value) / globalMultiplier;
    }

    /**
     * @brief Calculates the Percentage of Difference between @param value (percentage - value) / percentage
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfDifferenceBetweenFromTo(T value, T percentage) {
        return (percentage - value) / percentage;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (globalMultiplier * 100) / value
     * @param value The value to calculate the percentage of total of
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenFromTo(T value) {
        return (globalMultiplier * 100) / value;
    }

    /**
     * @brief Calculates the Percentage of Total between @param value (percentage * 100) / value
     * @param value The value to calculate the percentage of total of
     * @param percentage The percentage to calculate
     * @overload 
     * @return T The percentage of total value
    */
    static T calculatePercentageOfTotalBetweenFromTo(T value, T percentage) {
        return (percentage * 100) / value;
    }

    /**
     * @brief Calculate the Decimal Percentage of @param value (value * globalMultiplier) / 100.0
     * @param value The value to calculate the decimal percentage of
     * 
     * @return T The decimal percentage value
     * 
    **/
    static T calculateDecimalPercentage(T value) {
        return (value * globalMultiplier) / 100.0;
    }

    /**
     * @brief Calculate the Decimal Percentage of @param value (value * @param percentage) / 100.0
     * @param value The value to calculate the decimal percentage of
     * @param percentage The percentage to calculate
     * 
     * @return T The decimal percentage value
     **/
    static T calculateDecimalPercentage(T value, T percentage) {
        return (value * percentage) / 100.0;
    }

    /**
     * @brief Calculate the Decimal of @param value value / 100.0
    */
    static T calculateDecimal(T value) {
        return value / 100.0;
    }

    /**
     * @brief Calculate the Decimal of @param value value / @param percentage
     * @param value The value to calculate the decimal of
     * @param percentage The percentage to calculate
     * 
     * @return T The decimal value
    */
    static T calculateDecimal(T value, T percentage) {
        return value / percentage;
    }

    /**
     * @brief Divide @param value by 100
     * @param value The value to divide by 100
     * 
     * @return T The divided value
    */
    static T divideByHundred (T value) {
        return value / 100.0;
    }

    /**
     * @brief Calculate the Percentage of Value of @param value value / percentage
     * @param value The value to calculate the percentage of value of
     * @param percentage The percentage to calculate
     * 
     * @return T The percentage of value value
    */
    static T calculatePercentageOfValue (T value, T percentage) {
        return value / percentage;
    }

    /**
     * @brief divide @param value by @param divisor
     * @param value The value to divide
     * @param divisor The divisor to divide by
     * 
     * @return T The divided value
    */
    static T divide(T value, T divisor) {
        return value / divisor;
    }

    /**
     * @brief adds @param value to an internal array to be used for statistical calculations
     * @param value The value to add to the array
     * 
    */
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

    /**
     * @brief removes @param value from an internal array to be used for statistical calculations
     * @param value The value to remove from the array
     * 
    */
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

    /**
     * @brief gets @param value from an internal array to be used for statistical calculations
     * @param value The value to get from the array
     * 
     * @return T The value from the array
    */
    static T getItemsFromArray(T value){
        for(static T i = 0; i < 100; i++){
            if(array[i] == value){
                return array[i];
            }else {
                continue;
            }
        }
    }

    /**
     * @brief Calculates the average of the array
     * @param value The value to calculate the average of
     * @return T The average value
    */
    static T average(T value) {
        static T sum = 0;
        static T average = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        average = sum / value;
        return average;
    }

    /**
     * @brief Calculates the Median (middle value) of the array
     * @param value The value to calculate the median of
     * 
     * @return T The median value
    */
    static T median(T value) {
        static T median = 0;
        if (value % 2 == 0) {
            median = (array[(value - 1) / 2] + array[value / 2]) / 2;
        } else {
            median = array[value / 2];
        }
        return median;
    }

    /**
     * @brief Calculates the Mode (most common value) of the array
     * @param value The value to calculate the mode of
     * 
     * @return T The mode value
    */
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

    /**
     * @brief Calculates the Range (difference between the smallest and largest values) of the array
     * @param value The value to calculate the range of
     * 
     * @return T The range value
    */
    static T range(T value) {
        static T range = 0;
        range = array[value - 1] - array[0];
        return range;
    }

    /**
     * @brief Calculates Standard Deviation of the array.
     * @param value The value to calculate the standard deviation of
     * 
     * @return T The standard deviation value
    */
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

    /**
     * @brief Calculates Variance of the array.
     * @param value The value to calculate the variance of
     * 
     * @return T The variance value
    */
    static T variance(T value) {
        i
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

    /**
     * @brief Calculates the Sum of the array
     * @param value The value to calculate the sum of
     * 
     * @return T The sum value
    */
    static T sum(){
        static T sum = 0;
        for (static T i = 0; i < 100; i++) {
            sum += array[i];
        }
        return sum;
    }

    /**
     * @brief Calculates the Sum of @param value and the array
     * @param value The value to calculate the sum of
     * 
     * @return T The sum value
    */
    static T sum(T value) {
        static T sum = 0;
        for (static T i = 0; i < value; i++) {
            sum += array[i];
        }
        return sum;
    }

    /**
     * @brief Calculates the Product of the array
     * @param value The value to calculate the product of
     * 
     * @return T The product value
    */
    static T product(){
        static T product = 1;
        for (static T i = 0; i < 100; i++) {
            product *= array[i];
        }
        return product;
    }

    /**
     * @brief Calculates the Product of @param value and the array
     * @param value The value to calculate the product of
     * 
     * @return T The product value
    */
    static T product(T value) {
        static T product = 1;
        for (static T i = 0; i < value; i++) {
            product *= array[i];
        }
        return product;
    }

    /**
     * @brief Finds the Minimum value of the array
     * 
     * 
    */
    static T findMin() {
        static T minValue = array[0];
        for (static T i = 1; i < 100; i++) {
            if (array[i] < minValue) {
                minValue = array[i];
            }
        }
        return minValue;
    }

    /**
     * @brief Finds the Minimum value of @param value and the array
     * @param value The value to find the minimum of
     * 
     * @return T The minimum value
    */
    static T minimum(T* array, size_t size) {
        if (size == 0) {
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

    /**
     * @brief Finds the Maximum value of the array
     * 
 
    */
    static T findMax() {
        static T maxValue = array[0];
        for (static T i = 1; i < 100; i++) {
            if (array[i] > maxValue) {
                maxValue = array[i];
            }
        }
        return maxValue;
    }

    /**
     * @brief Returns count of @param value
     * @param value The value to count
     *  
     * @return T The count value
     * 
    */
    static T count(T value) {
        static T count = 0;
        for (static T i = 0; i < value; i++) {
            count++;
        }
        return count;
    }

    
    /**
     * @brief Multiply @param value by @param value2
     * @param value The value to multiply
     * @param value2 The value to multiply by
     * 
    */
    static T multiply(T value, T value2){
        return value * value2;
    }
};
#endif // OPERATORS_H