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
    void insert(const T& value, size_t position) {
        if (position == 0) {
            Serial.println("[DOUBLE LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (position >= Size) {
            Serial.println("[DOUBLE LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[DOUBLE LINKED LIST]: Inserting: " + String(value));
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
        ListNode<T>* current = head;
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
        Serial.println("[DOUBLE LINKED LIST]: Cleared list");
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
