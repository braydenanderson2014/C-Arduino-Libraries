#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <Arduino.h>

template <typename T>
class DoubleListNode {
public:
    T data;
    DoubleListNode* next;
    DoubleListNode* prev;  // Pointer to the previous node

    DoubleListNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoubleLinkedList {
private:
    DoubleListNode<T>* head;
    DoubleListNode<T>* tail;  // Pointer to the last node
    size_t Size;

    // Random Iterator Helper
    mutable bool* visited;  // Array to keep track of visited nodes
    mutable size_t visitedCount;  // Number of visited nodes

public:
class ForwardIterator {
    private:
        DoubleListNode<T>* current;

    public:
        ForwardIterator(DoubleListNode<T>* start) : current(start) {}

        T& operator*() {
            return current->data;
        }

        bool operator!=(const ForwardIterator& other) const {
            return current != other.current;
        }

        ForwardIterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
    };

    // Backward iterator class definition
    class BackwardIterator {
    private:
        DoubleListNode<T>* current;

    public:
        BackwardIterator(DoubleListNode<T>* start) : current(start) {}

        T& operator*() {
            return current->data;
        }

        bool operator!=(const BackwardIterator& other) const {
            return current != other.current;
        }

        BackwardIterator& operator--() {
            if (current) current = current->prev;
            return *this;
        }
    };

    // Random iterator class definition
    class RandomIterator {
    private:
        DoubleLinkedList<T>* list;

        void resetVisited() const {
            for (size_t i = 0; i < list->Size; ++i) {
                list->visited[i] = false;
            }
            list->visitedCount = 0;
        }

    public:
        RandomIterator(DoubleLinkedList<T>* list) : list(list) {
            list->visited = new bool[list->Size]();
            resetVisited();
        }

        ~RandomIterator() {
            delete[] list->visited;
        }

        T& operator*() {
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

        bool hasNext() const {
            return list->visitedCount < list->Size;
        }

        void reset() {
            resetVisited();
        }
    };

    // Methods to get iterators
    ForwardIterator begin() const {
        return ForwardIterator(head);
    }

    ForwardIterator end() const {
        return ForwardIterator(nullptr);
    }

    BackwardIterator rbegin() const {
        return BackwardIterator(tail);
    }

    BackwardIterator rend() const {
        return BackwardIterator(nullptr);
    }

    RandomIterator randomBegin() {
        return RandomIterator(this);
    }

    DoubleLinkedList() : head(nullptr), tail(nullptr), Size(0) {}

    ~DoubleLinkedList() {
        clear();
    }

    void append(const T& value) {
        DoubleListNode<T>* newNode = new DoubleListNode<T>(value);
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

    void prepend(const T& value) {
        DoubleListNode<T>* newNode = new DoubleListNode<T>(value);
        newNode->next = head;
        if (head) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        Size++;
    }

    void insert(const T& value){
        int randomNum = random(0, Size);
        Serial.println("[DOUBLE LINKED LIST]: Random Index to add data: " + String(randomNum));
        if (randomNum == 0) {
            Serial.println("[DOUBLE LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (randomNum >= Size) {
            Serial.println("[DOUBLE LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[DOUBLE LINKED LIST]: Inserting: " + String(value));
            DoubleListNode<T>* newNode = new DoubleListNode<T>(value);
            DoubleListNode<T>* current = head;
            for (size_t i = 1; i < randomNum; i++) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            Size++;
        }
    }
    // Insert an element at a specific position
    void insert(const T& value, size_t position) {
        if (position == 0) {
            Serial.println("[DOUBLE LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (position >= Size) {
            Serial.println("[DOUBLE LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[DOUBLE LINKED LIST]: Inserting: " + String(value));
            DoubleListNode<T>* newNode = new DoubleListNode<T>(value);
            DoubleListNode<T>* current = head;
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
    void remove(const T& value) {
        DoubleListNode<T>* current = head;
        Serial.println("[DOUBLE LINKED LIST]: Removing: " + String(value));
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
    T* get(size_t position) const {
        DoubleListNode<T>* current = head;
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
    // Check if the list contains a specific element
    bool contains(const T& value) const {
        DoubleListNode<T>* current = head;
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Get the number of elements in the list
    size_t size() const {
        return Size;
    }

    // Check if the list is empty
    bool isEmpty() const {
        return Size == 0;
    }

    // Clear the list and release memory
    void clear() {
        while (head) {
            DoubleListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        Size = 0;
        Serial.println("[DOUBLE LINKED LIST]: Cleared list");
    }

    bool valueExists(const T& value) const {
        DoubleListNode<T>* current = head;
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
