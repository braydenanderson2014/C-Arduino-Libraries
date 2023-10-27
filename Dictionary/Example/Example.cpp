#include <iostream.h>
#include <iterator.h>
#include <SimpleVector.h> // Include your SimpleVector class

template <typename K, typename V>
class Dictionary {
public:
    // Default constructor
    Dictionary() {}

    // Destructor
    virtual ~Dictionary() {}

    // Abstract methods
    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual SimpleVector<K> keys() const = 0; 
    virtual SimpleVector<V> elements() const = 0; 
    virtual V get(const K& key) const = 0;
    virtual void put(const K& key, const V& value) = 0;
    virtual void remove(const K& key) = 0;
};

class MyDictionary : public Dictionary<int, std::string> {
private:
    SimpleVector<int> keyContainer;
    SimpleVector<std::string> valueContainer;

public:
    // Implementation of Dictionary's pure virtual functions
    int size() const override {
        return keyContainer.size();
    }

    bool isEmpty() const override {
        return keyContainer.size() == 0;
    }

    SimpleVector<int> keys() const override {
        return keyContainer;
    }

    SimpleVector<std::string> elements() const override {
        return valueContainer;
    }

    std::string get(const int& key) const override {
        for (unsigned int i = 0; i < keyContainer.size(); ++i) {
            if (keyContainer[i] == key) {
                return valueContainer[i];
            }
        }
        return ""; // Return an empty string for key not found
    }

    void put(const int& key, const std::string& value) override {
        for (unsigned int i = 0; i < keyContainer.size(); ++i) {
            if (keyContainer[i] == key) {
                valueContainer[i] = value; // Update value if key exists
                return;
            }
        }
        keyContainer.push_back(key);
        valueContainer.push_back(value); // Add a new key-value pair
    }

    void remove(const int& key) override {
        for (unsigned int i = 0; i < keyContainer.size(); ++i) {
            if (keyContainer[i] == key) {
                keyContainer.erase(i);
                valueContainer.erase(i);
                return;
            }
        }
    }
};

int main() {
    MyDictionary dict;

    // Add key-value pairs
    dict.put(1, "One");
    dict.put(2, "Two");
    dict.put(3, "Three");

    // Get values by keys
    std::cout << "Value for key 2: " << dict.get(2) << std::endl;

    // Remove a key-value pair
    dict.remove(1);

    // Print all keys and values
    SimpleVector<int> keys = dict.keys();
    SimpleVector<std::string> values = dict.elements();

    for (unsigned int i = 0; i < keys.size(); ++i) {
        std::cout << "Key: " << keys[i] << ", Value: " << values[i] << std::endl;
    }

    return 0;
}
