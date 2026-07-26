#include "JSON.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

JSON::Node::Node() : key(nullptr), type(ValueType::Null), stringValue(nullptr), children(nullptr) {}

JSON::Node::Node(const Node& other) : key(nullptr), type(ValueType::Null), stringValue(nullptr), children(nullptr) {
    copyFrom(other);
}

JSON::Node& JSON::Node::operator=(const Node& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

JSON::Node::~Node() {
    clear();
}

void JSON::Node::clear() {
    if (key) {
        free(key);
        key = nullptr;
    }

    if (type == ValueType::String && stringValue) {
        free(stringValue);
    }
    stringValue = nullptr;

    if (children) {
        delete children;
        children = nullptr;
    }

    type = ValueType::Null;
    boolValue = false;
}

void JSON::Node::copyFrom(const Node& other) {
    key = nullptr;
    stringValue = nullptr;
    children = nullptr;
    type = other.type;

    if (other.key) {
        const size_t keyLength = std::strlen(other.key) + 1;
        key = static_cast<char*>(std::malloc(keyLength));
        if (key) {
            std::memcpy(key, other.key, keyLength);
        }
    }

    switch (type) {
        case ValueType::String:
            if (other.stringValue) {
                const size_t stringLength = std::strlen(other.stringValue) + 1;
                stringValue = static_cast<char*>(std::malloc(stringLength));
                if (stringValue) {
                    std::memcpy(stringValue, other.stringValue, stringLength);
                }
            }
            break;
        case ValueType::Number:
            numberValue = other.numberValue;
            break;
        case ValueType::Bool:
            boolValue = other.boolValue;
            break;
        case ValueType::Object:
        case ValueType::Array:
            if (other.children) {
                children = new SimpleVector<Node>(*other.children);
            }
            break;
        case ValueType::Null:
        default:
            boolValue = false;
            break;
    }
}

bool JSON::Node::operator==(const Node& other) const {
    const bool keysEqual =
        (key == nullptr && other.key == nullptr) ||
        (key != nullptr && other.key != nullptr && std::strcmp(key, other.key) == 0);

    if (!keysEqual || type != other.type) {
        return false;
    }

    switch (type) {
        case ValueType::Null:
            return true;
        case ValueType::Bool:
            return boolValue == other.boolValue;
        case ValueType::Number:
            return numberValue == other.numberValue;
        case ValueType::String:
            return (stringValue == nullptr && other.stringValue == nullptr) ||
                   (stringValue != nullptr && other.stringValue != nullptr &&
                    std::strcmp(stringValue, other.stringValue) == 0);
        case ValueType::Object:
        case ValueType::Array:
            if (children == nullptr || other.children == nullptr) {
                return children == other.children;
            }
            if (children->elements() != other.children->elements()) {
                return false;
            }
            for (size_t i = 0; i < children->elements(); ++i) {
                if (!(children->get(i) == other.children->get(i))) {
                    return false;
                }
            }
            return true;
        default:
            return false;
    }
}

JSON::JSON() {
    resetRoot(ValueType::Object);
    sdInstance.begin(SD_CS_PIN);
}

JSON::JSON(size_t cs_pin) {
    resetRoot(ValueType::Object);
    sdInstance.begin(static_cast<uint8_t>(cs_pin));
}

JSON::~JSON() {}

bool JSON::hasKey(const char* path) const {
    return findNode(path) != nullptr;
}

bool JSON::isNull(const char* path) const {
    Node* node = findNode(path);
    return node && node->type == ValueType::Null;
}

int JSON::readFromFile(const char* filename) {
    if (!filename || !*filename) {
        return JSON_FILE_NOT_FOUND;
    }

    FsFile file = sdInstance.open(filename, O_READ);
    if (!file) {
        return JSON_FILE_NOT_FOUND;
    }

    std::string fileData;
    char chunk[256];
    int bytesRead = 0;
    while ((bytesRead = file.read(chunk, sizeof(chunk))) > 0) {
        fileData.append(chunk, static_cast<size_t>(bytesRead));
    }
    file.close();

    if (fileData.empty()) {
        return JSON_READ_ERROR;
    }

    if (fileData.size() > sizeof(uint32_t)) {
        const unsigned char* header = reinterpret_cast<const unsigned char*>(fileData.data());
        const uint32_t expectedSize =
            static_cast<uint32_t>(header[0]) |
            (static_cast<uint32_t>(header[1]) << 8) |
            (static_cast<uint32_t>(header[2]) << 16) |
            (static_cast<uint32_t>(header[3]) << 24);

        if (expectedSize > 0 && fileData.size() > sizeof(uint32_t)) {
            static const uint32_t MAX_DECOMPRESSED_SIZE = 64u * 1024u * 1024u;
            static const uint32_t MAX_COMPRESSION_RATIO = 256u;
            const size_t compressedPayloadSize = fileData.size() - sizeof(uint32_t);
            if (expectedSize <= MAX_DECOMPRESSED_SIZE && expectedSize <= compressedPayloadSize * MAX_COMPRESSION_RATIO) {
                char* decompressedBuffer = static_cast<char*>(std::malloc(static_cast<size_t>(expectedSize) + 1));
                if (!decompressedBuffer) {
                    return JSON_DECOMPRESSION_ERROR;
                }

                const int decompressedSize = LZ4_decompress_safe(
                    fileData.data() + sizeof(uint32_t),
                    decompressedBuffer,
                    static_cast<int>(fileData.size() - sizeof(uint32_t)),
                    static_cast<int>(expectedSize)
                );

                if (decompressedSize >= 0) {
                    decompressedBuffer[decompressedSize] = '\0';
                    const bool parsed = readFromString(decompressedBuffer);
                    std::free(decompressedBuffer);
                    return parsed ? JSON_READ_SUCCESS : JSON_FILE_PARSE_ERROR;
                }

                std::free(decompressedBuffer);
            }
        }
    }

    std::string plainText = fileData;
    plainText.push_back('\0');
    if (readFromString(plainText.c_str())) {
        return JSON_READ_SUCCESS;
    }

    size_t outputCapacity = fileData.size() * 2;
    if (outputCapacity == 0) {
        outputCapacity = 256;
    }

    while (outputCapacity <= 1024 * 1024) {
        char* decompressedBuffer = static_cast<char*>(std::malloc(outputCapacity + 1));
        if (!decompressedBuffer) {
            return JSON_DECOMPRESSION_ERROR;
        }

        const int decompressedSize = LZ4_decompress_safe(
            fileData.data(),
            decompressedBuffer,
            static_cast<int>(fileData.size()),
            static_cast<int>(outputCapacity)
        );

        if (decompressedSize >= 0) {
            decompressedBuffer[decompressedSize] = '\0';
            const bool parsed = readFromString(decompressedBuffer);
            std::free(decompressedBuffer);
            return parsed ? JSON_READ_SUCCESS : JSON_FILE_PARSE_ERROR;
        }

        std::free(decompressedBuffer);
        outputCapacity *= 2;
    }

    return JSON_DECOMPRESSION_ERROR;
}

int JSON::writeToFile(const char* filename, bool pretty) {
    if (!filename || !*filename) {
        return JSON_FILE_OPEN_ERROR;
    }

    FsFile file = sdInstance.open(filename, O_CREAT | O_WRITE | O_TRUNC);
    if (!file) {
        return JSON_FILE_OPEN_ERROR;
    }

    char* serialized = writeToString(pretty);
    if (!serialized) {
        file.close();
        return JSON_WRITE_ERROR;
    }

    const int serializedLength = static_cast<int>(std::strlen(serialized));
    const int compressedCapacity = LZ4_compressBound(serializedLength);
    char* compressedBuffer = static_cast<char*>(std::malloc(static_cast<size_t>(compressedCapacity)));
    if (!compressedBuffer) {
        std::free(serialized);
        file.close();
        return JSON_WRITE_ERROR;
    }

    const int compressedSize =
        LZ4_compress_default(serialized, compressedBuffer, serializedLength, compressedCapacity);

    std::free(serialized);

    if (compressedSize <= 0) {
        std::free(compressedBuffer);
        file.close();
        return JSON_COMPRESSION_ERROR;
    }

    const unsigned char header[4] = {
        static_cast<unsigned char>(serializedLength & 0xFF),
        static_cast<unsigned char>((serializedLength >> 8) & 0xFF),
        static_cast<unsigned char>((serializedLength >> 16) & 0xFF),
        static_cast<unsigned char>((serializedLength >> 24) & 0xFF)
    };

    const bool wroteHeader = file.write(header, sizeof(header)) == sizeof(header);
    const bool wrotePayload =
        file.write(compressedBuffer, static_cast<size_t>(compressedSize)) == static_cast<size_t>(compressedSize);

    std::free(compressedBuffer);

    if (!wroteHeader || !wrotePayload) {
        file.close();
        return JSON_WRITE_ERROR;
    }

    file.close();
    return JSON_WRITE_SUCCESS;
}

bool JSON::readFromString(const char* jsonStr) {
    resetRoot(ValueType::Object);
    return parse(jsonStr);
}

char* JSON::writeToString(bool pretty) const {
    const size_t bufferSize = measureSerializedNode(root, 0, pretty) + 1;
    char* out = static_cast<char*>(std::malloc(bufferSize));
    if (!out) {
        return nullptr;
    }

    out[0] = '\0';
    size_t offset = 0;
    serializeNode(root, out, bufferSize, offset, 0, pretty);
    out[offset] = '\0';
    return out;
}

void JSON::setString(const char* path, const char* value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    releaseNodeValue(*node);
    node->type = ValueType::String;
    node->stringValue = strdupSafe(value ? value : "");
}

void JSON::setNumber(const char* path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    releaseNodeValue(*node);
    node->type = ValueType::Number;
    node->numberValue = value;
}

void JSON::setBool(const char* path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    releaseNodeValue(*node);
    node->type = ValueType::Bool;
    node->boolValue = value;
}

void JSON::setNull(const char* path) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    releaseNodeValue(*node);
    node->type = ValueType::Null;
}

