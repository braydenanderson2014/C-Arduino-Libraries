#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC };
    //Constructor and Destructor
    ArrayList(SizeType type = DYNAMIC, size_t initialSize = 8) 
    : sizeType(type), arrayCapacity(initialSize), count(0) {
        Serial.println("[ArrayList]: Creating new instance of ArrayList");
        array = new T[arrayCapacity];
    }

    ~ArrayList() {
        Serial.println("[ArrayList]: Deleting instance of ArrayList");
        delete[] array;
    }
    //Add
    void add(T item) {
        Serial.println("[ArrayList]: Adding item to ArrayList");
        if (sizeType == DYNAMIC && count == arrayCapacity) {
            Serial.println("[ArrayList]: Resizing ArrayList");
            resize();
        }
        if (count < arrayCapacity) {
            array[count++] = item;
        }else{
            Serial.println("[ArrayList]: Error - Array is full");
        }
    }

    bool addAll(const ArrayList<T>& other) {
        Serial.println("[ArrayList]: Adding all items from other ArrayList");
        if (sizeType == DYNAMIC && count + other.count > arrayCapacity) {
            Serial.println("[ArrayList]: Resizing ArrayList");
            resize();
        }
        if (count + other.count <= arrayCapacity) {
            memcpy(array + count, other.array, other.count * sizeof(T));
            count += other.count;
            return true;
        }
        Serial.println("[ArrayList]: Error - Array is full");
        return false;
    }

    bool addAll(const T* other, size_t length) {
        Serial.println("[ArrayList]: Adding all items from other array");
        if (sizeType == DYNAMIC && count + length > arrayCapacity) {
            Serial.println("[ArrayList]: Resizing ArrayList");
            resize();
        }
        if (count + length <= arrayCapacity) {
            memcpy(array + count, other, length * sizeof(T));
            count += length;
            return true;
        }
        Serial.println("[ArrayList]: Error - Array is full");
        return false;
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

    bool insertAll(size_t index, const ArrayList<T>& other) {
        Serial.println("[ArrayList]: Inserting all items from other ArrayList at index: " + String(index) + " into ArrayList");
        if (index > count) {
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return false;
        }
        if (count + other.count > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                Serial.println("[ArrayList]: Error - Array is full");
                return false;
            }
        }
        for (size_t i = count + other.count - 1; i >= index + other.count; --i) {
            array[i] = array[i - other.count];
        }
        memcpy(array + index, other.array, other.count * sizeof(T));
        count += other.count;
        return true;
    }

    bool insertAll(size_t index, const T* other, size_t length) {
        Serial.println("[ArrayList]: Inserting all items from other array at index: " + String(index) + " into ArrayList");
        if (index > count) {
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return false;
        }
        if (count + length > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                Serial.println("[ArrayList]: Error - Array is full");
                return false;
            }
        }
        for (size_t i = count + length - 1; i >= index + length; --i) {
            array[i] = array[i - length];
        }
        memcpy(array + index, other, length * sizeof(T));
        count += length;
        return true;
    }
    //Remove
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

    bool removeIf(bool (*predicate)(T)) {
        Serial.println("[ArrayList]: Removing items from ArrayList");
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (predicate(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }

    void removeRange(size_t fromIndex, size_t toIndex) {
        Serial.println("[ArrayList]: Removing items from ArrayList");
        if (fromIndex > toIndex) {
            Serial.println("[ArrayList]: Error - fromIndex is greater than toIndex");
            return;
        }
        if (fromIndex < 0 || toIndex > count) {
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return;
        }
        size_t removed = toIndex - fromIndex;
        for (size_t i = fromIndex; i < count - removed; ++i) {
            array[i] = array[i + removed];
        }
        count -= removed;
    }

    bool retainAll(const ArrayList<T>& other) {
        Serial.println("[ArrayList]: Retaining items from other ArrayList");
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (!other.contains(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }

    void clear() {
        Serial.println("[ArrayList]: Clearing ArrayList");
        delete[] array;
        array = new T[arrayCapacity];
        count = 0;
    }

    //Query and Access
    T get(size_t index) const {
        if (index < count) {
            return array[index];
        }
        return T(); // Return default value if index is out of bounds
    }

    bool contains(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }
    
    size_t indexOf(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return i;
            }
        }
        Serial.println("[ArrayList]: Error - Item not found");
        return static_cast<size_t>(-1); // Indicate not found
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

    //Modifying Elements
    bool set(size_t index, T item) {
        if (index < count) {
            array[index] = item;
            if(array[index] == item){
                return true;
            }
        }
        if(index >= count){
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return false;
        }
    }

    void replaceAll(T (*operatorFunction)(T)) {
        Serial.println("[ArrayList]: Replacing items in ArrayList");
        for (size_t i = 0; i < count; ++i) {
            array[i] = operatorFunction(array[i]);
        }
    }


    //Utility Functions
    void forEach(void (*consumer)(T)) const {
        Serial.println("[ArrayList]: Performing action on all items in ArrayList");
        for (size_t i = 0; i < count; ++i) {
            consumer(array[i]);
        }
    }

    T* toArray(T* outputArray) const {
        Serial.println("[ArrayList]: Converting ArrayList to array");
        memcpy(outputArray, array, count * sizeof(T));
        return outputArray;
    }

    ArrayList<T>* sublist(size_t fromIndex, size_t toIndex) const {
        Serial.println("[ArrayList]: Creating sublist from ArrayList");
        if (fromIndex > toIndex) {
            Serial.println("[ArrayList]: Error - fromIndex is greater than toIndex");
            return nullptr;
        }
        if (fromIndex < 0 || toIndex > count) {
            Serial.println("[ArrayList]: Error - Index out of bounds");
            return nullptr;
        }
        ArrayList<T>* newList = new ArrayList<T>(sizeType, toIndex - fromIndex);
        for (size_t i = fromIndex; i < toIndex; ++i) {
            newList->add(array[i]);
        }
        return newList;
    }

    ArrayList<T>* clone() const {
        Serial.println("[ArrayList]: Cloning ArrayList");
        ArrayList<T>* clone = new ArrayList<T>(sizeType, arrayCapacity);
        clone->addAll(*this);
        return clone;
    }

    void ensureCapacity(size_t minCapacity) {
        Serial.println("[ArrayList]: Ensuring ArrayList capacity");
        if (minCapacity > arrayCapacity) {
            Serial.println("[ArrayList]: Resizing ArrayList to " + String(minCapacity) + " elements");
            T* newArray = new T[minCapacity];
            memcpy(newArray, array, count * sizeof(T));
            delete[] array;
            array = newArray;
            arrayCapacity = minCapacity;
        }
    }

    void trimToSize() {
        Serial.println("[ArrayList]: Trimming ArrayList to size");
        if (sizeType == DYNAMIC && count < arrayCapacity) {
            T* newArray = new T[count];
            memcpy(newArray, array, count * sizeof(T));
            delete[] array;
            array = newArray;
            arrayCapacity = count;
        }
    }

    void sort(bool (*comparator)(T, T)) {
        Serial.println("[ArrayList]: Sorting ArrayList");
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                if (comparator(array[j], array[j + 1])) {
                    T temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
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
