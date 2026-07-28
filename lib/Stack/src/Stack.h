#ifndef STACK_h
#define STACK_h

#include <Arduino.h>

template <class T>
class Stack {
private:
    T *stack;
    int top;
    int size;

    void resize(){
        T *newStack = new T[size * 2];
        if(!newStack){
            return;
        }
        copy(newStack);
        delete[] stack;
        stack = newStack;
        size *= 2;
    }

    void copy(T *newStack){
        for(int i = 0; i <= top; i++){
            newStack[i] = stack[i];
        }
    }

public:
    /**
     * @brief Construct a new Stack with a default initial capacity of 10.
     */
    Stack() : stack(new T[10]), top(-1), size(10) {}

    /**
     * @brief Construct a new Stack with the specified initial capacity.
     * @param initialCapacity The initial capacity of the stack.
     */
    Stack(int initialCapacity) : top(-1), size(initialCapacity > 0 ? initialCapacity : 10) {
        stack = new T[size];
    }

    /**
     * @brief Copy constructor.
     */
    Stack(const Stack &other) : top(other.top), size(other.size) {
        stack = new T[size];
        for(int i = 0; i <= top; i++){
            stack[i] = other.stack[i];
        }
    }

    /**
     * @brief Assignment operator.
     */
    Stack &operator=(const Stack &other){
        if(this != &other){
            delete[] stack;
            top = other.top;
            size = other.size;
            stack = new T[size];
            for(int i = 0; i <= top; i++){
                stack[i] = other.stack[i];
            }
        }
        return *this;
    }

    ~Stack(){
        delete[] stack;
    }

    /**
     * @brief Push a value onto the top of the stack.
     * @param value The value to push.
     */
    void push(const T &value){
        if(isFull()){
            resize();
        }
        stack[++top] = value;
    }

    /**
     * @brief Remove and return the top element of the stack.
     * @return The top element, or a default-constructed T if the stack is empty.
     */
    T pop(){
        if(isEmpty()){
            return T();
        }
        return stack[top--];
    }

    /**
     * @brief Return the top element without removing it.
     * @return The top element, or a default-constructed T if the stack is empty.
     */
    T peek() const {
        if(isEmpty()){
            return T();
        }
        return stack[top];
    }

    /**
     * @brief Check if the stack is empty.
     * @return true if the stack has no elements, false otherwise.
     */
    bool isEmpty() const {
        return top == -1;
    }

    /**
     * @brief Check if the stack's backing array is at capacity.
     *        The stack will auto-resize on the next push.
     * @return true if the backing array is full.
     */
    bool isFull() const {
        return top == size - 1;
    }

    /**
     * @brief Print all elements in the stack (bottom to top).
     */
    void print() const {
        for(int i = 0; i <= top; i++){
            Serial.println(stack[i]);
        }
        Serial.println();
    }

    /**
     * @brief Return the number of elements in the stack.
     */
    uint16_t count() const {
        return (uint16_t)(top + 1);
    }

    /**
     * @brief Remove all elements from the stack.
     */
    void clear(){
        top = -1;
    }
};

#endif // STACK_h