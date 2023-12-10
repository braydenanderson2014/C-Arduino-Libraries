#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
    private:
        struct Item {
            void* data;
        };
        
        Item* array;
        unsigned int count;
        unsigned int capacity;
        bool debug;

        bool resize(unsigned int newCapacity) {
            if (newCapacity < count) {
                return false;
            }
            Item* newArray = new Item[newCapacity];
            for (unsigned int i = 0; i < count; i++) {
                newArray[i].data = new T(*static_cast<T*>(array[i].data)); // Use copy constructor
                delete static_cast<T*>(array[i].data); // Free old memory
            }
            delete[] array;
            array = newArray;
            capacity = newCapacity;
            return true;
        }


        bool decreaseSize(unsigned int newCapacity) {
            if (newCapacity >= count) {
                // No need to remove elements, just resize
                return resize(newCapacity);
            } else {
                // Need to remove elements beyond the new capacity
                for (unsigned int i = newCapacity; i < count; i++) {
                    delete static_cast<T*>(array[i].data); // Free the memory for each item
                }

                // Resize the array
                Item* newArray = new Item[newCapacity];
                for (unsigned int i = 0; i < newCapacity; i++) {
                    newArray[i].data = array[i].data; // Transfer ownership
                }

                delete[] array; // Free old array
                array = newArray; // Update array pointer
                count = newCapacity; // Update count to new capacity
                capacity = newCapacity; // Update capacity to new capacity
                return true;
            }
        }

    
    public:
        ArrayList(unsigned int capacity = 10) : capacity(capacity), count(0), debug(false) {
            array = new Item[capacity];
        }

        ~ArrayList() {
            for (unsigned int i = 0; i < count; i++) {
                delete static_cast<T*>(array[i].data);
            }
            delete[] array;
        }

        bool shrink() {
            shrinkToFit();
        }

        bool decrease() {
            return decreaseSize(capacity / 2);
        }

        bool decrease(unsigned int newCapacity) {
            return decreaseSize(newCapacity);
        }

        bool add(const T& data) {
            if (count == capacity) {
                if (!resize(capacity * 2)) {
                    return false;
                }
            }
            array[count].data = new T(data);
            count++;
            return true;
        }

        T get(unsigned int index) const {
            if (index >= count) {
                static T defaultVal; // Return a default value
                return defaultVal;
            }
            return *static_cast<T*>(array[index].data);
        }

        void setDebug(bool debugValue) {
            debug = debugValue;
        }

        size_t size() const {
            return count;
        }

        size_t capacity() const {
            return capacity;
        }

        // ... Other methods ...
    };

#endif // ARRAYLIST_H
