#include <SDList.h>

template <typename T>
SDList<T>::SDList(uint8_t csPin, const String& pageFileName)
    : data(nullptr), capacity(0), length(0), csPin(csPin), pageFileName(pageFileName) {
    Serial.flush();
    Serial.println("[SD LIST]: Initializing SDList Library");

    if (initializePageFile()) {
        // Now that the page file is ensured to exist, proceed with the rest of the initialization
        File file = SD.open(pageFileName.c_str(), FILE_READ);
        if (file && file.size() > 0) {
            // File exists and is not empty, load existing data from SD
            length = file.size() / sizeof(T);
            data = new T[length];
            file.read(data, length * sizeof(T));
            file.close();
        } else {
            // File does not exist or is empty, initialize with default capacity
            capacity = 10; // or any other initial capacity you deem appropriate
            data = new T[capacity];
        }
    } else {
        // Handle the error if the page file could not be initialized
        Serial.println("[SD LIST]: Unable to Initialize Page File");
    }
}

template <typename T>
SDList<T>::~SDList() {
    Serial.println("[SD LIST]: Deconstructing SDList Library");
    Serial.println("[SD LIST]: Saving Data to SD Card");
    Serial.println("[SD LIST]: Checking if SD Card is Available");
    if (checkSD()) {
        Serial.println("[SD LIST]: SD Card Detected");
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        Serial.println("[SD LIST]: Opening Page File");
        if (file) {
            Serial.println("[SD LIST]: Page File Opened");
            Serial.println("[SD LIST]: Writing Data to Page File");
            file.write(reinterpret_cast<byte*>(data), length * sizeof(T));
            Serial.println("[SD LIST]: Closing Page File");
            file.close();
        }
        Serial.println("[SD LIST]: Page File Closed");
    }
    Serial.println("[SD LIST]: Deleting Data from memory");
    delete[] data;
}

template <typename T>
bool SDList<T>::initializePageFile() {
    Serial.println("[SD LIST]: Initializing Page File");

    // Check if the SD card is available
    if (!checkSD()) {
        Serial.println("[SD LIST]: SD Card Not Detected");
        return false;
    }

    // Check if the page file exists
    if (SD.exists(pageFileName.c_str())) {
        Serial.println("[SD LIST]: Page File Exists");
        return true;
    } else {
        // Create a new page file
        Serial.println("[SD LIST]: Creating New Page File");
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            Serial.println("[SD LIST]: New Page File Created");
            file.close();
            return true;
        } else {
            Serial.println("[SD LIST]: Failed to Create Page File");
            return false;
        }
    }
}

template <typename T>
void SDList<T>::append(const T& value) {
    Serial.println("[SD LIST]: Appending Data to List");
    if (length == capacity) {
        if (!expandCapacity()) {
            Serial.println("[SD LIST]: Unable to expand capacity");
            Serial.println("[SD LIST]: Failed to add Data to List");
            // Handle the error (e.g., by not appending the item)
            return;
        }
    }
    Serial.println("[SD LIST]: Adding Data to List");
    data[length++] = value;
}

template <typename T>
T SDList<T>::get(uint16_t index) const {
    Serial.println("[SD LIST]: Getting Data from List");
    if (index < length) {
        Serial.println("[SD LIST]: Data Found");
        return data[index];
    }
    Serial.println("[SD LIST]: Data Not Found, returning default value");
    // Handle error case, for simplicity we'll return a default constructed T
    return T();
}

template <typename T>
void SDList<T>::set(uint16_t index, const T& value) { //Specialized for certain index
    Serial.println("[SD LIST]: Setting Data in List");
    if (index < length) {
        Serial.println("[SD LIST]: Inserting Data");
        data[index] = value;
    }
    Serial.println("[SD LIST]: Unable to insert data, index out of bounds");
    // Handle error case
}

template <typename T>
uint16_t SDList<T>::size() const {
    Serial.println("[SD LIST]: Getting Size of List");
    return length;
}

template <typename T>
bool SDList<T>::sdAvailable() const {
    Serial.println("[SD LIST]: Checking if SD Card is Available");
    return checkSD();
}

template <typename T>
bool SDList<T>::expandCapacity() {
    Serial.println("[SD LIST]: Expanding Capacity of List");
    uint16_t newCapacity = capacity * 2;
    T* newData = new T[newCapacity];
    if (newData) {
        for (uint16_t i = 0; i < length; ++i) {
            Serial.println("[SD LIST]: Copying Data to New Expanded List");
            newData[i] = data[i];
        }
        Serial.println("[SD LIST]: Deleting Old List");
        delete[] data;
        data = newData;
        capacity = newCapacity;
        return true;
    }
    Serial.println("[SD LIST]: Unable to expand capacity");
    return false;  // Failed to expand capacity
}

template <typename T>
bool SDList<T>::checkSD() const {
    Serial.println("[SD LIST]: Checking if SD Card is Available");
    Serial.println("[SD LIST]: " + String(SD.begin(csPin)));
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
