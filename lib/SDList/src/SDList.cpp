#include <SDList.h>

template <typename T>
SDList<T>::SDList(uint8_t csPin, const String& pageFileName)
    : data(nullptr), capacity(0), length(0), csPin(csPin), pageFileName(pageFileName) {
    if (checkSD()) {
        // Attempt to load existing data from SD
        File file = SD.open(pageFileName.c_str(), FILE_READ);
        if (file) {
            length = file.size() / sizeof(T);
            data = new T[length];
            file.read(data, length * sizeof(T));
            file.close();
        }
    } else {
        data = new T[10];  // start with a small capacity
        capacity = 10;
    }
}

template <typename T>
SDList<T>::~SDList() {
    if (checkSD()) {
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            file.write(reinterpret_cast<byte*>(data), length * sizeof(T));
            file.close();
        }
    }
    delete[] data;
}

template <typename T>
void SDList<T>::append(const T& value) {
    if (length == capacity) {
        if (!expandCapacity()) {
            // Handle the error (e.g., by not appending the item)
            return;
        }
    }
    data[length++] = value;
}

template <typename T>
T SDList<T>::get(uint16_t index) const {
    if (index < length) {
        return data[index];
    }
    // Handle error case, for simplicity we'll return a default constructed T
    return T();
}

template <typename T>
void SDList<T>::set(uint16_t index, const T& value) {
    if (index < length) {
        data[index] = value;
    }
    // Handle error case
}

template <typename T>
uint16_t SDList<T>::size() const {
    return length;
}

template <typename T>
bool SDList<T>::sdAvailable() const {
    return checkSD();
}

template <typename T>
bool SDList<T>::expandCapacity() {
    uint16_t newCapacity = capacity * 2;
    T* newData = new T[newCapacity];
    if (newData) {
        for (uint16_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
        return true;
    }
    return false;  // Failed to expand capacity
}

template <typename T>
bool SDList<T>::checkSD() const {
    return SD.begin(csPin);
}

// Explicitly instantiate the template with some expected types
template class SDList<int>;
template class SDList<float>;
template class SDList<String>;
template class SDList<char>;
template class SDList<bool>;
template class SDList<uint8_t>;
template class SDList<uint16_t>;
template class SDList<uint32_t>;
template class SDList<uint64_t>;
template class SDList<int8_t>;
template class SDList<int16_t>;
template class SDList<int32_t>;
template class SDList<int64_t>;
template class SDList<size_t>;
// Add more if necessary
