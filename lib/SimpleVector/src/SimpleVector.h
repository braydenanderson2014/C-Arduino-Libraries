#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <Arduino.h>
template<typename T>
class SimpleVector {
private:
    T* array;
    unsigned int count;
    unsigned int capacity;

    void resize(unsigned int newCapacity) {
        T* newArray = new T[newCapacity];
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

    SimpleVector() : array(new T[4]), count(0), capacity(4) {}
    
    ~SimpleVector() {
        delete[] array;
    }

    // ... Other methods ...

    void releaseMemory() {
        delete[] array;
        array = nullptr;
        capacity = 0;
        count = 0;
    }
//Changed to put() for better naming conventions.
    void put(const T& item) {
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
                continue;
            }
            array[index++] = array[i];
        }
        count = index;
    }

    T& operator[](unsigned int index) {
        if (index >= count) {
            exit(1);
        }
        return array[index];
    }

    unsigned int size() const {
        return capacity;
    }

    unsigned int elements() const {
        return count;
    }


// Get the element at the specified index
    T& get(unsigned int index) {
        if (index >= count) {
            exit(1); // You can handle this error differently if needed
        }
        return array[index];
    }

    // Get the index of the specified element
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
    SimpleVectorIterator begin() {
        return SimpleVectorIterator(array, array + count);
    }

    SimpleVectorIterator end() {
        return SimpleVectorIterator(array + count, array + count);
    }

    // The iterator type should be defined after the SimpleVectorIterator class.
    typedef SimpleVectorIterator iterator;

    // Definition of the SimpleVectorIterator class
    class SimpleVectorIterator {
    private:
        T* current;
        T* end;

    public:
        SimpleVectorIterator(T* start, T* stop) : current(start), end(stop) {}

        bool hasNext() {
            return current != end;
        }

        T& next() {
            if (current == end) {
                Serial.println("Error: Iterator has reached the end of the vector.");
            }
            return *current++;
        }

        // The iterator needs to be compatible with STL-style iteration
        SimpleVectorIterator& operator++() {
            ++current;
            return *this;
        }

        bool operator!=(const SimpleVectorIterator& other) const {
            return current != other.current;
        }

        T& operator*() {
            return *current;
        }
    };
};
#endif // SIMPLEVECTOR_H
