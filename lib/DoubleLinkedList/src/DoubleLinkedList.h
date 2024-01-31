#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <Arduino.h>

/**
 * @brief Double Linked List Node
 * @param T Data type
 * @note This class is used to create a node for the double linked list
 * 
 * @return Double Linked List Node
*/
template <typename T>
class DoubleListNode {
public:
    T data;
    DoubleListNode *next;
    DoubleListNode *prev; // Pointer to the previous node
    DoubleListNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

/**
 * @brief Double Linked List
 * @param T Data type
 * @note This class is used to create a double linked list
 * @details A Double Linked List is a List that has a pointer to the next and previous node
*/
template <typename T>
class DoubleLinkedList {
private:
    DoubleListNode<T> *head; // Pointer to the first node
    DoubleListNode<T> *tail; // Pointer to the last node
    size_t Size;            // Number of elements in the list
    // Random Iterator Helper
    mutable bool *visited;       // Array to keep track of visited nodes
    mutable size_t visitedCount; // Number of visited nodes

public:
    /**
     * @brief Forward Iterator
     * @param T Data type
     * @note This class is used to create a forward iterator for the double linked list
    */
    class ForwardIterator {
    private:
        DoubleListNode<T> *current;

    public:
        /**
         * @brief Construct a new Forward Iterator object
        */
        ForwardIterator(DoubleListNode<T> *start) : current(start) {}

        /**
         * @brief Destroy the Forward Iterator object
        */
        ~ForwardIterator(){}

        /**
         * @brief Dereference operator
        */
        T &operator*() {
            return current->data;
        }

        /**
         * @brief Not equal operator
        */
        bool operator!=(const ForwardIterator &other) const {
            return current != other.current;
        }

        /**
         * @brief Increment operator
        */
        ForwardIterator &operator++() {
            if (current) current = current->next;
            return *this;
        }
    };

    // Backward iterator class definition
    /**
     * @brief Backward Iterator
    */
    class BackwardIterator {
    private:
        DoubleListNode<T> *current;
    public:
        /**
         * @brief Construct a new Backward Iterator object
        */
        BackwardIterator(DoubleListNode<T> *start) : current(start) {}

        /**
         * @brief Destroy the Backward Iterator object
        */
        ~BackwardIterator() {}

        /**
         * @brief Dereference operator
        */
        T &operator*() {
            return current->data;
        }

        /**
         * @brief Not equal operator
        */
        bool operator!=(const BackwardIterator &other) const {
            return current != other.current;
        }

        /**
         * @brief Decrement operator
        */
        BackwardIterator &operator--() {
            if (current) current = current->prev;
            return *this;
        }
    };

    // Random iterator class definition
    /**
     * @brief Random Iterator
    */
    class RandomIterator {
    private:
        DoubleLinkedList<T> *list;

        /**
         * @brief Reset the visited list
         * @details This method resets the visited list
        */
        void resetVisited() const {
            for (size_t i = 0; i < list->Size; ++i) {
                list->visited[i] = false;
            }
            list->visitedCount = 0;
        }

    public:
        /**
         * @brief Construct a new Random Iterator object
        */
        RandomIterator(DoubleLinkedList<T> *list) : list(list) {
            list->visited = new bool[list->Size]();
            resetVisited();
        }

        /**
         * @brief Destroy the Random Iterator object
        */
        ~RandomIterator() {
            delete[] list->visited;
        }

        /**
         * @brief Dereference operator
        */
        T &operator*() {
            size_t index = random(0, list->Size - list->visitedCount);
            size_t count = 0;
            for (size_t i = 0; i < list->Size; ++i) {
                if (!list->visited[i]) {
                    if (count == index) {
                        list->visited[i] = true;
                        list->visitedCount++;
                        return *list->get(i);
                    }
                    count++;
                }
                // Return the first unvisited element as a fallback
                for (size_t i = 0; i < list->Size; ++i) {
                    if (!list->visited[i]) {
                        list->visited[i] = true;
                        list->visitedCount++;
                        return *list->get(i);
                    }
                }
                // Should not reach here
            }
        }

        /**
         * @brief has next operator
         * @details This method checks if there is a next element
        */
        bool hasNext() const {
            return list->visitedCount < list->Size;
        }

        /**
         * @brief Reset the iterator
         * @details This method resets the iterator
        */
        void reset() {
            resetVisited();
        }
    };

    // Methods to get iterators
    /**
     * @brief Get the begin Forward iterator
     * @details This method gets the begin forward iterator
     * 
     * @return ForwardIterator
    */
    ForwardIterator begin() const {
        return ForwardIterator(head);
    }

    /**
     * @brief Get the end Forward iterator
     * @details This method gets the end forward iterator
     * 
     * @return ForwardIterator
    */
    ForwardIterator end() const {
        return ForwardIterator(nullptr);
    }

