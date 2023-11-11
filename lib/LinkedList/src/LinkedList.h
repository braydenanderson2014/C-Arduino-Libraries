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
    LinkedList() : head(nullptr), Size(0) {}

    ~LinkedList();

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
        Serial.println("Random Index to add data: " + String(random));
        if (randomNum == 0) {
            Serial.println("Prepending");
            prepend(value);
        } else if (randomNum >= Size) {
            Serial.println("Appending");
            append(value);
        } else {
            Serial.println("Inserting");
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
            Serial.println("Prepending");
            prepend(value);
        } else if (position >= size) {
            Serial.println("Appending");
            append(value);
        } else {
            Serial.println("Inserting");
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
            Serial.println("List is empty");
            return; // List is empty
        }
        if (head->data == value) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
            Size--;
            Serial.println("Removed head");
            return;
        }
        ListNode<T>* current = head;
        while (current->next) {
            if (current->next->data == value) {
                ListNode<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                Size--;
                Serial.println("Removed element");
                return;
            }
            current = current->next;
        }
    }
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
