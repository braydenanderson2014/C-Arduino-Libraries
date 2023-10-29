#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

template<typename T>
class SimpleVector {
private:
    T* array;
    unsigned int count;
    unsigned int capacity;

    void resize(unsigned int newCapacity) {
        T* newArray = new T[newCapacity];
        for (unsigned int i = 0; i < count; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    SimpleVector() : count(0), capacity(4) {
        array = new T[capacity];
    }

    ~SimpleVector() {
        delete[] array;
    }

    void push_back(const T& item) {
        if (count == capacity) {
            resize(2 * capacity);
        }
        array[count++] = item;
    }

    T& operator[](unsigned int index) {
        if (index >= count) {
            exit(1);
        }
        return array[index];
    }

    unsigned int size() const {
        return count;
    }

    T* begin() {
        return &array[0];
    }

    T* end() {
        return &array[count];
    }

    void releaseMemory() {
        delete[] array;
        array = nullptr;
        capacity = 0;
        size = 0;
    }

    class SimpleVectorIterator {
        private:
            T* current;
            T* end;
        
        public:
            SimpleVectorIterator(T* start, T* stop) : current(start), end(stop) {}
        
            bool hasNext() {
                return current != end;
            }
        
            T& next() {
                return *current++;
            }
        };
};
#endif // SIMPLEVECTOR_H