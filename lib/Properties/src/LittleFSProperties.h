#ifndef LITTLEFS_LittleFSProperties_H
#define LITTLEFS_LittleFSProperties_H

#include <Hashtable.h>
#include <Arduino.h>

class LittleFSProperties {
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
        size_t chipSelect = 4; // Declaration of the chip select pin
    public:
        LittleFSProperties(); // Declaration of the constructor
        ~LittleFSProperties();

        void setChipSelect(const size_t cs); // Declaration of the setChipSelect method, which sets the chip select pin
        size_t getChipSelect(); // Declaration of the getChipSelect method, which returns the chip select pin

        bool beginLFS(); // Declaration of the begin method, which sets the identifier type... If not called, Default is EQUALS (=) (LFS )
        bool beginSD(size_t cs, IDENTIFIERTYPE identifierType); // Declaration of the begin method, which sets the identifier type... If not called, Default is EQUALS (=) (SD)
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
     * @brief LittleFSProperties Iterator
    */
    class LittleFSPropertiesIterator {
private:
    Hashtable<String, String>::Iterator it; // Declaration of the Hashtable iterator
public:
    // Constructor
    LittleFSPropertiesIterator(Hashtable<String, String>::Iterator begin)
        : it(begin) {}

    // Prefix increment
    LittleFSPropertiesIterator& operator++() {
        ++it; // Move to the next element
        return *this;
    }

    // Postfix increment
    LittleFSPropertiesIterator operator++(int) {
        LittleFSPropertiesIterator tmp(*this);
        ++(*this); // Use the prefix increment
        return tmp;
    }

    // Inequality check
    bool operator!=(const LittleFSPropertiesIterator& other) const {
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
    LittleFSPropertiesIterator begin() {
        return LittleFSPropertiesIterator(table.begin());
    }

// Iterator end
    LittleFSPropertiesIterator end() {
        return LittleFSPropertiesIterator(table.end());
    }
};






#endif//LITTLEFS_LittleFSProperties_H