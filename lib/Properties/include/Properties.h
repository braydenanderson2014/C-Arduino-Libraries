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
                return "EMPTY";
            }
            return keys[index++];
        }
    };

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
                return "EMPTY";
            }
            String* valuePtr = table.get(keys[index++]);
            if (valuePtr) {
                return *valuePtr;
            } else {
                return "";
            }
        }
    };

    KeyIterator keysIterator() const {
        return KeyIterator(table);
    }

    ValueIterator valuesIterator() const {
        return ValueIterator(table);
    }
};

#endif // PROPERTIES_H
