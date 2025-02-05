#include "JSON.h"

// --------------------------------------------------------------------
// Public Methods: File I/O
// --------------------------------------------------------------------
bool JSON::readFromFile(const String& filename) {
    FsFile file = sdInstance.open(filename.c_str(), O_READ);
    if (!file) {
        return false;
    }

    // Read entire file into a string (be mindful of memory on Arduino!)
    String json;
    while (file.available()) {
        json += (char)file.read();
    }
    file.close();

    return readFromString(json);
}

bool JSON::writeToFile(const String& filename, bool pretty)  {
    FsFile file = sdInstance.open(filename.c_str(), O_CREAT | O_WRITE);
    
    if (!file) {
        return false;
    }

    String out = writeToString(pretty);
    if(out.length() == 0) {
        Serial.println("Error: JSON serialization failed.");
    } else {
        Serial.println(out.length());
        Serial.println(root.children->size());
    }
    
    file.print(out);
    file.close();
    return true;
}

bool JSON::readFromString(const String& jsonStr) {
    // Clear the current root
    root.children->clear();
    root.type = ValueType::Object; // default to object (or could be array if top-level is [ ])
    return parse(jsonStr);
}

String JSON::writeToString(bool pretty) const {
    String out;
    serializeNode(root, out, 0, pretty);
    return out;
}

// --------------------------------------------------------------------
// Public Methods: Typed Setters
// --------------------------------------------------------------------
void JSON::setString(const String& path, const String& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::String;
    node->stringValue = value;
}


void JSON::setNumber(const String& path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Number;
    node->numberValue = value;
}

void JSON::setBool(const String& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Bool;
    node->boolValue = value;
}

void JSON::setNull(const String& path) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Null;
}

// For arrays: push a new element at the end
void JSON::pushBack(const String& path, const String& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    // If node isn't already an array, make it one
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children->clear();
    }
    Node child;
    child.type = ValueType::String;
    child.stringValue = value;
    node->children->push_back(child);
}

void JSON::pushBack(const String& path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children->clear();
    }
    Node child;
    child.type = ValueType::Number;
    child.numberValue = value;
    node->children->push_back(child);
}

void JSON::pushBack(const String& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children->clear();
    }
    Node child;
    child.type = ValueType::Bool;
    child.boolValue = value;
    node->children->push_back(child);
}

// --------------------------------------------------------------------
// Public Methods: Typed Getters
// --------------------------------------------------------------------
String JSON::getString(const String& path, const String& defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::String) {
        return node->stringValue;
    }
    // If it's something else (like Number), we can try to convert
    if (node->type == ValueType::Number) {
        String temp;
        temp += node->numberValue; // you might need your own double->String
        return temp;
    }
    if (node->type == ValueType::Bool) {
        return node->boolValue ? "true" : "false";
    }
    return defaultVal;
}

double JSON::getNumber(const String& path, double defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::Number) {
        return node->numberValue;
    }
    // If it's string, we can attempt to parse
    if (node->type == ValueType::String) {
        return atof(node->stringValue.c_str());
    }
    if (node->type == ValueType::Bool) {
        return node->boolValue ? 1.0 : 0.0;
    }
    return defaultVal;
}

bool JSON::getBool(const String& path, bool defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::Bool) {
        return node->boolValue;
    }
    // Convert from number or string if you'd like
    if (node->type == ValueType::Number) {
        return (node->numberValue != 0);
    }
    if (node->type == ValueType::String) {
        return (node->stringValue == "true" || node->stringValue == "1");
    }
    return defaultVal;
}

bool JSON::isNull(const String& path) const {
    Node* node = findNode(path);
    if (!node) return false;
    return (node->type == ValueType::Null);
}

// --------------------------------------------------------------------
// Public Methods: Remove
// --------------------------------------------------------------------
bool JSON::remove(const String& path) {
    // Find the parent path
    // e.g. if path = "obj.key", parentPath = "obj", childKey = "key"
    // if path = "key", then parentPath = "", childKey = "key"

    int lastDot = -1; // or use .lastIndexOf('.') from your custom string
    for (int i = path.length() - 1; i >= 0; i--) {
        if (path[i] == '.') {
            lastDot = i;
            break;
        }
    }

    if (lastDot < 0) {
        // Remove from the root's children
        return removeChild(root, path);
    } else {
        String parentPath = path.substring(0, lastDot);
        String keyOrIndex = path.substring(lastDot + 1, path.length() - (lastDot + 1));
        Node* parent = findNode(parentPath);
        if (!parent) return false;
        return removeChild(*parent, keyOrIndex);
    }
}

// --------------------------------------------------------------------
// Parsing (Minimal JSON Parser)
// --------------------------------------------------------------------
bool JSON::parse(const String& json) {
    const char* p = json.c_str();
    skipWhitespace(p);

    // We expect either object '{' or array '[' as the top-level.
    if (*p == '{') {
        root.type = ValueType::Object;
        return parseObject(p, root);
    } else if (*p == '[') {
        root.type = ValueType::Array;
        return parseArray(p, root);
    } else {
        // Not valid JSON top-level
        return false;
    }
}

