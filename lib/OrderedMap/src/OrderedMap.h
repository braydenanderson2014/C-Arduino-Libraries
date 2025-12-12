#include <Arduino.h>
#include "ArrayList.h"
#include <SD.h>
#include "JSON.h"
#include "TypeTraits.h"
template <typename K, typename V>
class OrderedMap {
private:
    ArrayList<K> internalKeys;
    ArrayList<V> internalValues;

    // Helper functions for conversions
    String keyToString(const K& key) const {
        if constexpr (is_same<K, const char*>::value || is_same<K, String>::value) {
            return String(key);
        } else if constexpr (is_Integral<K>::value || is_floating_point<K>::value) {
            return String(key);
        } else {
            return String("UnsupportedKey");
        }
    }

    String valueToString(const V& value) const {
        if constexpr (is_same<V, const char*>::value || is_same<V, String>::value) {
            return String(value);
        } else if constexpr (is_Integral<V>::value || is_floating_point<V>::value) {
            return String(value);
        } else if constexpr (is_Bool<V>::value) {
            return value ? "true" : "false";
        } else {
            return String("UnsupportedValue");
        }
    }

    K stringToKey(const String& str) const {
        if constexpr (is_same<K, const char*>::value) {
            return str.c_str();
        } else if constexpr (is_same<K, String>::value) {
            return str;
        } else if constexpr (is_Integral<K>::value) {
            return static_cast<K>(str.toInt());
        } else if constexpr (is_floating_point<K>::value) {
            return static_cast<K>(str.toFloat());
        } else {
            return K(); // Default value for unsupported types
        }
    }

    V stringToValue(const String& str) const {
        if constexpr (is_same<V, const char*>::value) {
            return str.c_str();
        } else if constexpr (is_same<V, String>::value) {
            return str;
        } else if constexpr (is_Integral<V>::value) {
            return static_cast<V>(str.toInt());
        } else if constexpr (is_floating_point<V>::value) {
            return static_cast<V>(str.toFloat());
        } else if constexpr (is_Bool<V>::value) {
            return (str == "true" || str == "1");
        } else {
            return V(); // Default value for unsupported types
        }
    }

public:
    OrderedMap() = default;

    void put(const K& key, const V& value) {
        for (size_t i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key) {
                internalValues.set(i, value);
                return;
            }
        }
        internalKeys.add(key);
        internalValues.add(value);
    }

    V get(const K& key) const {
        for (size_t i = 0; i < internalKeys.size(); i++) {
            if (internalKeys.get(i) == key) {
                return internalValues.get(i);
            }
        }
        return V();
    }

    size_t size() const { return internalKeys.size(); }

    // Serialize to JSON
    // Note: JSON object is created locally to minimize memory usage when not serializing.
    // This is optimal for Arduino where serialization is typically infrequent (startup/shutdown)
    // and memory is at a premium. The allocation cost is acceptable given the memory savings.
    void serializeToJSON(const String& filename) {
        JSON json; // Create JSON instance only when needed
        for (size_t i = 0; i < internalKeys.size(); i++) {
            String keyStr = keyToString(internalKeys.get(i));
            const V& value = internalValues.get(i);

            if constexpr (is_same<V, String>::value || is_same<V, const char*>::value) {
                json.setString(keyStr, valueToString(value));
            } else if constexpr (is_Integral<V>::value || is_floating_point<V>::value) {
                json.setNumber(keyStr, static_cast<double>(value));
            } else if constexpr (is_Bool<V>::value) {
                json.setBool(keyStr, value);
            } else {
                json.setString(keyStr, valueToString(value));
            }
        }
        Serial.println("Writing to file...");
        json.writeToFile(filename);
        Serial.println("Done.");
    }

    // Deserialize from JSON
    // Note: JSON object is created locally to minimize memory usage when not deserializing.
    // This is optimal for Arduino where deserialization is typically infrequent (startup)
    // and memory is at a premium. The allocation cost is acceptable given the memory savings.
    void deserializeFromJSON(const String& filename) {
        JSON json; // Create JSON instance only when needed
        if (!json.readFromFile(filename)) {
            Serial.println("Failed to read JSON file.");
            return;
        }

        clear(); // Clear current map

        const JSON::Node& root = json.getRoot();
        for (int i = 0; i < root.children->size(); i++) {
            const JSON::Node& child = root.children->get(i);

            K key = stringToKey(child.key);
            V value;

            if (child.type == JSON::ValueType::String) {
                value = stringToValue(child.stringValue);
            } else if (child.type == JSON::ValueType::Number) {
                value = static_cast<V>(child.numberValue);
            } else if (child.type == JSON::ValueType::Bool) {
                value = static_cast<V>(child.boolValue);
            }

            put(key, value);
        }
    }

    // Clear the map
    void clear() {
        internalKeys.clear();
        internalValues.clear();
    }
};
