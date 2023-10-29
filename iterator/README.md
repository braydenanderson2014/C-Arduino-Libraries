# Iterator Class

The `Iterator` class is a C++ template class that provides an abstract interface for creating iterators. Iterators are commonly used in C++ for traversing and manipulating data structures like arrays, linked lists, and containers. This class defines the core methods necessary for creating custom iterators.

## Usage

To use the `Iterator` class, follow these steps:

1. Include the `Iterator` header file in your C++ program.
2. Define a class that implements the `Iterator` interface for your specific data structure or container.
3. Implement the required methods: `operator*`, `operator++`, and `operator!=`.
4. Use the custom iterator class to traverse and manipulate your data structure.


## Installation

```powershell
git clone "https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/iterator.git"

```
## Header

If you want to Utilize this Library. Please include the 
```cpp 
#include <iterator.h> 
```

## ChangeLog
    Version 1.0.0:
        * Initial Release 
    
## Example

Here's an example of how to define and use an iterator class using the `Iterator` interface:

```cpp
#include <iostream>
#include "Iterator.h"

// Define a custom data structure (e.g., a linked list)
template <typename T>
class MyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;

public:
    MyLinkedList() : head(nullptr) {}

    // Add elements to the linked list (not shown here)

    // Iterator class for MyLinkedList
    class LinkedListIterator : public Iterator<T> {
    private:
        Node* current;

    public:
        LinkedListIterator(Node* start) : current(start) {}

        // Implement the operator* to get the current element
        T& operator*() override {
            return current->data;
        }

        // Implement the operator++ to move to the next element
        LinkedListIterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        // Implement the operator!= to compare iterators
        bool operator!=(const Iterator<T>& other) const override {
            const LinkedListIterator& otherIterator = static_cast<const LinkedListIterator&>(other);
            return current != otherIterator.current;
        }
    };

    // Get an iterator for the linked list
    LinkedListIterator begin() {
        return LinkedListIterator(head);
    }

    LinkedListIterator end() {
        return LinkedListIterator(nullptr);
    }
};

int main() {
    MyLinkedList<int> myList;

    // Add elements to myList (not shown here)

    // Use the custom iterator to traverse the linked list
    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}


```