void JSON::pushBack(const char* path, const char* value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    if (node->type != ValueType::Array) {
        releaseNodeValue(*node);
        node->type = ValueType::Array;
        node->children = new SimpleVector<Node>();
    } else if (!node->children) {
        node->children = new SimpleVector<Node>();
    }

    Node child;
    child.type = ValueType::String;
    child.stringValue = strdupSafe(value ? value : "");
    node->children->push_back(child);
}

void JSON::pushBack(const char* path, double value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    if (node->type != ValueType::Array) {
        releaseNodeValue(*node);
        node->type = ValueType::Array;
        node->children = new SimpleVector<Node>();
    } else if (!node->children) {
        node->children = new SimpleVector<Node>();
    }

    Node child;
    child.type = ValueType::Number;
    child.numberValue = value;
    node->children->push_back(child);
}

void JSON::pushBack(const char* path, bool value) {
    Node* node = findOrCreateNode(path, true);
    if (!node) {
        return;
    }

    if (node->type != ValueType::Array) {
        releaseNodeValue(*node);
        node->type = ValueType::Array;
        node->children = new SimpleVector<Node>();
    } else if (!node->children) {
        node->children = new SimpleVector<Node>();
    }

    Node child;
    child.type = ValueType::Bool;
    child.boolValue = value;
    node->children->push_back(child);
}

