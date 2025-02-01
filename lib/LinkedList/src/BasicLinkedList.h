// LinkedList.h
#ifndef BASICLINKEDLIST_H
#define BASICLINKEDLIST_H

#include <Arduino.h>

/**
 * @brief A node in a singly-linked list.
 * @tparam T - the type of the value to be stored in the node
*/
template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;

    ListNode(T value) : data(value), next(nullptr) {}
};

/**
 * @brief A singly-linked list.
 * @tparam T - the type of the value to be stored in the list
*/
template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    size_t Size;
public:
class ForwardIterator {
    private:
        ListNode<T>* current;
    public:
        /**
         * @brief Instantiate a new ForwardIterator object
         * @param start - the node to start iterating from
        */
        ForwardIterator(ListNode<T>* start) : current(start) {}

        /**
         * @brief Dereference operator.
         * @return the value of the current node
        */
        T& operator*() {
            return current->data;
        }
 
        /**
         * @brief Not equal operator.
         * @param other - the iterator to compare with
         * @return true if the iterators are not equal, false otherwise
        */
        bool operator!=(const ForwardIterator& other) const {
            return current != other.current;
        }

        /**
         * @brief Prefix increment operator.
         * @return the iterator after incrementing
        */
        ForwardIterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
    };
    /**
     * @brief Instantiate a new LinkedList object
     * 
    */
    LinkedList() : head(nullptr), Size(0) {}

    /**
     * @brief Destructor
     * @details Deletes all nodes in the list.
     * 
    */
    ~LinkedList() {
        clear();
    }

    /**
     * @brief Add an element to the Linked List (Adds it to the end of the list)
     * @param value - the value to be added
     * 
     * @details Creates a new node with the given value and appends it to the end of the list. (Just calls the append function)
    */
    void add(const T& value) {
        append(value);
        debugPrintList();
    }

    /**
     * @brief Append an element to the end of the list
     * @param value - the value to be appended
     * 
     * @details Creates a new node with the given value and appends it to the end of the list.
    */
    void append(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        if (!head) {
            head = newNode;
        } else {
            ListNode<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        Size++;
    }

    /**
     * @brief Prepend an element to the beginning of the list
     * @param value - the value to be prepended
     * 
     * @details Creates a new node with the given value and prepends it to the beginning of the list.
    */
    void prepend(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        newNode->next = head;
        head = newNode;
        Size++;
    }

    /**
     * @brief Insert an element at a random position
     * @param value - the value to be inserted
     * 
     * @details Creates a new node with the given value and inserts it at a random position in the list.
     * If the random position is 0, the value is prepended to the list.
     * If the random position is greater than or equal to the size of the list, the value is appended to the list.
     * Otherwise, the value is inserted at the random position.
    */
    void insert(const T& value){
        int randomNum = random(0, Size);
        if (randomNum == 0) {
            prepend(value);
        } else if (randomNum >= Size) {
            append(value);
        } else {
            ListNode<T>* newNode = new ListNode<T>(value);
            ListNode<T>* current = head;
            for (size_t i = 1; i < randomNum; i++) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            
            Size++;
        }
    }
    // Insert an element at a specific position
    /**
     * @brief Insert an element at a specific position
     * @param value - the value to be inserted
     * @param position - the position to insert the value at
     * 
     * @details Creates a new node with the given value and inserts it at the given position in the list.
     * If the position is 0, the value is prepended to the list.
     * If the position is greater than or equal to the size of the list, the value is appended to the list.
     * Otherwise, the value is inserted at the given position.
    */
    void insert(const T& value, size_t position) {
        if (position == 0) {
            prepend(value);
        } else if (position >= size) {
            append(value);
        } else {
            ListNode<T>* newNode = new ListNode<T>(value);
            ListNode<T>* current = head;
            for (size_t i = 1; i < position; i++) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            Size++;
        }
    }

    /**
     * @brief Remove an element at a specific position
     * @param index - the position of the element to remove
     * 
     * @details Removes the element at the given position in the list.
     * If the position is 0, the first element is removed.
     * If the position is greater than or equal to the size of the list, nothing happens.
     * Otherwise, the element at the given position is removed.
    */
    void remove(const int index){
        if(index == 0){
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
            Size--;
            return;
        }
        ListNode<T>* current = head;
        for(int i = 0; i < index - 1; i++){
            current = current->next;
        }
        ListNode<T>* temp = current->next;
        current->next = temp->next;
        delete temp;
        Size--;
    }
    
    /**
     * @brief Remove an element 
     * @param value - the value to be removed
     * 
    */
    void removeElement(T value) {
        if (!head) return;
    
        if (head->data == value) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
            Size--;
            return;
        }
    
        ListNode<T>* current = head;
        while (current->next) {
            if (current->next->data == value) {
                ListNode<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                Size--;
                return;
            }
            current = current->next;
        }
    }

    // Get the element at a specific position
    // Change the return type to a pointer
    /**
     * @brief Get the element at a specific position
     * @param position - the position of the element to get
     * @return the element at the given position
     * 
     * @details Returns a pointer to the element at the given position in the list.
     * If the position is out of bounds, nullptr is returned.
     * Otherwise, a pointer to the element at the given position is returned.
     * @warning This Function Returns a Pointer
    */
    T* get(size_t position) const {
        ListNode<T>* current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) return nullptr;
            current = current->next;
        }
        return current ? &(current->data) : nullptr;
    }


    T getElement(size_t position) const {
        ListNode<T>* current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) {
                return T(); // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            return current->data;
        } else {
            return T();
        }
    }

    /**
     * @brief Get the element at a specific position as a String
     * @param position - the position of the element to get
     * @return the element at the given position as a String
     * 
     * @details Returns the element at the given position in the list.
     * If the position is out of bounds, nullptr is returned.
     * Otherwise, the element at the given position is returned as a String
    */
    String getAsString(size_t position) const {
        ListNode<T>* current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) {
                return nullptr; // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            return String(current->data);
        } else {
            return nullptr;
        }
    }
    // Check if the list contains a specific element
    /**
     * @brief Check if the list contains a specific element
     * @param value - the value to check for
     * 
     * @details Returns true if the list contains the given value, false otherwise.
     * @return true if the list contains the given value, false otherwise.
    */
    bool contains(const T& value) const {
        ListNode<T>* current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Get the number of elements in the list
    /**
     * @brief Get the number of elements in the list
     * @return the number of elements in the list
    */
    size_t size() const {
        size_t count = 0;
        ListNode<T>* current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    // Check if the list is empty
    /**
     * @brief Check if the list is empty
     * @return true if the list is empty, false otherwise
    */
    bool isEmpty() const {
        return Size == 0;
    }

    // Clear the list and release memory
    /**
     * @brief Clear the list and release memory
     * 
     * @details Deletes all nodes in the list and sets the size to 0.
    */
    void clear() {
        while (head) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        Size = 0;
    }

    //Operators
    /**
     * @brief Overload the subscript operator
     * @param index - the index of the element to get
     * @return the element at the given index
     * 
     * @details Returns the element at the given index in the list.
     * If the index is out of bounds, the first element is returned.
     * Otherwise, the element at the given index is returned.
    */
    T& operator[](size_t index) {
        ListNode<T>* current = head;
        for (size_t i = 0; i < index; i++) {
            if (!current) {
                return current->data; // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            return current->data;
        } else {
            return current->data;
        }
    }

    /**
     * @brief Overload the subscript operator
     * @param index - the index of the element to get
     * @return the element at the given index
     * 
     * @details Returns the element at the given index in the list.
     * If the index is out of bounds, the first element is returned.
     * Otherwise, the element at the given index is returned.
    */
    const T& operator[](size_t index) const {
        ListNode<T>* current = head;
        for (size_t i = 0; i < index; i++) {
            if (!current) {
                return current->data; // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            return current->data;
        } else {
            return current->data;
        }
    }

    //= operator
    /**
     * @brief Overload the assignment operator
     * @param other - the list to copy
     * @return a reference to the list
     * 
     * @details Copies the elements from the given list to this list.
    */
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* current = other.head;
            while (current) {
                append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void debugPrintList() {
        Serial.print("List Size: ");
        Serial.println(Size);
        ListNode<T>* current = head;
        int index = 0;
        while (current) {
            Serial.print("Index ");
            Serial.print(index++);
            Serial.print(": ");
            Serial.println((uintptr_t)current->data, HEX);  // Print address
            current = current->next;
        }
    }


};

#endif // BASICLINKEDLIST_H