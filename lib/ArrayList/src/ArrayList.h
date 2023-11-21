#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC };

    ArrayList(SizeType type = DYNAMIC, size_t initialSize = 8) 
    : sizeType(type), arrayCapacity(initialSize), count(0) {
        Serial.println("[ArrayList]: Creating new instance of ArrayList");
        array = new T[arrayCapacity];
    }

    ~ArrayList() {
        Serial.println("[ArrayList]: Deleting instance of ArrayList");
        delete[] array;
    }

    void add(T item) {
        Serial.println("[ArrayList]: Adding item to ArrayList");
        if (sizeType == DYNAMIC && count == arrayCapacity) {
            Serial.println("[ArrayList]: Resizing ArrayList");
            resize();
        }
        if (count < arrayCapacity) {
            array[count++] = item;
        }
    }

    bool insert(size_t index, T item) {
        Serial.println("[ArrayList]: Inserting item at index: " + String(index) + " into ArrayList");
        if (index > count) {
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return false;
        }
        if (count == arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                Serial.println("[ArrayList]: Error - Array is full");
                return false;
            }
        }
        for (size_t i = count; i > index; --i) {
            array[i] = array[i - 1];
        }
        array[index] = item;
        ++count;
        return true;
    }

    bool removeItem(T item) {
        Serial.println("[ArrayList]: Removing item from ArrayList");
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                removeAt(i);
                return true;
            }
        }
        return false;
    }

    void remove(size_t index) {
        Serial.println("[ArrayList]: Removing item from ArrayList at index: " + String(index));
        if (index < count) {
            for (size_t i = index; i < count - 1; ++i) {
                array[i] = array[i + 1];
            }
            --count;
        }
    }
    
    void clear() {
        Serial.println("[ArrayList]: Clearing ArrayList");
        delete[] array;
        array = new T[arrayCapacity];
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
        return arrayCapacity;
    }

    size_t size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    size_t indexOf(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return i;
            }
        }
        return static_cast<size_t>(-1); // Indicate not found
    }

    // Iterator support
    T* begin() const { return &array[0]; }
    T* end() const { return &array[count]; }

private:
    T* array;
    SizeType sizeType;
    size_t arrayCapacity;
    size_t count;

    void resize() {
        Serial.println("[ArrayList]: Resizing ArrayList to " + String(arrayCapacity * 1.5) + " elements");
        size_t newCapacity = arrayCapacity * 1.5;
        T* newArray = new T[newCapacity];
        memcpy(newArray, array, count * sizeof(T));
        delete[] array;
        array = newArray;
        arrayCapacity = newCapacity;
    }
};

#endif // ARRAYLIST_H