String JSON::getString(const char* path, const char* defaultVal) const {
    Node* node = findNode(path);
    if (!node) {
        return String(defaultVal ? defaultVal : "");
    }

    switch (node->type) {
        case ValueType::String:
            return String(node->stringValue ? node->stringValue : "");
        case ValueType::Number: {
            char temp[32];
            std::snprintf(temp, sizeof(temp), "%.15g", node->numberValue);
            return String(temp);
        }
        case ValueType::Bool:
            return node->boolValue ? String("true") : String("false");
        case ValueType::Null:
            return String("null");
        default:
            return String(defaultVal ? defaultVal : "");
    }
}

double JSON::getNumber(const char* path, double defaultVal) const {
    Node* node = findNode(path);
    if (!node) {
        return defaultVal;
    }

    switch (node->type) {
        case ValueType::Number:
            return node->numberValue;
        case ValueType::String:
            return node->stringValue ? std::strtod(node->stringValue, nullptr) : defaultVal;
        case ValueType::Bool:
            return node->boolValue ? 1.0 : 0.0;
        default:
            return defaultVal;
    }
}

bool JSON::getBool(const char* path, bool defaultVal) const {
    Node* node = findNode(path);
    if (!node) {
        return defaultVal;
    }

    switch (node->type) {
        case ValueType::Bool:
            return node->boolValue;
        case ValueType::Number:
            return node->numberValue != 0.0;
        case ValueType::String:
            return node->stringValue &&
                   (std::strcmp(node->stringValue, "true") == 0 || std::strcmp(node->stringValue, "1") == 0);
        default:
            return defaultVal;
    }
}

