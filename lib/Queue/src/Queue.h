#ifndef QUEUE_h
#define QUEUE_h

#include <Arduino.h>

//#define Q_NO_SERIAL             // Uncomment to suppress all Serial output from this library (useful when Serial is not initialized)
//#define Q_ENABLE_NUMERIC_LIMITS // Uncomment to enable numeric_limits integration (requires Numeric_Limits library — optional dependency)
// PlatformIO: build_flags = -DQ_NO_SERIAL
//             build_flags = -DQ_ENABLE_NUMERIC_LIMITS

#ifdef Q_ENABLE_NUMERIC_LIMITS
    #include <Numeric_Limits.h>
#endif

template <class T>
class Queue {
private:
    T *queue;    // backing array
    int front;   // index of the front element
    int rear;    // index of the last element
    int size;    // capacity of the backing array
    int elements; // number of elements currently in the queue

    /**
     * @brief Compact elements to index 0 and double the backing array size.
     * @private
     */
    void resize(){
        T *newQueue = new T[size * 2];
        if(!newQueue){
#ifndef Q_NO_SERIAL
            Serial.println("Queue: memory allocation failed during resize.");
#endif
            return;
        }
        for(int i = 0; i < elements; i++){
            newQueue[i] = queue[front + i];
        }
        delete[] queue;
        queue = newQueue;
        front = 0;
        rear = elements - 1;
        size *= 2;
    }

public:
    /**
     * @brief Construct a new Queue with a default initial capacity of 10.
     */
    Queue() : queue(new T[10]), front(0), rear(-1), size(10), elements(0) {
        if(!queue){
#ifndef Q_NO_SERIAL
            Serial.println("Queue: memory allocation failed.");
#endif
        }
    }

    /**
     * @brief Construct a new Queue with the specified initial capacity.
     * @param initialCapacity The initial capacity of the queue.
     */
    Queue(int initialCapacity) : front(0), rear(-1), elements(0) {
        size = initialCapacity > 0 ? initialCapacity : 10;
        queue = new T[size];
        if(!queue){
#ifndef Q_NO_SERIAL
            Serial.println("Queue: memory allocation failed.");
#endif
        }
    }

    /**
     * @brief Copy constructor.
     */
    Queue(const Queue &other) : front(0), rear(other.elements - 1), size(other.size), elements(other.elements) {
        queue = new T[size];
        if(!queue){
#ifndef Q_NO_SERIAL
            Serial.println("Queue: memory allocation failed.");
#endif
            return;
        }
        for(int i = 0; i < elements; i++){
            queue[i] = other.queue[other.front + i];
        }
    }

    /**
     * @brief Assignment operator.
     */
    Queue &operator=(const Queue &other){
        if(this != &other){
            T *newQueue = new T[other.size];
            if(!newQueue){
#ifndef Q_NO_SERIAL
                Serial.println("Queue: memory allocation failed.");
#endif
                return *this;
            }
            delete[] queue;
            size = other.size;
            elements = other.elements;
            front = 0;
            rear = elements - 1;
            queue = newQueue;
            for(int i = 0; i < elements; i++){
                queue[i] = other.queue[other.front + i];
            }
        }
        return *this;
    }

    /**
     * @brief Destroy the Queue object.
     */
    ~Queue(){
        delete[] queue;
    }

    /**
     * @brief Check if the backing array was successfully allocated.
     * @return true if the queue is usable, false if allocation failed.
     */
    bool isValid() const {
        return queue != nullptr;
    }

    /**
     * @brief Add an element to the back of the queue.
     * @param value The value to add.
     */
    void enqueue(const T &value){
        if(!queue) return;
        if(rear == size - 1){
            if(elements == size){
                // Backing array is truly full — expand and compact.
                resize();
                if(elements == size){
#ifndef Q_NO_SERIAL
                    Serial.println("Queue: enqueue failed (out of memory).");
#endif
                    return;
                }
            } else {
                // There is room at the front; compact in-place to avoid allocation.
                for(int i = 0; i < elements; i++){
                    queue[i] = queue[front + i];
                }
                front = 0;
                rear = elements - 1;
            }
        }
        queue[++rear] = value;
        elements++;
    }

    /**
     * @brief Remove and return the element at the front of the queue.
     * @return The front element, or a default-constructed T if the queue is empty.
     */
    T dequeue(){
        if(isEmpty()){
            return T();
        }
        T val = queue[front]; // copy before advancing state
        front++;
        elements--;
        return val;
    }

    /**
     * @brief Return the element at the front without removing it.
     * @return The front element, or a default-constructed T if the queue is empty.
     */
    T peek() const {
        if(isEmpty()){
            return T();
        }
        return queue[front];
    }

    /**
     * @brief Check if the queue is empty.
     * @return true if the queue has no elements, false otherwise.
     */
    bool isEmpty() const {
        return elements == 0 || !queue;
    }

    /**
     * @brief Check if the queue's backing array is at capacity.
     *        The queue will auto-resize on the next enqueue.
     * @return true if the backing array is full.
     */
    bool isFull() const {
        return elements == size;
    }

    /**
     * @brief Print all elements in the queue (front to back).
     */
    void print() const {
        if(!queue) return;
        for(int i = 0; i < elements; i++){
#ifndef Q_NO_SERIAL
            Serial.println(queue[front + i]);
#endif
        }
    }

    /**
     * @brief Remove all elements from the queue.
     */
    void clear(){
        front = 0;
        rear = -1;
        elements = 0;
    }

    /**
     * @brief Return the number of elements in the queue.
     */
    int count() const {
        return elements;
    }

#ifdef Q_ENABLE_NUMERIC_LIMITS
    /**
     * @brief Returns the memory currently used by the backing array (in bytes).
     * @details Includes only the element storage array, not object overhead.
     * @return Bytes consumed by the backing array.
     */
    int memoryUsage() const {
        return size * static_cast<int>(sizeof(T));
    }

    /**
     * @brief Returns the theoretical maximum number of elements this Queue
     *        could hold, limited by the maximum value of int on this platform.
     * @return The upper bound on element count as reported by numeric_limits.
     */
    int theoreticalMaxElements() const {
        return numeric_limits<int>::Max();
    }

    /**
     * @brief Returns the ratio of elements stored to the theoretical maximum
     *        element count, expressed as a float in the range [0.0, 1.0].
     * @return Memory utilization fraction (current elements / theoretical max).
     */
    float memoryUtilization() const {
        const int maxElems = theoreticalMaxElements();
        if (maxElems <= 0) return 0.0f;
        return static_cast<float>(elements) / static_cast<float>(maxElems);
    }
#endif // Q_ENABLE_NUMERIC_LIMITS
};
#endif // QUEUE_h