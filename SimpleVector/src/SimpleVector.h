#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

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
    SimpleVector() : count(0), capacity(4) {
        array = new T[capacity];
    }

    ~SimpleVector() {
        delete[] array;
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
        return count;
    }

    T* begin() {
        return &array[0];
    }

    T* end() {
        return &array[count];
    }

    void releaseMemory() {
        delete[] array;
        array = nullptr;
        capacity = 0;
        size = 0;
    }

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
                return *current++;
            }
        };
};
#endif // SIMPLEVECTOR_H