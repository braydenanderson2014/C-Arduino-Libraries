#include <SD.h>
#include <Arduino.h>

template <typename T>
class SDList {
public:
    SDList(uint8_t csPin, const String& pageFileName);
    ~SDList();

    void append(const T& value);
    T get(uint16_t index) const;
    void set(uint16_t index, const T& value);
    uint16_t size() const;

private:
    uint8_t csPin;
    String pageFileName;
    uint16_t length;

    bool initializePageFile();
    bool checkSD() const;
    T loadElementFromSD(uint16_t index) const;
    void writeElementToSD(uint16_t index, const T& value);
};

template <typename T>
SDList<T>::SDList(uint8_t csPin, const String& pageFileName)
    : csPin(csPin), pageFileName(pageFileName), length(0) {
    Serial.println("[SD LIST]: Initializing SDList");
    if (!initializePageFile()) {
        Serial.println("[SD LIST]: SD Card not available or failed to initialize.");
    } else {
        Serial.println("[SD LIST]: SDList initialized in SD_CARD mode.");
    }
}

template <typename T>
SDList<T>::~SDList() {
    // No explicit action required for destructor
}

template <typename T>
void SDList<T>::append(const T& value) {
    writeElementToSD(length, value);
    length++;
    Serial.println("[SD LIST]: Element appended");
}

template <typename T>
T SDList<T>::get(uint16_t index) const {
    if (index >= length) {
        Serial.println("[SD LIST]: Error - Index out of bounds");
        return T(); // Return default-constructed object
    }
    return loadElementFromSD(index);
}

template <typename T>
void SDList<T>::set(uint16_t index, const T& value) {
    if (index >= length) {
        Serial.println("[SD LIST]: Error - Index out of bounds");
        return;
    }
    writeElementToSD(index, value);
    Serial.println("[SD LIST]: Element set at index " + String(index));
}

template <typename T>
uint16_t SDList<T>::size() const {
    return length;
}

template <typename T>
bool SDList<T>::initializePageFile() {
    if (!checkSD()) {
        Serial.println("[SD LIST]: Error - SD Card Not Detected");
        return false;
    }

    if (!SD.exists(pageFileName.c_str())) {
        File file = SD.open(pageFileName.c_str(), FILE_WRITE);
        if (file) {
            file.close();
            Serial.println("[SD LIST]: New Page File Created");
        } else {
            Serial.println("[SD LIST]: Error - Failed to Create Page File");
            return false;
        }
    } else {
        Serial.println("[SD LIST]: Page File Exists");
    }
    return true;
}

template <typename T>
bool SDList<T>::checkSD() const {
    return SD.begin(csPin);
}

template <typename T>
T SDList<T>::loadElementFromSD(uint16_t index) const {
    File file = SD.open(pageFileName.c_str(), FILE_READ);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for reading");
        return T();
    }

    file.seek(index * sizeof(T));
    T element;
    file.read(reinterpret_cast<byte*>(&element), sizeof(T));
    file.close();
    return element;
}

template <typename T>
void SDList<T>::writeElementToSD(uint16_t index, const T& value) {
    File file = SD.open(pageFileName.c_str(), FILE_WRITE);
    if (!file) {
        Serial.println("[SD LIST]: Error - Failed to open file for writing");
        return;
    }

    file.seek(index * sizeof(T));
    file.write(reinterpret_cast<const byte*>(&value), sizeof(T));
    file.close();
}

// Example explicit instantiation
// template class SDList<int>;
