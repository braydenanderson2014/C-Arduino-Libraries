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

    ~LinkedList() {
        clear();
    }

    // Add an element to the end of the list
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
        size++;
    }

    // Add an element to the beginning of the list
    void prepend(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    // Insert an element at a specific position
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
            size++;
        }
    }

    // Remove the first occurrence of an element from the list
    void remove(const T& value) {
        if (!head) {
            return; // List is empty
        }
        if (head->data == value) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
            size--;
            return;
        }
        ListNode<T>* current = head;
        while (current->next) {
            if (current->next->data == value) {
                ListNode<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
                return;
            }
            current = current->next;
        }
    }

    // Get the element at a specific position
    T& get(size_t position) const {
        if (position < size) {
            ListNode<T>* current = head;
            for (size_t i = 0; i < position; i++) {
                current = current->next;
            }
            return current->data;
        }
        throw std::out_of_range("Index out of bounds");
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
    size_t getSize() const {
        return size;
    }

    // Check if the list is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Clear the list and release memory
    void clear() {
        while (head) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
};

int main() {
    LinkedList<int> myList;

    myList.append(1);
    myList.append(2);
    myList.prepend(0);
    myList.insert(3, 3);

    Serial.println("Size: " + myList.getSize());

    for (size_t i = 0; i < myList.getSize(); i++) {
        Serial.println("Element at position " + String(i) + ": " + String(myList.get(i)));
    }

    if (myList.contains(2)) {
        Serial.println("List contains 2.");
    }

    myList.remove(2);
    myList.clear();

    return 0;
}