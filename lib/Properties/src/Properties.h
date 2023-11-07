#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <Hashtable.h>
#include <Arduino.h>

class Properties {
private:
    Hashtable<String, String> table;

public:
    Properties();
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
    
    class PropertiesIterator {
        private:
            Hashtable<String, String>::Iterator it;
        public:
            PropertiesIterator(Hashtable<String, String>::Iterator itr) : it(itr) {}

        PropertiesIterator& operator++() {
            ++it;
            return *this;
        }

        bool operator!=(const PropertiesIterator& other) const {
            return it != other.it;
        }

        // Assuming `Hashtable::Iterator` properly dereferences to an `Entry`
        Hashtable<String, String>::KeyValuePair operator*() const {
            auto entry = *it; // This assumes that `*it` gives you an `Entry` or something similar.
            return {entry->key, entry->value}; // Construct and return a KeyValuePair.
        }

    
        // To make it easier to use without std::pair, you could provide methods to get the key and value:
        String key() const { return (*it)->key; }
        String value() const { return (*it)->value; }
    };

    PropertiesIterator begin() {
        return PropertiesIterator(table.begin());
    }

    PropertiesIterator end() {
        return PropertiesIterator(table.end());
    }
};

#endif // PROPERTIES_H
