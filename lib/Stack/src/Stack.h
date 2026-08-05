#ifndef STACK_h
#define STACK_h

#include <Arduino.h>

//#define ST_NO_SERIAL             // Uncomment to suppress all Serial output from this library (useful when Serial is not initialized)
//#define ST_ENABLE_NUMERIC_LIMITS // Uncomment to enable numeric_limits integration (requires Numeric_Limits library — optional dependency)
// PlatformIO: build_flags = -DST_NO_SERIAL
//             build_flags = -DST_ENABLE_NUMERIC_LIMITS

#ifdef ST_ENABLE_NUMERIC_LIMITS
    #include <Numeric_Limits.h>
#endif

template <class T>
class Stack {
private:
    T *stack;
    int top;
    int size;

    void resize(){
        T *newStack = new T[size * 2];
        if(!newStack){
#ifndef ST_NO_SERIAL
            Serial.println("Stack: memory allocation failed during resize.");
#endif
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
        if(!stack){
#ifndef ST_NO_SERIAL
            Serial.println("Stack: memory allocation failed.");
#endif
        }
    }

    /**
     * @brief Construct a new Stack with the specified initial capacity.
     * @param initialCapacity The initial capacity of the stack.
     */
    Stack(int initialCapacity) : top(-1), size(initialCapacity > 0 ? initialCapacity : 10) {
        stack = new T[size];
        if(!stack){
#ifndef ST_NO_SERIAL
            Serial.println("Stack: memory allocation failed.");
#endif
        }
    }

    /**
     * @brief Copy constructor.
     */
    Stack(const Stack &other) : top(other.top), size(other.size) {
        stack = new T[size];
        if(!stack){
#ifndef ST_NO_SERIAL
            Serial.println("Stack: memory allocation failed.");
#endif
            return;
        }
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
            if(!newStack){
#ifndef ST_NO_SERIAL
                Serial.println("Stack: memory allocation failed.");
#endif
                return *this;
            }
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
            if(isFull()){
#ifndef ST_NO_SERIAL
                Serial.println("Stack: push failed (out of memory).");
#endif
                return;
            }
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
#ifndef ST_NO_SERIAL
            Serial.println(stack[i]);
#endif
        }
#ifndef ST_NO_SERIAL
        Serial.println();
#endif
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

#ifdef ST_ENABLE_NUMERIC_LIMITS
    /**
     * @brief Returns the memory currently used by the backing array (in bytes).
     * @details Includes only the element storage array, not object overhead.
     * @return Bytes consumed by the backing array.
     */
    int memoryUsage() const {
        return size * static_cast<int>(sizeof(T));
    }

    /**
     * @brief Returns the theoretical maximum number of elements this Stack
     *        could hold, limited by the maximum value of int on this platform.
     * @return The upper bound on element count as reported by numeric_limits.
     */
    int theoreticalMaxElements() const {
        return numeric_limits<int>::Max();
    }

    /**
     * @brief Returns the ratio of elements stored to the theoretical maximum
     *        element count, expressed as a float in the range [0.0, 1.0].
     * @return Memory utilization fraction (current count / theoretical max).
     */
    float memoryUtilization() const {
        const int maxElems = theoreticalMaxElements();
        if (maxElems <= 0) return 0.0f;
        return static_cast<float>(top + 1) / static_cast<float>(maxElems);
    }
#endif // ST_ENABLE_NUMERIC_LIMITS
};

#endif // STACK_h