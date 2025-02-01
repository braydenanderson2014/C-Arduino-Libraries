#include "JSON.h"

// --------------------------------------------------------------------
// Public Methods: File I/O
// --------------------------------------------------------------------

int JSON::readFromFile(const String& filename) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return JSON_FILE_NOT_FOUND;
    }

    String jsonData;
    while (file.available()) {
        jsonData += (char)file.read();
    }
    file.close();

    return deserialize(jsonData);
}

int JSON::writeToFile(const String& filename) const {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        return JSON_FILE_WRITE_ERROR;
    }

    String out;
    int result = serialize(out);
    if (result != JSON_SUCCESS) {
        return result;
    }

    file.print(out);
    file.close();
    return JSON_SUCCESS;
}

// --------------------------------------------------------------------
// Public Methods: Typed Setters
// --------------------------------------------------------------------

int JSON::setString(const String& path, const String& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return JSON_MEMORY_ALLOCATION_FAILED;

    node->type = ValueType::String;
    node->stringValue.setSingle(value);
    return JSON_SUCCESS;
}

int JSON::setNumber(const String& path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return JSON_MEMORY_ALLOCATION_FAILED;

    node->type = ValueType::Number;
    node->numberValue.setSingle(value);
    return JSON_SUCCESS;
}

int JSON::setBool(const String& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return JSON_MEMORY_ALLOCATION_FAILED;

    node->type = ValueType::Bool;
    node->boolValue.setSingle(value);
    return JSON_SUCCESS;
}

int JSON::setNull(const String& path) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return JSON_MEMORY_ALLOCATION_FAILED;

    node->type = ValueType::Null;
    return JSON_SUCCESS;
}

// --------------------------------------------------------------------
// Public Methods: Typed Getters
// --------------------------------------------------------------------

int JSON::getString(const String& path, String& value) const {
    Node* node = findNode(path);
    if (!node) return JSON_KEY_NOT_FOUND;

    if (node->type == ValueType::String) {
        value = node->stringValue.getSingle();
        return JSON_SUCCESS;
    }
    return JSON_TYPE_MISMATCH;
}

int JSON::getNumber(const String& path, double& value) const {
    Node* node = findNode(path);
    if (!node) return JSON_KEY_NOT_FOUND;

    if (node->type == ValueType::Number) {
        value = node->numberValue.getSingle();
        return JSON_SUCCESS;
    }
    return JSON_TYPE_MISMATCH;
}

int JSON::getBool(const String& path, bool& value) const {
    Node* node = findNode(path);
    if (!node) return JSON_KEY_NOT_FOUND;

    if (node->type == ValueType::Bool) {
        value = node->boolValue.getSingle();
        return JSON_SUCCESS;
    }
    return JSON_TYPE_MISMATCH;
}

// --------------------------------------------------------------------
// Public Methods: Remove
// --------------------------------------------------------------------

int JSON::remove(const String& path) {
    Node* node = findNode(path);
    if (!node) return JSON_KEY_NOT_FOUND;

    delete node;
    return JSON_SUCCESS;
}

// --------------------------------------------------------------------
// Public Methods: Serialization
// --------------------------------------------------------------------

int JSON::serialize(String& output) const {
    output = "{";
    for (size_t i = 0; i < root.children->size(); i++) {
        const Node& node = root.children->get(i);
        if (i > 0) output += ",";

        output += "\"" + node.key + "\":";

        switch (node.type) {
            case ValueType::Null:
                output += "null";
                break;
            case ValueType::Bool:
                output += (node.boolValue.getSingle() ? "true" : "false");
                break;
            case ValueType::Number:
                output += String(node.numberValue.getSingle());
                break;
            case ValueType::String:
                output += "\"" + node.stringValue.getSingle() + "\"";
                break;
            default:
                return JSON_SERIALIZE_ERROR;
        }
    }
    output += "}";
    return JSON_SUCCESS;
}

int JSON::deserialize(const String& input) {
    const char* p = input.c_str();  // Use a character pointer for efficient parsing
    skipWhitespaceAndComments(p);

    if (*p != '{') return JSON_PARSE_ERROR; // JSON must start with an object
    p++; // Move past '{'
    
    root.type = ValueType::Object;
    if (!root.children) root.children = new SimpleVector<Node>();
    root.children->clear(); // Reset root

    return parseObject(p, root);
}

