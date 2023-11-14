#include "SDList.h"

template <typename T>
SDList<T>::SDList(uint8_t csPin, const String& pageFileName)
    : data(nullptr), capacity(0), length(0), csPin(csPin), pageFileName(pageFileName), useSD(false) {
    Serial.println("[SD LIST]: Initializing SDList Library");

    if (initializePageFile()) {
        useSD = true;
        loadFromSD();
    } else {
        useSD = false;
        delay(50);
        Serial.println("[SD LIST]: SD Card not used. Switching to in-memory operation.");
        capacity = 10; // default initial capacity
        data = new T[capacity];
        Serial.println("[SD LIST]: SDList Library Initialized. [In-memory operation] [Default capacity: 10]");
    }
}

template <typename T>
void SDList<T>::loadFromSD() {
    // Assuming initializePageFile has been called and useSD is true
    File file = SD.open(pageFileName.c_str(), FILE_READ);
    if (file && file.size() > 0) {
        // Read existing data from SD
        length = file.size() / sizeof(T);
        capacity = length;
        data = new T[capacity];
        file.read(reinterpret_cast<byte*>(data), length * sizeof(T));
    } else {
        // Initialize with default capacity
        capacity = 10;
        data = new T[capacity];
    }
    file.close();
}

template <typename T>
void SDList<T>::writeToSD() {
    // Save the entire list to SD card
    if (useSD) {
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            file.seek(0); // Go to the beginning of the file
            file.write(reinterpret_cast<byte*>(data), length * sizeof(T));
            file.close();
        }
    }
}

template <typename T>
SDList<T>::~SDList() {
    writeToSD(); // Save data to SD card on destruction
    delete[] data; // Clean up memory
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
    if (length >= capacity) {
        if (!expandCapacity()) {
            Serial.println("[SD LIST]: Unable to expand capacity");
            Serial.println("[SD LIST]: Failed to add Data to List");
            return;
        }
    }

    // Check if SD card usage is intended and available before attempting to write
    if (useSD && checkSD()) {
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            file.seek(length * sizeof(T)); // Move to the end of the file
            if(file.write(reinterpret_cast<const byte*>(&value), sizeof(T)) == sizeof(T)) {
                Serial.println("[SD LIST]: Data Appended to SD File");
            } else {
                Serial.println("[SD LIST]: Failed to append data to SD File");
            }
            file.close();
        } else {
            Serial.println("[SD LIST]: Failed to open SD File for appending");
        }
        length++; // Increase length only if data is written
    } else {
        data[length++] = value; // In-memory operation
        Serial.println("[SD LIST]: Data Appended to In-Memory List");
    }
}


template <typename T>
T SDList<T>::get(uint16_t index) const {
    if (useSD && checkSD()) {
        File file = SD.open(pageFileName.c_str(), FILE_READ);
        T value;
        if (file) {
            file.seek(index * sizeof(T));
            if (file.read(reinterpret_cast<char*>(&value), sizeof(T)) == sizeof(T)) {
                file.close();
                return value;
            }
            file.close();
        }
        return T(); // Return default-constructed object if read failed
    } else {
        // In-memory operation as before
        if (index < length) {
            return data[index];
        }
        return T();
    }
}

template <typename T>
void SDList<T>::set(uint16_t index, const T& value) {
    Serial.println("[SD LIST]: Setting Data in List");
    if (index < length) {
        if (useSD && checkSD()) {
            File file = SD.open(pageFileName.c_str(), FILE_WRITE);
            if (file) {
                file.seek(index * sizeof(T));
                file.write(reinterpret_cast<const byte*>(&value), sizeof(T));
                file.close();
            }
        } else {
            data[index] = value;
        }
    } else {
        Serial.println("[SD LIST]: Unable to insert data, index out of bounds");
        // Handle error case
    }
}


template <typename T>
uint16_t SDList<T>::size() const {
    Serial.println("[SD LIST]: Getting Size of List");
    return length;
}

template <typename T>
bool SDList<T>::sdAvailable() const {
    Serial.println("[SD LIST]: Checking if SD Card is Available");
    Serial.println("[SD LIST]: " + String(checkSD()));
    return checkSD();
}

template <typename T>
bool SDList<T>::expandCapacity() {
    Serial.println("[SD LIST]: Expanding Capacity of List");
    uint16_t newCapacity = capacity * 2;

    if (useSD && checkSD()) {
        // Create a new file with increased capacity
        String newFileName = pageFileName + ".new";
        File oldFile = SD.open(pageFileName.c_str(), FILE_READ);
        File newFile = SD.open(newFileName.c_str(), FILE_WRITE);

        if (oldFile && newFile) {
            byte buffer[512]; // Buffer to hold data chunks
            while (oldFile.available()) {
                int bytesRead = oldFile.read(buffer, sizeof(buffer));
                newFile.write(buffer, bytesRead);
            }
            oldFile.close();
            newFile.close();

            // Remove the old file and check if the new file can be opened to confirm it was created successfully
            if (SD.remove(pageFileName.c_str())) {
                // Check if the new file exists to confirm the data is safe
                if (SD.exists(newFileName.c_str())) {
                    // At this point, you can choose to keep the new file with the new name
                    // or inform the user to manually rename it to the original file name if needed.
                    Serial.println("[SD LIST]: Old file removed, new file needs to be renamed manually.");
                    capacity = newCapacity;
                    return true;
                } else {
                    Serial.println("[SD LIST]: New file does not exist after old file removal.");
                    return false;
                }
            } else {
                Serial.println("[SD LIST]: Failed to remove old file on SD card.");
                return false;
            }
        } else {
            if (oldFile) oldFile.close();
            if (newFile) newFile.close();
            Serial.println("[SD LIST]: Failed to create expanded file on SD card.");
            return false;
        }
    } else {
        T* newData = new T[newCapacity];
        if (newData) {
            memcpy(newData, data, length * sizeof(T));
            delete[] data;
            data = newData;
            capacity = newCapacity;
            return true;
        }
    }

    Serial.println("[SD LIST]: Unable to expand capacity.");
    return false;
}

template <typename T>
bool SDList<T>::checkSD() const {
    Serial.print("[SD LIST]: Initializing SD Card on CS pin: ");
    Serial.println(csPin);
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
template class SDList<size_t>;
// Add more if necessary
