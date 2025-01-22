#include "JSON.h"

// --------------------------------------------------------------------
// Public Methods: File I/O
// --------------------------------------------------------------------
bool JSON::readFromFile(const Custom_String::String& filename) {
    File file = SD.open(filename.C_STR(), FILE_READ);
    if (!file) {
        return false;
    }

    // Read entire file into a string (be mindful of memory on Arduino!)
    Custom_String::String json;
    while (file.available()) {
        json += (char)file.read();
    }
    file.close();

    return readFromString(json);
}

bool JSON::writeToFile(const Custom_String::String& filename, bool pretty) const {
    File file = SD.open(filename.C_STR(), FILE_WRITE);
    if (!file) {
        return false;
    }

    Custom_String::String out = writeToString(pretty);
    file.print(out.C_STR());
    file.close();
    return true;
}

bool JSON::readFromString(const Custom_String::String& jsonStr) {
    // Clear the current root
    root.children.clear();
    root.type = ValueType::Object; // default to object (or could be array if top-level is [ ])
    return parse(jsonStr);
}

Custom_String::String JSON::writeToString(bool pretty) const {
    Custom_String::String out;
    serializeNode(root, out, 0, pretty);
    return out;
}

// --------------------------------------------------------------------
// Public Methods: Typed Setters
// --------------------------------------------------------------------
void JSON::setString(const Custom_String::String& path, const Custom_String::String& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::String;
    node->stringValue = value;
}

void JSON::setNumber(const Custom_String::String& path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Number;
    node->numberValue = value;
}

void JSON::setBool(const Custom_String::String& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Bool;
    node->boolValue = value;
}

void JSON::setNull(const Custom_String::String& path) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Null;
}

// For arrays: push a new element at the end
void JSON::pushBack(const Custom_String::String& path, const Custom_String::String& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    // If node isn't already an array, make it one
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children.clear();
    }
    Node child;
    child.type = ValueType::String;
    child.stringValue = value;
    node->children.push_back(child);
}

void JSON::pushBack(const Custom_String::String& path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children.clear();
    }
    Node child;
    child.type = ValueType::Number;
    child.numberValue = value;
    node->children.push_back(child);
}

void JSON::pushBack(const Custom_String::String& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children.clear();
    }
    Node child;
    child.type = ValueType::Bool;
    child.boolValue = value;
    node->children.push_back(child);
}

// --------------------------------------------------------------------
// Public Methods: Typed Getters
// --------------------------------------------------------------------
Custom_String::String JSON::getString(const Custom_String::String& path, const Custom_String::String& defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::String) {
        return node->stringValue;
    }
    // If it's something else (like Number), we can try to convert
    if (node->type == ValueType::Number) {
        Custom_String::String temp;
        temp += node->numberValue; // you might need your own double->String
        return temp;
    }
    if (node->type == ValueType::Bool) {
        return node->boolValue ? "true" : "false";
    }
    return defaultVal;
}

double JSON::getNumber(const Custom_String::String& path, double defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::Number) {
        return node->numberValue;
    }
    // If it's string, we can attempt to parse
    if (node->type == ValueType::String) {
        return atof(node->stringValue.C_STR());
    }
    if (node->type == ValueType::Bool) {
        return node->boolValue ? 1.0 : 0.0;
    }
    return defaultVal;
}

