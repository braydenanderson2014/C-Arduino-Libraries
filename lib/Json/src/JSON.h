#ifndef JSON_H
#define JSON_H

#include <SD.h>
#include "CustomString.h"
#include "SimpleVector.h"

// Forward declare JSON class
class JSON;

class JSON {
public:
    // Possible data types in JSON
    enum class ValueType {
        Null,
        Bool,
        Number,
        String,
        Object,
        Array
    };

    // A node in the JSON tree
    struct Node {
        Custom_String::String key;       // The key name (if inside an object).
        ValueType type;                 // What kind of value this node holds?
        
        // Depending on 'type', one of these will be valid:
        bool boolValue;
        double numberValue;
        Custom_String::String stringValue;
        
        // If type == Object or Array, children will hold sub-nodes
        SimpleVector<Node> children;

        Node()
            : type(ValueType::Null),
              boolValue(false),
              numberValue(0.0) {}

        bool operator==(const Node& other) const {
            return (key == other.key
                    && type == other.type
                    && boolValue == other.boolValue
                    && numberValue == other.numberValue
                    && stringValue == other.stringValue
                    // possibly compare children here if desired
                   );
        }
    };

private:
    Node root;  // Root of the JSON tree (top-level object or array)

public:
    JSON() {
        // By default, consider the root as an OBJECT (empty).
        root.type = ValueType::Object;
    }

    // ------------------------------
    // PUBLIC: Reading & Writing
    // ------------------------------

    // Read JSON from a file on SD and parse it into 'root'.
    bool readFromFile(const Custom_String::String& filename);

    // Write the current JSON tree to a file on SD.
    bool writeToFile(const Custom_String::String& filename, bool pretty = true) const;

    // Parse from string directly (no file).
    bool readFromString(const Custom_String::String& jsonStr);

    // Serialize to string (in-memory).
    Custom_String::String writeToString(bool pretty = true) const;


    // ------------------------------
    // PUBLIC: Typed Getters/Setters
    // ------------------------------

    void setString(const Custom_String::String& path, const Custom_String::String& value);
    void setNumber(const Custom_String::String& path, double value);
    void setBool(const Custom_String::String& path, bool value);
    void setNull(const Custom_String::String& path);

    // Overload for arrays: create or expand array at that path
    void pushBack(const Custom_String::String& path, const Custom_String::String& value);
    void pushBack(const Custom_String::String& path, double value);
    void pushBack(const Custom_String::String& path, bool value);

    // Retrieve typed data (returns default if not found or type mismatch)
    Custom_String::String getString(const Custom_String::String& path, const Custom_String::String& defaultVal = "") const;
    double getNumber(const Custom_String::String& path, double defaultVal = 0.0) const;
    bool getBool(const Custom_String::String& path, bool defaultVal = false) const;
    bool isNull(const Custom_String::String& path) const;

    // Remove a node (object member or array element) by path
    bool remove(const Custom_String::String& path);

    // Access the root node directly if needed
    const Node& getRoot() const { return root; }
    Node& getRoot() { return root; }
    bool operator==(const Node& other) const ;

private:
    // --------------- Parsing Helpers ---------------
    // A minimal JSON parser
    bool parse(const Custom_String::String& json);
    bool parseValue(const char* &p, Node &node);
    bool parseObject(const char* &p, Node &node);
    bool parseArray(const char* &p, Node &node);
    bool parseString(const char* &p, Custom_String::String &out);
    bool parseNumber(const char* &p, double &out);
    bool parseBool(const char* &p, bool &out);
    bool parseNull(const char* &p);

    void skipWhitespace(const char* &p);

    // --------------- Serialization Helpers ---------------
    void serializeNode(const Node &node, Custom_String::String &out, int indentLevel, bool pretty) const;
    void serializeValue(const Node &node, Custom_String::String &out, int indentLevel, bool pretty) const;

    // --------------- Path Helpers ---------------
    Node* findOrCreateNode(const Custom_String::String& path, bool createIntermediate);
    Node* findNode(const Custom_String::String& path) const;
    Node* findNodeImpl(Node* current, const Custom_String::String& path, int startIndex, bool createIntermediate) const;

    // --------------- Remove Helpers ---------------
    bool removeChild(Node &parent, const Custom_String::String &keyOrIndex);

    // Utility to convert a substring to int
    int toInt(const Custom_String::String& s) const { return atoi(s.C_STR()); }
};

#endif // JSON_H
