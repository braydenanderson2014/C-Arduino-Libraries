#ifndef AVL_TREE_H
#define AVL_TREE_H

// ---------------------------------------------------------------------------
// Platform detection
//
// Arduino (AVR, SAMD, ESP32/ESP8266 Arduino framework, Teensy, …)
//   ARDUINO is always defined by the Arduino build system.
//
// Non-Arduino platforms (ESP-IDF, mbed, desktop/host, …)
//   Only standard C/C++ headers are used.  By default the print helpers
//   fall back to std::cout.  Define AVL_TREE_NO_STD_IO=1 before including
//   this header (or pass -DAVL_TREE_NO_STD_IO=1) to disable std::cout and
//   make the print helpers compile to no-ops — useful on bare-metal targets
//   that ship without a C++ iostream implementation.
// ---------------------------------------------------------------------------
#if defined(ARDUINO)
  #include <Arduino.h>
  #define _AVL_PRINT(x)          Serial.print(x)
  #define _AVL_PRINTLN(x)        Serial.println(x)
  #define _AVL_PRINTLN_EMPTY()   Serial.println()
#else
  #include <stddef.h>  // size_t
  #ifndef AVL_TREE_NO_STD_IO
    #include <iostream>
    #define _AVL_PRINT(x)        (static_cast<void>(std::cout << (x)))
    #define _AVL_PRINTLN(x)      (static_cast<void>(std::cout << (x) << '\n'))
    #define _AVL_PRINTLN_EMPTY() (static_cast<void>(std::cout << '\n'))
  #else
    // No I/O available — print methods compile to no-ops
    #define _AVL_PRINT(x)        ((void)0)
    #define _AVL_PRINTLN(x)      ((void)0)
    #define _AVL_PRINTLN_EMPTY() ((void)0)
  #endif
#endif

// Define AVL_TREE_ENABLE_ERROR_CODES=1 before including this header (or via
// a compiler flag) to enable lightweight error-code tracking.  When disabled
// (the default) the feature compiles away completely, saving flash and RAM on
// resource-constrained microcontrollers.
#ifndef AVL_TREE_ENABLE_ERROR_CODES
#define AVL_TREE_ENABLE_ERROR_CODES 0
#endif

#if AVL_TREE_ENABLE_ERROR_CODES
#define AVL_ERR_NONE       0  // no error
#define AVL_ERR_DUPLICATE  1  // insert ignored: key already present
#define AVL_ERR_NOT_FOUND  2  // remove/find: key not in tree
#define AVL_ERR_EMPTY      3  // findMin/findMax called on empty tree
#define AVL_ERR_ALLOC      4  // memory allocation failed
#endif

template <typename T>
class AVLTree {
private:
    struct AVLNode {
        T data;
        AVLNode* left;
        AVLNode* right;
        int height;

        explicit AVLNode(const T& value)
            : data(value), left(nullptr), right(nullptr), height(0) {}
    };

    AVLNode* root;
    size_t nodeCount;
#if AVL_TREE_ENABLE_ERROR_CODES
    mutable int lastError;
#endif

    static int maxInt(int a, int b) {
        return (a > b) ? a : b;
    }

    static int heightOf(const AVLNode* node) {
        return node ? node->height : -1;
    }

    static int balanceOf(const AVLNode* node) {
        return node ? (heightOf(node->left) - heightOf(node->right)) : 0;
    }

    static AVLNode* rotateRight(AVLNode* node) {
        AVLNode* newRoot = node->left;
        AVLNode* transfer = newRoot->right;

        newRoot->right = node;
        node->left = transfer;

        node->height = 1 + maxInt(heightOf(node->left), heightOf(node->right));
        newRoot->height = 1 + maxInt(heightOf(newRoot->left), heightOf(newRoot->right));
        return newRoot;
    }

    static AVLNode* rotateLeft(AVLNode* node) {
        AVLNode* newRoot = node->right;
        AVLNode* transfer = newRoot->left;

        newRoot->left = node;
        node->right = transfer;

        node->height = 1 + maxInt(heightOf(node->left), heightOf(node->right));
        newRoot->height = 1 + maxInt(heightOf(newRoot->left), heightOf(newRoot->right));
        return newRoot;
    }

