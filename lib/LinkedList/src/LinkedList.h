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
    bool debug;
public:
class ForwardIterator {
    private:
        ListNode<T>* current;
        bool debug;
    public:
        ForwardIterator(ListNode<T>* start) : current(start), debug(debug) {}

        T& operator*() {
            if(debug){
                Serial.println("[LINKED LIST]: Iterator Dereference");
            }
            return current->data;
        }

        bool operator!=(const ForwardIterator& other) const {
            if(debug){
                Serial.println("[LINKED LIST]: Iterator Inequality check");
            }
            return current != other.current;
        }

        ForwardIterator& operator++() {
            if (current) current = current->next;
            if(debug){
                Serial.println("[LINKED LIST]: Iterator Prefix increment");
            }
            return *this;
        }
    };
    LinkedList() : head(nullptr), Size(0), debug(debug) {}

    ~LinkedList() {
        if(debug){
            Serial.println("[LINKED LIST]: Destroying LinkedList");
        }
        clear();
    }


    void append(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        if (!head) {
            head = newNode;
            if(debug){
                Serial.println("[LINKED LIST]: Appended to empty list");
            }
        } else {
            ListNode<T>* current = head;
            while (current->next) {
                current = current->next;
                if(debug){
                    Serial.println("[LINKED LIST]: Appended to non-empty list");
                }
            }
            current->next = newNode;
            if(debug){
                Serial.println("[LINKED LIST]: Appended to non-empty list" + String(value));
            }
        }
        Size++;
    }

    void prepend(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        newNode->next = head;
        head = newNode;
        if(debug){
            Serial.println("[LINKED LIST]: Prepended: " + String(value));
        }
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
            if(debug){
                Serial.println("[LINKED LIST]: Inserted: " + String(value));
            }
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
            if(debug){
                Serial.println("[LINKED LIST]: Inserted: " + String(value));
            }
            Size++;
        }
    }

    void setDebug(bool debug){
        if(debug){
            Serial.println("[LINKED LIST]: Debug mode enabled");
        } else {
            Serial.println("[LINKED LIST]: Debug mode disabled");
        }
        this->debug = debug;
    }

    bool getDebug(){
        return debug;
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
                if(debug){
                    Serial.println("[LINKED LIST]: Out of bounds" + String(position));
                }
                return nullptr; // Out of bounds
            }
            if(debug){
                Serial.println("[LINKED LIST]: Getting element at position" + String(position));
            }
            current = current->next;
        }
        if (current) {
            if(debug){
                Serial.println("[LINKED LIST]: Returning element at position" + String(position));
            }
            return &(current->data);
        } else {
            if(debug){
                Serial.println("[LINKED LIST]: Out of bounds or unable to get element at position" + String(position));
            }
            return nullptr;
        }
    }

    String getAsString(size_t position) const {
        ListNode<T>* current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) {
                if(debug){
                    Serial.println("[LINKED LIST]: Out of bounds" + String(position));
                }
                return nullptr; // Out of bounds
            }
            if(debug){
                Serial.println("[LINKED LIST]: Getting element at position" + String(position));
            }
            current = current->next;
        }
        if (current) {
            if(debug){
                Serial.println("[LINKED LIST]: Returning element at position" + String(position));
            }
            return String(current->data);
        } else {
            if(debug){
                Serial.println("[LINKED LIST]: Out of bounds or unable to get element at position" + String(position));
            }
            return nullptr;
        }
    }
    // Check if the list contains a specific element
    bool contains(const T& value) const {
        ListNode<T>* current = head;
        while (current) {
            if (current->data == value) {
                if(debug){
                    Serial.println("[LINKED LIST]: Contains element" + String(value));
                }
                return true;
            }
            current = current->next;
        }
        if(debug){
            Serial.println("[LINKED LIST]: Does not contain element" + String(value));
        }
        return false;
    }

    // Get the number of elements in the list
    size_t size() const {
        if(debug){
            Serial.println("[LINKED LIST]: Size: " + String(Size));
        }
        return Size;
    }

    // Check if the list is empty
    bool isEmpty() const {
        if(debug){
            Serial.println("[LINKED LIST]: Is empty: " + String(Size == 0));
        }
        return Size == 0;
    }

    // Clear the list and release memory
    void clear() {
        while (head) {
            ListNode<T>* temp = head;
            head = head->next;
            if(debug){
                Serial.println("[LINKED LIST]: Deleting node: " + String(temp->data) + " from list");
            }
            delete temp;
        }
        Size = 0;
        Serial.println("[LINKED LIST]: Cleared list");
    }

    bool keyExists(const T& key) const {
        ListNode<T>* current = head;
        while (current) {
            if (current->data == key) {
                if(debug){
                    Serial.println("[LINKED LIST]: Key exists" + String(key));
                }
                return true;
            }
            current = current->next;
        }
        if(debug){
            Serial.println("[LINKED LIST]: Key does not exist" + String(key));
        }
        return false;
    }

    bool valueExists(const T& value) const {
        ListNode<T>* current = head;
        while (current) {
            if (current->data == value) {
                if(debug){
                    Serial.println("[LINKED LIST]: Value exists" + String(value));
                }
                return true;
            }
            current = current->next;
        }
        if(debug){
            Serial.println("[LINKED LIST]: Value does not exist" + String(value));
        }
        return false;
    }
};

#endif // LINKEDLIST_H
