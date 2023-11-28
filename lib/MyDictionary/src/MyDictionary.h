#ifndef MY_DICTIONARY_H
#define MY_DICTIONARY_H

#include <Arduino.h>
#include "Dictionary.h"
#include "UnorderedMap.h"

class MyDictionary : public Dictionary<String, String> {
private:
    bool debug;
    UnorderedMap<String, String> data = UnorderedMap<String, String>(debug);

public:
    int size() const override {
        if(debug){
            Serial.println("[Dictionary]: Size: " + String(data.getSize()));
        }
        return data.getSize();
    }

    bool isEmpty() const override {
        if(debug){
            Serial.println("[Dictionary]: Is Empty: " + String(data.isEmpty()));
        }
        return data.isEmpty();
    }

    String get(const String& key) const override {
        String value;
        if (data.get(key)) {
            if(debug){
                Serial.println("[Dictionary]: Key: " + key + " Value: " + data.get(key));
            }
            return value;
        } else {
            if(debug){
                Serial.println("[Dictionary]: Unable to get(), Key not found");
            }
            return "null"; // or some other error indicator
        }
    }

    void put(const String& key, const String& value) override {
        if(debug){
            Serial.println("[Dictionary]: Key: " + key + " Value: " + value);
        }
        data.put(key, value);
    }

    String remove(const String& key) override {
        if (!data.remove(key)) {
            if(debug){
                Serial.println("[Dictionary]: Unable to remove(), Key not found");
            }
            return "null"; // or some other error indicator
        }
        if(debug){
            Serial.println("[Dictionary]: Key: " + key + " removed");
        }
        return "success"; // or some other success indicator
    }

    void clear()  {
        data.clear();
        if(debug){
            Serial.println("[Dictionary]: Dictionary cleared");
        }
    }
};

#endif // MY_DICTIONARY_H
