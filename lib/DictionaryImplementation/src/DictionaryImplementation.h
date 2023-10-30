#include <Dictionary.h>
#include <unordered_map>
class MyDictionary : public Dictionary<String, int> {
    private:
    std::unordered_map<K, V> data;

public:
    int size() const override {
        return data.size();
    }

    bool isEmpty() const override {
        return data.empty();
    }

    V get(const K& key) const override {
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        }
        throw std::runtime_error("Key not found");
    }

    V put(const K& key, const V& value) override {
        V oldValue = data[key];
        data[key] = value;
        return oldValue;
    }

    V remove(const K& key) override {
        auto it = data.find(key);
        if (it != data.end()) {
            V removedValue = it->second;
            data.erase(it);
            return removedValue;
        }
        throw std::runtime_error("Key not found");
    }

    // Custom iterator for keys
    class KeyIterator {
    private:
        typename std::unordered_map<K, V>::const_iterator current;
        typename std::unordered_map<K, V>::const_iterator end;

    public:
        KeyIterator(typename std::unordered_map<K, V>::const_iterator start, typename std::unordered_map<K, V>::const_iterator stop)
            : current(start), end(stop) {}

        bool operator!=(const KeyIterator& other) const {
            return current != other.current;
        }

        const K& operator*() const {
            return current->first;
        }

        KeyIterator& operator++() {
            ++current;
            return *this;
        }
    };

    // Custom iterator for values
    class ValueIterator {
    private:
        typename std::unordered_map<K, V>::const_iterator current;
        typename std::unordered_map<K, V>::const_iterator end;

    public:
        ValueIterator(typename std::unordered_map<K, V>::const_iterator start, typename std::unordered_map<K, V>::const_iterator stop)
            : current(start), end(stop) {}

        bool operator!=(const ValueIterator& other) const {
            return current != other.current;
        }

        const V& operator*() const {
            return current->second;
        }

        ValueIterator& operator++() {
            ++current;
            return *this;
        }
    };

    KeyIterator keys() const {
        return KeyIterator(data.begin(), data.end());
    }

    ValueIterator elements() const {
        return ValueIterator(data.begin(), data.end());
    }
};

public:

    // Implementation of Dictionary's pure virtual functions
   

    void useDictionary() {
        // Adding key-value pairs to the dictionary
        this->put(String("apple"), 5);
        this->put(String("banana"), 3);
        this->put(String("cherry"), 8);

        // Retrieving values by key
        int appleCount = this->get(String("apple"));
        int bananaCount = this->get(String("banana"));
    }
};
