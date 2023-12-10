#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
    private:
        /**
         * @struct Item
         * @brief A struct that represents an item in the ArrayList.
         *
         * This struct is used to wrap the data in the ArrayList. It contains a single member, data, which is a pointer to the actual data.
         */
        struct Item {
            void* data;
        };
        
        Item* array; // The array of items
        unsigned int count; // The number of items in the array
        unsigned int capacity; // The capacity of the array
        bool debug; // Whether or not to print debug messages


        /**
         * @brief Resizes the array to the new capacity.
         *
         * This method is used to resize the array to the new capacity. It will copy all of the data from the old array to the new array.
         * @param newCapacity The new capacity of the array.
         * @return True if the resize was successful, false otherwise.
         */
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

        /**
         * @brief Decreases the size of the array to the new capacity.
         *
         * This method is used to decrease the size of the array to the new capacity. It will copy all of the data from the old array to the new array AS LONG AS
         * THE CAPACITY OF THE NEW ARRAY WILL ALLOW. If the new capacity is less than the current number of elements, then the elements beyond the new capacity will
         * be deleted.
         * @warning This method will delete elements if the new capacity is less than the current number of elements.
         * @param newCapacity The new capacity of the array.
         * @return True if the resize was successful, false otherwise.
         */
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
        /**
         * @brief Constructs a new ArrayList with the specified capacity.
         *
         * This constructor creates a new ArrayList that can hold up to the specified number of items.
         * The capacity defaults to 10 if not specified.
         * The constructor initializes the count of items to 0 and the debug mode to false.
         *
         * @param capacity The maximum number of items that the ArrayList can hold.
        */
        ArrayList(unsigned int capacity = 10) : capacity(capacity), count(0), debug(false) {
            array = new Item[capacity];
        }

        /**
         * @brief Destroys the ArrayList.
         *
         * This destructor deletes all the items in the ArrayList and then deletes the array itself.
         * The items are deleted using the delete operator, so they must be pointers to dynamically allocated memory.
        */
        ~ArrayList() {
            for (unsigned int i = 0; i < count; i++) {
                delete static_cast<T*>(array[i].data);
            }
            delete[] array;
        }


        /**
         * @brief Shrinks the ArrayList to its current size.
         *
         * This function reduces the capacity of the ArrayList to its current size, i.e., the number of items it contains.
         * It calls the decreaseSize function with the current count as the argument.
         *
         * @return True if the operation was successful, false otherwise.
        */
        bool shrink() {
            return decreaseSize(count);
        }

        /**
         * @brief Decreases the capacity of the ArrayList by half.
         *
         * This function reduces the capacity of the ArrayList to half of its current capacity.
         * It calls the decreaseSize function with half the current capacity as the argument.
         *
         * @return True if the operation was successful, false otherwise.
        */
        bool decrease() {
            return decreaseSize(capacity / 2);
        }

        /**
         * @brief Decreases the capacity of the ArrayList to the specified capacity.
         *
         * This function reduces the capacity of the ArrayList to the specified new capacity.
         * It calls the decreaseSize function with the new capacity as the argument.
         *
         * @param newCapacity The new capacity of the ArrayList.
         * @return True if the operation was successful, false otherwise.
        */
        bool decrease(unsigned int newCapacity) {
            return decreaseSize(newCapacity);
        }

        /**
         * @brief Adds a new item to the ArrayList.
         *
         * This function adds a new item to the ArrayList. If the ArrayList is full, it attempts to resize the ArrayList to double its current capacity.
         * The function creates a new copy of the item and stores a pointer to the copy in the ArrayList.
         *
         * @param data The item to add.
         * @return True if the item was added successfully, false if the ArrayList could not be resized.
        */
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

        /**
         * @brief Retrieves the item at the specified index in the ArrayList.
         *
         * This function returns the item at the specified index in the ArrayList.
         * If the index is out of bounds (i.e., it is greater than or equal to the count of items in the ArrayList), the function returns a default value.
         *
         * @param index The index of the item to retrieve.
         * @return The item at the specified index, or a default value if the index is out of bounds.
        */
        T get(unsigned int index) const {
            if (index >= count) {
                static T defaultVal; // Return a default value
                return defaultVal;
            }
            return *static_cast<T*>(array[index].data);
        }

        /**
         * @brief Sets the debug mode of the ArrayList.
         * 
         * This function sets the debug mode of the ArrayList. If debug mode is enabled, the ArrayList will print debug messages to the Serial port.
        */
        void setDebug(bool debugValue) {
            debug = debugValue;
        }

        /**
         * @brief Returns the debug mode of the ArrayList.
         * 
         * This function returns the debug mode of the ArrayList. If debug mode is enabled, the ArrayList will print debug messages to the Serial port.
         * @return The debug mode of the ArrayList.
        */
        bool getDebug() {
            return debug;
        }

        /**
         * @brief Removes the item at the specified index in the ArrayList.
         *
         * This function removes the item at the specified index in the ArrayList.
         * If the index is out of bounds (i.e., it is greater than or equal to the count of items in the ArrayList), the function returns false.
         * Otherwise, the function deletes the item and shifts all the items after it down by one index.
         *
         * @param index The index of the item to remove.
         * @return True if the item was removed successfully, false if the index is out of bounds.
        */
        bool remove(unsigned int index) {
            if (index >= count) {
                return false;
            }
            delete static_cast<T*>(array[index].data);
            for (unsigned int i = index; i < count - 1; i++) {
                array[i].data = array[i + 1].data;
            }
            count--;
            return true;
        }


        /**
         * @brief Returns the number of items in the ArrayList.
         *
         * This function returns the count of items in the ArrayList.
         *
         * @return The number of items in the ArrayList.
        */
        size_t size() const {
            return count;
        }

        /**
         * @brief Returns the current capacity of the ArrayList.
         *
         * This function returns the current capacity of the ArrayList, i.e., the maximum number of items it can hold without resizing.
         *
         * @return The current capacity of the ArrayList.
        */
        size_t capacity() const {
            return capacity;
        }

        // ... Other methods ...
    };

#endif // ARRAYLIST_H