SimpleVector<char*> JSON::getKeys() const {
    SimpleVector<char*> keys;
    if (root.type != ValueType::Object || !root.children) {
        return keys;
    }

    for (size_t i = 0; i < root.children->elements(); ++i) {
        keys.put(root.children->get(i).key);
    }

    return keys;
}

bool JSON::remove(const char* path) {
    if (!path || !*path) {
        return false;
    }

    const char* lastDot = std::strrchr(path, '.');
    if (!lastDot) {
        return removeChild(root, path);
    }

    const size_t parentLength = static_cast<size_t>(lastDot - path);
    char* parentPath = strdupSafe(path, parentLength);
    if (!parentPath) {
        return false;
    }

    Node* parent = findNode(parentPath);
    std::free(parentPath);
    if (!parent) {
        return false;
    }

    return removeChild(*parent, lastDot + 1);
}

bool JSON::parse(const char* json) {
    if (!json) {
        return false;
    }

    skipWhitespace(json);

    bool parsed = false;
    if (*json == '{') {
        root.type = ValueType::Object;
        if (!root.children) {
            root.children = new SimpleVector<Node>();
        }
        parsed = parseObject(json, root);
    } else if (*json == '[') {
        root.type = ValueType::Array;
        if (!root.children) {
            root.children = new SimpleVector<Node>();
        }
        parsed = parseArray(json, root);
    }

    if (!parsed) {
        return false;
    }

    skipWhitespace(json);
    return *json == '\0';
}

bool JSON::parseValue(const char*& p, Node& node) {
    skipWhitespace(p);

    if (*p == '{') {
        node.type = ValueType::Object;
        if (!node.children) {
            node.children = new SimpleVector<Node>();
        }
        return parseObject(p, node);
    }

    if (*p == '[') {
        node.type = ValueType::Array;
        if (!node.children) {
            node.children = new SimpleVector<Node>();
        }
        return parseArray(p, node);
    }

    if (*p == '\"') {
        char tempBuffer[256];
        if (!parseString(p, tempBuffer, sizeof(tempBuffer))) {
            return false;
        }
        node.type = ValueType::String;
        node.stringValue = strdupSafe(tempBuffer);
        return true;
    }

    if (*p == 't' || *p == 'f') {
        node.type = ValueType::Bool;
        return parseBool(p, node.boolValue);
    }

    if (*p == 'n') {
        node.type = ValueType::Null;
        return parseNull(p);
    }

    node.type = ValueType::Number;
    return parseNumber(p, node.numberValue);
}

