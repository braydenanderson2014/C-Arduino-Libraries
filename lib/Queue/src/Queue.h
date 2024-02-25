#ifndef QUEUE_h
#define QUEUE_h




template <class T>
class Queue {


private:
    int *queue;
    int front;
    int rear;
    int size;
    void resize(){
        int *newQueue = new int[size * 2];
        copy(newQueue);
        delete[] queue;
        queue = newQueue;
        size *= 2;
    }
    void copy(int *newQueue){
        for(int i = front; i <= rear; i++){
            newQueue[i] = queue[i];
        }
    }
public:
    Queue(){
        queue = new T[10];
        front = 0;
        rear = -1;
        size = 10;
    }

    ~Queue(){
        delete[] queue;
    }
    void enqueue(int value){
        if(isFull()){
            resize();
        }
        queue[++rear] = value;
    }

    int dequeue(){
        if(isEmpty()){
            return -1;
        }
        return queue[front++];
    }

    int peek(){
        if(isEmpty()){
            return -1;
        }
        return queue[front];
    }
    bool isEmpty(){
        return front > rear;
    
    }
    bool isFull(){
        return rear == size - 1;
    }
    void print(){
        for(int i = front; i <= rear; i++){
            Serial.println(queue[i]);
        }
    }
};


#endif // QUEUE_h

