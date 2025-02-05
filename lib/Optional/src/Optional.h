#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T>
class Optional {
public:
    Optional() : has_value(false), value() {}
    Optional(const T& value) : has_value(true), value(value) {}

    bool hasValue() const { return has_value; }
    
    const T& getValue() const {
        if (!has_value) {
            static T default_value = T(); // ✅ Safe default value
            return default_value;
        }
        return value;
    }

    T& getValue() {
        if (!has_value) {
            static T default_value = T(); // ✅ Safe default value
            return default_value;
        }
        return value;
    }

private:
    bool has_value;
    T value;
};

#endif // OPTIONAL_H
