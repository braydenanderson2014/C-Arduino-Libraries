#ifndef JSON_H
#define JSON_H

#include <Arduino.h>
#include <SdFat.h>
#include <SimpleVector.h>
#include <lz4.h>

#ifndef JSON_ENABLE_OPTIONAL_RETURNS
#define JSON_ENABLE_OPTIONAL_RETURNS 0
#endif

#if JSON_ENABLE_OPTIONAL_RETURNS
// Enable Optional<T> JSON getters by defining JSON_ENABLE_OPTIONAL_RETURNS=1 in every
// compilation unit that includes JSON.h *and* in the unit that compiles JSON.cpp
// (e.g. pass -DJSON_ENABLE_OPTIONAL_RETURNS=1 to every g++ invocation that touches
// either file). Optional.h (lib/Optional/src) must also be on the include path.
#include <Optional.h>
#endif

#define SD_CS_PIN 4

#define JSON_ERROR_CODE_MULTIPLIER 17

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

    struct Node {
        char* key;
        ValueType type;
        union {
            bool boolValue;
            double numberValue;
            char* stringValue;
        };
        SimpleVector<Node>* children;

        Node();
        Node(const Node& other);
        Node& operator=(const Node& other);
        ~Node();

        void clear();
        bool operator==(const Node& other) const;

    private:
        void copyFrom(const Node& other);
    };

private:
    Node root;
    SdFat sdInstance;

public:
    JSON();
    explicit JSON(size_t cs_pin);
    ~JSON();

    int readFromFile(const char* filename);
    int readFromFile(const String& filename) { return readFromFile(filename.c_str()); }

    int writeToFile(const char* filename, bool pretty = true);
    int writeToFile(const String& filename, bool pretty = true) { return writeToFile(filename.c_str(), pretty); }

    bool readFromString(const char* jsonStr);
    bool readFromString(const String& jsonStr) { return readFromString(jsonStr.c_str()); }

    char* writeToString(bool pretty = true) const;

    void setString(const char* path, const char* value);
    void setString(const char* path, const String& value) { setString(path, value.c_str()); }
    void setString(const String& path, const char* value) { setString(path.c_str(), value); }
    void setString(const String& path, const String& value) { setString(path.c_str(), value.c_str()); }

    void setNumber(const char* path, double value);
    void setNumber(const String& path, double value) { setNumber(path.c_str(), value); }

    void setBool(const char* path, bool value);
    void setBool(const String& path, bool value) { setBool(path.c_str(), value); }

    void setNull(const char* path);
    void setNull(const String& path) { setNull(path.c_str()); }

    void pushBack(const char* path, const char* value);
    void pushBack(const char* path, const String& value) { pushBack(path, value.c_str()); }
    void pushBack(const String& path, const char* value) { pushBack(path.c_str(), value); }
    void pushBack(const String& path, const String& value) { pushBack(path.c_str(), value.c_str()); }
    void pushBack(const char* path, double value);
    void pushBack(const String& path, double value) { pushBack(path.c_str(), value); }
    void pushBack(const char* path, bool value);
    void pushBack(const String& path, bool value) { pushBack(path.c_str(), value); }

    String getString(const char* path, const char* defaultVal = "") const;
    String getString(const char* path, const String& defaultVal) const { return getString(path, defaultVal.c_str()); }
    String getString(const String& path, const char* defaultVal = "") const { return getString(path.c_str(), defaultVal); }
    String getString(const String& path, const String& defaultVal) const { return getString(path.c_str(), defaultVal.c_str()); }

    double getNumber(const char* path, double defaultVal = 0.0) const;
    double getNumber(const String& path, double defaultVal = 0.0) const { return getNumber(path.c_str(), defaultVal); }

    bool getBool(const char* path, bool defaultVal = false) const;
    bool getBool(const String& path, bool defaultVal = false) const { return getBool(path.c_str(), defaultVal); }

#if JSON_ENABLE_OPTIONAL_RETURNS
    Optional<String> tryGetString(const char* path) const;
    Optional<String> tryGetString(const String& path) const { return tryGetString(path.c_str()); }

    Optional<double> tryGetNumber(const char* path) const;
    Optional<double> tryGetNumber(const String& path) const { return tryGetNumber(path.c_str()); }

    Optional<bool> tryGetBool(const char* path) const;
    Optional<bool> tryGetBool(const String& path) const { return tryGetBool(path.c_str()); }
#endif

    bool isNull(const char* path) const;
    bool isNull(const String& path) const { return isNull(path.c_str()); }

    SimpleVector<char*> getKeys() const;

    bool remove(const char* path);
    bool remove(const String& path) { return remove(path.c_str()); }

    bool hasKey(const char* path) const;
    bool hasKey(const String& path) const { return hasKey(path.c_str()); }

    const Node& getRoot() const { return root; }
    Node& getRoot() { return root; }

private:
    bool parse(const char* json);
    bool parseValue(const char*& p, Node& node);
    bool parseObject(const char*& p, Node& node);
    bool parseArray(const char*& p, Node& node);
    bool parseString(const char*& p, char* out, size_t outSize);
    bool parseNumber(const char*& p, double& out);
    bool parseBool(const char*& p, bool& out);
    bool parseNull(const char*& p);

    void skipWhitespace(const char*& p);

    void serializeNode(const Node& node, char* out, size_t outSize, size_t& offset, int indentLevel, bool pretty) const;
    void serializeValue(const Node& node, char* out, size_t outSize, size_t& offset, int indentLevel, bool pretty) const;
    void appendToBuffer(char* out, size_t outSize, size_t& offset, const char* text) const;
    void appendCharToBuffer(char* out, size_t outSize, size_t& offset, char value) const;
    void appendIndent(char* out, size_t outSize, size_t& offset, int indentLevel) const;
    void appendEscapedString(char* out, size_t outSize, size_t& offset, const char* text) const;
    size_t measureSerializedNode(const Node& node, int indentLevel, bool pretty) const;
    size_t measureSerializedValue(const Node& node, int indentLevel, bool pretty) const;
    size_t measureEscapedString(const char* text) const;

    Node* findOrCreateNode(const char* path, bool createIntermediate);
    Node* findNode(const char* path) const;
    Node* findNodeImpl(Node* current, const char* path, size_t startIndex, bool createIntermediate) const;

    bool removeChild(Node& parent, const char* keyOrIndex);
    bool isArrayIndex(const char* token) const;

    void resetRoot(ValueType type);
    void releaseNodeValue(Node& node);

    int toInt(const char* s) const { return s ? atoi(s) : 0; }
    char* strdupSafe(const char* src) const;
    char* strdupSafe(const char* src, size_t length) const;
};

#endif // JSON_H
