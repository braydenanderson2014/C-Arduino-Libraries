#ifndef JSON_H
#define JSON_H

#include <CustomString.h>
#include <SimpleVector.h>
#include <SD.h>

class JSON {
public:
    struct Node {
        Custom_String::String key;
        Custom_String::String value;
        SimpleVector<Node> children;

        Node(const Custom_String::String& key = "", const Custom_String::String& value = "") : key(key), value(value) {}
    };

private:
    Node root;

public:
    JSON() {}

    // Adds or updates a value by its path, supporting nested structures.
    void set(const Custom_String::String& path, const Custom_String::String& value) {
        auto& node = findOrCreateNode(path);
        node.value = value;
    }

    // Retrieves a value by its path as a string. Returns an empty string if not found.
    Custom_String::String get(const Custom_String::String& path) const {
        const Node* node = findNode(path);
        if (node) {
            return node->value;
        }
        return "";
    }

    // Writes the JSON structure to a file.
    void writeToFile(const Custom_String::String& filename) {
        File file = SD.open(filename.C_STR(), FILE_WRITE);
        if (file) {
            Custom_String::String jsonStr;
            serializeNode(root, jsonStr, 0);
            file.println(jsonStr.C_STR());
            file.close();
        }
    }
    
    //Deserializes the JSON structure from a string.
    void readFromFile(const Custom_String::String& filename) {
        File file = SD.open(filename.C_STR(), FILE_READ);
        if (file) {
            String jsonStr =file.readString();
            Custom_String::String jsonSTR = jsonStr.c_str();
            file.close();
            deserializeNode(jsonSTR, root);
        }
    }

private:
    // Recursively finds or creates a node based on a path.
    Node& findOrCreateNode(const Custom_String::String& path) {
        size_t pos = 0;
        Node* node = &root;
        while ((pos = path.indexOf('.', pos)) != Custom_String::String::NPOS) {
            Custom_String::String key = path.Sub_String(0, pos);
            node->children.push_back(Node(key));
            node = &node->children.back();
            pos++;
        }
        return *node;
    }

    // Recursively searches for a node by path.
    const Node* findNode(const Custom_String::String& path, const Node* currentNode = nullptr) const {
        if (!currentNode) {
            currentNode = &root;
        }
        size_t pos = path.indexOf('.');
        if (pos == Custom_String::String::NPOS) {
            for (const auto& child : currentNode->children) {
                if (child.key == path) {
                    return &child;
                }
            }
        } else {
            Custom_String::String key = path.Sub_String(0, pos);
            Custom_String::String rest = path.Sub_String(pos + 1, path.size() - pos - 1);
            for (const auto& child : currentNode->children) {
                if (child.key == key) {
                    return findNode(rest, &child);
                }
            }
        }
        return nullptr;
    }

    void serializeNode(const Node& node, Custom_String::String& output, int depth) const {
    for (const auto& child : node.children) {
        // Create indentation String
        Custom_String::String indentation(depth * 2, ' ');
        
        // Key with quotes
        Custom_String::String keyWithQuotes = (Custom_String::String)"\"" + child.key + "\": ";
        
        // Concatenate indentation, key with quotes
        output += indentation + keyWithQuotes;

        if (!child.children.isEmpty()) {
            output += "{\n";
            serializeNode(child, output, depth + 1);
            output += indentation + "},\n";
        } else {
            // Value with quotes
            Custom_String::String valueWithQuotes = (Custom_String::String)"\"" + child.value + "\",\n";
            output += valueWithQuotes;
        }
    }
}


    // Deserializes the JSON structure from a string.
    void deserializeNode(const Custom_String::String& input, Node& node) {
        size_t pos = 0;
        while (pos < input.size()) {
            size_t keyStart = input.indexOf('\"', pos);
            if (keyStart == Custom_String::String::NPOS) {
                break;
            }
            size_t keyEnd = input.indexOf('\"', keyStart + 1);
            if (keyEnd == Custom_String::String::NPOS) {
                break;
            }
            size_t valueStart = input.indexOf('\"', keyEnd + 1);
            if (valueStart == Custom_String::String::NPOS) {
                break;
            }
            size_t valueEnd = input.indexOf('\"', valueStart + 1);
            if (valueEnd == Custom_String::String::NPOS) {
                break;
            }
            Custom_String::String key = input.Sub_String(keyStart + 1, keyEnd - keyStart - 1);
            Custom_String::String value = input.Sub_String(valueStart + 1, valueEnd - valueStart - 1);
            node.children.push_back(Node(key, value));
            pos = valueEnd + 1;
        }
    }
};

#endif // JSON_H