bool JSON::parseValue(const char* &p, Node &node) {
    skipWhitespace(p);

    if (*p == '{') {
        node.type = ValueType::Object;
        return parseObject(p, node);
    }
    if (*p == '[') {
        node.type = ValueType::Array;
        return parseArray(p, node);
    }
    if (*p == '\"') {
        node.type = ValueType::String;
        return parseString(p, node.stringValue);
    }
    if (*p == 't' || *p == 'f') {
        node.type = ValueType::Bool;
        return parseBool(p, node.boolValue);
    }
    if (*p == 'n') {
        node.type = ValueType::Null;
        return parseNull(p);
    }
    // Otherwise, assume number
    node.type = ValueType::Number;
    return parseNumber(p, node.numberValue);
}

// Parse object: '{' <pair> (, <pair>)* '}'
bool JSON::parseObject(const char* &p, Node &node) {
    // Expect '{'
    if (*p != '{') return false;
    p++; // skip '{'
    skipWhitespace(p);

    node.children->clear();

    if (*p == '}') {
        p++; // empty object
        return true;
    }

    while (*p) {
        // Parse a key
        Node child;
        child.type = ValueType::Null;

        // Key must be a string
        if (*p != '\"') return false;
        p++; // skip quote
        // read until next quote
        while (*p && *p != '\"') {
            child.key += *p;
            p++;
        }
        if (*p != '\"') return false; // missing closing quote
        p++; // skip closing quote

        skipWhitespace(p);
        if (*p != ':') return false;
        p++; // skip ':'
        skipWhitespace(p);

        // Parse value
        if (!parseValue(p, child)) return false;
        node.children->push_back(child);

        skipWhitespace(p);
        if (*p == '}') {
            p++; // end object
            return true;
        }
        if (*p != ',') return false;
        p++; // skip comma
        skipWhitespace(p);
    }
    return false; // incomplete or error
}

// Parse array: '[' <value> (, <value>)* ']'
bool JSON::parseArray(const char* &p, Node &node) {
    if (*p != '[') return false;
    p++; // skip '['
    skipWhitespace(p);

    node.children->clear();

    if (*p == ']') {
        p++; // empty array
        return true;
    }

    while (*p) {
        Node child;
        if (!parseValue(p, child)) return false;
        node.children->push_back(child);

        skipWhitespace(p);
        if (*p == ']') {
            p++; // end array
            return true;
        }
        if (*p != ',') return false;
        p++; // skip comma
        skipWhitespace(p);
    }
    return false;
}

// Parse string in quotes (p points to opening quote)
bool JSON::parseString(const char* &p, String &out) {
    if (*p != '\"') return false;
    p++; // skip opening quote
    out.~String();
    while (*p) {
        if (*p == '\"') {
            p++; // skip closing quote
            return true;
        }
        // (Optional) handle escapes: \n, \", etc.
        // For simplicity, just read the raw char:
        out += *p;
        p++;
    }
    return false; // missing closing quote
}

// Parse bool: 'true' or 'false'
bool JSON::parseBool(const char* &p, bool &out) {
    if (strncmp(p, "true", 4) == 0) {
        out = true;
        p += 4;
        return true;
    } else if (strncmp(p, "false", 5) == 0) {
        out = false;
        p += 5;
        return true;
    }
    return false;
}

// Parse null
bool JSON::parseNull(const char* &p) {
    if (strncmp(p, "null", 4) == 0) {
        p += 4;
        return true;
    }
    return false;
}

// Parse number: minimal approach
bool JSON::parseNumber(const char* &p, double &out) {
    char buffer[32];
    int i = 0;

    // Optional sign
    if (*p == '-' || *p == '+') {
        if (i < 31) buffer[i++] = *p;
        p++;
    }

    bool hasDot = false;
    while ((*p >= '0' && *p <= '9') || (*p == '.' && !hasDot)) {
        if (*p == '.') hasDot = true;
        if (i < 31) {
            buffer[i++] = *p;
        }
        p++;
    }
    buffer[i] = '\0';
    out = atof(buffer);
    return true;
}

void JSON::skipWhitespace(const char* &p) {
    while (*p == ' ' || *p == '\r' || *p == '\n' || *p == '\t') {
        p++;
    }
}

