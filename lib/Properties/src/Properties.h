#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Hashtable.h"
#include <Arduino.h>

class Properties {
private:
        bool debug;
        Hashtable<String, String> table;

    public:
        Properties(bool debug = false); // Declaration of the constructor
        ~Properties();

        void setProperty(const String& key, const String& value);
        void setProperty(const String& key, const String& value, const String& filePath);
        String getProperty(const String& key);
        String getProperty(const String& key, const String& defaultValue, const String& filePath);
        void removeProperty(const String& key);
        void clear();
        int size();
        int elements();
        bool isEmpty();
        void saveToSD(const String& filename);
        void loadFromSD(const String& filename);
        bool containsKey(const String& key);
        bool deleteFile(const String& filename);
        void setDebug(bool debug);
        bool getDebug();
    
    class PropertiesIterator {
private:
    Hashtable<String, String>::Iterator it;
    bool debug;
public:
    // Constructor
    PropertiesIterator(Hashtable<String, String>::Iterator begin, bool debug = false)
        : it(begin), debug(debug) {}

    // Prefix increment
    PropertiesIterator& operator++() {
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Prefix increment");
        }
        ++it; // Move to the next element
        return *this;
    }

    // Postfix increment
    PropertiesIterator operator++(int) {
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Postfix increment");
        }
        PropertiesIterator tmp(*this);
        ++(*this); // Use the prefix increment
        return tmp;
    }

    // Inequality check
    bool operator!=(const PropertiesIterator& other) const {
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Inequality check");
        }
        return it != other.it; // Directly compare the Hashtable iterators
    }

    // Dereference operator
    Hashtable<String, String>::KeyValuePair operator*() const {
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Dereference operator (Delegating to Hashtable Library Iterator (*it))");
        }
        return *it; // Delegate to the Hashtable iterator
    }

    // Key accessor
    String key() const { 
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Key accessor");
        }
        return it.operator*().key; // Access the key of the current KeyValuePair
    }

    // Value accessor
    String value() const { 
        if(debug){
            Serial.println("[SIMPLE PROPERTIES]: Iterator Value accessor");
        }
        return it.operator*().value; // Access the value of the current KeyValuePair
    }
};

// Iterator begin
PropertiesIterator begin() {
    if(debug){
        Serial.println("[SIMPLE PROPERTIES]: Iterator begin");
    }
    return PropertiesIterator(table.begin());
}

// Iterator end
PropertiesIterator end() {
    if(debug){
        Serial.println("[SIMPLE PROPERTIES]: Iterator end");
    }
    return PropertiesIterator(table.end());
}
};

#endif // PROPERTIES_H
