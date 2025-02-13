#ifndef VARIANT_H
#define VARIANT_H

#include <ArrayList.h>
#include <SimpleVector.h>

//#define useSimpleVector //Uncomment this line to use the SimpleVector library instead of ArrayList
template <typename T>
class Variant {
private:
    bool isList; // true if ArrayList/SimpleVector (if you cant figure out why its called isList, then maybe you shouldnt be using this class :))    T singleValue;
    T singleValue; // Holds the single value
    #ifndef useSimpleVector
    ArrayList<T> listValues; // ArrayList of values
    #elif defined(useSimpleVector)
    SimpleVector<T> listValues;
    #endif

public:
    /**
     * @brief Default constructor
     * 
     * Creates a Variant object with a single value
     * 
    */
    Variant() : isList(false), singleValue(T()) {} // Default to single value
    
    /**
     * @brief Constructor
     * 
     * Creates a Variant object with a single value
     * 
     * @param value The value to store
     */
    Variant(const T& value) : isList(false), singleValue(value) {}

    #ifndef useSimpleVector

    /**
     * @brief Constructor
     * 
     * Creates a Variant object with an ArrayList/SimpleVector
     * 
     * @param values The ArrayList/SimpleVector of values to store
     */
    Variant(const ArrayList<T>& values) : isList(true), listValues(values) {}
    #elif defined(useSimpleVector)
    Variant(const SimpleVector<T>& values) : isList(true), listValues(values) {}
    #endif
    /**
     * @brief Checks if the Variant is a single value
     * 
     * @return true if the Variant is a single value
     * @return false if the Variant is an ArrayList
     */
    bool isSingle() const { return !isList; }

    #ifndef useSimpleVector
    /**
     * @brief Checks if the Variant is an ArrayList/SimpleVector
     * 
     * @return true if the Variant is an ArrayList/SimpleVector
     * @return false if the Variant is a single value
     */
    bool isArrayList() const { return isList; }
    #elif defined(useSimpleVector)
    bool isSimpleVector() const { return isList; }
    #endif

    /**
     * @brief Sets the value of the Variant to a single value
     * 
     * @param value The value to set
     */
    void setSingle(const T& value) {
        isList = false;
        singleValue = value;
    }

    /**
     * @brief Sets the value of the Variant to an ArrayList
     * 
     * @param values The ArrayList of values to set
     */
    void setList(const ArrayList<T>& values) {
        isList = true;
        listValues = values;
    }

    /**
     * @brief Gets the single value of the Variant
     * 
     * @return The single value of the Variant
     */
    T getSingle() const {
        return isList ? listValues.get(0) : singleValue;
    }


    #ifndef useSimpleVector
    /**
     * @brief Gets the ArrayList/SimpleVector of values of the Variant
     * 
     * @return The ArrayList/SimpleVector of values of the Variant
     */
    ArrayList<T>& getList() {
        if (!isList) {
            listValues.clear();
            listValues.add(singleValue);
            isList = true;
        }
        return listValues;
    }

    /**
     * @brief Gets the ArrayList of values of the Variant
     * 
     * @return The ArrayList of values of the Variant
     */
    const ArrayList<T>& getList() const {
        return listValues;
    }
    #elif defined(useSimpleVector)
    SimpleVector<T>& getList() {
        if (!isList) {
            listValues.clear();
            listValues.add(singleValue);
            isList = true;
        }
        return listValues;
    }

    /**
     * @brief Gets the ArrayList of values of the Variant
     * 
     * @return The ArrayList of values of the Variant
     */
    const SimpleVector<T>& getList() const {
        return listValues;
    }
    #endif

    /**
     * @brief Adds a value to the ArrayList/SimpleVector of values of the Variant
     * 
     * @param value The value to add
     * 
     * @note If the Variant is a single value, it will be converted to an ArrayList/SimpleVector
     */
    void addValue(const T& value) {
        if (!isList) {
            listValues.add(singleValue);
            isList = true;
        }
        listValues.add(value);
    }

    #ifndef useSimpleVector
    /**
     * @brief Gets the size of the Variant
     * 
     * @return The size of the Variant
     */
    int size() const {
        return isList ? listValues.size() : 1;
    }
    #elif defined(useSimpleVector)
    int size() const {
        return isList ? listValues.elements() : 1;
    }
    #endif

    /**
     * @brief Clears the Variant
     * 
     */
    void clear() {
        isList = false;
        singleValue = T();
        listValues.clear();
    }
};


#endif