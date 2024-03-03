#ifndef INTERVAL_TREE_h
#define INTERVAL_TREE_h

#include <Arduino.h>
#include <TypeTraits.h>

template <typename T>
struct Interval {
    static_assert(is_arithmetic<T>::value, "Interval only supports int types");
    T low, high;
};

template <typename T>
struct IntervalTreeNode {
    static_assert(is_arithmetic<T>::value, "IntervalTreeNode only supports int types");
    Interval<T> i; // interval
    int max; // max endpoint in subtree rooted with this node
    IntervalTreeNode *left, *right;
};

template <typename T>
class IntervalTree {
    static_assert(is_arithmetic<T>::value, "IntervalTree only supports int types");
    IntervalTreeNode<T>* root = nullptr;

    IntervalTreeNode<T>* insert(IntervalTreeNode<T>* node, Interval<T> i){
        if(node == nullptr){
            return new IntervalTreeNode<T>(i);
        }
        int l = node->i.low;
        if(i.low < l){
            node->left = insert(node->left, i);
        } else {
            node->right = insert(node->right, i);
        }
        if(node->max < i.high){
            node->max = i.high;
        }
        return node;
    }

    IntervalTreeNode<T>* deleteNode(IntervalTreeNode<T>* node, Interval<T> i){
        if(node == nullptr){
            return node;
        }
        if(i.low < node->i.low){
            node->left = deleteNode(node->left, i);
        } else if(i.low > node->i.low){
            node->right = deleteNode(node->right, i);
        } else {
            if(node->left == nullptr){
                IntervalTreeNode<T>* temp = node->right;
                delete node;
                return temp;
            } else if(node->right == nullptr){
                IntervalTreeNode<T>* temp = node->left;
                delete node;
                return temp;
            }
            IntervalTreeNode<T>* temp = minValueNode(node->right);
            node->i = temp->i;
            node->right = deleteNode(node->right, temp->i);
        }
        return node;
    }
    void inorder(IntervalTreeNode<T>* node){
        if(node == nullptr){
            return;
        }
        inorder(node->left);
        Serial.print("[" + String(node->i.low) + ", " + String(node->i.high) + "] ");
        inorder(node->right);
    }

    bool doOverlap(Interval<T> i1, Interval<T> i2){
        if(i1.low <= i2.high && i2.low <= i1.high){
            return true;
        }
        return false;
    }

    IntervalTreeNode<T>* search(IntervalTreeNode<T>* node, Interval<T> i){
        if(node == nullptr){
            return nullptr;
        }
        if(doOverlap(node->i, i)){
            return node;
        }
        if(node->left != nullptr && node->left->max >= i.low){
            return search(node->left, i);
        }
        return search(node->right, i);
    }

    int Max(int a, int b){
        return (a > b) ? a : b;
    }

    int height(IntervalTreeNode<T>* node){
        if(node == nullptr){
            return 0;
        }
        return 1 + Max(height(node->left), height(node->right));
    }
    int getBalance(IntervalTreeNode<T>* node){
        if(node == nullptr){
            return 0;
        }
        return height(node->left) - height(node->right);
    }
    IntervalTreeNode<T>* leftRotate(IntervalTreeNode<T>* node){
        IntervalTreeNode<T>* y = node->right;
        IntervalTreeNode<T>* T2 = y->left;
        y->left = node;
        node->right = T2;
        node->max = Max(node->i.high, Max((node->left != nullptr) ? node->left->max : 0, (node->right != nullptr) ? node->right->max : 0));
        y->max = Max(y->i.high, Max((y->left != nullptr) ? y->left->max : 0, (y->right != nullptr) ? y->right->max : 0));
        return y;
    }
    IntervalTreeNode<T>* rightRotate(IntervalTreeNode<T>* node){
        IntervalTreeNode<T>* x = node->left;
        IntervalTreeNode<T>* T2 = x->right;
        x->right = node;
        node->left = T2;
        node->max = Max(node->i.high, Max((node->left != nullptr) ? node->left->max : 0, (node->right != nullptr) ? node->right->max : 0));
        x->max = Max(x->i.high, Max((x->left != nullptr) ? x->left->max : 0, (x->right != nullptr) ? x->right->max : 0));
        return x;
    }
    IntervalTreeNode<T>* minValueNode(IntervalTreeNode<T>* node){
        IntervalTreeNode<T>* current = node;
        while(current->left != nullptr){
            current = current->left;
        }
        return current;
    }
    
    
public:

    void insert(Interval<T> i){
        root = insert(root, i);
    }
    void deleteNode(Interval<T> i){
        root = deleteNode(root, i);
    }
    void inorder(){
        inorder(root);
    }
    IntervalTreeNode<T>* search(Interval<T> i){
        return search(root, i);
    }
};

#endif // INTERVAL_TREE_h
