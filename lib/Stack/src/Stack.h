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
            Serial.println("Stack: memory allocation failed during resize.");
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
    Stack() : stack(new T[10]), top(-1), size(10) {
        if(!stack){ Serial.println("Stack: memory allocation failed."); }
    }

    /**
     * @brief Construct a new Stack with the specified initial capacity.
     * @param initialCapacity The initial capacity of the stack.
     */
    Stack(int initialCapacity) : top(-1), size(initialCapacity > 0 ? initialCapacity : 10) {
        stack = new T[size];
        if(!stack){ Serial.println("Stack: memory allocation failed."); }
    }

    /**
     * @brief Copy constructor.
     */
    Stack(const Stack &other) : top(other.top), size(other.size) {
        stack = new T[size];
        if(!stack){ Serial.println("Stack: memory allocation failed."); return; }
        for(int i = 0; i <= top; i++){
            stack[i] = other.stack[i];
        }
    }

    /**
     * @brief Assignment operator.
     */
    Stack &operator=(const Stack &other){
        if(this != &other){
            T *newStack = new T[other.size];
            if(!newStack){ Serial.println("Stack: memory allocation failed."); return *this; }
            delete[] stack;
            top = other.top;
            size = other.size;
            stack = newStack;
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
     * @brief Check if the backing array was successfully allocated.
     * @return true if the stack is usable, false if allocation failed.
     */
    bool isValid() const {
        return stack != nullptr;
    }

    /**
     * @brief Push a value onto the top of the stack.
     * @param value The value to push.
     */
    void push(const T &value){
        if(!stack) return;
        if(isFull()){
            resize();
        }
        if(stack){ stack[++top] = value; }
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
        return top == -1 || !stack;
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
        if(!stack) return;
        for(int i = 0; i <= top; i++){
            Serial.println(stack[i]);
        }
        Serial.println();
    }

    /**
     * @brief Return the number of elements in the stack.
     */
    int count() const {
        return top + 1;
    }

    /**
     * @brief Remove all elements from the stack.
     */
    void clear(){
        top = -1;
    }
};

#endif // STACK_h