bool JSON::getBool(const Custom_String::String& path, bool defaultVal) const {
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

bool JSON::isNull(const Custom_String::String& path) const {
    Node* node = findNode(path);
    if (!node) return false;
    return (node->type == ValueType::Null);
}

// --------------------------------------------------------------------
// Public Methods: Remove
// --------------------------------------------------------------------
bool JSON::remove(const Custom_String::String& path) {
    // Find the parent path
    // e.g. if path = "obj.key", parentPath = "obj", childKey = "key"
    // if path = "key", then parentPath = "", childKey = "key"

    int lastDot = -1; // or use .lastIndexOf('.') from your custom string
    for (int i = path.size() - 1; i >= 0; i--) {
        if (path[i] == '.') {
            lastDot = i;
            break;
        }
    }

    if (lastDot < 0) {
        // Remove from the root's children
        return removeChild(root, path);
    } else {
        Custom_String::String parentPath = path.Sub_String(0, lastDot);
        Custom_String::String keyOrIndex = path.Sub_String(lastDot + 1, path.size() - (lastDot + 1));
        Node* parent = findNode(parentPath);
        if (!parent) return false;
        return removeChild(*parent, keyOrIndex);
    }
}

// --------------------------------------------------------------------
// Parsing (Minimal JSON Parser)
// --------------------------------------------------------------------
bool JSON::parse(const Custom_String::String& json) {
    const char* p = json.C_STR();
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

    node.children.clear();

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
        node.children.push_back(child);

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

    node.children.clear();

    if (*p == ']') {
        p++; // empty array
        return true;
    }

    while (*p) {
        Node child;
        if (!parseValue(p, child)) return false;
        node.children.push_back(child);

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
bool JSON::parseString(const char* &p, Custom_String::String &out) {
    if (*p != '\"') return false;
    p++; // skip opening quote
    out.clear();

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
void JSON::serializeNode(const Node &node, Custom_String::String &out, int indentLevel, bool pretty) const {
    if (node.type == ValueType::Object) {
        out += '{';
        if (pretty) out += '\n';
        for (int i = 0; i < node.children.size(); i++) {
            if (pretty) {
                for (int s = 0; s < indentLevel + 2; s++) out += ' ';
            }
            out += '\"';
            out += node.children[i].key;
            out += "\":";
            if (pretty) out += ' ';
            serializeValue(node.children[i], out, indentLevel + 2, pretty);

            if (i < node.children.size() - 1) {
                out += ',';
            }
            if (pretty) out += '\n';
        }
        if (pretty) {
            for (int s = 0; s < indentLevel; s++) out += ' ';
        }
        out += '}';
    }
    else if (node.type == ValueType::Array) {
        out += '[';
        if (pretty) out += '\n';
        for (int i = 0; i < node.children.size(); i++) {
            if (pretty) {
                for (int s = 0; s < indentLevel + 2; s++) out += ' ';
            }
            serializeValue(node.children[i], out, indentLevel + 2, pretty);

            if (i < node.children.size() - 1) {
                out += ',';
            }
            if (pretty) out += '\n';
        }
        if (pretty) {
            for (int s = 0; s < indentLevel; s++) out += ' ';
        }
        out += ']';
    }
    else {
        // If root is not object/array, just serialize the value directly
        serializeValue(node, out, indentLevel, pretty);
    }
}

void JSON::serializeValue(const Node &node, Custom_String::String &out, int indentLevel, bool pretty) const {
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
JSON::Node* JSON::findOrCreateNode(const Custom_String::String& path, bool createIntermediate) {
    return findNodeImpl(&root, path, 0, createIntermediate);
}

JSON::Node* JSON::findNode(const Custom_String::String& path) const {
    // We cast away const here to reuse the same function,
    // but we won't modify anything unless createIntermediate=true.
    return const_cast<JSON*>(this)->findNodeImpl(const_cast<Node*>(&root), path, 0, false);
}

// Recursive helper: path like "obj.arr.0.key"
JSON::Node* JSON::findNodeImpl(Node* current, const Custom_String::String& path, int startIndex, bool createIntermediate) const {
    if (!current) return nullptr;
    if (startIndex >= path.size()) {
        // Reached the end
        return current;
    }

    // Find next dot or end
    int dotPos = -1;
    for (int i = startIndex; i < path.size(); i++) {
        if (path[i] == '.') {
            dotPos = i;
            break;
        }
    }

    Custom_String::String token;
    int nextIndex = 0;
    if (dotPos == -1) {
        // No more dots
        token = path.Sub_String(startIndex, path.size() - startIndex);
        nextIndex = path.size();
    } else {
        token = path.Sub_String(startIndex, dotPos - startIndex);
        nextIndex = dotPos + 1;
    }

    // If current is an array, 'token' is likely an index "[0], [1]" style or just "0", "1"
    // For simplicity, let's assume the path uses plain digits for array indices.
    if (current->type == ValueType::Array) {
        // Convert token to index
        int index = toInt(token);
        // If the index is out of range, either fail or create
        if (index < 0 || index > current->children.size()) {
            if (!createIntermediate) return nullptr;
            // We can fill with null or something until we have the desired index
            while (current->children.size() < index) {
                Node dummy;
                dummy.type = ValueType::Null;
                current->children.push_back(dummy);
            }
            // Now create the new element
            Node newNode;
            newNode.type = ValueType::Object; // or Null
            current->children.push_back(newNode);
        }
        if (index == current->children.size()) {
            return &current->children[index - 1]; // double-check logic if you allow "push" style
        }
        // Recurse
        return findNodeImpl(&current->children[index], path, nextIndex, createIntermediate);
    }
    else if (current->type == ValueType::Object || current == &root) {
        // Look for child with matching key
        for (int i = 0; i < current->children.size(); i++) {
            if (current->children[i].key == token) {
                return findNodeImpl(&current->children[i], path, nextIndex, createIntermediate);
            }
        }
        // Not found
        if (!createIntermediate) {
            return nullptr;
        } else {
            // Create a new child
            Node newChild;
            newChild.key = token;
            newChild.type = ValueType::Object; // by default
            current->children.push_back(newChild);
            return findNodeImpl(&current->children.back(), path, nextIndex, createIntermediate);
        }
    } else {
        // If it's neither an array nor an object, we can't go deeper unless we transform it
        if (!createIntermediate) return nullptr;

        // Convert the current node into an object so we can add children
        current->type = ValueType::Object;
        current->children.clear();

        // Now create a new child
        Node newChild;
        newChild.key = token;
        newChild.type = ValueType::Object;
        current->children.push_back(newChild);
        return findNodeImpl(&current->children.back(), path, nextIndex, createIntermediate);
    }
}

// --------------------------------------------------------------------
// Remove Helpers
// --------------------------------------------------------------------
bool JSON::removeChild(Node &parent, const Custom_String::String &keyOrIndex) {
    if (parent.type == ValueType::Object) {
        // Remove the child with the matching key
        for (int i = 0; i < parent.children.size(); i++) {
            if (parent.children[i].key == keyOrIndex) {
                parent.children.erase(i);
                return true;
            }
        }
        return false;
    }
    else if (parent.type == ValueType::Array) {
        // interpret keyOrIndex as integer
        int index = toInt(keyOrIndex);
        if (index < 0 || index >= parent.children.size()) {
            return false;
        }
        parent.children.erase(index); 
        return true;
    }
    // If parent is neither object nor array, nothing to remove
    return false;
}




