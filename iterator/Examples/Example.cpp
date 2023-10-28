#include <Arduino.h>
#include <Iterator.h>

template <typename T>
class SampleIterator : public Iterator<T> {
private:
    T* ptr;

public:
    SampleIterator(T* p) : ptr(p) {}

    T& operator*() override {
        return *ptr;
    }

    SampleIterator& operator++() override {
        ++ptr;
        return *this;
    }

    bool operator!=(const Iterator<T>& other) const override {
        const SampleIterator& rhs = static_cast<const SampleIterator&>(other);
        return ptr != rhs.ptr;
    }
};

int main() {
    int array[] = {1, 2, 3, 4, 5};
    
    // Create an iterator for the array
    SampleIterator<int> it(array);
    
    // Iterate through the array
    while (it != SampleIterator<int>(&array[5])) {
        int value = *it;
        Serial.println(value);
        ++it;
    }

    return 0;
}
