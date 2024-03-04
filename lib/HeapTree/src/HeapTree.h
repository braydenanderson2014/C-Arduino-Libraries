#ifndef HEAP_TREE_h
#define HEAP_TREE_h

#include <Arduino.h>
template <typename T>
class HeapTree {
public:
    HeapTree() : size(0), capacity(2) {
        arr = new T[capacity];
    }

    ~HeapTree() {
        delete[] arr;
    }

    void insert(T value) {
        if (size == capacity) {
            resize();
        }
        arr[size] = value;
        siftUp(size);
        size++;
    }

    T extractMax() {
        T result = arr[0];
        arr[0] = arr[size - 1];
        size--;
        siftDown(0);
        return result;
    }

    T extractMin() {
        T result = arr[size - 1];
        arr[size - 1] = arr[0];
        size--;
        siftDown(0);
        return result;
    }

    bool empty() {
        return size == 0;
    }

    void clear() {
        size = 0;
    }

    int getSize() {
        return size;
    }

    void remove(int i, T val) {
        arr[i] -= val;
        if (arr[i] > arr[parent(i)]) {
            siftUp(i);
        } else {
            siftDown(i);
        }
    }

    void remove(int i) {
        arr[i] = arr[0] + 1;
        siftUp(i);
        extractMax();
    }

    void removeItem(T value) {
        for (int i = 0; i < size; i++) {
            if (arr[i] == value) {
                remove(i);
                break;
            }
        }
    }

    HeapTree<T> getTree() {
        return *this;
    }

    T* getArray() {
        return arr;
    }

    void print() {
        for (int i = 0; i < size; i++) {
            Serial.print(arr[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
private:
    T* arr;
    int size;
    int capacity;

    void siftUp(int i) {
        while (i > 0 && arr[parent(i)] < arr[i]) {
            swap(arr[parent(i)], arr[i]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int maxIndex = i;
        int l = leftChild(i);
        if (l < size && arr[l] > arr[maxIndex]) {
            maxIndex = l;
        }
        int r = rightChild(i);
        if (r < size && arr[r] > arr[maxIndex]) {
            maxIndex = r;
        }
        if (i != maxIndex) {
            swap(arr[i], arr[maxIndex]);
            siftDown(maxIndex);
        }
    }

    void resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = arr[i];
        }
        delete[] arr;
        arr = temp;
    }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
};

#endif // HEAP_TREE_h
