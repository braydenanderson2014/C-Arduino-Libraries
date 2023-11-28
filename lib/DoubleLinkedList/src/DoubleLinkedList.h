#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <Arduino.h>

template <typename T>
class DoubleListNode {
public:
    T data;
    DoubleListNode *next;
    DoubleListNode *prev; // Pointer to the previous node
    DoubleListNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoubleLinkedList {
private:
    DoubleListNode<T> *head;
    DoubleListNode<T> *tail; // Pointer to the last node
    size_t Size;
    // Random Iterator Helper
    mutable bool *visited;       // Array to keep track of visited nodes
    mutable size_t visitedCount; // Number of visited nodes
    bool debug;

public:
    class ForwardIterator {
    private:
        DoubleListNode<T> *current;
        bool debug;

    public:
        ForwardIterator(DoubleListNode<T> *start) : current(start), debug(debug) {}

        ~ForwardIterator()
        {
            if (debug)
            {
                Serial.println("[DOUBLE LINKED LIST]: Forward Iterator Destroyed");
            }
        }

        T &operator*()
        {
            if (debug)
            {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
            }
            return current->data;
        }

        bool operator!=(const ForwardIterator &other) const
        {
            if (debug)
            {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
                Serial.println("[DOUBLE LINKED LIST]: Other Node: " + String(other.current->data));
            }
            return current != other.current;
        }

        ForwardIterator &operator++()
        {
            if (current)
                current = current->next;

            if (debug)
            {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
            }
            return *this;
        }
    };

    // Backward iterator class definition
    class BackwardIterator {
    private:
        DoubleListNode<T> *current;
        bool debug;

    public:
        BackwardIterator(DoubleListNode<T> *start) : current(start), debug(debug) {}

        ~BackwardIterator() {
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Backward Iterator Destroyed");
            }
        }

        T &operator*() {
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
            }
            return current->data;
        }

