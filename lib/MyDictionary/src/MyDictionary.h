#ifndef MY_DICTIONARY_H
#define MY_DICTIONARY_H

#include <Arduino.h>
#include "Dictionary.h"
#include "UnorderedMap.h"

class MyDictionary : public Dictionary<String, int> {
private:
    UnorderedMap<String, int> data;

public:
    int size() const override {
        return data.getSize();
    }

    bool isEmpty() const override {
        return data.isEmpty();
    }

    int get(const String& key) const override {
        int value;
        if (data.get(key, value)) {
            return value;
        } else {
            Serial.println("Key not found");
            return -1; // or some other error indicator
        }
    }

    int put(const String& key, const int& value) override {
        data.insert(key, value);
        return 1; // or some other success indicator
    }

    int remove(const String& key) override {
        if (!data.remove(key)) {
            Serial.println("Key not found");
            return 0; // or some other error indicator
        }
        return 1; // or some other success indicator
    }
};

#endif // MY_DICTIONARY_H
