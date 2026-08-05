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
    ExtremeVariant() : type(NONE), singleValue(), listValues(nullptr), keyValuePairs(nullptr) {}

    ExtremeVariant(const ExtremeVariant<K, V>& other)
        : type(other.type), singleValue(other.singleValue), listValues(nullptr), keyValuePairs(nullptr) {
        if (other.listValues) {
            listValues = new SimpleVector<V>(*other.listValues);
        }
        if (other.keyValuePairs) {
            keyValuePairs = new Hashtable<K, V>(*other.keyValuePairs);
        }
    }

    ExtremeVariant<K, V>& operator=(const ExtremeVariant<K, V>& other) {
        if (this != &other) {
            delete listValues;
            delete keyValuePairs;
            listValues = nullptr;
            keyValuePairs = nullptr;
            type = other.type;
            singleValue = other.singleValue;
            if (other.listValues) {
                listValues = new SimpleVector<V>(*other.listValues);
            }
            if (other.keyValuePairs) {
                keyValuePairs = new Hashtable<K, V>(*other.keyValuePairs);
            }
        }
        return *this;
    }

    ~ExtremeVariant() {
        delete listValues;
        delete keyValuePairs;
        listValues = nullptr;
        keyValuePairs = nullptr;
    }

    // Set a single element
    void setSingle(const V& value) {
        clear();
        singleValue = value;
        type = SINGLE_ELEMENT;
    }

    // Set a list of elements
    void setList(const SimpleVector<V>& values) {
        clear();
        if (!listValues) {
            listValues = new SimpleVector<V>(values);
        } else {
            *listValues = values;
        }
        type = SINGLE_LIST;
    }

    // Set key-value pairs
    void setKeyValuePairs(const Hashtable<K, V>& pairs) {
        clear();
        if (!keyValuePairs) {
            keyValuePairs = new Hashtable<K, V>(pairs);
        } else {
            *keyValuePairs = pairs;
        }
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
        if (type == SINGLE_LIST && listValues) {
            return Optional<SimpleVector<V>>(*listValues);
        }
        return Optional<SimpleVector<V>>();
    }

    // Get the key-value pairs
    Optional<Hashtable<K, V>> getKeyValuePairs() const {
        if (type == KEY_VALUE_PAIRS && keyValuePairs) {
            return Optional<Hashtable<K, V>>(*keyValuePairs);
        }
        return Optional<Hashtable<K, V>>();
    }

    // Clear the variant
    void clear() {
        type = NONE;
        if (listValues) {
            listValues->clear();
        }
        if (keyValuePairs) {
            keyValuePairs->clear();
        }
    }

    // Get the current type
    VariantType getType() const {
        return type;
    }

private:
    VariantType type;
    V singleValue;
    SimpleVector<V>* listValues;
    Hashtable<K, V>* keyValuePairs;
};

#endif // EXTREME_VARIANT_H