        bool operator!=(const BackwardIterator &other) const {
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
                Serial.println("[DOUBLE LINKED LIST]: Other Node: " + String(other.current->data));
            }
            return current != other.current;
        }

        BackwardIterator &operator--() {
            if (current)
                current = current->prev;

            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
            }
            return *this;
        }
    };

    // Random iterator class definition
    class RandomIterator {
    private:
        DoubleLinkedList<T> *list;
        bool debug;

        void resetVisited() const {
            for (size_t i = 0; i < list->Size; ++i) {
                list->visited[i] = false;
            }
            list->visitedCount = 0;
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Resetted Visited list");
            }
        }

    public:
        RandomIterator(DoubleLinkedList<T> *list) : list(list), debug(debug) {
            list->visited = new bool[list->Size]();
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Random Iterator Created");
            }
            resetVisited();
        }

        ~RandomIterator() {
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Random Iterator Destroyed");
            }
            delete[] list->visited;
        }

        T &operator*() {
            size_t index = random(0, list->Size - list->visitedCount);
            size_t count = 0;
            for (size_t i = 0; i < list->Size; ++i) {
                if (!list->visited[i]) {
                    if (count == index) {
                        list->visited[i] = true;
                        list->visitedCount++;
                        if (debug) {
                            Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(list->get(i)->data));
                        }
                        return *list->get(i);
                    }
                    count++;
                    if (debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(list->get(i)->data));
                    }
                }
                if (debug) {
                    Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(list->get(i)->data));
                }
                // Return the first unvisited element as a fallback
                for (size_t i = 0; i < list->Size; ++i) {
                    if (!list->visited[i]) {
                        list->visited[i] = true;
                        list->visitedCount++;
                        if (debug) {
                            Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(list->get(i)->data));
                        }
                        return *list->get(i);
                    }
                    if (debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(list->get(i)->data));
                    }
                }
                // Should not reach here
            }
        }
        bool hasNext() const {
            if (debug) {
                Serial.println("[DOUBLE LINKED LIST]: Has Next: " + String(list->visitedCount < list->Size));
            }
            return list->visitedCount < list->Size;
        }
        void reset() {
            resetVisited();
        }
    };

    // Methods to get iterators
    ForwardIterator begin() const {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Forward Iterator Begin Defined");
        }
        return ForwardIterator(head);
    }

    ForwardIterator end() const {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Forward Iterator End Defined");
        }
        return ForwardIterator(nullptr);
    }

    BackwardIterator rbegin() const {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Backward Iterator Begin Defined");
        }
        return BackwardIterator(tail);
    }

    BackwardIterator rend() const {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Backward Iterator End Defined");
        }
        return BackwardIterator(nullptr);
    }

    RandomIterator randomBegin() {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Random Iterator Created");
        }
        return RandomIterator(this);
    }

    DoubleLinkedList() : head(nullptr), tail(nullptr), Size(0), debug(debug) {}

    ~DoubleLinkedList() {
        if (debug) {
            Serial.println("[DOUBLE LINKED LIST]: Double Linked List Destroyed");
        }
        clear();
    }

    void append(const T &value) {
        DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
        if (!head) {
            head = newNode;
            tail = newNode;
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Head and Tail are the same");
            }
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Tail is now: " + String(tail->data));
            }
        }
        Size++;
    }

    void prepend(const T &value) {
        DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
        newNode->next = head;
        if (head) {
            head->prev = newNode;
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Head is now: " + String(head->data));
            }
        } else {
            tail = newNode;
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Tail is now: " + String(tail->data));
            }
        }
        head = newNode;
        Size++;
    }

    void insert(const T &value) {
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
    void insert(const T &value, size_t position) {
        if (position == 0) {
            Serial.println("[DOUBLE LINKED LIST]: Prepending: " + String(value));
            prepend(value);
        } else if (position >= Size) {
            Serial.println("[DOUBLE LINKED LIST]: Appending: " + String(value));
            append(value);
        } else {
            Serial.println("[DOUBLE LINKED LIST]: Inserting: " + String(value));
            DoubleListNode<T> *newNode = new DoubleListNode<T>(value);
            DoubleListNode<T> *current = head;
            for (size_t i = 0; i < position; i++) {
                current = current->next;
                if(debug) {
                    Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
                }
            }
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
            Size++;
            if(debug){
                Serial.println("[DOUBLE LINKED LIST]: Size: " + String(Size));
            }
        }
    }

    // Remove the first occurrence of an element from the list
    void remove(const T &value) {
        DoubleListNode<T> *current = head;
        Serial.println("[DOUBLE LINKED LIST]: Removing: " + String(value));
        while (current) {
            if (current->data == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                    if(debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Previous Node: " + String(current->prev->data));
                    }
                } else {
                    head = current->next;
                    if(debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Head is now: " + String(head->data));
                    }
                }
                if (current->next) {
                    current->next->prev = current->prev;
                    if(debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Next Node: " + String(current->next->data));
                    }
                } else {
                    tail = current->prev;
                    if(debug) {
                        Serial.println("[DOUBLE LINKED LIST]: Tail is now: " + String(tail->data));
                    }
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
    T *get(size_t position) const {
        DoubleListNode<T> *current = head;
        for (size_t i = 0; i < position; i++) {
            if (!current) {
                if(debug) {
                    Serial.println("[DOUBLE LINKED LIST]: Out of bounds");
                }
                return nullptr; // Out of bounds
            }
            current = current->next;
        }
        if (current) {
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Current Node: " + String(current->data));
            }
            return &(current->data);
        } else {
            if(debug) {
                Serial.println("[DOUBLE LINKED LIST]: Out of bounds or unable to get data");
            }
            return nullptr;
        }
    }
    // Check if the list contains a specific element
    bool contains(const T &value) const {
        DoubleListNode<T> *current = head;
        while (current) {
            if (current->data == value) {
                if(debug) {
                    Serial.println("[DOUBLE LINKED LIST]: Value exists: " + String(value));
                }
                return true;
            }
            current = current->next;
        }
        if(debug) {
            Serial.println("[DOUBLE LINKED LIST]: Value does not exist: " + String(value));
        }
        return false;
    }

    // Get the number of elements in the list
    size_t size() const {
        if(debug) {
            Serial.println("[DOUBLE LINKED LIST]: Size: " + String(Size));
        }
        return Size;
    }

    // Check if the list is empty
    bool isEmpty() const {
        if(debug) {
            Serial.println("[DOUBLE LINKED LIST]: Is Empty: " + String(Size == 0));
        }
        return Size == 0;
    }

    // Clear the list and release memory
    void clear() {
        while (head) {
            DoubleListNode<T> *temp = head;
            head = head->next;
            delete temp;
        }
        Size = 0;
        Serial.println("[DOUBLE LINKED LIST]: Cleared list");
    }

    bool valueExists(const T &value) const {
        DoubleListNode<T> *current = head;
        while (current) {
            if (current->data == value) {
                if(debug) {
                    Serial.println("[DOUBLE LINKED LIST]: Value exists: " + String(value));
                }
                return true;
            }
            current = current->next;
        }
        if(debug) {
            Serial.println("[DOUBLE LINKED LIST]: Value does not exist: " + String(value));
        }
        return false;
    }

    void setDebug(bool debug) {
        if(debug) {
            Serial.println("[DOUBLE LINKED LIST]: Debug Mode: " + String(debug));
        }
        this->debug = debug;
    }

    bool getDebug() const {
        return debug;
    }
    
};
#endif // DOUBLELINKEDLIST_H
