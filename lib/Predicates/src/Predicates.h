#ifndef PREDICATES_H
#define PREDICATES_H

#include <Arduino.h>
template <typename T>
class Predicates {
private:
    static int globalMultiplier;
public:


    /**
     * @brief Sets the global multiplier for the Predicates class.
     * 
     * This Function sets the Global Multiplier which is useful if you dont want to pass in the same value over and over again.
     * 
     * @param globalMultiplier The value that you want to set the global multiplier to.
     * 
    */
    static void setGlobalMultiplier(int globalMultiplier) {
        Predicates<T>::globalMultiplier = globalMultiplier;
    }

    /**
     * @brief Gets the global multiplier for the Predicates class.
     * 
     * This Function gets the Global Multiplier which is useful if you dont want to pass in the same value over and over again.
     * 
     * @return The value of the global multiplier.
     * 
    */
    static int getGlobalMultiplier() {
        return Predicates<T>::globalMultiplier;
    }

    /**
     * @brief Checks if the value is even.
     * 
     * This function checks if the value is even.
     * 
     * @param value The value you want to check if it is even.
     * @return A boolean value indicating whether the value is even (true) or odd (false).
    */
    static bool isEven(const T& value) {
        return value % 2 == 0;
    }

    /**
     * @brief Checks if the value is odd.
     * 
     * This function checks if the value is odd.
     * 
     * @param value The value you want to check if it is odd
     * @return A boolean value indicating whether the value is odd (true) or even (false).
    */
    static bool isOdd(const T& value) {
        return value % 2 != 0;
    }

    /**
     * @brief Checks if the value is positive.
     * 
     * This function checks if the value is positive.
     * @param value The value you want to check is positive.
     * @return A boolean value indicating whether the value is positive (true) or negative (false).
    */
    static bool isPositive(const T& value) {
        return value > 0;
    }

    /**
     * @brief Checks if the value is negative.
     * 
     * This function checks if the value is negative.
     * @param value The value you want to check is positive
     * @return A boolean value indicating whether the value is negative (true) or positive (false).
    */
    static bool isNegative(const T& value) {
        return value < 0;
    }

    /**
     * @brief Checks if the value is zero.
     * 
     * This function checks if the value is zero.
     * @param value The value you want to check is zero.
     * @return A boolean value indicating whether the value is zero (true) or non-zero (false).
    */
    static bool isZero(const T& value) {
        return value == 0;
    }

    /**
     * @brief Checks if the value is non-zero.
     * 
     * This function checks if the value is non-zero.
     * @param value The value you want to check is not zero.
     * @return A boolean value indicating whether the value is non-zero (true) or zero (false).
    */
    static bool isNonZero(const T& value) {
        return value != 0;
    }

    /**
     * @brief Checks if the value is greater than tje Global Multiplier.
     * 
     * This function checks if the value is greater than the Global Multiplier.
     * @param value The value you want to check if it is greater then the Global Multiplier.
     * @return A boolean value indicating whether the value is greater than zero (true) or less than or equal to zero (false).
    */
    static bool isGreaterThan(T value) {
        return value > globalMultiplier;
    }

    /**
     * @brief Checks if the value is greater than the Global the 2nd Passed in Value.
     * 
     * @param value The value that you want to check if it is greater than the 2nd passed in value.
     * 
     * @param value2 The value that you want to check if the 1st passed in value is greater than.
     * 
     * @return A boolean value indicating whether the value is greater than the 2nd passed in value (true) or less than or equal to the 2nd passed in value (false).
    */
    static bool isGreaterThan(T value, T value2){
        return value > value2;
    }

    /**
     * @brief Checks if the value is less than the Global Multiplier.
     * 
     * This function checks if the value is less than the Global Multiplier.
     * @param value The value you want to check is less than the Global Multiplier
     * @return A boolean value indicating whether the value is less than the Global Multiplier (true) or greater than the Global Multiplier (false).
    */
    static bool isLessThan(T value){
        return value < globalMultiplier;
    }

    /**
     * @brief Checks if value is lessthan value2
     * 
     * This function checks if the value is less than value2
     * @param value value you are checking if it is less then value2
     * @param value2 value that determines if value one will return true or false
     * @return A boolean value indicating whether the value is less than value2 (true) or greater than value2 (false)
    */
    static bool isLessThan(T value, T value2){
        return value < value2;
    }