bool JSON::parseObject(const char*& p, Node& node) {
    if (*p != '{') {
        return false;
    }

    ++p;
    skipWhitespace(p);

    if (!node.children) {
        node.children = new SimpleVector<Node>();
    } else {
        node.children->clear();
    }

    if (*p == '}') {
        ++p;
        return true;
    }

    while (*p) {
        Node child;
        char keyBuffer[256];
        if (!parseString(p, keyBuffer, sizeof(keyBuffer))) {
            return false;
        }

        child.key = strdupSafe(keyBuffer);

        skipWhitespace(p);
        if (*p != ':') {
            return false;
        }

        ++p;
        if (!parseValue(p, child)) {
            return false;
        }

        node.children->push_back(child);

        skipWhitespace(p);
        if (*p == '}') {
            ++p;
            return true;
        }

        if (*p != ',') {
            return false;
        }

        ++p;
        skipWhitespace(p);
    }

    return false;
}

bool JSON::parseArray(const char*& p, Node& node) {
    if (*p != '[') {
        return false;
    }

    ++p;
    skipWhitespace(p);

    if (!node.children) {
        node.children = new SimpleVector<Node>();
    } else {
        node.children->clear();
    }

    if (*p == ']') {
        ++p;
        return true;
    }

    while (*p) {
        Node child;
        if (!parseValue(p, child)) {
            return false;
        }

        node.children->push_back(child);

        skipWhitespace(p);
        if (*p == ']') {
            ++p;
            return true;
        }

        if (*p != ',') {
            return false;
        }

        ++p;
        skipWhitespace(p);
    }

    return false;
}

bool JSON::parseString(const char*& p, char* out, size_t outSize) {
    if (*p != '\"' || !out || outSize == 0) {
        return false;
    }

    ++p;
    size_t index = 0;
    while (*p) {
        if (*p == '\"') {
            ++p;
            out[index] = '\0';
            return true;
        }

        char nextChar = *p;
        if (*p == '\\') {
            ++p;
            if (*p == '\0') {
                return false;
            }

            switch (*p) {
                case 'n':
                    nextChar = '\n';
                    break;
                case 'r':
                    nextChar = '\r';
                    break;
                case 't':
                    nextChar = '\t';
                    break;
                case '\\':
                    nextChar = '\\';
                    break;
                case '\"':
                    nextChar = '\"';
                    break;
                default:
                    nextChar = *p;
                    break;
            }
        }

        if (index + 1 >= outSize) {
            return false;
        }

        out[index++] = nextChar;
        ++p;
    }

    return false;
}

bool JSON::parseBool(const char*& p, bool& out) {
    if (std::strncmp(p, "true", 4) == 0) {
        out = true;
        p += 4;
        return true;
    }

    if (std::strncmp(p, "false", 5) == 0) {
        out = false;
        p += 5;
        return true;
    }

    return false;
}

bool JSON::parseNull(const char*& p) {
    if (std::strncmp(p, "null", 4) != 0) {
        return false;
    }

    p += 4;
    return true;
}

bool JSON::parseNumber(const char*& p, double& out) {
    char* endPtr = nullptr;
    out = std::strtod(p, &endPtr);
    if (endPtr == p) {
        return false;
    }

    p = endPtr;
    return true;
}

void JSON::skipWhitespace(const char*& p) {
    while (*p == ' ' || *p == '\r' || *p == '\n' || *p == '\t') {
        ++p;
    }
}

