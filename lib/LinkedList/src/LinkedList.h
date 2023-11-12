// LinkedList.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <Arduino.h>

template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;

    ListNode(T value) : data(value), next(nullptr) {}
};

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
        ForwardIterator(ListNode<T>* start) : current(start) {}

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
    LinkedList() : head(nullptr), Size(0) {}

    ~LinkedList() {
        clear();
    }


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

    void prepend(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        newNode->next = head;
        head = newNode;
        Size++;
    }

    void insert(const T& value){
        int randomNum = random(0, Size);
        Serial.println("[LINKED LIST]: Random Index to add data: " + String(randomNum));
        if (randomNum == 0) {
            Serial.println("[LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (randomNum >= Size) {
            Serial.println("[LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[LINKED LIST]: Inserting: " + String(value));
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
            Serial.println("[LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (position >= size) {
            Serial.println("[LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[LINKED LIST]: Inserting: " + String(value));
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
    // Remove the first occurrence of an element from the list
    void remove(const T& value) {
        if (!head) {
            Serial.println("[LINKED LIST]: List is empty, nothing to remove");
            return; // List is empty
        }
        if (head->data == value) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
            Size--;
            Serial.println("[LINKED LIST]: Removed head element");
            return;
        }
        ListNode<T>* current = head;
        while (current->next) {
            if (current->next->data == value) {
                ListNode<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                Size--;
                Serial.println("[LINKED LIST]: Removed element from list");
                return;
            }
            current = current->next;
        }
    }
    // Get the element at a specific position
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
        Serial.println("[LINKED LIST]: Cleared list");
    }

    bool keyExists(const T& key) const {
        ListNode<T>* current = head;
        while (current) {
            if (current->data == key) {
                return true;
            }
            current = current->next;
        }
        return false;
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

#endif // LINKEDLIST_H