    /**
     * @brief Get the begin Backward iterator
     * @details This method gets the begin backward iterator
     * 
     * @return BackwardIterator
    */
    BackwardIterator rbegin() const {
        return BackwardIterator(tail);
    }

    /**
     * @brief Get the end Backward iterator
     * @details This method gets the end backward iterator
     * 
     * @return BackwardIterator
    */
    BackwardIterator rend() const {
        return BackwardIterator(nullptr);
    }

    /**
     * @brief Get the Random Iterator
     * @details This method gets the random iterator
     * 
     * @return RandomIterator
    */
    RandomIterator randomBegin() {
        return RandomIterator(this);
    }

    /**
     * @brief Construct a new Double Linked List object
     * @details This method constructs a new double linked list
    */
    DoubleLinkedList() : head(nullptr), tail(nullptr), Size(0) {}

    /**
     * @brief Destroy the Double Linked List object
     * @details This method destroys the double linked list
    */
    ~DoubleLinkedList() {
        clear();
    }

    /**
     * @brief Append an element to the end of the list
     * @details This method appends an element to the end of the list
     * 
     * @param value Value to append
    */
    void append(const T &value) {
        DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        Size++;
    }

    /**
     * @brief Prepend an element to the beginning of the list
     * @details This method prepends an element to the beginning of the list
     * 
     * @param value Value to prepend
    */
    void prepend(const T &value) {
        DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
        newNode->next = head;
        if (head) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        Size++;
    }

    /**
     * @brief Insert an element at a random position
     * @details This method inserts an element at a random position
     * 
     * @param value Value to insert
    */
    void insert(const T &value) {
        int randomNum = random(0, Size);
        if (randomNum == 0) {
            prepend(value);
        } else if (randomNum >= Size) {
            append(value);
        } else {
            DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
            DoubleListNode<T> *current = head;
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
     * @details This method inserts an element at a specific position
     * 
     * @param value Value to insert
     * @param position Position to insert
     * @note If the position is 0, the element will be prepended
     * @note If the position is greater than or equal to the size, the element will be appended
     * @note If the position is within the size, the element will be inserted at that position
    */
    void insert(const T &value, size_t position) {
        if (position == 0) {
            prepend(value);
        } else if (position >= Size) {
            append(value);
        } else {
            DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
            DoubleListNode<T> *current = head;
            for (size_t i = 0; i < position; i++) {
                current = current->next;
            }
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
            Size++;
        }
    }

    // Remove the first occurrence of an element from the list
    /**
     * @brief Remove the first occurrence of an element from the list
     * @details This method removes the first occurrence of an element from the list
    */
    void remove(const T &value) {
        DoubleListNode<T> *current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                delete current;
                Size--;
                return;
            }
            current = current->next;
        }
    }

    // Rest of the methods...
    // Change the return type to a pointer
    /**
     * @brief Get the element at a specific position
     * @details This method gets the element at a specific position
     * 
     * @param position Position to get
     * @return T* Pointer to the element
     * @note If the position is out of bounds, nullptr will be returned
     * @note If the position is within bounds, a pointer to the element will be returned
     * @note If the position is within bounds, but the element is nullptr, nullptr will be returned
     * @warning This method returns a pointer to the element, not the element itself
    */
    T *get(size_t position) const {
        DoubleListNode<T> *current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) {
                return nullptr; // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            return &(current->data);
        } else {
            return nullptr;
        }
    }

    /**
     * @brief Get the element at a specific position as a string
     * @details This method gets the element at a specific position and returns it as a string
     * 
     * @param position Position to get
     * @return String Element at the position
     * 
    */
    String getAsString(size_t position) const {
        DoubleListNode<T> *current = head;
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
     * @details This method checks if the list contains a specific element
     * 
     * @param value Value to check
     * @return true If the list contains the element
    */
    bool contains(const T &value) const {
        DoubleListNode<T> *current = head;
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
     * @details This method gets the number of elements in the list
     * 
     * @return size_t Number of elements in the list
    */
    size_t size() const {
        return Size;
    }

    // Check if the list is empty
    /**
     * @brief Check if the list is empty
     * @details This method checks if the list is empty
     * 
     * @return true If the list is empty
    */
    bool isEmpty() const {
        return Size == 0;
    }

    // Clear the list and release memory
    /**
     * @brief Clear the list and release memory
     * @details This method clears the list and releases memory
    */
    void clear() {
        while (head) {
            DoubleListNode<T> *temp = head;
            head = head->next;
            delete temp;
        }
        Size = 0;
    }

    /**
     * @brief checks if the value exists in the list
     * @details This method checks if the value exists in the list
     * @deprecated This method is deprecated and will be removed in the next version
     * @see contains()
     * 
     * @param value Value to check
    */
    bool valueExists(const T &value) const {
        DoubleListNode<T> *current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};
#endif // DOUBLELINKEDLIST_H
