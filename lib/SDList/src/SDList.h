#ifndef SDLIST_H
#define SDLIST_H

#include <Arduino.h>
#include <SD.h>

template <typename T>
class SDList {
public:
    enum StorageMode { MEMORY, SD_CARD };

    SDList(uint8_t csPin, const String& pageFileName, StorageMode mode = SD_CARD);
    ~SDList();

    void append(const T& value);
    T get(uint16_t index) const;
    void set(uint16_t index, const T& value);
    uint16_t size() const;
    bool sdAvailable() const;

private:
    T* data;
    uint16_t capacity;
    uint16_t length;
    uint8_t csPin;
    String pageFileName;
    StorageMode storageMode;

    bool initializePageFile();
    bool checkSD() const;
    bool expandCapacity();
    void loadFromSD();
    void writeToSD();
    T loadElementFromSD(uint16_t index) const;
    void writeElementToSD(uint16_t index, const T& value);
};


#endif // SDLIST_H