    /**
     * @brief Checks value against globalMultiplier to see if they are equal.
     * 
     * This function checks if the value is equal to the global Multiplier
     * @param value Value you are checking to see is equal to the Global Multiplier
     * @return A boolean value to indicate whether the value was equals to the Global Multiplier (true) or if it is not equal (false)
    */
    static bool isEqual(T value){
        return value == globalMultiplier;
    }

     /**
     * @brief Checks value against value2 to see if they are equal.
     * 
     * This function checks if the value is equal to value2
     * @param value Value you are checking to see is equal to value2
     * @param value2 Comparater value. (Value you are checking against)
     * @return A boolean value to indicate whether the value was equals to value2 (true) or if it is not equal (false)
    */
    static bool isEqual(T value, T value2){
        return value == value2;
    }

    /**
     * @brief Checks value against globalMultiplier to see if they are lessThan or equal.
     * 
     * This function checks if the value is lessthan or equal to the global Multiplier
     * @param value Value you are checking to see is Lessthan or equal to the Global Multiplier
     * @return A boolean value to indicate whether the value was Less than or equal to to the Global Multiplier (true) or if it is Not (false)
    */
    static bool isLessThan(const T& value, const T& threshold) {
        return value < threshold;
    }

    /**
     * @brief Checks value against value2 to see if they are lessThan or equal.
     * 
     * This function checks if the value is lessthan or equal to value2
     * @param value Value you are checking to see is Lessthan or equal to value2
     * @param threshold Comparater value. (Value you are checking against)
     * @return A boolean value to indicate whether the value was Less than or equal to to value2 (true) or if it is Not (false)
    */
    static bool isGreaterThanOrEqualTo(const T& value, const T& threshold) {
        return value >= threshold;
    }

    /**
     * @brief Checks value against value2 to see if they are greaterThan or equal.
     * 
     * This function checks if the value is greaterThan or equal to value2
     * @param value Value you are checking to see is greaterThan or equal to value2
     * @param threshold Comparater value. (Value you are checking against)
     * @return A boolean value to indicate whether the value was greaterThan or equal to to value2 (true) or if it is Not (false)
    */
    static bool isLessThanOrEqualTo(const T& value, const T& threshold) {
        return value <= threshold;
    }

    /**
     * @brief Checks value against the threshold to see if they are equal.
     * 
     * This function checks if the value is equal to the threshold
     * @param value Value you are checking to see is equal to the threshold
     * @param threshold Comparater value. (Value you are checking against)
     * @return A boolean value to indicate whether the value was equal to to the Threshold (true) or if it is Not (false)
    */
    static bool isEqualTo(const T& value, const T& threshold) {
        return value == threshold;
    }

    /**
     * @brief Checks value against the threshold to see if they are not equal.
     * 
     * This function checks if the value is not equal to the threshold
     * @param value Value you are checking to see is not equal to the threshold
     * @param threshold Comparater value. (Value you are checking against)
     * @return A boolean value to indicate whether the value was not equal to to the Threshold (true) or if it is (false)
    */
    static bool isNotEqualTo(const T& value, const T& threshold) {
        return value != threshold;
    }

    /**
     * @brief Checks if a value is between two other values (inclusive).
     *
     * This function checks if the provided value is greater than or equal to the lower value
     * and less than or equal to the upper value. If the value is between the lower and upper values,
     * the function returns true; otherwise, it returns false.
     *
     * @param value The value to check.
     * @param lower The lower bound.
     * @param upper The upper bound.
     * @return true if the value is between the lower and upper values (inclusive), false otherwise.
    */
    static bool isBetween(const T& value, const T& lower, const T& upper) {
        return value >= lower && value <= upper;
    }

