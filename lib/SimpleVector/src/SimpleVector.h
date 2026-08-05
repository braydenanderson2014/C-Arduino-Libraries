#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <Arduino.h>

//#define SV_ENABLE_NUMERIC_LIMITS  // Uncomment to enable numeric_limits integration (requires Numeric_Limits library — optional dependency)
// PlatformIO: build_flags = -DSV_ENABLE_NUMERIC_LIMITS

#ifdef SV_ENABLE_NUMERIC_LIMITS
    #include <Numeric_Limits.h>
#endif

#if defined(ESP32) || defined(ESPRESSIF32) || defined(ESP8266) || defined(ESP32S2) || defined(ESP32C3)
    //#include <initializer_list>
    #define useInit
#endif
template<typename T>
class SimpleVector {
private:
    static const unsigned int DEFAULT_CAPACITY = 4;
    T* array;
    mutable unsigned int count;
    mutable unsigned int capacity;
    static const unsigned int MIN_CAPACITY = 4;

#ifdef SIMPLE_VECTOR_SMART_RESIZE
    // Smart-resize state
    mutable unsigned int smartResizeCount;   // number of grow resizes since last reset
    mutable unsigned int smartShrinkCount;   // number of shrink resizes since last reset
    unsigned int         customResizeAmount; // 0 = adaptive, >0 = fixed extra slots per resize
    static const unsigned int SMART_RESIZE_THRESHOLD  = 3; // resizes before switching to large steps
    static const unsigned int SMART_RESIZE_MULTIPLIER = 4; // growth multiplier once threshold hit
    static const unsigned int SMART_SHRINK_THRESHOLD  = 3; // shrinks before switching to large cuts
#endif

    static unsigned int normalizeCapacity(unsigned int requestedCapacity) {
        return requestedCapacity == 0 ? DEFAULT_CAPACITY : requestedCapacity;
    }

    /**
     * @brief Resize the array to the specified capacity
     * @param newCapacity The new capacity of the array
     * 
     * @private This method is private because it is only used internally.
     * 
     * @note This method will copy all elements from the old array to the new array.
    */
    void resize(unsigned int newCapacity) {
        const unsigned int targetCapacity = normalizeCapacity(newCapacity);
        T* newArray = new T[targetCapacity];
        if (!newArray) {
            Serial.println("Memory allocation failed during resize.");
            return;
        }
        const unsigned int copiedCount = (count < targetCapacity) ? count : targetCapacity;
        if (array != nullptr) {
            for (unsigned int i = 0; i < copiedCount; i++) {
                newArray[i] = array[i];
            }
        }
        delete[] array;
        array = newArray;
        count = copiedCount;
        capacity = targetCapacity;
    }

    /**
     * @brief Ensure that the vector has enough capacity to add a new element
     * 
     * @private This method is private because it is only used internally.
    */
    void ensureCapacity() {
        if (capacity == 0 || array == nullptr) {
            resize(DEFAULT_CAPACITY);
            return;
        }
        if (count == capacity) {
            resize(2 * capacity);
        }
    }

#ifdef SIMPLE_VECTOR_SMART_RESIZE
    /**
     * @brief Smart capacity growth: tracks resize frequency and grows by larger
     *        steps once resizes happen frequently, reducing total allocations.
     *
     *        If a custom resize amount has been set via setResizeAmount(), that
     *        fixed extra-slot count is always used instead of the adaptive logic.
     */
    void smartEnsureCapacity() {
        if (capacity == 0 || array == nullptr) {
            resize(DEFAULT_CAPACITY);
            return;
        }
        if (count == capacity) {
            smartResizeCount++;
            unsigned int newCapacity;
            if (customResizeAmount > 0) {
                newCapacity = capacity + customResizeAmount;
            } else if (smartResizeCount >= SMART_RESIZE_THRESHOLD) {
                newCapacity = capacity * SMART_RESIZE_MULTIPLIER;
            } else {
                newCapacity = 2 * capacity;
            }
            resize(newCapacity);
        }
    }
#endif // SIMPLE_VECTOR_SMART_RESIZE

public:
    // The SimpleVectorIterator class will be defined below
    class SimpleVectorIterator;

