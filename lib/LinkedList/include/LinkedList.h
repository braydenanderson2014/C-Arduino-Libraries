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
    size_t size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList();

    void append(const T& value);
    void prepend(const T& value);
    void insert(const T& value, size_t position);
    void remove(const T& value);
    T& get(size_t position) const;
    bool contains(const T& value) const;
    size_t getSize() const;
    bool isEmpty() const;
    void clear();
};

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// The rest of your methods would go here, following the same pattern as the destructor.
// For example:

template <typename T>
void LinkedList<T>::append(const T& value) {
    // Your implementation...
}

// Continue with the rest of the method implementations...

#endif // LINKEDLIST_H
