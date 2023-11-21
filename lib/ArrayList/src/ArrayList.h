#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC };

    ArrayList(SizeType type = DYNAMIC, size_t initialSize = 8) 
    : sizeType(type), capacity(initialSize), count(0) {
        array = new T[capacity];
    }

    ~ArrayList() {
        delete[] array;
    }

    void add(T item) {
        if (sizeType == DYNAMIC && count == capacity) {
            resize();
        }
        if (count < capacity) {
            array[count++] = item;
        }
    }

    bool remove(T item) {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                removeAt(i);
                return true;
            }
        }
        return false;
    }

    void clear() {
        delete[] array;
        array = new T[capacity];
        count = 0;
    }

    bool contains(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }

    T get(size_t index) const {
        if (index < count) {
            return array[index];
        }
        return T(); // Return default value if index is out of bounds
    }

    size_t capacity() const {
        return capacity;
    }

    size_t size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }
    // Iterator support
    T* begin() const { return &array[0]; }
    T* end() const { return &array[count]; }

private:
    T* array;
    SizeType sizeType;
    size_t capacity;
    size_t count;

    void resize() {
        size_t newCapacity = capacity * 1.5;
        T* newArray = new T[newCapacity];
        memcpy(newArray, array, count * sizeof(T));
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    void removeAt(size_t index) {
        for (size_t i = index; i < count - 1; ++i) {
            array[i] = array[i + 1];
        }
        --count;
    }
};

#endif // ARRAYLIST_H