    /**
     * @brief Checks if a value is not between two other values (exclusive).
     *
     * This function checks if the provided value is less than the lower value
     * or greater than the upper value. If the value is not between the lower and upper values,
     * the function returns true; otherwise, it returns false.
     *
     * @param value The value to check.
     * @param lower The lower bound.
     * @param upper The upper bound.
     * @return true if the value is not between the lower and upper values (exclusive), false otherwise.
     */
    static bool isNotBetween(const T& value, const T& lower, const T& upper) {
        return value < lower || value > upper;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value.
     *
     * This function checks if the provided value is within a certain radius of a center value.
     * In other words, it checks if the value is greater than or equal to (center - radius) and
     * less than or equal to (center + radius). If the value is within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, false otherwise.
    */
    static bool isWithin(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value.
     *
     * This function checks if the provided value is not within a certain radius of a center value.
     * In other words, it checks if the value is less than (center - radius) or
     * greater than (center + radius). If the value is not within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, false otherwise.
    */
    static bool isNotWithin(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding the boundaries.
     *
     * This function checks if the provided value is within a certain radius of a center value, but not equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is strictly greater than 
     * (center - radius) and strictly less than (center + radius). If the value is within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, excluding the boundaries, false otherwise.
    */
    static bool isWithinExclusive(const T& value, const T& center, const T& radius) {
        return value > center - radius && value < center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the boundaries.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) and greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the boundaries, false otherwise.
    */
    static bool isNotWithinExclusive(const T& value, const T& center, const T& radius) {
        return value <= center - radius || value >= center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the boundaries.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, including the boundaries, false otherwise.
    */
    static bool isWithinInclusive(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the boundaries.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than 
     * (center - radius) or greater than (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the boundaries, false otherwise.
    */
    static bool isNotWithinInclusive(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding the lower boundary and including the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, but not equal to
     * (center - radius) and less than or equal to (center + radius). In other words, it checks if the value is strictly greater than 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, excluding the lower boundary and including the upper boundary, false otherwise.
    */
    static bool isWithinLeftExclusive(const T& value, const T& center, const T& radius) {
        return value > center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than (center + radius). If the value is not within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isNotWithinLeftExclusive(const T& value, const T& center, const T& radius) {
        return value <= center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and not equal to (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and strictly less than (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isWithinRightExclusive(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value < center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than 
     * (center - radius) or greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isNotWithinRightExclusive(const T& value, const T& center, const T& radius) {
        return value < center - radius || value >= center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @param debug A boolean value indicating whether or not to print debug statements.
     * @return true if the value is within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isWithinLeftInclusive(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) and strictly greater than (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
     */
    static bool isNotWithinLeftInclusive(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @param debug A boolean value indicating whether or not to print debug statements.
     * @return true if the value is within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isWithinRightInclusive(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) and strictly greater than (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isNotWithinRightInclusive(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding the lower boundary and including the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, but not equal to
     * (center - radius) and less than or equal to (center + radius). In other words, it checks if the value is strictly greater than 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isWithinLeft(const T& value, const T& center, const T& radius) {
        return value > center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than (center + radius). If the value is not within this range, the function
     * returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     * 
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isNotWithinLeft(const T& value, const T& center, const T& radius) {
        return value <= center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding the lower boundary and including the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and not equal to (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and strictly less than (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isWithinRight(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value < center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than 
     * (center - radius) or greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isNotWithinRight(const T& value, const T& center, const T& radius) {
        return value < center - radius || value >= center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isWithinInclusiveLeft(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
    */
    static bool isNotWithinInclusiveLeft(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, including the lower boundary and excluding the upper boundary.
     *
     * This function checks if the provided value is within a certain radius of a center value, including
     * (center - radius) and (center + radius). In other words, it checks if the value is greater than or equal to 
     * (center - radius) and less than or equal to (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * 
     * @return true if the value is within the specified radius of the center value, including the lower boundary and excluding the upper boundary, false otherwise.
    */
    static bool isWithinInclusiveRight(const T& value, const T& center, const T& radius) {
        return value >= center - radius && value <= center + radius;
    }
    
    /**
     * @brief Checks if a value is not within a certain radius of a center value, excluding the lower boundary and including the upper boundary.
     *
     * This function checks if the provided value is not within a certain radius of a center value, strictly less than
     * (center - radius) or equal to (center + radius). In other words, it checks if the value is less than 
     * (center - radius) or greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, excluding the lower boundary and including the upper boundary, false otherwise.
    */
    static bool isNotWithinInclusiveRight(const T& value, const T& center, const T& radius) {
        return value < center - radius || value > center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding both boundaries.
     *
     * This function checks if the provided value is within a certain radius of a center value, strictly greater than
     * (center - radius) and strictly less than (center + radius). In other words, it checks if the value is greater than 
     * (center - radius) and less than (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     *
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle.
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, excluding both boundaries, false otherwise.
    */
    static bool isWithinExclusiveLeft(const T& value, const T& center, const T& radius) {
        return value > center - radius && value < center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, excluding both boundaries.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     * 
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, excluding both boundaries, false otherwise.
    */
    static bool isNotWithinExclusiveLeft(const T& value, const T& center, const T& radius) {
        return value <= center - radius || value >= center + radius;
    }

    /**
     * @brief Checks if a value is within a certain radius of a center value, excluding both boundaries.
     *
     * This function checks if the provided value is within a certain radius of a center value, strictly greater than
     * (center - radius) and strictly less than (center + radius). In other words, it checks if the value is greater than 
     * (center - radius) and less than (center + radius). If the value is within this range, 
     * the function returns true; otherwise, it returns false.
     * 
     * This can be visualized as checking if a point is within a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is within the specified radius of the center value, excluding both boundaries, false otherwise.
    */
    static bool isWithinExclusiveRight(const T& value, const T& center, const T& radius) {
        return value > center - radius && value < center + radius;
    }

    /**
     * @brief Checks if a value is not within a certain radius of a center value, excluding both boundaries.
     *
     * This function checks if the provided value is not within a certain radius of a center value, or equal to
     * (center - radius) or (center + radius). In other words, it checks if the value is less than or equal to 
     * (center - radius) or greater than or equal to (center + radius). If the value is not within this range, 
     * the function returns true; otherwise, it returns false.
     * 
     * This can be visualized as checking if a point is outside a circle in a one-dimensional space,
     * where the center value represents the center of the circle and the radius represents the radius of the circle. 
     *
     * @param value The value to check.
     * @param center The center value.
     * @param radius The radius.
     * @return true if the value is not within the specified radius of the center value, excluding both boundaries, false otherwise.
    */
    static bool isNotWithinExclusiveRight(const T& value, const T& center, const T& radius) {
        return value <= center - radius || value >= center + radius;
    }

    /**
     * @brief Checks if a value is a prime number.
     *
     * This function checks if the provided value is a prime number. A prime number is a natural number greater than 1 
     * that has no positive divisors other than 1 and itself. The function checks all numbers from 2 to the value - 1 
     * and returns false if the value is divisible by any of these numbers. If the value is not divisible by any of these numbers, 
     * it is a prime number, and the function returns true.
     *
     * @param value The value to check.
     * @return true if the value is a prime number, false otherwise.
    */
    static bool isPrime(T value){
        if (value <= 1) {
            return false;
        }
        for (static T i = 2; i < value; i++) {
            if (value % i == 0) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Checks if a value is divisible by the global multiplier.
     *
     * This function checks if the provided value is divisible by the global multiplier. If the value modulo the global multiplier equals zero,
     * it means that the value is divisible by the global multiplier, and the function returns true. Otherwise, the function returns false.
     *
     * @param value The value to check.
     * @return true if the value is divisible by the global multiplier, false otherwise.
    */
    static bool isDivisibleBy(T value) {
        if (value % globalMultiplier == 0) {
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if a value is divisible by a divisor.
     *
     * This function checks if the provided value is divisible by a divisor. If the value modulo the divisor equals zero,
     * it means that the value is divisible by the divisor, and the function returns true. Otherwise, the function returns false.
     *
     * @param value The value to check.
     * @param divisor The divisor.
     * @return true if the value is divisible by the divisor, false otherwise.
    */
    static bool isDivisibleBy(T value, T divisor) {
        if (value % divisor == 0) {
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if a value is divisible by two.
     *
     * This function checks if the provided value is divisible by two. If the value modulo two equals zero,
     * it means that the value is divisible by two, and the function returns true. Otherwise, the function returns false.
     *
     * @param value The value to check.
     * @return true if the value is divisible by two, false otherwise.
    */
    static bool isDivisibleByTwo(T value) {
        if (value % 2 == 0) {
            return true;
        }
        return false;
    }
};

#endif // PREDICATES_H