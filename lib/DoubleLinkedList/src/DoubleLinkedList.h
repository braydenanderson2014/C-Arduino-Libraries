#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <Arduino.h>

template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;
    ListNode* prev;  // Pointer to the previous node

    ListNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoubleLinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;  // Pointer to the last node
    size_t Size;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), Size(0) {}

    ~DoubleLinkedList() {
        clear();
    }

    void append(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
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
        ListNode<T>* newNode = new ListNode<T>(value);
        newNode->next = head;
        if (head) {
            head->prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
        Size++;
    }

    // Insert an element at a specific position
    void insert(const T& value, size_t position) {
        if (position == 0) {
            prepend(value);
        } else if (position >= Size) {
            append(value);
        } else {
            ListNode<T>* newNode = new ListNode<T>(value);
            ListNode<T>* current = head;
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
        ListNode<T>* current = head;
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
     // Get the element at a specific position
    T& get(size_t position) const {
        if (position < Size) {
            ListNode<T>* current = head;
            for (size_t i = 0; i < position; i++) {
                current = current->next;
            }
            return current->data;
        }
        Serial.println("Index out of bounds");
        return 0;
    }    
    // Check if the list contains a specific element
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
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        Size = 0;
        Serial.println("Cleared list");
    }


    bool valueExists(const T& value) const {
        ListNode<T>* current = head;
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
