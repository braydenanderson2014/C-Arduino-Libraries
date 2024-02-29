#ifndef STACK_h
#define STACK_h

template <class T>
class Stack {
private:
    int *stack;
    int top;
    int size;
    void resize(){
        int *newStack = new int[size * 2];
        copy(newStack);
        delete[] stack;
        stack = newStack;
        size *= 2;
    }
    void copy(int *newStack){
        for(int i = 0; i <= top; i++){
            newStack[i] = stack[i];
        }
    }
public:
    Stack(){
        stack = new T[10];
        top = -1;
        size = 10;
    }

    ~Stack(){
        delete[] stack;
    }
    void push(int value){
        if(isFull()){
            resize();
        }
        stack[++top] = value;
    }

    int pop(){
        if(isEmpty()){
            return -1;
        }
        return stack[top--];
    }

    int peek(){
        if(isEmpty()){
            return -1;
        }
        return stack[top];
    }
    bool isEmpty(){
        return top == -1;
    
    }
    bool isFull(){
        return top == size - 1;
    }
    void print(){
        for(int i = 0; i <= top; i++){
            Serial.println(stack[i])
        }
        Serial.println();
    }
};

#endif // STACK_h