#include "JSON.h"

// --------------------------------------------------------------------
// Information
// --------------------------------------------------------------------
bool JSON::hasKey(const char*& path) const {
    return findNode(path) != nullptr;
}

bool JSON::isNull(const char*& path) const {
    Node* node = findNode(path);
    return node && node->type == ValueType::Null;
}


// --------------------------------------------------------------------
// Public Methods: File I/O
// --------------------------------------------------------------------
int JSON::readFromFile(const char* filename) {
    FsFile file = sdInstance.open(filename, O_READ);
    if (!file) {
        return JSON_FILE_NOT_FOUND; // Error 119
    }

    // Read the compressed file into a buffer
    char compressedBuffer[1024];  // Adjust size as needed
    int compressedSize = file.read(compressedBuffer, sizeof(compressedBuffer));
    file.close();

    if (compressedSize <= 0) {
        return JSON_READ_ERROR;
    }

    // Allocate buffer for decompressed data
    char decompressedBuffer[2048];  // Adjust size based on expected JSON length
    memset(decompressedBuffer, 0, sizeof(decompressedBuffer));

    // Decompress the JSON data
    int decompressedSize = LZ4_decompress_safe(compressedBuffer, decompressedBuffer, compressedSize, sizeof(decompressedBuffer));
    if (decompressedSize < 0) {
        return JSON_DECOMPRESSION_ERROR;
    }

    // Convert the decompressed buffer into a char* for parsing
    const char* jsonStr = decompressedBuffer;
    int parseResult = readFromString(jsonStr);

    return parseResult;  // Return success or error from parsing
}


/*
bool JSON::readFromFile(const FsFile& file) {
    
}
*/

int JSON::writeToFile(const char* filename, bool pretty) {
    FsFile file = sdInstance.open(filename, O_CREAT | O_WRITE);
    if (!file) return JSON_FILE_OPEN_ERROR;

    // Buffer for serialized JSON
    char outBuffer[2048];  // Increased buffer size for larger JSON
    memset(outBuffer, 0, sizeof(outBuffer));

    // Serialize JSON to string
    serializeNode(root, outBuffer, sizeof(outBuffer), 0, pretty);

    // Buffer for compressed data
    char compressedBuffer[2048];
    memset(compressedBuffer, 0, sizeof(compressedBuffer));

    // Compress JSON using LZ4
    int compressedSize = LZ4_compress_default(outBuffer, compressedBuffer, sizeof(outBuffer), sizeof(compressedBuffer));
    if (compressedSize <= 0) {
        file.close();
        return JSON_COMPRESSION_ERROR;
    }

    // Write compressed data to file
    if (file.write(compressedBuffer, compressedSize) != compressedSize) {
        file.close();
        return JSON_WRITE_ERROR;
    }

    file.close();
    return JSON_WRITE_SUCCESS;
}



/*
bool JSON::writeToFile(const FsFile& file, bool pretty)  {
    
}
*/

bool JSON::readFromString(const char*& jsonStr) {
    // Clear the current root
    root.children->clear();
    root.type = ValueType::Object; // default to object (or could be array if top-level is [ ])
    return parse(jsonStr);
}

char* JSON::writeToString(bool pretty) const {
    size_t bufferSize = 2048;  // Set a larger default buffer size
    char* out = (char*)malloc(bufferSize);
    if (!out) return nullptr;  // Check for allocation failure
    memset(out, 0, bufferSize);

    serializeNode(root, out, bufferSize, 0, pretty);

    return out;
}



// --------------------------------------------------------------------
// Public Methods: Typed Setters
// --------------------------------------------------------------------
void JSON::setString(const char* path, const char* value) {
    if (!path || strlen(path) == 0) return; // Prevent empty keys

    Node* node = findOrCreateNode(path, true);
    if (!node) return; // Ensure node is valid

    node->type = ValueType::String;
    node->stringValue = strdupSafe(value);

    if (!root.children) root.children = new SimpleVector<Node>(); // Ensure root has children

    // Ensure the node is added to children
    if (root.type != ValueType::Object) {
        root.type = ValueType::Object;
    }

    bool exists = false;
    for (size_t i = 0; i < root.children->elements(); i++) {
        if (strcmp(root.children->get(i).key, path) == 0) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        root.children->push_back(*node); // Ensure it's in the tree
    }
}


