#ifndef EXTREME_VARIANT_H
#define EXTREME_VARIANT_H

#include <Optional.h>
#include <Hashtable.h>
#include <SimpleVector.h>
#include <Arduino.h>

/**
 * @brief Represents the type of the variant
 */
enum VariantType {
    SINGLE_ELEMENT,
    SINGLE_LIST,
    KEY_VALUE_PAIRS,
    NONE
};

template <typename K, typename V>
class ExtremeVariant {
public:
    ExtremeVariant() : type(NONE) {}

    // Set a single element
    void setSingle(const V& value) {
        clear();
        singleValue = value;
        type = SINGLE_ELEMENT;
    }

    // Set a list of elements
    void setList(const SimpleVector<V>& values) {
        clear();
        listValues = values;
        type = SINGLE_LIST;
    }

    // Set key-value pairs
    void setKeyValuePairs(const Hashtable<K, V>& pairs) {
        clear();
        keyValuePairs = pairs;
        type = KEY_VALUE_PAIRS;
    }

    // Get the single element
    Optional<V> getSingle() const {
        if (type == SINGLE_ELEMENT) {
            return Optional<V>(singleValue);
        }
        return Optional<V>();
    }

    // Get the list of elements
    Optional<SimpleVector<V>> getList() const {
        if (type == SINGLE_LIST) {
            return Optional<SimpleVector<V>>(listValues);
        }
        return Optional<SimpleVector<V>>();
    }

    // Get the key-value pairs
    Optional<Hashtable<K, V>> getKeyValuePairs() const {
        if (type == KEY_VALUE_PAIRS) {
            return Optional<Hashtable<K, V>>(keyValuePairs);
        }
        return Optional<Hashtable<K, V>>();
    }

    // Clear the variant
    void clear() {
        type = NONE;
    }

    // Get the current type
    VariantType getType() const {
        return type;
    }

private:
    VariantType type;
    V singleValue;
    SimpleVector<V> listValues;
    Hashtable<K, V> keyValuePairs;
};

#endif // EXTREME_VARIANT_H