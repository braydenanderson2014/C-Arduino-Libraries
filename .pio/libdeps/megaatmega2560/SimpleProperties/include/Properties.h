#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Hashtable.h"
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
    bool isEmpty();
    void saveToSD(const String& filename);
    void loadFromSD(const String& filename);
    bool containsKey(const String& key);
    class KeyIterator {
        private:
            const Hashtable<String, String>& table;
            int index;
            SimpleVector<String> keys;
    
        public:
            KeyIterator(const Hashtable<String, String>& t) : table(t), index(0) {
                keys = table.keys();
            }
    
            bool hasNext() {
                return index < keys.size();
            }
    
            String next() {
                if (!hasNext()) {
                    // Handle iterator reaching the end
                    return "";
                }
                return keys[index++];
            }
    };

    // Iterator for values
    class ValueIterator {
        private:
            const Hashtable<String, String>& table;
            int index;
            SimpleVector<String> keys;

        public:
            ValueIterator(const Hashtable<String, String>& t) : table(t), index(0) {
                keys = table.keys();
            }

            bool hasNext() {
                return index < keys.size();
            }

            String next() {
                if (!hasNext()) {
                    // Handle iterator reaching the end
                    return "";
                }
                String key = keys[index++];
                return table.get(key);
            }
    };

    // Functions to get iterators
    KeyIterator keysIterator() const {
        return KeyIterator(table);
    }

    ValueIterator valuesIterator() const {
        return ValueIterator(table);
    }
};

#endif // PROPERTIES_H