    static AVLNode* rebalance(AVLNode* node) {
        if (!node) {
            return nullptr;
        }

        node->height = 1 + maxInt(heightOf(node->left), heightOf(node->right));
        const int balance = balanceOf(node);

        if (balance > 1) {
            if (balanceOf(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            if (balanceOf(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    static AVLNode* findMinNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    static const AVLNode* findMinNode(const AVLNode* node) {
        const AVLNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    static AVLNode* findMaxNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->right) {
            current = current->right;
        }
        return current;
    }

    static const AVLNode* findMaxNode(const AVLNode* node) {
        const AVLNode* current = node;
        while (current && current->right) {
            current = current->right;
        }
        return current;
    }

    static AVLNode* findNode(AVLNode* node, const T& data) {
        AVLNode* current = node;
        while (current) {
            if (data < current->data) {
                current = current->left;
            } else if (data > current->data) {
                current = current->right;
            } else {
                return current;
            }
        }
        return nullptr;
    }

    static const AVLNode* findNode(const AVLNode* node, const T& data) {
        const AVLNode* current = node;
        while (current) {
            if (data < current->data) {
                current = current->left;
            } else if (data > current->data) {
                current = current->right;
            } else {
                return current;
            }
        }
        return nullptr;
    }

    static AVLNode* cloneNode(const AVLNode* node) {
        if (!node) {
            return nullptr;
        }
        AVLNode* copy = new AVLNode(node->data);
        copy->height = node->height;
        copy->left = cloneNode(node->left);
        copy->right = cloneNode(node->right);
        return copy;
    }

    static void clearNode(AVLNode* node) {
        if (!node) {
            return;
        }
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

    static void inOrderPrint(const AVLNode* node) {
        if (!node) {
            return;
        }
        inOrderPrint(node->left);
        _AVL_PRINTLN(node->data);
        inOrderPrint(node->right);
    }

    static void preOrderPrint(AVLNode* node) {
        if (!node) {
            return;
        }
        _AVL_PRINTLN(node->data);
        preOrderPrint(node->left);
        preOrderPrint(node->right);
    }

    static void postOrderPrint(AVLNode* node) {
        if (!node) {
            return;
        }
        postOrderPrint(node->left);
        postOrderPrint(node->right);
        _AVL_PRINTLN(node->data);
    }

    static void printTree(AVLNode* node, int space) {
        if (!node) {
            return;
        }

        space += 10;
        printTree(node->right, space);
        _AVL_PRINTLN_EMPTY();
        for (int i = 10; i < space; ++i) {
            _AVL_PRINT(" ");
        }
        _AVL_PRINTLN(node->data);
        printTree(node->left, space);
    }

    AVLNode* insertNode(AVLNode* node, const T& data, bool& inserted) {
        if (!node) {
            AVLNode* newNode = new AVLNode(data);
#if AVL_TREE_ENABLE_ERROR_CODES
            if (!newNode) {
                lastError = AVL_ERR_ALLOC;
                return nullptr;
            }
#endif
            inserted = true;
            return newNode;
        }

        if (data < node->data) {
            node->left = insertNode(node->left, data, inserted);
        } else if (data > node->data) {
            node->right = insertNode(node->right, data, inserted);
        } else {
            return node;
        }

        return rebalance(node);
    }

    AVLNode* removeNode(AVLNode* node, const T& data, bool& removed) {
        if (!node) {
            return nullptr;
        }

        if (data < node->data) {
            node->left = removeNode(node->left, data, removed);
        } else if (data > node->data) {
            node->right = removeNode(node->right, data, removed);
        } else {
            removed = true;
            if (!node->left || !node->right) {
                AVLNode* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }

            AVLNode* successor = findMinNode(node->right);
            node->data = successor->data;
            bool successorRemoved = false;
            node->right = removeNode(node->right, successor->data, successorRemoved);
        }

        return rebalance(node);
    }

public:
    AVLTree() : root(nullptr), nodeCount(0)
#if AVL_TREE_ENABLE_ERROR_CODES
        , lastError(AVL_ERR_NONE)
#endif
    {}

    AVLTree(const AVLTree& other)
        : root(cloneNode(other.root)), nodeCount(other.nodeCount)
#if AVL_TREE_ENABLE_ERROR_CODES
        , lastError(AVL_ERR_NONE)
#endif
    {}

    AVLTree& operator=(const AVLTree& other) {
        if (this == &other) {
            return *this;
        }

        AVLNode* newRoot = cloneNode(other.root);
        clearNode(root);
        root = newRoot;
        nodeCount = other.nodeCount;
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = AVL_ERR_NONE;
#endif
        return *this;
    }

    AVLTree(AVLTree&& other) noexcept
        : root(other.root), nodeCount(other.nodeCount)
#if AVL_TREE_ENABLE_ERROR_CODES
        , lastError(other.lastError)
#endif
    {
        other.root = nullptr;
        other.nodeCount = 0;
#if AVL_TREE_ENABLE_ERROR_CODES
        other.lastError = AVL_ERR_NONE;
#endif
    }

    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        clearNode(root);
        root = other.root;
        nodeCount = other.nodeCount;
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = other.lastError;
        other.lastError = AVL_ERR_NONE;
#endif
        other.root = nullptr;
        other.nodeCount = 0;
        return *this;
    }

    ~AVLTree() {
        clear();
    }

    void insert(const T& data) {
        bool inserted = false;
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = AVL_ERR_NONE;
#endif
        root = insertNode(root, data, inserted);
        if (inserted) {
            ++nodeCount;
        }
#if AVL_TREE_ENABLE_ERROR_CODES
        else if (lastError == AVL_ERR_NONE) {
            lastError = AVL_ERR_DUPLICATE;
        }
#endif
    }

    void remove(const T& data) {
        bool removed = false;
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = AVL_ERR_NONE;
#endif
        root = removeNode(root, data, removed);
        if (removed && nodeCount > 0) {
            --nodeCount;
        }
#if AVL_TREE_ENABLE_ERROR_CODES
        else if (!removed) {
            lastError = AVL_ERR_NOT_FOUND;
        }
#endif
    }

    bool erase(const T& data) {
        const size_t before = nodeCount;
        remove(data);
        return nodeCount != before;
    }

    void deleteNode(const T& data) {
        remove(data);
    }

    bool contains(const T& data) const {
        return findNode(static_cast<const AVLNode*>(root), data) != nullptr;
    }

    T find(const T& data) const {
        const AVLNode* found = findNode(static_cast<const AVLNode*>(root), data);
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = found ? AVL_ERR_NONE : AVL_ERR_NOT_FOUND;
#endif
        return found ? found->data : T();
    }

    T findMin() const {
#if AVL_TREE_ENABLE_ERROR_CODES
        if (!root) {
            lastError = AVL_ERR_EMPTY;
            return T();
        }
        lastError = AVL_ERR_NONE;
#endif
        const AVLNode* node = findMinNode(static_cast<const AVLNode*>(root));
        return node ? node->data : T();
    }

    T findMax() const {
#if AVL_TREE_ENABLE_ERROR_CODES
        if (!root) {
            lastError = AVL_ERR_EMPTY;
            return T();
        }
        lastError = AVL_ERR_NONE;
#endif
        const AVLNode* node = findMaxNode(static_cast<const AVLNode*>(root));
        return node ? node->data : T();
    }

    int height() const {
        return heightOf(root);
    }

    int getBalance() const {
        return balanceOf(root);
    }

    size_t size() const {
        return nodeCount;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
        nodeCount = 0;
#if AVL_TREE_ENABLE_ERROR_CODES
        lastError = AVL_ERR_NONE;
#endif
    }

    void inOrder() const {
        inOrderPrint(root);
    }

    void preOrder() const {
        preOrderPrint(root);
    }

    void postOrder() const {
        postOrderPrint(root);
    }

    void print() const {
        inOrder();
    }

    void printTree() const {
        printTree(root, 0);
    }

#if AVL_TREE_ENABLE_ERROR_CODES
    // Returns the error code set by the most recent mutating or querying
    // operation, or AVL_ERR_NONE (0) when the operation succeeded.
    int getLastError() const {
        return lastError;
    }

    // Clears the stored error code back to AVL_ERR_NONE.
    void clearLastError() {
        lastError = AVL_ERR_NONE;
    }
#endif
};

#endif // AVL_TREE_H