    SimpleVector() : array(new T[DEFAULT_CAPACITY]), count(0), capacity(DEFAULT_CAPACITY)
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        , smartResizeCount(0), smartShrinkCount(0), customResizeAmount(0)
#endif
    {
        if(!array){
            Serial.println("Memory allocation failed.");
        }
    }

    SimpleVector(unsigned int initialCapacity) : array(new T[normalizeCapacity(initialCapacity)]), count(0), capacity(normalizeCapacity(initialCapacity))
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        , smartResizeCount(0), smartShrinkCount(0), customResizeAmount(0)
#endif
    {
        if(!array){
            Serial.println("Memory allocation failed.");
        }
    }

    SimpleVector(const SimpleVector& other) : array(new T[other.capacity]), count(other.count), capacity(other.capacity)
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        , smartResizeCount(other.smartResizeCount), smartShrinkCount(other.smartShrinkCount), customResizeAmount(other.customResizeAmount)
#endif
    {
        for (unsigned int i = 0; i < count; i++) {
            array[i] = other.array[i];
        }
    }

    #ifdef useInit
    template<typename... Args>
    SimpleVector(T first, Args... rest) {
        count = sizeof...(rest) + 1;  // Number of elements
        capacity = count;
        array = new T[capacity];

        // Initialize the first value
        array[0] = first;

        // Unpack the rest using recursion
        fillArray(1, rest...);
    }

    // Recursive unpacking function
    template<typename... Args>
    void fillArray(size_t index, T first, Args... rest) {
        array[index] = first;
        fillArray(index + 1, rest...);
    }

    // Base case for recursion
    void fillArray(size_t) {}
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
        if (count == 0) {
            if (capacity != MIN_CAPACITY || array == nullptr) {
                resize(MIN_CAPACITY);
                count = 0;
                return array != nullptr && capacity == MIN_CAPACITY;
            }
            return false;
        }
        if (count < capacity) {
            const unsigned int targetCapacity = normalizeCapacity(count);
            resize(targetCapacity);
            return array != nullptr && capacity == targetCapacity;
        }
        return false;
    }

#ifdef SIMPLE_VECTOR_SMART_RESIZE
    /**
     * @brief Smart shrink: tracks how often shrinking is requested and cuts
     *        by larger steps once shrinks happen frequently.
     *
     *        Falls back to shrinkToFit() behaviour when usage is low.
     */
    bool smartShrinkToFit() {
        if (count == 0) {
            return shrinkToFit();
        }
        if (count < capacity) {
            smartShrinkCount++;
            unsigned int targetCapacity;
            if (customResizeAmount > 0) {
                targetCapacity = count + customResizeAmount;
                if (targetCapacity > capacity) return false;
            } else if (smartShrinkCount >= SMART_SHRINK_THRESHOLD) {
                // Aggressive: shrink to exact count
                targetCapacity = normalizeCapacity(count);
            } else {
                // Gentle: keep some headroom
                targetCapacity = normalizeCapacity(count + count / 2);
                if (targetCapacity >= capacity) return false;
            }
            resize(targetCapacity);
            return array != nullptr;
        }
        return false;
    }

    /**
     * @brief Reset smart-resize counters (e.g. after a bulk operation is done).
     */
    void resetSmartResizeCounters() {
        smartResizeCount = 0;
        smartShrinkCount = 0;
    }

    /**
     * @brief Set a fixed number of extra slots to allocate on each smart resize.
     *        Set to 0 to return to adaptive behaviour.
     * @param amount Number of extra slots per resize step.
     */
    void setResizeAmount(unsigned int amount) {
        customResizeAmount = amount;
    }

    /**
     * @brief Reserve at least \p estimatedTotal slots up front so that a
     *        subsequent bulk-add of that many elements needs no intermediate
     *        resizes at all.
     * @param estimatedTotal Expected final element count.
     */
    void reserveEstimated(unsigned int estimatedTotal) {
        if (estimatedTotal > capacity) {
            resize(estimatedTotal);
            resetSmartResizeCounters();
        }
    }
