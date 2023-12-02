#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <Arduino.h>
template<typename T>
class SimpleVector {
private:
    T* array;
    unsigned int count;
    unsigned int capacity;
    bool debug;

    void resize(unsigned int newCapacity) {
        T* newArray = new T[newCapacity];
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Resizing from " + String(capacity) + " to " + String(newCapacity));
        }
        for (unsigned int i = 0; i < count; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    // The SimpleVectorIterator class will be defined below
    class SimpleVectorIterator;

    SimpleVector() : array(new T[4]), count(0), capacity(4), debug(debug) {}
    
    ~SimpleVector() {
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Destroying SimpleVector Instance");
        }
        delete[] array;
    }

    // ... Other methods ...

    void releaseMemory() {
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Releasing Memory");
        }
        delete[] array;
        array = nullptr;
        capacity = 0;
        count = 0;
    }
//Changed to put() for better naming conventions.
    void put(const T& item) {
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Adding item to vector");
        }
        if (count == capacity) {
            resize(2 * capacity);
        }
        array[count++] = item;
    }

    // Remove an element from the vector by shifting elements (Added in Version 1.0.1)
    void remove(const T& item) {
        unsigned int index = 0;
        for (unsigned int i = 0; i < count; i++) {
            if (array[i] == item) {
                // Element found, skip it
                if (debug) {
                    Serial.println("[SIMPLE VECTOR]: Removing item from vector");
                }
                continue;
            }
            array[index++] = array[i];
        }
        count = index;
    }

    T& operator[](unsigned int index) {
        if (index >= count) {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Index out of bounds");
            }
            return nullptr; // You can handle this error differently if needed
        }
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Returning item at index " + String(index));
        }
        return array[index];
    }

    unsigned int size() const {
        if(this -> capacity == 0){
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Size is 0");
            }
            return 0;
        }
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Size is " + String(this -> capacity));
        }
        return this -> capacity;
    }

    unsigned int elements() const {
        if(this -> count == 0){
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Elements is 0");
            }
            return 0;
        }
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Elements is " + String(this -> count));
        }
        return this -> count;
    }
    void setDebug(bool debug){
        this -> debug = debug;
    }

    bool getDebug(){
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Debug is on");
        } else {
            Serial.println("[SIMPLE VECTOR]: Debug is off");
        }
        return this -> debug;
    }

// Get the element at the specified index
    T& get(unsigned int index) {
        if (index >= count) {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Index out of bounds");
            }
            return nullptr; // You can handle this error differently if needed
        }
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Returning item at index " + String(index));
        }
        return array[index];
    }

    // Get the index of the specified element
    int indexOf(const T& element) {
        for (unsigned int i = 0; i < count; i++) {
            if (array[i] == element) {
                if(debug){
                    Serial.println("[SIMPLE VECTOR]: Returning index of item " + String(element));
                }
                return i;
            }
        }
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Item not found");
        }
        return 0; // Element not found
    }

    // ... Other methods ...

    // SimpleVectorIterator related methods
    SimpleVectorIterator begin() {
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Returning beginning of iterator for vector");
        }
        return SimpleVectorIterator(array, array + count);
    }

    SimpleVectorIterator end() {
        if(debug){
            Serial.println("[SIMPLE VECTOR]: Returning ending of iterator for vector");
        }
        return SimpleVectorIterator(array + count, array + count);
    }

    // The iterator type should be defined after the SimpleVectorIterator class.
    typedef SimpleVectorIterator iterator;

    // Definition of the SimpleVectorIterator class
    class SimpleVectorIterator {
    private:
        T* current;
        T* end;
        bool debug;
    public:
        SimpleVectorIterator(T* start, T* stop) : current(start), end(stop), debug(debug) {}

        bool hasNext() {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Checking if iterator has next" + String(current != end));
            }
            return current != end;
        }

        T& next() {
            if (current == end) {
                if(debug){
                    Serial.println("[SIMPLE VECTOR]: WARNING: Iterator has reached the end of the vector.");
                }
            }
            return *current++;
        }

        // The iterator needs to be compatible with STL-style iteration
        SimpleVectorIterator& operator++() {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Incrementing iterator" + String(current != end));
            }
            ++current;
            return *this;
        }

        bool operator!=(const SimpleVectorIterator& other) const {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Checking if iterators are not equal" + String(current != other.current));
            }
            return current != other.current;
        }

        T& operator*() {
            if(debug){
                Serial.println("[SIMPLE VECTOR]: Dereferencing iterator");
            }
            return *current;
        }
    };
};
#endif // SIMPLEVECTOR_H