void JSON::setNumber(const char* path, double value) {

    Node* node = findOrCreateNode(path, true);
    if (!node || node == &root) {
        return;
    }
    node->type = ValueType::Number;
    node->numberValue = value;

    if (!root.children) {
        root.children = new SimpleVector<Node>();
    }

    bool exists = false;
    for (size_t i = 0; i < root.children->elements(); i++) {
        if (strcmp(root.children->get(i).key, path) == 0) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        Serial.println("DEBUG: Adding node to root.children");
        root.children->push_back(*node);

        // Debugging: Print stored node key after push
        Node& storedNode = root.children->get(root.children->elements() - 1);
        Serial.print("DEBUG: Stored Node Key After Push: ");
        Serial.println(storedNode.key ? storedNode.key : "(null)");
    }

    Serial.println("DEBUG: Value set successfully");
}



void JSON::setBool(const char*& path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Bool;
    node->boolValue = value;
}

void JSON::setNull(const char*& path) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    node->type = ValueType::Null;
}

// For arrays: push a new element at the end
void JSON::pushBack(const char*& path, const char*& value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) return;
    // If node isn't already an array, make it one
    if (node->type != ValueType::Array) {
        node->type = ValueType::Array;
        node->children->clear();
    }
    Node child;
    child.type = ValueType::String;
    child.stringValue = strdupSafe(value);
    node->children->push_back(child);
}

