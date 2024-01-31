#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Hashtable.h"
#include <Arduino.h>

class Properties {
private:
        bool debug;
        Hashtable<String, String> table;

    public:
        Properties(); // Declaration of the constructor
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
    
    class PropertiesIterator {
private:
    Hashtable<String, String>::Iterator it;
public:
    // Constructor
    PropertiesIterator(Hashtable<String, String>::Iterator begin)
        : it(begin) {}

    // Prefix increment
    PropertiesIterator& operator++() {
        ++it; // Move to the next element
        return *this;
    }

    // Postfix increment
    PropertiesIterator operator++(int) {
        PropertiesIterator tmp(*this);
        ++(*this); // Use the prefix increment
        return tmp;
    }

    // Inequality check
    bool operator!=(const PropertiesIterator& other) const {
        return it != other.it; // Directly compare the Hashtable iterators
    }

    // Dereference operator
    Hashtable<String, String>::KeyValuePair operator*() const {
        return *it; // Delegate to the Hashtable iterator
    }

    // Key accessor
    String key() const { 
        return it.operator*().key; // Access the key of the current KeyValuePair
    }

    // Value accessor
    String value() const { 
        return it.operator*().value; // Access the value of the current KeyValuePair
    }
};

// Iterator begin
    PropertiesIterator begin() {
        return PropertiesIterator(table.begin());
    }

// Iterator end
    PropertiesIterator end() {
        return PropertiesIterator(table.end());
    }
};

#endif // PROPERTIES_H
