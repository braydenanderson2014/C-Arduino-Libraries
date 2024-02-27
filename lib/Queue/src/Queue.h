#ifndef QUEUE_h
#define QUEUE_h

template <class T>
class Queue {
private:
    int *queue; // array
    int front; // front of the queue
    int rear; // rear of the queue
    int size; // size of the queue
    int elements; // number of elements in the queue

    /**
     * @brief Resize the queue
     * @private
     * 
     * @return void
    */
    void resize(){
        int *newQueue = new int[size * 2]; // create a new array with double the size
        copy(newQueue);
        delete[] queue;
        queue = newQueue;
        size *= 2;
    }

    /**
     * @brief Copy the elements from the old queue to the new queue
     * @private
     * 
     * @param newQueue - the new queue
     * @return void
    */
    void copy(int *newQueue){
        for(int i = front; i <= rear; i++){
            newQueue[i] = queue[i];
        }
    }
public:
    /**
     * @brief Construct a new Queue object
     * 
    */
    Queue(){
        queue = new T[10]; // create a new array with size 10
        front = 0; // set the front to 0
        rear = -1; // set the rear to -1
        size = 10; // set the size to 10
        elements = 0; // set the number of elements to 0
    }

    /**
     * @brief Destroy the Queue object
     * 
    */
    ~Queue(){
        delete[] queue;
    }

    /**
     * @brief Add an element to the queue
     * 
     * @param value - the value to add
     * @return void
    */
    void enqueue(int value){
        if(isFull()){
            resize();
        }
        queue[++rear] = value;
        elements++;
    }

    /**
     * @brief Remove an element from the queue
     * 
     * @return int - the value removed
    */
    int dequeue(){
        if(isEmpty()){
            return -1;
        }
        return queue[front++];
        elements--;
    }

    /**
     * @brief Get the value at the front of the queue
     * 
     * @return int - the value at the front of the queue
    */
    int peek(){
        if(isEmpty()){
            return -1;
        }
        return queue[front];
    }

    /**
     * @brief Check if the queue is empty
     * 
     * @return bool - true if the queue is empty, false otherwise
    */
    bool isEmpty(){
        return front > rear;
    
    }

    /**
     * @brief Check if the queue is full
     * 
     * @return bool - true if the queue is full, false otherwise
    */
    bool isFull(){
        return rear == size - 1;
    }

    /**
     * @brief Print the elements in the queue
     * 
     * @return void
    */
    void print(){
        for(int i = front; i <= rear; i++){
            Serial.println(queue[i]);
        }
    }
};
#endif // QUEUE_h