void JSON::pushBack(const char*& path, double value) {
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

void JSON::pushBack(const char*& path, bool value) {
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
String JSON::getString(const char* path, const char* defaultVal) const {
    Node* node = findNode(path);
    if (!node) return String(defaultVal); // Convert `const char*` to `String`

    if (node->type == ValueType::String) {
        return String(node->stringValue); // Ensure `String` conversion
    }

    // Convert Number to String safely
    if (node->type == ValueType::Number) {
        char temp[32]; // Buffer for number-to-string conversion
        dtostrf(node->numberValue, 0, 6, temp); // Arduino-specific float-to-string
        return String(temp);
    }

    // Convert Boolean to "true"/"false" String
    if (node->type == ValueType::Bool) {
        return node->boolValue ? String("true") : String("false");
    }

    return String(defaultVal);
}


double JSON::getNumber(const char* path, double defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;

    if (node->type == ValueType::Number) {
        return node->numberValue;
    }

    // If it's a string, attempt to parse as a number
    if (node->type == ValueType::String) {
        return (node->stringValue) ? atof(node->stringValue) : defaultVal;
    }

    // Convert boolean to 1.0 or 0.0
    if (node->type == ValueType::Bool) {
        return node->boolValue ? 1.0 : 0.0;
    }

    return defaultVal;
}


bool JSON::getBool(const char*& path, bool defaultVal) const {
    Node* node = findNode(path);
    if (!node) return defaultVal;
    if (node->type == ValueType::Bool) {
        return node->boolValue;
    }
    // Convert from number or char* if you'd like
    if (node->type == ValueType::Number) {
        return (node->numberValue != 0);
    }
    if (node->type == ValueType::String) {
        return (node->stringValue == "true" || node->stringValue == "1");
    }
    return defaultVal;
}



SimpleVector<char*> JSON::getKeys() const {
    SimpleVector<char*> keys;
    
    if (root.type != ValueType::Object) {
        return keys;  // Return empty if root is not an object
    }

    for (size_t i = 0; i < root.children->elements(); i++) {
        keys.put(root.children->get(i).key);
    }

    return keys;
}

// --------------------------------------------------------------------
// Public Methods: Remove
// --------------------------------------------------------------------
bool JSON::remove(const char* path) {
    if (!path || strlen(path) == 0) return false; // Ensure valid input

    // Find the last dot in the path (to separate parent path and key/index)
    int lastDot = -1;
    size_t pathLen = strlen(path);
    for (int i = pathLen - 1; i >= 0; i--) {
        if (path[i] == '.') {
            lastDot = i;
            break;
        }
    }

    if (lastDot < 0) {
        // No dot found → Remove from the root's children
        return removeChild(root, path);
    } else {
        // Declare character arrays for parentPath and keyOrIndex
        char parentPath[128];  // Adjust buffer size as needed
        char keyOrIndex[64];   // Buffer for the key/index

        // Copy the parent path
        if (lastDot < sizeof(parentPath)) {
            strncpy(parentPath, path, lastDot);
            parentPath[lastDot] = '\0'; // Null-terminate
        } else {
            return false; // Path too long, prevent buffer overflow
        }

        // Copy the key/index
        size_t keyLen = pathLen - lastDot - 1;
        if (keyLen < sizeof(keyOrIndex)) {
            strncpy(keyOrIndex, path + lastDot + 1, keyLen);
            keyOrIndex[keyLen] = '\0'; // Null-terminate
        } else {
            return false; // Key too long, prevent buffer overflow
        }

        // Pass `parentPath` as `char*` to `findNode` (safe because char[] decays to char*)
        Node* parent = findNode(parentPath);
        if (!parent) return false;

        return removeChild(*parent, keyOrIndex);
    }
}




// --------------------------------------------------------------------
// Parsing (Minimal JSON Parser)
// --------------------------------------------------------------------
bool JSON::parse(const char* json) {
    if (!json) return false; // Handle null input

    skipWhitespace(json);

    // We expect either object '{' or array '[' as the top-level.
    if (*json == '{') {
        root.type = ValueType::Object;
        return parseObject(json, root);
    } else if (*json == '[') {
        root.type = ValueType::Array;
        return parseArray(json, root);
    } else {
        return false; // Not valid JSON
    }
}


bool JSON::parseValue(const char*& p, Node &node) {
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

        char tempBuffer[256];  // Temporary buffer for the string
        if (parseString(p, tempBuffer, sizeof(tempBuffer))) {
            node.stringValue = strdupSafe(tempBuffer); // Allocate memory
            return true;
        }
        return false; // String parsing failed
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

        // Key must be a char*
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

// Parse char* in quotes (p points to opening quote)
bool JSON::parseString(const char*& p, char* out, size_t outSize) {
    if (*p != '\"') return false;
    p++; // Skip opening quote

    size_t index = 0;
    while (*p && index < outSize - 1) { // Ensure space for null terminator
        if (*p == '\"') {
            p++; // Skip closing quote
            out[index] = '\0'; // Null-terminate
            return true;
        }

        // Handle escape sequences (e.g., \n, \", \\)
        if (*p == '\\') {
            p++; // Move past '\'
            switch (*p) {
                case 'n': out[index++] = '\n'; break;
                case 't': out[index++] = '\t'; break;
                case 'r': out[index++] = '\r'; break;
                case '\\': out[index++] = '\\'; break;
                case '\"': out[index++] = '\"'; break;
                default: out[index++] = *p; break; // Copy unknown escape as-is
            }
        } else {
            out[index++] = *p; // Copy character
        }

        p++; // Move to next character
    }

    return false; // Missing closing quote
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
void JSON::serializeNode(const Node &node, char* out, size_t outSize, int indentLevel, bool pretty) const {
    Serial.println("DEBUG: serializeNode called");

    size_t currentLength = strlen(out);
    if (node.type == ValueType::Object) {
        Serial.println("DEBUG: Serializing an object.");
        strncat(out, "{", outSize - currentLength - 1);
        if (pretty) strncat(out, "\n", outSize - strlen(out) - 1);

        if (!node.children) {
            Serial.println("ERROR: node.children is null! Nothing to serialize.");
            return;
        }

        size_t numChildren = node.children->elements();
        Serial.print("DEBUG: Number of children: ");
        Serial.println(numChildren);

        if (numChildren == 0) {
            Serial.println("ERROR: No children found in object, serialization will be empty!");
        }

        bool firstEntry = true;
        for (size_t i = 0; i < numChildren; i++) {
            const Node& child = node.children->get(i);

            Serial.print("DEBUG: Serializing child: ");
            Serial.println(child.key ? child.key : "(null)");

            if (!child.key || strlen(child.key) == 0) {
                Serial.println("ERROR: Child node has no key! Skipping.");
                continue;
            }

            if (!firstEntry) strncat(out, ",", outSize - strlen(out) - 1);
            firstEntry = false;

            if (pretty) {
                for (int s = 0; s < indentLevel + 2; s++) strncat(out, " ", outSize - strlen(out) - 1);
            }

            char temp[512];
            snprintf(temp, sizeof(temp), "\"%s\": ", child.key);
            strncat(out, temp, outSize - strlen(out) - 1);

            serializeValue(child, out, outSize, indentLevel + 2, pretty);

            if (pretty) strncat(out, "\n", outSize - strlen(out) - 1);
        }

        strncat(out, "}", outSize - strlen(out) - 1);
    }
}




void JSON::serializeValue(const Node &node, char* out, size_t outSize, int indentLevel, bool pretty) const {
    Serial.println("DEBUG: serializeValue called");
    size_t currentLength = strlen(out);

    switch (node.type) {
        case ValueType::Null:
            strncat(out, "null", outSize - currentLength - 1);
            break;
        case ValueType::Bool:
            strncat(out, (node.boolValue ? "true" : "false"), outSize - currentLength - 1);
            break;
        case ValueType::Number: {
            char buf[32];
            dtostrf(node.numberValue, 0, 6, buf);  // Arduino function for double-to-string conversion
            strncat(out, buf, outSize - strlen(out) - 1);
        } break;
        case ValueType::String:
            if (node.stringValue && strlen(node.stringValue) > 0) {
                strncat(out, "\"", outSize - strlen(out) - 1);
                strncat(out, node.stringValue, outSize - strlen(out) - 1);
                strncat(out, "\"", outSize - strlen(out) - 1);
            } else {
                strncat(out, "\"\"", outSize - strlen(out) - 1); // Empty string
            }
            break;
        case ValueType::Object:
        case ValueType::Array:
            serializeNode(node, out, outSize, indentLevel, pretty);
            break;
    }
}


// --------------------------------------------------------------------
// Path-based Node Lookup (dot notation) 
// --------------------------------------------------------------------
JSON::Node* JSON::findOrCreateNode(const char* path, bool createIntermediate) {
    if (!path || strlen(path) == 0) {
        Serial.println("ERROR: Empty path in findOrCreateNode!");
        return nullptr;
    }

    Serial.print("DEBUG: Finding/Creating node for path: ");
    Serial.println(path);

    if (!root.children) {
        Serial.println("DEBUG: Initializing root.children");
        root.children = new SimpleVector<Node>();
    }

    // Check if node already exists
    for (size_t i = 0; i < root.children->elements(); i++) {
        if (strcmp(root.children->get(i).key, path) == 0) {
            Serial.println("DEBUG: Node already exists, returning existing node.");
            return &root.children->get(i);
        }
    }

    // If not found, create and store a new node
    Serial.println("DEBUG: Creating a new node.");
    root.children->push_back(Node());  // Push an empty node first
    Node& newNode = root.children->get(root.children->elements() - 1);  // Get reference to the new node
    
    newNode.key = strdupSafe(path);  // Assign key
    newNode.type = ValueType::Object;
    newNode.children = new SimpleVector<Node>();

    Serial.print("DEBUG: Successfully assigned key: ");
    Serial.println(newNode.key ? newNode.key : "(null)");

    return &newNode;
}




JSON::Node* JSON::findNode(const char* path) const {
    // We cast away const here to reuse the same function,
    // but we won't modify anything unless createIntermediate=true.
    return const_cast<JSON*>(this)->findNodeImpl(const_cast<Node*>(&root), path, 0, false);
}

// Recursive helper: path like "obj.arr.0.key"
JSON::Node* JSON::findNodeImpl(Node* current, const char* path, size_t startIndex, bool createIntermediate) const {
    if (!current || !path) {
        Serial.println("ERROR: findNodeImpl received null arguments!");
        return nullptr;
    }

    if (startIndex >= strlen(path)) {
        Serial.println("DEBUG: Reached end of path, returning node.");
        return current;
    }

    Serial.print("DEBUG: Parsing path segment from index ");
    Serial.println(startIndex);

    const char* dotPos = strchr(path + startIndex, '.');
    size_t tokenLength = (dotPos) ? (size_t)(dotPos - (path + startIndex)) : strlen(path + startIndex);

    char token[64];
    strncpy(token, path + startIndex, tokenLength);
    token[tokenLength] = '\0';

    Serial.print("DEBUG: Token found: ");
    Serial.println(token);

    char* dynamicToken = strdupSafe(token);
    if (!dynamicToken) {
        Serial.println("ERROR: strdupSafe failed!");
        return nullptr;
    }

    size_t nextIndex = (dotPos) ? (size_t)(dotPos - path) + 1 : strlen(path);

    if (current->type == ValueType::Array) {
        Serial.println("DEBUG: Node is an array, handling index lookup.");
    } else if (current->type == ValueType::Object) {
        Serial.println("DEBUG: Node is an object, searching for key.");
    } else {
        Serial.println("DEBUG: Node is neither object nor array, creating object.");
        current->type = ValueType::Object;
        current->children = new SimpleVector<Node>();
    }

    free(dynamicToken);
    return current;
}




// --------------------------------------------------------------------
// Remove Helpers
// --------------------------------------------------------------------
bool JSON::removeChild(JSON::Node &parent, const char* keyOrIndex) {
    if (!parent.children || parent.children->elements() == 0) return false;

    if (parent.type == ValueType::Object) {
        for (size_t i = 0; i < parent.children->elements(); i++) {
            if (strcmp(parent.children->get(i).key, keyOrIndex) == 0) {
                // Free the key before deleting the node
                free(parent.children->get(i).key);
                parent.children->erase(i);
                return true;
            }
        }
    } 
    else if (parent.type == ValueType::Array) {
        int index = toInt(keyOrIndex);
        if (index < 0) return false; // Prevent negative indices
        size_t newIndex = static_cast<size_t>(index);

        if (newIndex >= parent.children->elements()) return false; // Prevent out-of-bounds access

        parent.children->erase(newIndex);
        return true;
    }
    
    return false;
}



// --------------------------------------------------------------------
// Utility Functions
// --------------------------------------------------------------------
char* JSON::strdupSafe(const char* src) const {
    if (!src) return nullptr;
    size_t len = strlen(src) + 1;
    char* copy = (char*)malloc(len);
    if (copy) strcpy(copy, src);
    return copy;
}


// Overloaded version to handle `char[]`
char* JSON::strdupSafe(const char src[], size_t length) {
    if (!src || length == 0) return nullptr;
    char* copy = (char*)malloc(length + 1);
    if (copy) {
        strncpy(copy, src, length);
        copy[length] = '\0'; // Ensure null termination
    }
    return copy;
}

const char* JSON::valueTypeToString(ValueType type) const {
    switch (type) {
        case ValueType::Null: return "Null";
        case ValueType::Bool: return "Bool";
        case ValueType::Number: return "Number";
        case ValueType::String: return "String";
        case ValueType::Object: return "Object";
        case ValueType::Array: return "Array";
        default: return "Unknown";
    }
}