// --------------------------------------------------------------------
// Serialization
// --------------------------------------------------------------------
void JSON::serializeNode(const Node &node, String &out, int indentLevel, bool pretty) const {
    if (node.type == ValueType::Object) {
        out += '{';
        if (pretty) out += '\n';
        for (int i = 0; i < node.children->size(); i++) {
            if (pretty) {
                for (int s = 0; s < indentLevel + 2; s++) out += ' ';
            }
            out += '\"';
            out += node.children->get(i).key;
            out += "\":";
            if (pretty) out += ' ';
            serializeValue(node.children->get(i), out, indentLevel + 2, pretty);

            if (i < node.children->size() - 1) {
                out += ',';
            }
            if (pretty) out += '\n';
        }
        if (pretty) {
            for (int s = 0; s < indentLevel; s++) out += ' ';
        }
        out += '}';
    } else if (node.type == ValueType::Array) {
        out += '[';
        if (pretty) out += '\n';
        for (int i = 0; i < node.children->size(); i++) {
            if (pretty) {
                for (int s = 0; s < indentLevel + 2; s++) out += ' ';
            }
            serializeValue(node.children->get(i), out, indentLevel + 2, pretty);

            if (i < node.children->size() - 1) {
                out += ',';
            }
            if (pretty) out += '\n';
        }
        if (pretty) {
            for (int s = 0; s < indentLevel; s++) out += ' ';
        }
        out += ']';
    } else {
        serializeValue(node, out, indentLevel, pretty);
    }
}


void JSON::serializeValue(const Node &node, String &out, int indentLevel, bool pretty) const {
    switch (node.type) {
    case ValueType::Null:
        out += "null";
        break;
    case ValueType::Bool:
        out += (node.boolValue ? "true" : "false");
        break;
    case ValueType::Number: {
        // Convert double to string (basic)
        char buf[32];
        dtostrf(node.numberValue, 0, 6, buf); // Arduino function, or use sprintf
        out += buf;
    } break;
    case ValueType::String:
        out += '\"';
        out += node.stringValue;
        out += '\"';
        break;
    case ValueType::Object:
    case ValueType::Array:
        // Recursively serialize
        serializeNode(node, out, indentLevel, pretty);
        break;
    }
}

// --------------------------------------------------------------------
// Path-based Node Lookup (dot notation) 
// --------------------------------------------------------------------
JSON::Node* JSON::findOrCreateNode(const String& path, bool createIntermediate) {
    return findNodeImpl(&root, path, 0, createIntermediate);
}

JSON::Node* JSON::findNode(const String& path) const {
    // We cast away const here to reuse the same function,
    // but we won't modify anything unless createIntermediate=true.
    return const_cast<JSON*>(this)->findNodeImpl(const_cast<Node*>(&root), path, 0, false);
}

// Recursive helper: path like "obj.arr.0.key"
JSON::Node* JSON::findNodeImpl(Node* current, const String& path, int startIndex, bool createIntermediate) const {
    if (!current) return nullptr;
    if (startIndex >= path.length()) {
        return current;
    }

    int dotPos = path.indexOf('.', startIndex);
    String token = (dotPos == -1) ? path.substring(startIndex) : path.substring(startIndex, dotPos);
    int nextIndex = (dotPos == -1) ? path.length() : dotPos + 1;

    if (current->type == ValueType::Array) {
        int index = toInt(token);
        if (index < 0 || index >= current->children->size()) {
            if (!createIntermediate) return nullptr;
            while (current->children->size() <= index) {
                Node dummy;
                dummy.type = ValueType::Null;
                current->children->push_back(dummy);
            }
        }
        return findNodeImpl(&current->children->get(index), path, nextIndex, createIntermediate);
    } else if (current->type == ValueType::Object) {
        for (int i = 0; i < current->children->size(); i++) {
            if (current->children->get(i).key == token) {
                return findNodeImpl(&current->children->get(i), path, nextIndex, createIntermediate);
            }
        }
        if (!createIntermediate) return nullptr;
        Node newChild;
        newChild.key = token;
        newChild.type = ValueType::Object;
        current->children->push_back(newChild);
        return findNodeImpl(&current->children->get(current->children->size() - 1), path, nextIndex, createIntermediate);
    } else {
        if (!createIntermediate) return nullptr;
        current->type = ValueType::Object;
        current->children = new SimpleVector<Node>();
        Node newChild;
        newChild.key = token;
        newChild.type = ValueType::Object;
        current->children->push_back(newChild);
        return findNodeImpl(&current->children->get(0), path, nextIndex, createIntermediate);
    }
}


// --------------------------------------------------------------------
// Remove Helpers
// --------------------------------------------------------------------
bool JSON::removeChild(Node &parent, const String &keyOrIndex) {
    if (!parent.children) return false;

    if (parent.type == ValueType::Object) {
        for (int i = 0; i < parent.children->size(); i++) {
            if (parent.children->get(i).key == keyOrIndex) {
                parent.children->erase(i);
                return true;
            }
        }
    } else if (parent.type == ValueType::Array) {
        int index = toInt(keyOrIndex);
        if (index < 0 || index >= parent.children->size()) return false;
        parent.children->erase(index);
        return true;
    }
    return false;
}