void JSON::serializeNode(const Node& node, char* out, size_t outSize, size_t& offset, int indentLevel, bool pretty) const {
    if (node.type == ValueType::Array) {
        appendCharToBuffer(out, outSize, offset, '[');

        const size_t childCount = node.children ? node.children->elements() : 0;
        if (pretty && childCount > 0) {
            appendCharToBuffer(out, outSize, offset, '\n');
        }

        for (size_t i = 0; i < childCount; ++i) {
            if (pretty) {
                appendIndent(out, outSize, offset, indentLevel + 2);
            }

            serializeValue(node.children->get(i), out, outSize, offset, indentLevel + 2, pretty);

            if (i + 1 < childCount) {
                appendCharToBuffer(out, outSize, offset, ',');
            }

            if (pretty) {
                appendCharToBuffer(out, outSize, offset, '\n');
            }
        }

        if (pretty && childCount > 0) {
            appendIndent(out, outSize, offset, indentLevel);
        }

        appendCharToBuffer(out, outSize, offset, ']');
        return;
    }

    appendCharToBuffer(out, outSize, offset, '{');

    const size_t childCount = node.children ? node.children->elements() : 0;
    if (pretty && childCount > 0) {
        appendCharToBuffer(out, outSize, offset, '\n');
    }

    for (size_t i = 0; i < childCount; ++i) {
        const Node& child = node.children->get(i);

        if (pretty) {
            appendIndent(out, outSize, offset, indentLevel + 2);
        }

        appendEscapedString(out, outSize, offset, child.key ? child.key : "");
        appendToBuffer(out, outSize, offset, pretty ? ": " : ":");
        serializeValue(child, out, outSize, offset, indentLevel + 2, pretty);

        if (i + 1 < childCount) {
            appendCharToBuffer(out, outSize, offset, ',');
        }

        if (pretty) {
            appendCharToBuffer(out, outSize, offset, '\n');
        }
    }

    if (pretty && childCount > 0) {
        appendIndent(out, outSize, offset, indentLevel);
    }

    appendCharToBuffer(out, outSize, offset, '}');
}

void JSON::serializeValue(const Node& node, char* out, size_t outSize, size_t& offset, int indentLevel, bool pretty) const {
    switch (node.type) {
        case ValueType::Null:
            appendToBuffer(out, outSize, offset, "null");
            break;
        case ValueType::Bool:
            appendToBuffer(out, outSize, offset, node.boolValue ? "true" : "false");
            break;
        case ValueType::Number: {
            char buffer[32];
            std::snprintf(buffer, sizeof(buffer), "%.15g", node.numberValue);
            appendToBuffer(out, outSize, offset, buffer);
            break;
        }
        case ValueType::String:
            appendEscapedString(out, outSize, offset, node.stringValue ? node.stringValue : "");
            break;
        case ValueType::Object:
        case ValueType::Array:
            serializeNode(node, out, outSize, offset, indentLevel, pretty);
            break;
    }
}

void JSON::appendToBuffer(char* out, size_t outSize, size_t& offset, const char* text) const {
    if (!out || outSize == 0 || !text) {
        return;
    }

    while (*text && offset + 1 < outSize) {
        out[offset++] = *text++;
    }

    out[offset < outSize ? offset : outSize - 1] = '\0';
}

void JSON::appendCharToBuffer(char* out, size_t outSize, size_t& offset, char value) const {
    if (!out || outSize == 0 || offset + 1 >= outSize) {
        return;
    }

    out[offset++] = value;
    out[offset] = '\0';
}

void JSON::appendIndent(char* out, size_t outSize, size_t& offset, int indentLevel) const {
    for (int i = 0; i < indentLevel; ++i) {
        appendCharToBuffer(out, outSize, offset, ' ');
    }
}

void JSON::appendEscapedString(char* out, size_t outSize, size_t& offset, const char* text) const {
    appendCharToBuffer(out, outSize, offset, '\"');

    if (text) {
        while (*text) {
            switch (*text) {
                case '\\':
                    appendToBuffer(out, outSize, offset, "\\\\");
                    break;
                case '\"':
                    appendToBuffer(out, outSize, offset, "\\\"");
                    break;
                case '\n':
                    appendToBuffer(out, outSize, offset, "\\n");
                    break;
                case '\r':
                    appendToBuffer(out, outSize, offset, "\\r");
                    break;
                case '\t':
                    appendToBuffer(out, outSize, offset, "\\t");
                    break;
                default:
                    appendCharToBuffer(out, outSize, offset, *text);
                    break;
            }
            ++text;
        }
    }

    appendCharToBuffer(out, outSize, offset, '\"');
}

