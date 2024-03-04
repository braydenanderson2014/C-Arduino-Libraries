#ifndef BTREE_H
#define BTREE_H

#include <Arduino.h>

template <typename T>
struct BTreeNode {
    T *keys;
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // Array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
};

template <typename T>
class BTree {
private:
    BTreeNode<T> *root; // Pointer to root node
    int t;  // Minimum degree

    void balance(BTreeNode<T>* x) {
        if (x->n < t - 1) {
            // If x has fewer than t-1 keys, we need to balance it
            // Find the sibling node
            BTreeNode<T>* sibling = getSibling(x);

            if (sibling->n > t - 1) {
                // If the sibling has more than t-1 keys, borrow a key from it
                borrowKey(x, sibling);
            } else {
                // If the sibling has t-1 keys, merge x and the sibling
                merge(x, sibling);
            }
        }
    }

    BTreeNode<T>* getSibling(BTreeNode<T>* x) {
        // Find the parent of x
        BTreeNode<T>* parent = findParent(root, x);

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

    BTreeNode<T>* findParent(BTreeNode<T>* x, BTreeNode<T>* child) {
        // If the root is the parent, return the root
        if (x->C[0]->leaf) {
            return x;
        }

        // If the child is in the child array of x, return x
        for (int i = 0; i < x->n; i++) {
            if (x->C[i] == child) {
                return x;
            }
        }

        // If the child is not in the child array of x, search the children of x
        for (int i = 0; i < x->n; i++) {
            BTreeNode<T>* parent = findParent(x->C[i], child);
            if (parent != nullptr) {
                return parent;
            }
        }

        return nullptr;
    }

    void borrowKey(BTreeNode<T>* x, BTreeNode<T>* sibling) {
        // Find the parent of x
        BTreeNode<T>* parent = findParent(root, x);

        // Find the index of x in the parent's child array
        int i;
        for (i = 0; i < parent->n; i++) {
            if (parent->C[i] == x) {
                break;
            }
        }

        // If x is the leftmost child, borrow a key from the right sibling
        if (i == 0) {
            // Move the key from the parent to the left child
            x->keys[x->n] = parent->keys[i];
            x->n++;

            // Move the key from the right sibling to the parent
            parent->keys[i] = sibling->keys[0];

            // Move the child from the right sibling to the left sibling
            x->C[x->n] = sibling->C[0];
            for (int j = 0; j < sibling->n - 1; j++) {
                sibling->keys[j] = sibling->keys[j + 1];
                sibling->C[j] = sibling->C[j + 1];
            }
            sibling->n--;
        } else {
            // Move the key from the parent to the right child
            x->keys[x->n] = parent->keys[i - 1];
            x->n++;

            // Move the key from the left sibling to the parent
            parent->keys[i - 1] = sibling->keys[sibling->n - 1];

            // Move the child from the left sibling to the right sibling
            x->C[x->n] = sibling->C[sibling->n];
            sibling->n--;
        }
    }

    void merge(BTreeNode<T>* x, BTreeNode<T>* sibling) {
        // Find the parent of x
        BTreeNode<T>* parent = findParent(root, x);

        // Find the index of x in the parent's child array
        int i;
        for (i = 0; i < parent->n; i++) {
            if (parent->C[i] == x) {
                break;
            }
        }

        // If x is the leftmost child, merge x and the right sibling
        if (i == 0) {
            // Move the key from the parent to the right sibling
            sibling->keys[sibling->n] = parent->keys[i];

            // Move the keys and children from the right sibling to x
            for (int j = 0; j < x->n; j++) {
                sibling->keys[sibling->n + 1 + j] = x->keys[j];
                sibling->C[sibling->n + 1 + j] = x->C[j];
            }
            sibling->C[sibling->n + 1 + x->n] = x->C[x->n];

            // Update the number of keys in the right sibling
            sibling->n += x->n + 1;

            // Delete x from the parent
            for (int j = i; j < parent->n - 1; j++) {
                parent->keys[j] = parent->keys[j + 1];
                parent->C[j] = parent->C[j + 1];
            }
            parent->n--;
        } else {
            // Move the key from the parent to x
            x->keys[x->n] = parent->keys[i - 1];

            // Move the keys and children from x to the left sibling
            for (int j = 0; j < x->n; j++) {
                sibling->keys[sibling->n + 1 + j] = x->keys[j];
                sibling->C[sibling->n + 1 + j] = x->C[j];
            }
            sibling->C[sibling->n + 1 + x->n] = x->C[x->n];

            // Update the number of keys in the left sibling
            sibling->n += x->n + 1;

            // Delete x from the parent
            for (int j = i - 1; j < parent->n - 1; j++) {
                parent->keys[j] = parent->keys[j + 1];
                parent->C[j] = parent->C[j + 1];
            }
            parent->n--;
        }
    }

    
public:
    BTree(int _t) {
        root = nullptr; 
        t = _t;
    }

    void clear(){
        delete root;
        root = nullptr;
    }

    BTreeNode<T>* getRoot() {
        return root;
    }
    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(BTreeNode<T>* x, T k){
        // Initialize index as index of rightmost element
        int i = x->n - 1;

        // If this is a leaf node
        if (x->leaf == true){
            // The following loop does two things
            // a) Finds the location of new key to be inserted
            // b) Moves all greater keys to one place ahead
            while (i >= 0 && x->keys[i] > k){
                x->keys[i + 1] = x->keys[i];
                i--;
            }

            // Insert the new key at found location
            x->keys[i + 1] = k;
            x->n = x->n + 1;
        } else { // If this node is not leaf
            // Find the child which is going to have the new key
            while (i >= 0 && x->keys[i] > k){
                i--;
            }

            // See if the found child is full
            if (x->C[i + 1]->n == 2 * t - 1){
                // If the child is full, then split it
                splitChild(i + 1, x->C[i + 1], x);

                // After split, the middle key of C[i] goes up and
                // C[i] is split into two.  See which of the two is going to
                // have the new key
                if (x->keys[i + 1] < k){
                    i++;
                }
            }
            insertNonFull(x->C[i + 1], k);
        }
    }

    // A utility function to split the child y of this node. i is index of y in
    // child array C[].  The Child y must be full when this function is called
    void splitChild(int i, BTreeNode<T>* y, BTreeNode<T>* x){
        // Create a new node which is going to store (t-1) keys
        // of y
        BTreeNode<T>* z = new BTreeNode<T>;
        z->n = t - 1;
        z->leaf = y->leaf;

        // Copy the last (t-1) keys of y to z
        for (int j = 0; j < t - 1; j++){
            z->keys[j] = y->keys[j + t];
        }

        // Copy the last t children of y to z
        if (y->leaf == false){
            for (int j = 0; j < t; j++){
                z->C[j] = y->C[j + t];
            }
        }

        // Reduce the number of keys in y
        y->n = t - 1;

        // Since this node is going to have a new child,
        // create space of new child
        for (int j = x->n; j >= i + 1; j--){
            x->C[j + 1] = x->C[j];
        }

        // Link the new child to this node
        x->C[i + 1] = z;

        // A key of y will move to this node. Find the location of
        // new key and move all greater keys one space ahead
        for (int j = x->n - 1; j >= i; j--){
            x->keys[j + 1] = x->keys[j];
        }

        // Copy the middle key of y to this node
        x->keys[i] = y->keys[t - 1];

        // Increment count of keys in this node
        x->n = x->n + 1;
    }

    // Inserts a new key k
    void insert(T k){
        // If tree is empty
        if (root == nullptr){
            // Allocate memory for root
            root = new BTreeNode<T>;
            root->keys = new T[2 * t - 1];
            root->C = new BTreeNode<T>*[2 * t];
            root->leaf = true;
            root->n = 0;
            root->keys[0] = k;
            root->n = 1;
        } else { // If tree is not empty
            // If root is full, then tree grows in height
            if (root->n == 2 * t - 1){
                // Allocate memory for new root
                BTreeNode<T>* s = new BTreeNode<T>;
                s->keys = new T[2 * t - 1];
                s->C = new BTreeNode<T>*[2 * t];
                s->leaf = false;
                s->n = 0;
                s->C[0] = root;

                // Split the old root and move 1 key to the new root
                splitChild(0, root, s);

                // New root has two children now.  Decide which of the
                // two children is going to have new key
                int i = 0;
                if (s->keys[0] < k){
                    i++;
                }
                insertNonFull(s->C[i], k);

                // Change root
                root = s;
            } else { // If root is not full, call insertNonFull for root
                insertNonFull(root, k);
            }
        }
    }

    // Function to traverse all nodes in a subtree rooted with this node
    void traverse(BTreeNode<T>* x){
        // There are n keys and n+1 children, travers through n keys
        // and first n children
        int i;
        for (i = 0; i < x->n; i++){
            // If this is not leaf, then before printing key[i],
            // traverse the subtree rooted with child C[i]
            if (x->leaf == false){
                traverse(x->C[i]);
            }
            Serial.print(x->keys[i]);
            Serial.print(" ");
        }

        // Print the subtree rooted with last child
        if (x->leaf == false){
            traverse(x->C[i]);
        }
    
    }

    // Function to search key k in subtree rooted with this node
    BTreeNode<T>* search(BTreeNode<T>* x, T k){
        // Find the first key greater than or equal to k
        int i = 0;
        while (i < x->n && k > x->keys[i]){
            i++;
        }

        // If the found key is equal to k, return this node
        if (i < x->n && k == x->keys[i]){
            return x;
        }

        // If the key is not found here and this is a leaf node
        if (x->leaf == true){
            return nullptr;
        }

        // Go to the appropriate child
        return search(x->C[i], k);
    }

    // Function to remove the key k from the subtree rooted with this node
    void remove(BTreeNode<T>* x, T k){
        int idx = findKey(x, k);

        // The key to be removed is present in this node
        if (idx < x->n && x->keys[idx] == k){
            // If the node is a leaf node - removeFromLeaf is called
            if (x->leaf){
                removeFromLeaf(x, idx);
            } else {
                // If the node is not a leaf node - removeFromNonLeaf is called
                removeFromNonLeaf(x, idx);
            }
        } else {
            // If this node is a leaf node, then the key is not present in tree
            if (x->leaf){
                Serial.println("The key is does not exist in the tree");
                return;
            }

            // The key to be removed is present in the sub-tree rooted with this node
            // The flag indicates whether the key is present in the sub-tree rooted
            // with the last child of this node
            bool flag = (idx == x->n);

            // If the child where the key is supposed to exist has less that t keys,
            // we fill that child
            if (x->C[idx]->n < t){
                fill(x, idx);
            }

            // If the last child has been merged, it must have merged with the previous
            // child and so we recurse on the (idx-1)th child. Else, we recurse on the
            // (idx)th child which now has at least t keys
            if (flag && idx > x->n){
                remove(x->C[idx - 1], k);
            } else {
                remove(x->C[idx], k);
            }
        }
    }

    // Function to remove the key present in idx-th position in this node which is a leaf
    void removeFromLeaf(BTreeNode<T>* x, int idx){
        // Move all the keys after the idx-th pos one place backward
        for (int i = idx + 1; i < x->n; i++){
            x->keys[i - 1] = x->keys[i];
        }

        // Reduce the count of keys
        x->n--;
    }

    // Function to remove the key present in idx-th position in this node which is a non-leaf node
    void removeFromNonLeaf(BTreeNode<T>* x, int idx){
        T k = x->keys[idx];

        // If the child that precedes k (C[idx]) has at least t keys, then find the
        // predecessor 'pred' of k in the subtree rooted at C[idx]. Replace k by pred.
        // Recursively delete pred in C[idx]
        if (x->C[idx]->n >= t){
            T pred = getPred(x, idx);
            x->keys[idx] = pred;
            remove(x->C[idx], pred);
        } else if (x->C[idx + 1]->n >= t) {
            // If the child C[idx] has less that t keys, examine C[idx+1]. If C[idx+1] has
            // at least t keys, then find the successor 'succ' of k in the subtree rooted at
            // C[idx+1]. Replace k by succ. Recursively delete succ in C[idx+1]
            T succ = getSucc(x, idx);
            x->keys[idx] = succ;
            remove(x->C[idx + 1], succ);
        } else {
            // If both C[idx] and C[idx+1] have less that t keys, then merge k and all of C[idx+1]
            // into C[idx], then free C[idx+1] and recursively delete k from C[idx]
            merge(x, idx);
            remove(x->C[idx], k);
        }
    }

    // Function to get predecessor of keys[idx]
    T getPred(BTreeNode<T>* x, int idx){
        // Keep moving to the right most node until we reach a leaf
        BTreeNode<T>* cur = x->C[idx];
        while (!cur->leaf){
            cur = cur->C[cur->n];
        }

        // Return the last key of the leaf
        return cur->keys[cur->n - 1];
    }

    // Function to get successor of keys[idx]
    T getSucc(BTreeNode<T>* x, int idx){
        // Keep moving the left most node starting from C[idx+1] until we reach a leaf
        BTreeNode<T>* cur = x->C[idx + 1];
        while (!cur->leaf){
            cur = cur->C[0];
        }

        // Return the first key of the leaf
        return cur->keys[0];
    }

    // Function to fill child C[idx] which has less than t-1 keys
    void fill(BTreeNode<T>* x, int idx){
        // If the previous child (C[idx-1]) has more than t-1 keys, borrow a key from that child
        if (idx != 0 && x->C[idx - 1]->n >= t){
            borrowFromPrev(x, idx);
        } else if (idx != x->n && x->C[idx + 1]->n >= t){
            // If the next child (C[idx+1]) has more than t-1 keys, borrow a key from that child
            borrowFromNext(x, idx);
        } else {
            // Merge C[idx] with its sibling
            if (idx != x->n){
                merge(x, idx);
            } else {
                merge(x, idx - 1);
            }
        }
    }

    // Function to borrow a key from C[idx-1] and insert it into C[idx]
    void borrowFromPrev(BTreeNode<T>* x, int idx){
        BTreeNode<T>* child = x->C[idx];
        BTreeNode<T>* sibling = x->C[idx - 1];

        // The last key from C[idx-1] goes up to the parent and key[idx-1] from parent is inserted as the first key in C[idx]. Thus, the number of keys in C[idx] becomes n+1 and the number of keys in C[idx-1] becomes n-1
        for (int i = child->n - 1; i >= 0; i--){
            child->keys[i + 1] = child->keys[i];
        }

        // If C[idx] is not a leaf, move all its child pointers one step ahead
        if (!child->leaf){
            for (int i = child->n; i >= 0; i--){
                child->C[i + 1] = child->C[i];
            }
        }

        // Setting child's first key equal to keys[idx-1] from the current node
        child->keys[0] = x->keys[idx - 1];

        // Moving sibling's last child as C[idx]'s first child
        if (!child->leaf){
            child->C[0] = sibling->C[sibling->n];
        }

        // Moving the key from the sibling to the parent
        // This reduces the number of keys in the sibling
        x->keys[idx - 1] = sibling->keys[sibling->n - 1];

        child->n += 1;
        sibling->n -= 1;
    }

    // Function to borrow a key from C[idx+1] and place it in C[idx]
    void borrowFromNext(BTreeNode<T>* x, int idx){
        BTreeNode<T>* child = x->C[idx];
        BTreeNode<T>* sibling = x->C[idx + 1];

        // keys[idx] is inserted as the last key in C[idx]
        child->keys[(child->n)] = x->keys[idx];

        // Sibling's first child is inserted as the last child into C[idx]
        if (!(child->leaf)){
            child->C[(child->n) + 1] = sibling->C[0];
        }

        // The first key from sibling is inserted into keys[idx]
        x->keys[idx] = sibling->keys[0];

        // Moving all keys in sibling one step behind
        for (int i = 1; i < sibling->n; ++i){
            sibling->keys[i - 1] = sibling->keys[i];
        }

        // Moving the child pointers one step behind
        if (!sibling->leaf){
            for (int i = 1; i <= sibling->n; ++i){
                sibling->C[i - 1] = sibling->C[i];
            }
        }

        // Increasing and decreasing the key count of C[idx] and C[idx+1]
        // respectively
        child->n += 1;
        sibling->n -= 1;
    }

    // A function to merge C[idx] with C[idx+1]
    // C[idx+1] is freed after merging
    void merge(BTreeNode<T>* x, int idx){
        BTreeNode<T>* child = x->C[idx];
        BTreeNode<T>* sibling = x->C[idx + 1];

        // Pulling a key from the current node and inserting it into (t-1)th
        // position of C[idx]
        child->keys[t - 1] = x->keys[idx];

        // Copying the keys from C[idx+1] to C[idx] at the end
        for (int i = 0; i < sibling->n; ++i){
            child->keys[i + t] = sibling->keys[i];
        }

        // Copying the child pointers from C[idx+1] to C[idx]
        if (!child->leaf){
            for (int i = 0; i <= sibling->n; ++i){
                child->C[i + t] = sibling->C[i];
            }
        }

        // Moving all keys after idx in the current node one step before -
        // to fill the gap created by moving keys[idx] to C[idx]
        for (int i = idx + 1; i < x->n; ++i){
            x->keys[i - 1] = x->keys[i];
        }

        // Moving the child pointers after (idx+1) in the current node one
        // step before
        for (int i = idx + 2; i <= x->n; ++i){
            x->C[i - 1] = x->C[i];
        }

        // Updating the key count of child and the current node
        child->n += sibling->n + 1;
        x->n--;

        // Freeing the memory occupied by sibling
        delete(sibling);
    }

    // A function to find the key k in the subtree rooted with this node
    int findKey(BTreeNode<T>* x, T k){
        int idx = 0;
        while (idx < x->n && x->keys[idx] < k){
            ++idx;
        }
        return idx;
    }

    // Function to remove the key k from the tree
    void remove(T k){
        if (!root){
            Serial.println("The tree is empty");
            return;
        }

        // Call the remove function for root
        remove(root, k);

        // If the root node has 0 keys, make its first child as the new root if it has a child, otherwise set root as NULL
        if (root->n == 0){
            BTreeNode<T>* tmp = root;
            if (root->leaf){
                root = nullptr;
            } else {
                root = root->C[0];
            }

            // Free the old root
            delete(tmp);
        }
    }


    // Function to traverse the tree
    void traverse(){
        if (root != nullptr){
            traverse(root);
        }
    }

    // Function to search a key in this tree
    BTreeNode<T>* search(T k){
        return (root == nullptr) ? nullptr : search(root, k);
    }


    

  


};
#endif // BTREE_H