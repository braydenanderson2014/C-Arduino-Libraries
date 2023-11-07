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
            Hashtable<String, String>::Iterator hashtableEnd;  // Add this line

        public:
            PropertiesIterator(Hashtable<String, String>::Iterator begin, Hashtable<String, String>::Iterator end)
        : it(begin), hashtableEnd(end) {}  // Modify this line

        PropertiesIterator& operator++() {
            // Advance the iterator at least once before checking for empty values.
            ++it;
            // Skip over empty values. Check if the length of the value string is 0.
            while (it != hashtableEnd && (*it).value.length() == 0) {
                ++it;
            }
            return *this;
        }





        // Postfix increment
        PropertiesIterator operator++(int) {
            PropertiesIterator tmp(*this);
            operator++(); // prefix-increment this instance
            return tmp;   // return value before increment
        }

        bool operator!=(const PropertiesIterator& other) const {
            return it != other.it;
        }

        // Assuming `Hashtable::Iterator` properly dereferences to an `Entry`
            Hashtable<String, String>::KeyValuePair operator*() const {
                return *it; // This assumes that `*it` dereferences to a KeyValuePair
            }
        /*
        // Dereference operator to return a key-value pair
            

             Hashtable<String, String>::KeyValuePair operator*() const {
            auto entry = *it; // This assumes that `*it` gives you an `Entry` or something similar.
            return {entry->key, entry->value}; // Construct and return a KeyValuePair.
        }

        */
        // To make it easier to use without std::pair, you could provide methods to get the key and value:
        String key() const { 
            return (*it).key; 
        }

        String value() const { 
            return (*it).value; 
        }
    };

PropertiesIterator begin() {
    return PropertiesIterator(table.begin(), table.end());
}

PropertiesIterator end() {
    return PropertiesIterator(table.end(), table.end());
}
};

#endif // PROPERTIES_H