size_t JSON::measureSerializedNode(const Node& node, int indentLevel, bool pretty) const {
    if (node.type == ValueType::Array) {
        size_t total = 2;
        const size_t childCount = node.children ? node.children->elements() : 0;

        if (pretty && childCount > 0) {
            total += 1;
        }

        for (size_t i = 0; i < childCount; ++i) {
            if (pretty) {
                total += static_cast<size_t>(indentLevel + 2);
            }

            total += measureSerializedValue(node.children->get(i), indentLevel + 2, pretty);

            if (i + 1 < childCount) {
                total += 1;
            }

            if (pretty) {
                total += 1;
            }
        }

        if (pretty && childCount > 0) {
            total += static_cast<size_t>(indentLevel);
        }

        return total;
    }

    size_t total = 2;
    const size_t childCount = node.children ? node.children->elements() : 0;

    if (pretty && childCount > 0) {
        total += 1;
    }

    for (size_t i = 0; i < childCount; ++i) {
        const Node& child = node.children->get(i);

        if (pretty) {
            total += static_cast<size_t>(indentLevel + 2);
        }

        total += measureEscapedString(child.key ? child.key : "");
        total += pretty ? 2 : 1;
        total += measureSerializedValue(child, indentLevel + 2, pretty);

        if (i + 1 < childCount) {
            total += 1;
        }

        if (pretty) {
            total += 1;
        }
    }

    if (pretty && childCount > 0) {
        total += static_cast<size_t>(indentLevel);
    }

    return total;
}

size_t JSON::measureSerializedValue(const Node& node, int indentLevel, bool pretty) const {
    switch (node.type) {
        case ValueType::Null:
            return 4;
        case ValueType::Bool:
            return node.boolValue ? 4 : 5;
        case ValueType::Number: {
            char buffer[32];
            const int written = std::snprintf(buffer, sizeof(buffer), "%.15g", node.numberValue);
            if (written < 0) {
                return 0;
            }
            if (written >= static_cast<int>(sizeof(buffer))) {
                return sizeof(buffer) - 1;
            }
            return static_cast<size_t>(written);
        }
        case ValueType::String:
            return measureEscapedString(node.stringValue ? node.stringValue : "");
        case ValueType::Object:
        case ValueType::Array:
            return measureSerializedNode(node, indentLevel, pretty);
        default:
            return 0;
    }
}

size_t JSON::measureEscapedString(const char* text) const {
    size_t total = 2;
    if (!text) {
        return total;
    }

    while (*text) {
        switch (*text) {
            case '\\':
            case '\"':
            case '\n':
            case '\r':
            case '\t':
                total += 2;
                break;
            default:
                total += 1;
                break;
        }
        ++text;
    }

    return total;
}

JSON::Node* JSON::findOrCreateNode(const char* path, bool createIntermediate) {
    if (!path || !*path) {
        return nullptr;
    }

    if (root.type != ValueType::Object && root.type != ValueType::Array) {
        root.type = ValueType::Object;
    }

    if (!root.children) {
        root.children = new SimpleVector<Node>();
    }

    return findNodeImpl(&root, path, 0, createIntermediate);
}

JSON::Node* JSON::findNode(const char* path) const {
    if (!path || !*path) {
        return nullptr;
    }

    if ((root.type != ValueType::Object && root.type != ValueType::Array) || !root.children) {
        return nullptr;
    }

    return const_cast<JSON*>(this)->findNodeImpl(const_cast<Node*>(&root), path, 0, false);
}

