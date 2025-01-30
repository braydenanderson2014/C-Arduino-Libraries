#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <Arduino.h>
#ifdef ESP32
    #include <initializer_list>
    #define useInit
#endif
#ifdef ESPRESSIF32
    #include <initializer_list>
    #define useInit
#endif
#ifdef ESP8266
    #include <initializer_list>
    #define useInit
#endif
#ifdef ESP32S2
    #include <initializer_list>
    #define useInit
#endif
#ifdef ESP32C3
    #include <initializer_list>
    #define useInit
#endif
template<typename T>
class SimpleVector {
private:
    T* array;
    unsigned int count;
    unsigned int capacity;

    /**
     * @brief Resize the array to the specified capacity
     * @param newCapacity The new capacity of the array
     * 
     * @private This method is private because it is only used internally.
     * 
     * @note This method will copy all elements from the old array to the new array.
    */
    void resize(unsigned int newCapacity) {
        T* newArray = new T[newCapacity];
        if (!newArray) {
            Serial.println("Memory allocation failed during resize.");
            return;
        }
        for (unsigned int i = 0; i < count; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    /**
     * @brief Ensure that the vector has enough capacity to add a new element
     * 
     * @private This method is private because it is only used internally.
    */
    void ensureCapacity() {
        if (count == capacity) {
            resize(2 * capacity);
        }
    }
public:
    // The SimpleVectorIterator class will be defined below
    class SimpleVectorIterator;

    SimpleVector() : array(new T[4]), count(0), capacity(4) {
        if(!array){
            Serial.println("Memory allocation failed.");
        }
    }

    SimpleVector(unsigned int initialCapacity) : array(new T[initialCapacity]), count(0), capacity(initialCapacity) {
        if(!array){
            Serial.println("Memory allocation failed.");
        }
    }

    SimpleVector(const SimpleVector& other) : array(new T[other.capacity]), count(other.count), capacity(other.capacity) {
        for (unsigned int i = 0; i < count; i++) {
            array[i] = other.array[i];
        }
    }

    #ifdef useInit
    SimpleVector(initializer_list<T> initList) : array(new T[initList.size()]), count(initList.size()), capacity(initList.size()) {
        int i = 0;
        for (const auto& value : initList) {
            array[i++] = value;
        }
    }
    #endif

    ~SimpleVector() {
        releaseMemory();
    }


    // ... Other methods ...

    /**
     * @brief Release the memory used by the vector
     * 
     * @public This method is public because it is meant to be called by the user.
    */
    void releaseMemory() {
        if(array){
            delete[] array;
            array = nullptr;
            capacity = 0;
            count = 0;
        }
    }

    /**
     * @brief Shrink the capacity of the vector to match the count of elements
     * @param newCapacity The new capacity of the vector
     * @return True if the memory was successfully reserved, false otherwise
     * 
     * @public This method is public because it is meant to be called by the user.
    */
    bool shrinkToFit() {
        if (count < capacity) {
            resize(count);
            return true;
        }
        return false;
    }

    /**
    * @brief Clears the vector by setting all elements to their default value and resetting the count.
    * 
    * @public This method is public because it is meant to be called by the user.
    */
    void clear() {
        if(array){
            delete[] array;
        }
        array = new T[4];
        count = 0;
        capacity = 4;

    }

    /**
     * @brief Clears the vector and sets the capacity to the specified value.
     * @param newCapacity The new capacity of the vector
     * 
     * @public This method is public because it is meant to be called by the user.
    */
    void clear(size_t newCapacity) {
        if(array){
            delete[] array;
        }
        array = new T[newCapacity];
        count = 0;
        capacity = newCapacity;
    }

    /**
     * @brief Removes an item by index instead of feeding in the item itself.
     * 
     * @public This method is public
     * 
     * @date Added: 01/20/2025
     * @version Added Version 1.0.7 (PlatformIO) and Version 1.02 (Arduino Library Manager)
     */
    void erase(int index){
        remove(get(index));
    }

//Changed to put() for better naming conventions.
    /**
     * @brief Add an element to the vector
     * @param item The item to be added to the vector
     * 
    */
    void put(const T& item) {
        if (count == capacity) {
            resize(2 * capacity);
        }
        array[count++] = item;
    }

    template<typename... Args>
    void bulkAdd(Args... args) {
        const T temp[] = {args...};
        for (auto& val : temp) {
            put(val);
        }
    }

    void emplace_back() {  
        ensureCapacity();
        // Default-construct in place
        new (elements + size()) T();
        count++;
    }

    void emplace_back(const T& value) {
        ensureCapacity();
        // Copy-construct in place
        new (elements + size()) T(value);
        count++;
    }

    //back() method
    T& back() {
        return array[count - 1];
    }

    /**
     * @brief Add an element to the vector
     * @param item The item to be added to the vector
     * 
     * @note This method is an alias for the put method.
    */
    void push_back(const T& item) {
        put(item);
    }

    // Remove an element from the vector by shifting elements (Added in Version 1.0.1)
    /**
     * @brief Remove an element from the vector
     * @param item The item to be removed from the vector
     * 
     * @note This method will shift all elements after the specified element to the left by one.
    */
    void remove(const T& item) {
        unsigned int index = 0;
        for (unsigned int i = 0; i < count; i++) {
            if (array[i] == item) {
                // Element found, skip it
                continue;
            }
            array[index++] = array[i];
        }
        count = index;
    }

    /**
     * Overloads the subscript operator for the SimpleVector class.
     * 
     * This function is used to access elements in the vector at a given index.
     * If the index is out of bounds (i.e., greater than or equal to the count of elements), it returns nullptr.
     * 
     * @param index The index of the element to be accessed.
     * @return Reference to the element at the given index, or nullptr if the index is out of bounds.
     */
    T& operator[](unsigned int index) {
        static T dummy;
        if (index >= count || index < 0) {
            return dummy; // You can handle this error differently if needed
        }
        return array[index];
    }


    // Overload [] operator for const objects.
    const T& operator[](unsigned int index) const {
        static T dummy = T();
        if (index >= count) {
            return dummy;
        }
        return array[index];
    }

    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            delete[] array;
            array = new T[other.capacity];
            count = other.count;
            capacity = other.capacity;
            for (unsigned int i = 0; i < count; i++) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }

    const SimpleVector& operator=(const SimpleVector& other) const {
        if (this != &other) {
            delete[] array;
            array = new T[other.capacity];
            count = other.count;
            capacity = other.capacity;
            for (unsigned int i = 0; i < count; i++) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }

    /**
     * @brief Get the size of the vector
     * @return The size of the vector
     * 
     * @note This method returns the capacity of the vector, not the count of elements.
    */
    unsigned int size() const {
        if(this -> capacity == 0){
            return 0;
        }
        return this -> capacity;
    }

    /**
     * @brief Get the count of elements in the vector
     * @return The count of elements in the vector
    */
    unsigned int elements() const {
        if(this -> count == 0){
            return 0;
        }
        return this -> count;
    }

// Get the element at the specified index
    /**
     * @brief Get the element at the specified index
     * @param index The index of the element to be accessed
     * @return Reference to the element at the specified index
    */
    T& get(unsigned int index) {
        static T dummy = T();
        if (index >= count) {
            return dummy;
        }
        return array[index];
    }

    //NEW
    T* getPtr(unsigned int index) {
        if (index >= count) {
            return nullptr;
        }
        return &array[index];
    }



    bool isEmpty() const {
        return count == 0;
    }

    // Get the index of the specified element
    /**
     * @brief Get the index of the specified element
     * @param element The element to be searched for
     * @return The index of the specified element, or 0 if the element is not found
    */
    int indexOf(const T& element) {
        for (unsigned int i = 0; i < count; i++) {
            if (array[i] == element) {
                return i;
            }
        }
        return 0; // Element not found
    }

    // ... Other methods ...

    // SimpleVectorIterator related methods
    /**
     * @brief Get an iterator pointing to the first element in the vector
     * @return An iterator pointing to the first element in the vector
    */
    SimpleVectorIterator begin() {
        return SimpleVectorIterator(array, array + count);
    }

    /**
     * @brief Get an iterator pointing to the end of the vector
     * @return An iterator pointing to the end of the vector
    */
    SimpleVectorIterator end() {
        return SimpleVectorIterator(array + count, array + count);
    }

    const SimpleVectorIterator begin() const {
        return SimpleVectorIterator(array, array + count);
    }

    const SimpleVectorIterator end() const {
        return SimpleVectorIterator(array + count, array + count);
    }

    // The iterator type should be defined after the SimpleVectorIterator class.
    // This is because the SimpleVectorIterator class needs to be defined first.
    typedef SimpleVectorIterator iterator;

    // Definition of the SimpleVectorIterator class
    class SimpleVectorIterator {
    private:
        T* current;
        T* end;
    public:
        SimpleVectorIterator(T* start, T* stop) : current(start), end(stop) {}

        /**
         * @brief Check if the iterator has reached the end of the vector
         * @return True if the iterator has reached the end of the vector, false otherwise
        */
        bool hasNext() {
            return current != end;
        }

        /**
         * @brief Get the next element in the vector
         * @return The next element in the vector
        */
        T& next() {
            return *current++;
        }

        // The iterator needs to be compatible with STL-style iteration
        /**
         * @brief Overload the prefix increment operator
        */
        SimpleVectorIterator& operator++() {
            ++current;
            return *this;
        }

        /**
         * @brief Overload the postfix increment operator
        */
        bool operator!=(const SimpleVectorIterator& other) const {
            return current != other.current;
        }

        /**
         * @brief Overload the dereference operator
        */
        T& operator*() {
            return *current;
        }
    };
};
#endif // SIMPLEVECTOR_H
