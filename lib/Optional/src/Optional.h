#ifndef OPTIONAL_H
#define OPTIONAL_H




//#include <utility.h> // std::forward
//#define MINIMUM_IMPLEMENTATION
template <typename T>
class Optional {
public:
    Optional() : has_value(false), value() {} // Constructor with no value
    Optional(const T& value) : has_value(true), value(value) {} // Constructor with value

    bool hasValue() const { return has_value; } // Check if the Optional has a value
    
    /**
     * @brief Retrieves the stored value.
     * 
     * If the Optional does not contain a value, a static default value is returned.
     * CONST
     * @return The stored value or a static default value if no value is present.
     */
    const T& getValue() const {
        if (!has_value) {
            static T default_value = T(); // ✅ Safe default value
            return default_value;
        }
        return value;
    }

    /**
     * @brief Retrieves the stored value.
     * 
     * If the Optional does not contain a value, a static default value is returned.
     * 
     * @return The stored value or a static default value if no value is present.
     */
    T& getValue() {
        if (!has_value) {
            static T default_value = T(); // ✅ Safe default value
            return default_value;
        }
        return value;
    }
    #ifndef MINIMUM_IMPLEMENTATION // Optional methods for advanced usage
    /**
     * @brief Resets the Optional to an empty state.
     * 
     * This function clears any stored value and sets the Optional to indicate
     * that it does not contain a value.
     */
    void reset() {
        has_value = false;
    }
    
    /**
     * @brief Assignment operator for Optional.
     * 
     * Assigns a new value to the Optional.
     * 
     * @param new_value The new value to assign.
     * @return A reference to this Optional after assignment.
     */
    Optional& operator=(const T& new_value) {
        value = new_value;
        has_value = true;
        return *this;
    }

    /**
     * @brief Copy assignment operator for Optional.
     * 
     * Assigns the value and state from another Optional to this one.
     * 
     * @param other The Optional to assign from.
     * @return A reference to this Optional after assignment.
     */
    Optional& operator=(const Optional& other) {
        if (this != &other) {
            value = other.value;
            has_value = other.has_value;
        }
        return *this;
    }

    /**
     * @brief Conversion operator to bool.
     * 
     * Allows an Optional to be used in a boolean context.
     * 
     * @return true if the Optional has a value, false otherwise.
     */
    explicit operator bool() const {
        return has_value;
    }

    /**
     * @brief Checks if two Optionals are equal.
     * 
     * Two Optionals are equal if they either both have a value or both do not have a value.
     * If both have a value, the values must be equal.
     * 
     * @param other The Optional to compare with.
     * 
     * @return true if the two Optionals are equal, false otherwise.
     */
    bool operator==(const Optional& other) const {
        return has_value == other.has_value && (!has_value || value == other.value);
    }


    /**
     * @brief Checks if two Optionals are not equal.
     * 
     * This is the negation of the operator== function.
     * 
     * @param other The Optional to compare with.
     * 
     * @return true if the two Optionals are not equal, false otherwise.
     */
    bool operator!=(const Optional& other) const {
        return !(*this == other);
    }

    /**
     * @brief Returns the stored value if present, otherwise returns the provided default value.
     * 
     * @param defaultValue The value to return if the Optional does not contain a value.
     * 
     * @return The stored value if present, otherwise the provided default value.
     */
    T orElse(const T& defaultValue) const {
        return has_value ? value : defaultValue;
    }
    
    /**
     * @brief Transforms the value of the Optional using a given function.
     * 
     * If the Optional has a value, the function will be executed with the value as its argument.
     * If the Optional does not have a value, the function will not be executed.
     * 
     * @tparam Function The type of the function to apply to the value of the Optional.
     * @param func The function to apply to the value of the Optional.
     * @return The result of the function applied to the value of the Optional, if it has a value.
     *         Otherwise, the result of the function.
     */
    template<typename Function>
    T orElseGet(Function defaultFunc) const {
        return has_value ? value : defaultFunc();
    }
     
    
    /**
     * @brief Applies a function to the value of the Optional if it exists.
     * 
     * @param func The function to apply to the value of the Optional.
     * 
     * If the Optional has a value, the function will be executed with the value as its argument.
     * If the Optional does not have a value, this function does nothing.
     * 
     * @note This function does not return anything, it only applies the function to the value of the Optional.
     */
    template<typename Function>
    void ifPresent(Function func) const {
        if (has_value) {
            func(value);
        }
    }
    
    /**
     * @brief Swaps the contents of this Optional with another Optional.
     * 
     * This function exchanges the value and state (has_value) of the current
     * Optional object with those of the provided Optional object.
     * 
     * @param other The Optional object to swap with.
     */
    void swap(Optional& other) {
        Optional temp = *this; // Sets temp to a copy of this object
        *this = other; // Sets this object to a copy of the other object
        other = temp; // Sets the other object to the copy of this object
    }

    /**
     * @brief Checks if the Optional contains the default value of the underlying type.
     * 
     * This method checks if the Optional does not contain a value or if the value it contains is
     * equal to the default-constructed value of its type.
     * 
     * @return true if the Optional contains the default value of the underlying type, false otherwise.
     */
    bool isDefaultValue() const {
        return !has_value || value == T();
    }

    /**
     * @brief Constructs and assigns a new value to the Optional using the provided arguments.
     * 
     * This method constructs a new value of type T in-place using the provided arguments.
     * It sets the Optional to indicate that it contains a value.
     * 
     * @tparam Args The types of the arguments to pass to the constructor of T.
     * @param args The arguments to pass to the constructor of T.
     */
    template<typename... Args>
    void emplace(Args... args) {
        value = T(args...);  // Construct new value in place
        has_value = true;
    }


    /**
     * @brief Clears the Optional.
     * 
     * This function sets the Optional to indicate that it does not contain a value.
     */
    void clear() {
        has_value = false;
    }

    /**
     * @brief Returns a reference to the stored value.
     * 
     * If the Optional contains a value, this function returns a reference to the stored value.
     * If the Optional does not contain a value, this function returns a reference to a statically
     * allocated default-constructed value of type T.
     * 
     * @return A reference to the stored value if the Optional contains a value, otherwise a reference
     *         to a statically allocated default-constructed value of type T.
     */
    T& ref() {
        if (!has_value) {
            static T default_value = T();
            return default_value;
        }
        return value;
    }
    
    #endif // MINIMUM_IMPLEMENTATION
private:
    bool has_value;
    T value;
};


template<>
class Optional<void> {
public:
    Optional() : has_value(false) {}

    bool hasValue() const { return has_value; }
    void set() { has_value = true; }
    void reset() { has_value = false; }
    explicit operator bool() const { return has_value; }

private:
    bool has_value;
};


#endif // OPTIONAL_H