JSON::Node* JSON::findNodeImpl(Node* current, const char* path, size_t startIndex, bool createIntermediate) const {
    if (!current || !path) {
        return nullptr;
    }

    if (path[startIndex] == '\0') {
        return current;
    }

    size_t endIndex = startIndex;
    while (path[endIndex] != '\0' && path[endIndex] != '.') {
        ++endIndex;
    }

    const size_t tokenLength = endIndex - startIndex;
    if (tokenLength == 0) {
        return nullptr;
    }

    char* token = strdupSafe(path + startIndex, tokenLength);
    if (!token) {
        return nullptr;
    }

    Node* next = nullptr;

    if (current->type == ValueType::Array) {
        if (!isArrayIndex(token) || !current->children) {
            std::free(token);
            return nullptr;
        }

        const size_t index = static_cast<size_t>(std::strtoul(token, nullptr, 10));
        if (index < current->children->elements()) {
            next = &current->children->get(index);
        } else if (createIntermediate) {
            while (current->children->elements() <= index) {
                current->children->push_back(Node());
            }
            next = &current->children->get(index);
        }
    } else {
        if (current->type != ValueType::Object) {
            if (!createIntermediate) {
                std::free(token);
                return nullptr;
            }

            if (current->type == ValueType::String && current->stringValue) {
                std::free(current->stringValue);
                current->stringValue = nullptr;
            }

            current->type = ValueType::Object;
            if (!current->children) {
                current->children = new SimpleVector<Node>();
            } else {
                current->children->clear();
            }
        }

        if (!current->children) {
            current->children = new SimpleVector<Node>();
        }

        for (size_t i = 0; i < current->children->elements(); ++i) {
            Node& candidate = current->children->get(i);
            if (candidate.key && std::strcmp(candidate.key, token) == 0) {
                next = &candidate;
                break;
            }
        }

        if (!next && createIntermediate) {
            Node child;
            child.key = strdupSafe(token);
            current->children->push_back(child);
            next = &current->children->get(current->children->elements() - 1);
        }
    }

    std::free(token);

    if (!next) {
        return nullptr;
    }

    if (path[endIndex] == '\0') {
        return next;
    }

    return findNodeImpl(next, path, endIndex + 1, createIntermediate);
}

bool JSON::removeChild(Node& parent, const char* keyOrIndex) {
    if (!parent.children || parent.children->elements() == 0 || !keyOrIndex) {
        return false;
    }

    if (parent.type == ValueType::Object) {
        for (size_t i = 0; i < parent.children->elements(); ++i) {
            if (parent.children->get(i).key && std::strcmp(parent.children->get(i).key, keyOrIndex) == 0) {
                parent.children->erase(static_cast<int>(i));
                return true;
            }
        }
        return false;
    }

    if (parent.type == ValueType::Array) {
        if (!isArrayIndex(keyOrIndex)) {
            return false;
        }

        const size_t index = static_cast<size_t>(toInt(keyOrIndex));
        if (index >= parent.children->elements()) {
            return false;
        }

        parent.children->erase(static_cast<int>(index));
        return true;
    }

    return false;
}

bool JSON::isArrayIndex(const char* token) const {
    if (!token || !*token) {
        return false;
    }

    for (const char* cursor = token; *cursor; ++cursor) {
        if (*cursor < '0' || *cursor > '9') {
            return false;
        }
    }

    return true;
}

void JSON::resetRoot(ValueType type) {
    root.clear();
    root.type = type;
    if (type == ValueType::Object || type == ValueType::Array) {
        root.children = new SimpleVector<Node>();
    }
}

void JSON::releaseNodeValue(Node& node) {
    if (node.type == ValueType::String && node.stringValue) {
        std::free(node.stringValue);
        node.stringValue = nullptr;
    }

    if ((node.type == ValueType::Object || node.type == ValueType::Array) && node.children) {
        delete node.children;
        node.children = nullptr;
    }

    node.type = ValueType::Null;
    node.boolValue = false;
}

char* JSON::strdupSafe(const char* src) const {
    if (!src) {
        return nullptr;
    }

    return strdupSafe(src, std::strlen(src));
}

char* JSON::strdupSafe(const char* src, size_t length) const {
    if (!src) {
        return nullptr;
    }

    char* copy = static_cast<char*>(std::malloc(length + 1));
    if (!copy) {
        return nullptr;
    }

    std::memcpy(copy, src, length);
    copy[length] = '\0';
    return copy;
}