#endif // SIMPLE_VECTOR_SMART_RESIZE

    /**
    * @brief Clears the vector by setting all elements to their default value and resetting the count.
    * 
    * @public This method is public because it is meant to be called by the user.
    */
    void clear() {
        if(array){
            delete[] array;
        }
        array = new T[DEFAULT_CAPACITY];
        count = 0;
        capacity = DEFAULT_CAPACITY;

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
        const unsigned int targetCapacity = normalizeCapacity(static_cast<unsigned int>(newCapacity));
        array = new T[targetCapacity];
        count = 0;
        capacity = targetCapacity;
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
        if (index < 0 || index >= (int)count) {
            return; // Index out of bounds
        }
        // Shift elements after the specified index to the left
        for (unsigned int i = (unsigned int)index; i < count - 1; i++) {
            array[i] = array[i + 1];
        }
        count--;
    }

//Changed to put() for better naming conventions.
    /**
     * @brief Add an element to the vector
     * @param item The item to be added to the vector
     * 
    */
    void put(const T& item) {
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        smartEnsureCapacity();
#else
        ensureCapacity();
#endif
        if (array == nullptr || count >= capacity) {
            return;
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
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        smartEnsureCapacity();
#else
        ensureCapacity();
#endif
        if (array == nullptr || count >= capacity) {
            return;
        }
        array[count].~T();
        new (array + count) T();
        count++;
    }

    void emplace_back(const T& value) {
#ifdef SIMPLE_VECTOR_SMART_RESIZE
        smartEnsureCapacity();
#else
        ensureCapacity();
#endif
        if (array == nullptr || count >= capacity) {
            return;
        }
        array[count].~T();
        new (array + count) T(value);
        count++;
    }

    //back() method
    T& back() {
        static T dummy = T();
        if (count == 0 || array == nullptr) {
            return dummy;
        }
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
        if (index >= count) {
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
#ifdef SIMPLE_VECTOR_SMART_RESIZE
            smartResizeCount  = other.smartResizeCount;
            smartShrinkCount  = other.smartShrinkCount;
            customResizeAmount = other.customResizeAmount;
#endif
        }
        return *this;
    }

bool operator==(const SimpleVector<T>& other) const {
    if (count != other.elements()) return false;
    for (unsigned int i = 0; i < count; i++) {
        if (array[i] != other[i]) return false;
    }
    return true;
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

#ifdef SV_ENABLE_NUMERIC_LIMITS
    /**
     * @brief Returns the memory currently used by the internal array (in bytes).
     * @details Includes only the element storage array, not object overhead.
     * @return Bytes consumed by the backing array.
     */
    unsigned int memoryUsage() const {
        return capacity * sizeof(T);
    }

    /**
     * @brief Returns the theoretical maximum number of elements this SimpleVector
     *        could hold, limited by the maximum value of unsigned int on this platform.
     * @return The upper bound on element count as reported by numeric_limits.
     */
    unsigned int theoreticalMaxElements() const {
        return numeric_limits<unsigned int>::Max();
    }

    /**
     * @brief Returns the ratio of elements stored to the theoretical maximum
     *        element count, expressed as a float in the range [0.0, 1.0].
     * @return Memory utilization fraction (current count / theoretical max).
     */
    float memoryUtilization() const {
        const unsigned int maxElems = theoreticalMaxElements();
        if (maxElems == 0) return 0.0f;
        return static_cast<float>(count) / static_cast<float>(maxElems);
    }
#endif // SV_ENABLE_NUMERIC_LIMITS

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
     * @return The index of the specified element, or -1 if the element is not found
    */
    int indexOf(const T& element) {
        for (unsigned int i = 0; i < count; i++) {
            if (array[i] == element) {
                return i;
            }
        }
        return -1; // Element not found
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