int JSON::parseObject(const char* &p, Node &node) {
    node.type = ValueType::Object;
    if (!node.children) node.children = new SimpleVector<Node>();

    while (*p) {
        skipWhitespaceAndComments(p);  // Skip comments and whitespace
        if (*p == '}') { p++; return JSON_SUCCESS; }  // End of object

        // Parse Key
        String key;
        if (parseString(p, key) != JSON_SUCCESS) return JSON_PARSE_ERROR;

        skipWhitespaceAndComments(p);
        if (*p != ':') return JSON_PARSE_ERROR;
        p++; // Move past ':'
        
        // Parse Value
        Node child;
        child.key = key;
        if (parseValue(p, child) != JSON_SUCCESS) return JSON_PARSE_ERROR;

        node.children->push_back(child);
        
        skipWhitespaceAndComments(p);
        if (*p == '}') { p++; return JSON_SUCCESS; }
        if (*p != ',') return JSON_PARSE_ERROR;
        p++; // Move past ','
    }

    return JSON_PARSE_ERROR;
}


int JSON::parseValue(const char* p, Node &node) {  // ✅ NO REFERENCE
    skipWhitespaceAndComments(p);

    if (*p == '{') return parseObject(p, node); // Nested object
    if (*p == '[') return parseArray(p, node);  // Array
    
    if (*p == '\"') { // String
        node.type = ValueType::String;
        String tempString = node.stringValue.getSingle();  // ✅ Store in modifiable String
        int result = parseString(p, tempString);
        node.stringValue.setSingle(tempString);  // ✅ Update Variant
        return result;
    }

    if ((*p >= '0' && *p <= '9') || *p == '-' || *p == '.') { // Number
        node.type = ValueType::Number;
        char buffer[32] = {0};
        int i = 0;
        while ((*p >= '0' && *p <= '9') || *p == '.' || *p == '-') {
            if (i < 31) buffer[i++] = *p;
            p++;
        }
        buffer[i] = '\0'; // Null-terminate buffer
        node.numberValue.setSingle(atof(buffer));
        return JSON_SUCCESS;
    }

    if (strncmp(p, "true", 4) == 0) { // Boolean: true
        node.type = ValueType::Bool;
        node.boolValue.setSingle(true);
        p += 4;
        return JSON_SUCCESS;
    }
    
    if (strncmp(p, "false", 5) == 0) { // Boolean: false
        node.type = ValueType::Bool;
        node.boolValue.setSingle(false);
        p += 5;
        return JSON_SUCCESS;
    }

    if (strncmp(p, "null", 4) == 0) { // Null
        node.type = ValueType::Null;
        p += 4;
        return JSON_SUCCESS;
    }

    return JSON_PARSE_ERROR;
}


int JSON::parseArray(const char* &p, Node &node) {
    node.type = ValueType::Array;
    if (!node.children) node.children = new SimpleVector<Node>();

    p++; // Move past '['
    skipWhitespaceAndComments(p);

    if (*p == ']') { p++; return JSON_SUCCESS; } // Empty array

    while (*p) {
        Node child;
        if (parseValue(p, child) != JSON_SUCCESS) return JSON_PARSE_ERROR;

        node.children->push_back(child);

        skipWhitespaceAndComments(p);
        if (*p == ']') { p++; return JSON_SUCCESS; }
        if (*p != ',') return JSON_PARSE_ERROR;
        p++; // Move past ','
    }

    return JSON_PARSE_ERROR;
}

int JSON::parseString(const char* &p, String &out) {
    if (*p != '\"') return JSON_PARSE_ERROR;
    p++; // Move past opening quote

    while (*p) {
        if (*p == '\"') {
            p++; // Move past closing quote
            return JSON_SUCCESS;
        }

        if (*p == '\\') {  // Handle Escape Sequences
            p++; // Move past '\'
            switch (*p) {
                case 'n': out += '\n'; break;
                case 't': out += '\t'; break;
                case 'r': out += '\r'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case '\"': out += '\"'; break;
                case '\\': out += '\\'; break;
                default: return JSON_PARSE_ERROR; // Invalid escape
            }
        } else {
            out += *p;
        }
        p++;
    }
    return JSON_PARSE_ERROR;  // Missing closing quote
}


void JSON::skipWhitespaceAndComments(const char* &p) {
    while (*p) {
        // Skip Whitespace
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
            p++;
            continue;
        }

        // Skip `//` (Single-line Comment)
        if (*p == '/' && *(p + 1) == '/') {
            while (*p && *p != '\n') p++;  // Move to end of line
            continue;
        }

        // Skip `/* */` (Multi-line Comment)
        if (*p == '/' && *(p + 1) == '*') {
            p += 2;  // Move past `/*`
            while (*p && !(*p == '*' && *(p + 1) == '/')) p++;
            if (*p) p += 2;  // Move past `*/`
            continue;
        }

        break;  // No more whitespace/comments
    }
}


// --------------------------------------------------------------------
// Private Methods: Deep Navigation
// --------------------------------------------------------------------

JSON::Node* JSON::findOrCreateNode(const String& path, bool createIntermediate) {
    // Implementation for navigating or creating deep JSON nodes
    return &root; // Placeholder
}

JSON::Node* JSON::findNode(const String& path) const {
    // Implementation for navigating existing JSON nodes
    return nullptr; // Placeholder
}
