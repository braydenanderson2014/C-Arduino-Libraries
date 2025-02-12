#ifndef JSON_H
#define JSON_H

#include <SDFat.h>
#include <Arduino.h>
#include <SimpleVector.h>
#include <lz4.h>

#define SD_CS_PIN 4

#define JSON_ERROR_CODE_MULTIPLIER 17 //So no error codes are the same between libraries.

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

    enum JSON_Error {
        JSON_GENERAL_SUCCESS = 3 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_WRITE_SUCCESS = 1 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_READ_SUCCESS = 2 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_WRITE_ERROR = 5 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_READ_ERROR = 6 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_FILE_NOT_FOUND = 7 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_FILE_OPEN_ERROR = 8 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_FILE_PARSE_ERROR = 9 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_COMPRESSED_FILE_ERROR = 10 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_DECOMPRESSED_FILE_ERROR = 11 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_COMPRESSION_ERROR = 12 * JSON_ERROR_CODE_MULTIPLIER,
        JSON_DECOMPRESSION_ERROR = 13 * JSON_ERROR_CODE_MULTIPLIER
    };
    

    // A node in the JSON tree
    struct Node {
        char* key;       // The key name (if inside an object).
        ValueType type;                 // What kind of value this node holds?
        union {
            bool boolValue;
            double numberValue;
            char* stringValue;
        };
        // Depending on 'type', one of these will be valid:
        Node() : key(nullptr), type(ValueType::Null), children(nullptr) {
            boolValue = false;
            numberValue = 0.0;
            stringValue = nullptr;
        }
        
        

        // If type == Object or Array, children will hold sub-nodes
        SimpleVector<Node>* children;
        //Node() : type(ValueType::Null), boolValue(false), numberValue(0.0), key(nullptr), stringValue(nullptr), children(nullptr) {}


        bool operator==(const Node& other) const {
            return (key == other.key
                 && type == other.type
                 && boolValue == other.boolValue
                 && numberValue == other.numberValue
                 && stringValue == other.stringValue
                 // possibly compare children here if desired
                );
        }

        ~Node() {
            free(key);
            if (type == ValueType::String) {
                free(stringValue);
            }
            if (children) {
                children->clear();
                delete children;
            }
        }
    };

private:
    Node root;  // Root of the JSON tree (top-level object or array)

public:
    JSON() {
        Serial.println("DEBUG: JSON Constructor Called");
        root.type = ValueType::Object;
        if (!root.children) {
            Serial.println("DEBUG: Initializing root.children");
            root.children = new SimpleVector<Node>();
        }
        sdInstance.begin(SD_CS_PIN);
    }


    JSON(size_t cs_pin){
        root.type = ValueType::Object;
        if (root.children) {
            root.children->clear();
        }
        sdInstance.begin(cs_pin);
    }

    ~JSON() {
        if (root.children) {
            root.children->clear();
            delete root.children;
        }
    }

    // ------------------------------
    // PUBLIC: Reading & Writing
    // ------------------------------

    // Read JSON from a file on SD and parse it into 'root'.
    int readFromFile(const char* filename);

    // Write the current JSON tree to a file on SD.
    int writeToFile(const char* filename, bool pretty = true);

    // Parse from string directly (no file).
    bool readFromString(const char*& jsonStr);
    

    // Serialize to string (in-memory).
    char* writeToString(bool pretty = true) const;


    // ------------------------------
    // PUBLIC: Typed Getters/Setters
    // ------------------------------

    void setString(const char* path, const char* value);
    void setNumber(const char* path, double value);
    void setBool(const char*& path, bool value);
    void setNull(const char*& path);

    // Overload for arrays: create or expand array at that path
    void pushBack(const char*& path, const char*& value);
    void pushBack(const char*& path, double value);
    void pushBack(const char*& path, bool value);

    // Retrieve typed data (returns default if not found or type mismatch)
    String getString(const char* path, const char* defaultVal = "") const;
    double getNumber(const char* path, double defaultVal = 0.0) const;
    bool getBool(const char*& path, bool defaultVal = false) const;
    bool isNull(const char*& path) const;
    SimpleVector<char*>getKeys() const;


    // Remove a node (object member or array element) by path
    bool remove(const char* path);

    // Access the root node directly if needed
    const Node& getRoot() const { return root; }
    Node& getRoot() { return root; }
    bool operator==(const Node& other) const ;

    bool hasKey(const char*& path) const;

private:
    // --------------- Parsing Helpers ---------------
    // A minimal JSON parser
    SdFat sdInstance;  // Use SdFat instead of Arduino's SD library

    bool parse(const char* json);
    bool parseValue(const char* &p, Node &node);
    bool parseObject(const char* &p, Node &node);
    bool parseArray(const char* &p, Node &node);
    bool parseString(const char*& p, char* out, size_t outSize);
    bool parseNumber(const char* &p, double &out);
    bool parseBool(const char* &p, bool &out);
    bool parseNull(const char* &p);

    void skipWhitespace(const char* &p);

    // --------------- Serialization Helpers ---------------
    void serializeNode(const Node &node, char* out, size_t outSize, int indentLevel, bool pretty) const;
    void serializeValue(const Node &node, char* out, size_t outSize, int indentLevel, bool pretty) const;

    // --------------- Path Helpers ---------------
    Node* findOrCreateNode(const char* path, bool createIntermediate);
    Node* findNode(const char* path) const;
    Node* findNodeImpl(Node* current, const char* path, size_t startIndex, bool createIntermediate) const;
    // --------------- Remove Helpers ---------------
    bool removeChild(JSON::Node &parent, const char* keyOrIndex);

    // Utility to convert a substring to int
    int toInt(const char*& s) const { return atoi(s); }

    char* strdupSafe(const char* src) const;
    char* strdupSafe(const char src[], size_t length);
    const char* valueTypeToString(ValueType type) const ;
};

#endif // JSON_H
