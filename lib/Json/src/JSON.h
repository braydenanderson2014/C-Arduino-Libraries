#ifndef JSON_H
#define JSON_H

#include <SD.h>
#include <Arduino.h>
#include <SimpleVector.h>
#include "Variant.h"

// JSON Error Codes
#define JSON_SUCCESS 0
#define JSON_FILE_WRITE_ERROR 1
#define JSON_FILE_READ_ERROR 2
#define JSON_PARSE_ERROR 3
#define JSON_FILE_NOT_FOUND 4
#define JSON_TYPE_MISMATCH 5
#define JSON_KEY_NOT_FOUND 6
#define JSON_MEMORY_ALLOCATION_FAILED 7
#define JSON_SERIALIZE_ERROR 8
#define JSON_DESERIALIZE_ERROR 9
#define JSON_UNKNOWN_ERROR 99

class JSON {
public:
    enum class ValueType {
        Null,
        Bool,
        Number,
        String,
        Object,
        Array
    };

    struct Node {
        String key;
        ValueType type;
        Variant<String> stringValue;
        Variant<double> numberValue;
        Variant<bool> boolValue;
        SimpleVector<Node>* children;

        Node() : type(ValueType::Null), children(nullptr) {}

        ~Node() { if (children) delete children; }
    };

private:
    Node root;

public:
    JSON() { root.type = ValueType::Object; }

    // File I/O
    int readFromFile(const String& filename);
    int writeToFile(const String& filename) const;

    // Setters
    int setString(const String& path, const String& value);
    int setNumber(const String& path, double value);
    int setBool(const String& path, bool value);
    int setNull(const String& path);

    // Getters
    int getString(const String& path, String& value) const;
    int getNumber(const String& path, double& value) const;
    int getBool(const String& path, bool& value) const;

    // Serialization
    int serialize(String& output) const;
    int deserialize(const String& input);
    int parseObject(const char* &p, Node &node);
    int parseValue(const char* p, Node &node);    
    int parseArray(const char* &p, Node &node);
    int parseString(const char* &p, String &out);
    void skipWhitespaceAndComments(const char* &p);


    // Remove
    int remove(const String& path);

    // Deep Navigation
    Node* findOrCreateNode(const String& path, bool createIntermediate);
    Node* findNode(const String& path) const;

    const Node& getRoot() const { return root; }
};

#endif // JSON_H
