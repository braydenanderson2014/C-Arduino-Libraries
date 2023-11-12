#ifndef MY_DICTIONARY_H
#define MY_DICTIONARY_H

#include <Arduino.h>
#include "Dictionary.h"
#include "UnorderedMap.h"

class MyDictionary : public Dictionary<String, String> {
private:
    UnorderedMap<String, String> data;

public:
    int size() const override {
        return data.getSize();
    }

    bool isEmpty() const override {
        return data.isEmpty();
    }

    String get(const String& key) const override {
        String value;
        if (data.get(key)) {
            return value;
        } else {
            Serial.println("[Dictionary]: Unable to get(), Key not found");
            return "null"; // or some other error indicator
        }
    }

    void put(const String& key, const String& value) override {
        data.put(key, value);
    }

    String remove(const String& key) override {
        if (!data.remove(key)) {
            Serial.println("Dictionary]: Unable to remove(), Key not found");
            return "null"; // or some other error indicator
        }
        return "success"; // or some other success indicator
    }
};

#endif // MY_DICTIONARY_H
