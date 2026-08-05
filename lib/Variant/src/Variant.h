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
    ArrayList<T>* listValues; // Lazily allocated ArrayList of values
    #elif defined(useSimpleVector)
    SimpleVector<T>* listValues; // Lazily allocated SimpleVector of values
    #endif

    // Ensure listValues is allocated, seeding it with singleValue if transitioning from single mode.
    void ensureList() {
        if (!listValues) {
            #ifndef useSimpleVector
            listValues = new ArrayList<T>();
            #else
            listValues = new SimpleVector<T>();
            #endif
        }
        if (!isList) {
            listValues->clear();
            listValues->add(singleValue);
            isList = true;
        }
    }

public:
    /**
     * @brief Default constructor
     * 
     * Creates a Variant object with a single value
     * 
    */
    Variant() : isList(false), singleValue(T()), listValues(nullptr) {} // Default to single value
    
    /**
     * @brief Constructor
     * 
     * Creates a Variant object with a single value
     * 
     * @param value The value to store
     */
    Variant(const T& value) : isList(false), singleValue(value), listValues(nullptr) {}

    #ifndef useSimpleVector

    /**
     * @brief Constructor
     * 
     * Creates a Variant object with an ArrayList/SimpleVector
     * 
     * @param values The ArrayList/SimpleVector of values to store
     */
    Variant(const ArrayList<T>& values) : isList(true), singleValue(T()), listValues(new ArrayList<T>(values)) {}
    #elif defined(useSimpleVector)
    Variant(const SimpleVector<T>& values) : isList(true), singleValue(T()), listValues(new SimpleVector<T>(values)) {}
    #endif

    /**
     * @brief Copy constructor
     */
    Variant(const Variant<T>& other) : isList(other.isList), singleValue(other.singleValue), listValues(nullptr) {
        if (other.listValues) {
            #ifndef useSimpleVector
            listValues = new ArrayList<T>(*other.listValues);
            #else
            listValues = new SimpleVector<T>(*other.listValues);
            #endif
        }
    }

    /**
     * @brief Copy assignment operator
     */
    Variant<T>& operator=(const Variant<T>& other) {
        if (this != &other) {
            delete listValues;
            listValues = nullptr;
            isList = other.isList;
            singleValue = other.singleValue;
            if (other.listValues) {
                #ifndef useSimpleVector
                listValues = new ArrayList<T>(*other.listValues);
                #else
                listValues = new SimpleVector<T>(*other.listValues);
                #endif
            }
        }
        return *this;
    }

    /**
     * @brief Destructor
     */
    ~Variant() {
        delete listValues;
        listValues = nullptr;
    }

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
        if (listValues) {
            listValues->clear();
        }
    }

    /**
     * @brief Sets the value of the Variant to a list
     * 
     * Uses ArrayList<T> by default, or SimpleVector<T> when useSimpleVector is defined.
     * 
     * @param values The list of values to set (ArrayList<T> or SimpleVector<T> depending on configuration)
     */
    #ifdef useSimpleVector
    void setList(const SimpleVector<T>& values) {
        isList = true;
        if (!listValues) {
            listValues = new SimpleVector<T>(values);
        } else {
            *listValues = values;
        }
    }
    #else
    void setList(const ArrayList<T>& values) {
        isList = true;
        if (!listValues) {
            listValues = new ArrayList<T>(values);
        } else {
            *listValues = values;
        }
    }
    #endif

    /**
     * @brief Gets the single value of the Variant
     * 
     * @return The single value of the Variant
     */
    T getSingle() const {
        if (isList && listValues && listValues->size() > 0) {
            return listValues->get(0);
        }
        return singleValue;
    }


    #ifndef useSimpleVector
    /**
     * @brief Gets the ArrayList/SimpleVector of values of the Variant
     * 
     * @return The ArrayList/SimpleVector of values of the Variant
     */
    ArrayList<T>& getList() {
        ensureList();
        return *listValues;
    }

    /**
     * @brief Gets the ArrayList of values of the Variant
     * 
     * @return The ArrayList of values of the Variant
     */
    const ArrayList<T>& getList() const {
        return *listValues;
    }
    #elif defined(useSimpleVector)
    SimpleVector<T>& getList() {
        ensureList();
        return *listValues;
    }

    /**
     * @brief Gets the ArrayList of values of the Variant
     * 
     * @return The ArrayList of values of the Variant
     */
    const SimpleVector<T>& getList() const {
        return *listValues;
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
        ensureList();
        listValues->add(value);
    }

    #ifndef useSimpleVector
    /**
     * @brief Gets the size of the Variant
     * 
     * @return The size of the Variant
     */
    int size() const {
        return isList && listValues ? listValues->size() : 1;
    }
    #elif defined(useSimpleVector)
    int size() const {
        return isList && listValues ? listValues->elements() : 1;
    }
    #endif

    /**
     * @brief Clears the Variant
     * 
     */
    void clear() {
        isList = false;
        singleValue = T();
        if (listValues) {
            listValues->clear();
        }
    }
};


#endif