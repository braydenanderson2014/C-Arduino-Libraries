#ifndef SDLIST_H
#define SDLIST_H

#include <Arduino.h>
#include <SD.h>

template <typename T>
class SDList {
public:
   SDList(uint8_t csPin, const String& pageFileName);
    ~SDList();

    void append(const T& value);
    T get(uint16_t index) const;
    void set(uint16_t index, const T& value);
    uint16_t size() const;
    bool sdAvailable() const;
    void loadFromSD();
    void writeToSD();

private:
    T* data;
    uint16_t capacity;
    uint16_t length;
    uint8_t csPin;
    String pageFileName;
    bool useSD;

    bool initializePageFile();
    bool checkSD() const;
    bool expandCapacity();
};

#endif // SDLIST_H
