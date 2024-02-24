#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Hashtable.h"
#include <Arduino.h>

class Properties {
private:
        Hashtable<String, String> table; // Declaration of the Hashtable (Uses the Hashtable class from the Hashtable.h file in the background)
        enum IDENTIFIERTYPE{ // Enumeration of the identifier types (=, :, ;, -, , , /, \)
            EQUALS,
            COLEN,
            SEMICOLEN,
            HYPHEN,
            COMMA,
            FORWARD_SLASH,
            BACKWARD_SLASH
        };
        IDENTIFIERTYPE identifierType = EQUALS;
    public:
        Properties(); // Declaration of the constructor
        ~Properties();

        void identify(const IDENTIFIERTYPE identifierType); // Declaration of the begin method, which sets the identifier type... If not called, Default is EQUALS (=)
        void setProperty(const String& key, const String& value);
        void setProperty(const String& key, const String& value, const String& filePath);
        String getProperty(const String& key);
        String getProperty(const String& key, const String& defaultValue, const String& filePath);
        void removeProperty(const String& key);
        void clear();
        int size();
        int elements();
        bool isEmpty();
        bool saveToSD(const String& filename);
        bool loadFromSD(const String& filename);
        bool save(const String& filename);
        bool load(const String& filename);
        bool store(const String& filename, const String& comments);
        bool exists(const String& key);
        bool exists(const String& key, const String& value);
        
        bool storeToXML(const String& filename, const String& comments);
        bool loadFromXML(const String& filename);

        bool storeToJSON(const String& filename, const String& comments);
        bool loadFromJSON(const String& filename);

        bool storeToYAML(const String& filename, const String& comments);
        bool loadFromYAML(const String& filename);

        bool storeToINI(const String& filename, const String& comments);
        bool loadFromINI(const String& filename);

        bool storeToCSV(const String& filename, const String& comments);
        bool loadFromCSV(const String& filename);

        bool storeToTOML(const String& filename, const String& comments);
        bool loadFromTOML(const String& filename);

        bool storeToMsgPack(const String& filename, const String& comments);
        bool loadFromMsgPack(const String& filename);

        bool containsKey(const String& key);
        bool deleteFile(const String& filename);
    
    /**
     * @brief Properties Iterator
    */
    class PropertiesIterator {
private:
    Hashtable<String, String>::Iterator it; // Declaration of the Hashtable iterator
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
