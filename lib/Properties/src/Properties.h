#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Hashtable.h"
#include <Arduino.h>
#include <SD.h>

#if defined(__has_include) && __has_include(<FS.h>)
  #include <FS.h>
#else
  namespace fs {
    class FS {
    public:
      virtual ~FS() = default;
      virtual File open(const char* path, const char* mode) = 0;
      virtual bool exists(const char* path) = 0;
      virtual bool remove(const char* path) = 0;
    };
  }
#endif

class Properties {
public:
        enum IDENTIFIERTYPE{ // Enumeration of the identifier types (=, :, ;, -, , , /, \)
            EQUALS,
            COLEN,
            SEMICOLEN,
            HYPHEN,
            COMMA,
            FORWARD_SLASH,
            BACKWARD_SLASH
        };

private:
        Hashtable<String, String> table; // Declaration of the Hashtable (Uses the Hashtable class from the Hashtable.h file in the background)
        IDENTIFIERTYPE identifierType = EQUALS;

        size_t chipSelect = 4;       // Chip select pin for SD card
        bool _bypassSDBegin = false; // When true, skip SD.begin() (caller has already initialized SD)
        fs::FS* _filesystem = nullptr; // Optional external filesystem (LittleFS, SD, SPIFFS, etc.)

        // Private helpers
        bool beginSDIfNeeded();                          // Calls SD.begin(chipSelect) unless bypassed
        char getSeparator() const;                       // Returns the separator character for the current identifierType
        String normalizePath(const String& filename) const; // Prepends '/' if path doesn't start with one
        File openForRead(const String& filename);        // Opens file for reading (using _filesystem or SD)
        File openForWrite(const String& filename);       // Opens file for writing/truncating (using _filesystem or SD)
        bool fileExists(const String& filename);         // Checks file existence (using _filesystem or SD)
        bool removeFile(const String& filename);         // Removes a file (using _filesystem or SD)

    public:
        Properties(); // Default constructor — uses SD card
        explicit Properties(fs::FS& filesystem); // Constructor that injects an external filesystem
        ~Properties();

        bool begin(size_t cs, IDENTIFIERTYPE identifierType = EQUALS); // Initialize/use the SD backend
        bool begin(fs::FS& filesystem, IDENTIFIERTYPE identifierType = EQUALS); // Initialize/use an injected filesystem backend
        void setChipSelect(const size_t cs); // Declaration of the setChipSelect method, which sets the chip select pin
        size_t getChipSelect(); // Declaration of the getChipSelect method, which returns the chip select pin
        void setBypassSDBegin(bool bypass); // When true, skip SD.begin() on every operation
        bool getBypassSDBegin();
        void setFilesystem(fs::FS& filesystem); // Set (or change) the filesystem used for all file operations
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
