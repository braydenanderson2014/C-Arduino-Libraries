#include "SDList.h"

template <typename T>
SDList<T>::SDList(uint8_t csPin, const String& pageFileName, StorageMode mode)
    : data(nullptr), capacity(0), length(0), csPin(csPin), pageFileName(pageFileName), storageMode(mode) {
    
    Serial.println("[SD LIST]: Initializing SDList");

    if (storageMode == SD_CARD) {
        if (!initializePageFile()) {
            Serial.println("[SD LIST]: SD Card not available or failed to initialize. Switching to in-memory operation.");
            storageMode = MEMORY; // Fallback to memory if SD card initialization fails
        }
    }

    if (storageMode == MEMORY) {
        capacity = 10; // default initial capacity for in-memory operation
        data = new T[capacity];
        Serial.println("[SD LIST]: SDList initialized in MEMORY mode with default capacity.");
    } else {
        loadFromSD(); // Load existing data from SD card
        Serial.println("[SD LIST]: SDList initialized in SD_CARD mode.");
    }
}


template <typename T>
void SDList<T>::loadFromSD() {
    Serial.println("[SD LIST]: Loading data from SD");

    File file = SD.open(pageFileName.c_str(), FILE_READ);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for reading");
        return;
    }

    // Calculate the number of elements
    length = file.size() / sizeof(T);
    capacity = length;

    // Allocate memory for data
    delete[] data; // Free existing data
    data = new T[capacity];
    if (!data) {
        Serial.println("[SD LIST]: Error - Memory allocation failed for loading");
        file.close();
        return;
    }

    // Read the data
    file.read(reinterpret_cast<byte*>(data), length * sizeof(T));
    file.close();
    Serial.println("[SD LIST]: Data loaded from SD");
}


template <typename T>
void SDList<T>::writeToSD() {
    Serial.println("[SD LIST]: Writing data to SD");

    File file = SD.open(pageFileName.c_str(), FILE_WRITE);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for writing");
        return;
    }

    file.seek(0); // Go to the beginning of the file
    file.write(reinterpret_cast<const byte*>(data), length * sizeof(T));
    file.close();
    Serial.println("[SD LIST]: Data written to SD");
}

template <typename T>
T SDList<T>::loadElementFromSD(uint16_t index) const {
    Serial.println("[SD LIST]: Loading element from SD at index " + String(index));

    File file = SD.open(pageFileName.c_str(), FILE_READ);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for reading");
        return T(); // Return default-constructed object
    }

    file.seek(index * sizeof(T)); // Move the file pointer to the correct position

    T element;
    if (file.read(reinterpret_cast<byte*>(&element), sizeof(T)) != sizeof(T)) {
        Serial.println("[SD LIST]: Error - Failed to read element");
        file.close();
        return T(); // Return default-constructed object
    }

    file.close();
    return element;
}

template <typename T>
void SDList<T>::writeElementToSD(uint16_t index, const T& value) {
    Serial.println("[SD LIST]: Writing element to SD at index " + String(index));

    File file = SD.open(pageFileName.c_str(), FILE_WRITE);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for writing");
        return;
    }

    file.seek(index * sizeof(T)); // Move the file pointer to the correct position

    if (file.write(reinterpret_cast<const byte*>(&value), sizeof(T)) != sizeof(T)) {
        Serial.println("[SD LIST]: Error - Failed to write element");
    } else {
        Serial.println("[SD LIST]: Element written to SD successfully");
    }

    file.close();
}

template <typename T>
SDList<T>::~SDList() {
    writeToSD(); // Save data to SD card on destruction
    delete[] data; // Clean up memory
}

template <typename T>
bool SDList<T>::initializePageFile() {
    Serial.println("[SD LIST]: Initializing Page File");

    if (!checkSD()) {
        Serial.println("[SD LIST]: Error - SD Card Not Detected");
        return false;
    }

    if (SD.exists(pageFileName.c_str())) {
        Serial.println("[SD LIST]: Page File Exists");
        return true;
    } else {
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            file.close();
            Serial.println("[SD LIST]: New Page File Created");
            return true;
        } else {
            Serial.println("[SD LIST]: Error - Failed to Create Page File");
            return false;
        }
    }
}


template <typename T>
void SDList<T>::append(const T& value) {
    if (length >= capacity) {
        if (!expandCapacity()) {
            Serial.println("[SD LIST]: Error - Unable to expand capacity");
            return;
        }
    }

    if (storageMode == SD_CARD && checkSD()) {
        writeElementToSD(length, value);
    } else {
        data[length] = value;
    }
    length++;
    Serial.println("[SD LIST]: Element appended");
}



template <typename T>
T SDList<T>::get(uint16_t index) const {
    if (index >= length) {
        Serial.println("[SD LIST]: Error - Index out of bounds");
        return T(); // Return default-constructed object
    }

    if (storageMode == SD_CARD && checkSD()) {
        return loadElementFromSD(index);
    } else {
        return data[index];
    }
}


template <typename T>
void SDList<T>::set(uint16_t index, const T& value) {
    if (index >= length) {
        Serial.println("[SD LIST]: Error - Index out of bounds");
        return;
    }

    if (storageMode == SD_CARD && checkSD()) {
        writeElementToSD(index, value);
    } else {
        data[index] = value;
    }
    Serial.println("[SD LIST]: Element set at index " + String(index));
}



template <typename T>
uint16_t SDList<T>::size() const {
    Serial.println("[SD LIST]: Getting size of list");
    return length;
}


template <typename T>
bool SDList<T>::sdAvailable() const {
    bool isAvailable = checkSD();
    Serial.println("[SD LIST]: SD Card is " + String(isAvailable ? "available" : "not available"));
    return isAvailable;
}


template <typename T>
bool SDList<T>::expandCapacity() {
    Serial.println("[SD LIST]: Expanding Capacity");
    uint16_t newCapacity = capacity * 2;

    if (storageMode == SD_CARD) {
        // Handle SD card capacity expansion (you may need a different approach here)
        Serial.println("[SD LIST]: Currently, SD Card expansion is not supported");
        return false;
    } else {
        T* newData = new T[newCapacity];
        if (!newData) {
            Serial.println("[SD LIST]: Error - Memory allocation failed");
            return false;
        }
        memcpy(newData, data, length * sizeof(T));
        delete[] data;
        data = newData;
        capacity = newCapacity;
        Serial.println("[SD LIST]: In-memory capacity expanded");
        return true;
    }
}


template <typename T>
bool SDList<T>::checkSD() const {
    Serial.println("[SD LIST]: Checking SD Card");
    if (!SD.begin(csPin)) {
        Serial.println("[SD LIST]: SD Card initialization failed!");
        return false;
    }
    Serial.println("[SD LIST]: SD Card initialized successfully");
    return true;
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
// Add more if necessary
