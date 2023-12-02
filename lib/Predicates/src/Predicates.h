#ifndef PREDICATES_H
#define PREDICATES_H

#include <Arduino.h>
template <typename T>
class Predicates {
private:
    static bool debug;
    static int globalMultiplier;
public:
    static void setDebug(bool debug) {
        Predicates<T>::debug = debug;
    }

    static bool isDebug() {
        return Predicates<T>::debug;
    }

    static void setGlobalMultiplier(int globalMultiplier) {
        Predicates<T>::globalMultiplier = globalMultiplier;
    }

    static int getGlobalMultiplier() {
        return Predicates<T>::globalMultiplier;
    }

    static bool isEven(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isEven: " + String(value % 2 == 0) + " for value: " + String(value));
        }
        return value % 2 == 0;
    }

    static bool isOdd(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isOdd: " + String(value % 2 != 0) + " for value: " + String(value));
        }
        return value % 2 != 0;
    }

    static bool isPositive(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isPositive: " + String(value > 0) + " for value: " + String(value));
        }
        return value > 0;
    }

    static bool isNegative(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isNegative: " + String(value < 0) + " for value: " + String(value));
        }
        return value < 0;
    }

    static bool isZero(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isZero: " + String(value == 0) + " for value: " + String(value));
        }
        return value == 0;
    }

    static bool isNonZero(const T& value) {
        if(debug) {
            Serial.println("[PREDICATES]: isNonZero: " + String(value != 0) + " for value: " + String(value));
        }
        return value != 0;
    }

    static bool isGreaterThan(T value) {
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is greater than globalMultiplier: " + String(globalMultiplier));
        }
        return value > globalMultiplier;
    }

    static bool isGreaterThan(T value, T value2){
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is greater than " + String(value2));
        }
        return value > value2;
    }

    static bool isLessThan(T value){
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is less than globalMultiplier: " + String(globalMultiplier));
        }
        return value < globalMultiplier;
    }

    static bool isLessThan(T value, T value2){
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is less than " + String(value2));
        }
        return value < value2;
    }

    static bool isEqual(T value){
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is equal to globalMultiplier: " + String(globalMultiplier));
        }
        return value == globalMultiplier;
    }

    static bool isEqual(T value, T value2){
        if(debug){
            Serial.println("[PREDICATES]: Checking if " + String(value) + " is equal to " + String(value2));
        }
        return value == value2;
    }

    static bool isGreaterThanOrEqualTo(const T& value, const T& threshold) {
        if(debug) {
            Serial.println("[PREDICATES]: isGreaterThanOrEqualTo: " + String(value >= threshold) + " for value: " + String(value) + " and threshold: " + String(threshold));
        }
        return value >= threshold;
    }

    static bool isLessThanOrEqualTo(const T& value, const T& threshold) {
        if(debug) {
            Serial.println("[PREDICATES]: isLessThanOrEqualTo: " + String(value <= threshold) + " for value: " + String(value) + " and threshold: " + String(threshold));
        }
        return value <= threshold;
    }

    static bool isEqualTo(const T& value, const T& threshold) {
        if(debug) {
            Serial.println("[PREDICATES]: isEqualTo: " + String(value == threshold) + " for value: " + String(value) + " and threshold: " + String(threshold));
        }
        return value == threshold;
    }

    static bool isNotEqualTo(const T& value, const T& threshold) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotEqualTo: " + String(value != threshold) + " for value: " + String(value) + " and threshold: " + String(threshold));
        }
        return value != threshold;
    }

    static bool isBetween(const T& value, const T& lower, const T& upper) {
        if(debug) {
            Serial.println("[PREDICATES]: isBetween: " + String(value >= lower && value <= upper) + " for value: " + String(value) + " and lower: " + String(lower) + " and upper: " + String(upper));
        }
        return value >= lower && value <= upper;
    }

    static bool isNotBetween(const T& value, const T& lower, const T& upper) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotBetween: " + String(value < lower || value > upper) + " for value: " + String(value) + " and lower: " + String(lower) + " and upper: " + String(upper));
        }
        return value < lower || value > upper;
    }

    static bool isWithin(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithin: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithin(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithin: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinExclusive: " + String(value > center - radius && value < center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value > center - radius && value < center + radius;
    }

    static bool isNotWithinExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinExclusive: " + String(value <= center - radius || value >= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value <= center - radius || value >= center + radius;
    }

    static bool isWithinInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinInclusive: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithinInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinInclusive: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinLeftExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinLeftExclusive: " + String(value > center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value > center - radius && value <= center + radius;
    }

    static bool isNotWithinLeftExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinLeftExclusive: " + String(value <= center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value <= center - radius || value > center + radius;
    }

    static bool isWithinRightExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinRightExclusive: " + String(value >= center - radius && value < center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value >= center - radius && value < center + radius;
    }

    static bool isNotWithinRightExclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinRightExclusive: " + String(value < center - radius || value >= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value < center - radius || value >= center + radius;
    }

    static bool isWithinLeftInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinLeftInclusive: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithinLeftInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinLeftInclusive: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinRightInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinRightInclusive: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithinRightInclusive(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinRightInclusive: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinLeft: " + String(value > center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value > center - radius && value <= center + radius;
    }

    static bool isNotWithinLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinLeft: " + String(value <= center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value <= center - radius || value > center + radius;
    }

    static bool isWithinRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinRight: " + String(value >= center - radius && value < center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value >= center - radius && value < center + radius;
    }

    static bool isNotWithinRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinRight: " + String(value < center - radius || value >= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value < center - radius || value >= center + radius;
    }

    static bool isWithinInclusiveLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinInclusiveLeft: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithinInclusiveLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinInclusiveLeft: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinInclusiveRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinInclusiveRight: " + String(value >= center - radius && value <= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value >= center - radius && value <= center + radius;
    }

    static bool isNotWithinInclusiveRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinInclusiveRight: " + String(value < center - radius || value > center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius)); 
        }
        return value < center - radius || value > center + radius;
    }

    static bool isWithinExclusiveLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinExclusiveLeft: " + String(value > center - radius && value < center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));  
        }
        return value > center - radius && value < center + radius;
    }

    static bool isNotWithinExclusiveLeft(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinExclusiveLeft: " + String(value <= center - radius || value >= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));  
        }
        return value <= center - radius || value >= center + radius;
    }

    static bool isWithinExclusiveRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isWithinExclusiveRight: " + String(value > center - radius && value < center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));  
        }
        return value > center - radius && value < center + radius;
    }

    static bool isNotWithinExclusiveRight(const T& value, const T& center, const T& radius) {
        if(debug) {
            Serial.println("[PREDICATES]: isNotWithinExclusiveRight: " + String(value <= center - radius || value >= center + radius) + " for value: " + String(value) + " and center: " + String(center) + " and radius: " + String(radius));  
        }
        return value <= center - radius || value >= center + radius;
    }

    static bool isPrime(T value){
        if (value <= 1) {
            if(debug){
                Serial.println("[PREDICATES]: Value is Negative: " + value);
            }
            return false;
        }
        for (static T i = 2; i < value; i++) {
            if (value % i == 0) {
                if(debug){
                    Serial.println("[PREDICATES]: Value is Not Prime: " + value);
                }
                return false;
            }
        }
        if(debug){
            Serial.println("[PREDICATES]: Value is Prime: " + value);
        }
        return true;
    }

    static bool isDivisibleBy(T value) {
        if (value % globalMultiplier == 0) {
            if(debug){
                Serial.println("[PREDICATES]: Value is Divisible by Global Multiplier: " + value);
            }
            return true;
        }
        if(debug){
            Serial.println("[PREDICATES]: Value is Not Divisible by Global Multiplier: " + value);
        }
        return false;
    }

    static bool isDivisibleBy(T value, T divisor) {
        if (value % divisor == 0) {
            if(debug){
                Serial.println("[PREDICATES]: Value is Divisible by Divisor: " + value);
            }
            return true;
        }
        if(debug){
            Serial.println("[PREDICATES]: Value is Not Divisible by Divisor: " + value);
        }
        return false;
    }

    static bool isDivisibleByTwo(T value) {
        if (value % 2 == 0) {
            if(debug){
                Serial.println("[PREDICATES]: Value is Divisible by Two: " + value);
            }
            return true;
        }
        if(debug){
            Serial.println("[PREDICATES]: Value is Not Divisible by Two: " + value);
        }
        return false;
    }
};

#endif // PREDICATES_H