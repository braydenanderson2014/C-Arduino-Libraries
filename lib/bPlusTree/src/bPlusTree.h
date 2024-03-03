#ifndef B_PLUS_TREE_h
#define B_PLUS_TREE_h

#include <Arduino.h>
template <typename T>
struct BPlusTreeNode {
    T *keys;
    int t;      // Minimum degree (defines the range for number of keys)
    BPlusTreeNode **C; // Array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
};

template <typename T>
class BPlusTree {
private:
    BPlusTreeNode<T> *root; // Pointer to root node
    int t;  // Minimum degree

    void balance(BPlusTreeNode<T>* x) {
    if (x->n < t - 1) {
        // If x has fewer than t-1 keys, we need to balance it
        // Find the sibling node
        BPlusTreeNode<T>* sibling = getSibling(x);

        if (sibling->n > t - 1) {
            // If the sibling has more than t-1 keys, borrow a key from it
            borrowKey(x, sibling);
        } else {
            // If the sibling has t-1 keys, merge x and the sibling
            merge(x, sibling);
        }
    }
}

BPlusTreeNode<T>* getSibling(BPlusTreeNode<T>* x) {
    // Find the parent of x
    BPlusTreeNode<T>* parent = findParent(root, x);

    // Find the index of x in the parent's child array
    int i;
    for (i = 0; i < parent->n; i++) {
        if (parent->C[i] == x) {
            break;
        }
    }

    // If x is the leftmost child, return the right sibling
    if (i == 0) {
        return parent->C[i + 1];
    }

    // If x is the rightmost child, return the left sibling
    if (i == parent->n) {
        return parent->C[i - 1];
    }

    // If x is in the middle, return the left sibling
    return parent->C[i - 1];
}

BPlusTreeNode<T>* findParent(BPlusTreeNode<T>* x, BPlusTreeNode<T>* child) {
    // If the root is the parent of x, return the root
    if (x == child) {
        return nullptr;
    }

    // If the child is in the child array of x, then x is the parent
    for (int i = 0; i < x->n + 1; i++) {
        if (x->C[i] == child) {
            return x;
        }
    }

    // If the child is not in the child array of x, recursively find the parent
    for (int i = 0; i < x->n + 1; i++) {
        BPlusTreeNode<T>* parent = findParent(x->C[i], child);
        if (parent != nullptr) {
            return parent;
        }
    }

    // If the child is not in any child array, return nullptr
    return nullptr;
}

void borrowKey(BPlusTreeNode<T>* x, BPlusTreeNode<T>* sibling) {
    // Find the parent of x
    BPlusTreeNode<T>* parent = findParent(root, x);

    // Find the index of x in the parent's child array
    int i;
    for (i = 0; i < parent->n; i++) {
        if (parent->C[i] == x) {
            break;
        }
    }

    // If x is the leftmost child, borrow a key from the right sibling
    if (i == 0) {
        // Move the parent's key to the left sibling
        sibling->keys[sibling->n] = parent->keys[i];
        sibling->n++;

        // Move the first key of x to the parent
        parent->keys[i] = x->keys[0];

        // Move the child pointer from the right sibling to x
        if (!sibling->leaf) {
            x->C[x->n + 1] = sibling->C[0];
            for (int j = 0; j < sibling->n; j++) {
                sibling->C[j] = sibling->C[j + 1];
            }
        }

        // Move the keys of x to the left
        for (int j = 0; j < x->n - 1; j++) {
            x->keys[j] = x->keys[j + 1];
        }

        // Move the child pointers of x to the left
        for (int j = 0; j < x->n; j++) {
            x->C[j] = x->C[j + 1];
        }

        x->n--;
    } else {
        // If x is the rightmost child, borrow a key from the left sibling
        // Move the parent's key to the right sibling
        for (int j = sibling->n; j > 0; j--) {
            sibling->keys[j] = sibling->keys[j - 1];
        }
        sibling->keys[0] = parent->keys[i - 1];
        sibling->n++;

        // Move the last key of x to the parent
        parent->keys[i - 1] = x->keys[x->n - 1];

        // Move the child pointer from the left sibling to x
        if (!sibling->leaf) {
            x->C[x->n + 1] = x->C[x->n];
        }

        // Move the keys of x to the right
        x->n--;

        // Move the child pointers of x to the right
    }
}

void merge(BPlusTreeNode<T>* x, BPlusTreeNode<T>* sibling) {
    // Find the parent of x
    BPlusTreeNode<T>* parent = findParent(root, x);

    // Find the index of x in the parent's child array
    int i;
    for (i = 0; i < parent->n; i++) {
        if (parent->C[i] == x) {
            break;
        }
    }

    // If x is the leftmost child, merge it with the right sibling
    if (i == 0) {
        // Move the parent's key to the right sibling
        sibling->keys[sibling->n] = parent->keys[i];
        sibling->n++;

        // Move the keys of x to the right sibling
        for (int j = 0; j < x->n; j++) {
            sibling->keys[sibling->n + j] = x->keys[j];
        }
        sibling->n += x->n;

        // Adjust the child pointers of the right sibling
        if (!sibling->leaf) {
            for (int j = 0; j < x->n + 1; j++) {
                sibling->C[sibling->n + j] = x->C[j];
            }
        }

        // Remove the parent's key and pointer
        for (int j = i; j < parent->n - 1; j++) {
            parent->keys[j] = parent->keys[j + 1];
        }
        for (int j = i + 1; j < parent->n; j++) {
            parent->C[j] = parent->C[j + 1];
        }
        parent->n--;
    } else {
        // If x is the rightmost child, merge it with the left sibling
        // Move the parent's key to x
        x->keys[x->n] = parent->keys[i - 1];
        x->n++;

        // Move the keys of the right sibling to x
        for (int j = 0; j < sibling->n; j++) {
            x->keys[x->n + j] = sibling->keys[j];
        }
        x->n += sibling->n;

        // Adjust the child pointers of x
        if (!sibling->leaf) {
            for (int j = 0; j < sibling->n + 1; j++) {
                x->C[x->n + j] = sibling->C[j];
            }
        }

        // Remove the parent's key and pointer
        for (int j = i - 1; j < parent->n - 1; j++) {
            parent->keys[j] = parent->keys[j + 1];
        }
        for (int j = i; j < parent->n; j++) {
            parent->C[j] = parent->C[j + 1];
        }
        parent->n--;
    }
}


public:
    BPlusTree(int _t) {
        root = nullptr; 
        t = _t;
    }

    BPlusTreeNode<T>* getRoot() {
        return root;
    }
    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(BPlusTreeNode<T>* x, T k);

    // A utility function to split the child y of this node. i is index of y in
    // child array C[].  The Child y must be full when this function is called
    void splitChild(int i, BPlusTreeNode<T>* y, BPlusTreeNode<T>* x);

    // Inserts a new key k
    void insert(T k);

    // Function to traverse all nodes in a subtree rooted with this node
    void traverse(BPlusTreeNode<T>* x);

    // Function to search key k in subtree rooted with this node
    BPlusTreeNode<T>* search(BPlusTreeNode<T>* x, T k);

    void remove(T k) {
        if (!root) {
            Serial.println("The tree is empty\n");
            return;
        }

        // Call the remove function for root
        root->remove(k);

        // If the root node has 0 keys, make its first child as the new root
        // if it has a child, otherwise set root as NULL
        if (root->n==0) {
            BPlusTreeNode<T> *tmp = root;
            if (root->leaf)
                root = NULL;
            else
                root = root->C[0];

            // Free the old root
            delete tmp;
        }
        return;
    }
};
#endif // B_PLUS_TREE_h