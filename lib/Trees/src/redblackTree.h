#ifndef REDBLACK_TREE_h
#define REDBLACK_TREE_h

#include <Arduino.h>


enum Color {RED, BLACK};

template <typename T>
struct RedBlackNode {
    T data;
    bool color;
    RedBlackNode *left, *right, *parent;

    // Constructor
    RedBlackNode(T data) {
       this->data = data;
       left = right = parent = NULL;
       this->color = RED;
    }
};


template <typename T>
class RedBlackTree{
    private:
        RedBlackNode<T> *root;

        RedBlackNode<T>* BSTInsert(RedBlackNode<T>* root, RedBlackNode<T>* newNode){
            if(root == NULL){
                return newNode;
            }
            if(newNode->data < root->data){
                root->left = BSTInsert(root->left, newNode);
                root->left->parent = root;
            }else if(newNode->data > root->data){
                root->right = BSTInsert(root->right, newNode);
                root->right->parent = root;
            }
            return root;
        }

        void fixViolation(RedBlackNode<T>* &root, RedBlackNode<T>* &newNode){
            RedBlackNode<T> *parent = NULL;
            RedBlackNode<T> *grandparent = NULL;

            while((newNode != root) && (newNode->color != BLACK) && (newNode->parent->color == RED)){
                parent = newNode->parent;
                grandparent = newNode->parent->parent;

                if(parent == grandparent->left){
                    RedBlackNode<T> *uncle = grandparent->right;
                    if(uncle != NULL && uncle->color == RED){
                        grandparent->color = RED;
                        parent->color = BLACK;
                        uncle->color = BLACK;
                        newNode = grandparent;
                    }else{
                        if(newNode == parent->right){
                            leftRotate(root, parent);
                            newNode = parent;
                            parent = newNode->parent;
                        }
                        rightRotate(root, grandparent);
                        swap(parent->color, grandparent->color);
                        newNode = parent;
                    }
                }else{
                    RedBlackNode<T> *uncle = grandparent->left;
                    if((uncle != NULL) && (uncle->color == RED)){
                        grandparent->color = RED;
                        parent->color = BLACK;
                        uncle->color = BLACK;
                        newNode = grandparent;
                    }else{
                        if(newNode == parent->left){
                            rightRotate(root, parent);
                            newNode = parent;
                            parent = newNode->parent;
                        }
                        leftRotate(root, grandparent);
                        swap(parent->color, grandparent->color);
                        newNode = parent;
                    }
                }
            }
            root->color = BLACK;
        }
    public:
        RedBlackTree() {root = NULL;}
        ~RedBlackTree(){
            clear(root);
        }

        void insert(const T &data){
            RedBlackNode<T> *newNode = new RedBlackNode<T>(data);
            root = BSTInsert(root, newNode);
            fixViolation(root, newNode);
        }
        
        

    
};

#endif //   REDBLACK_TREE_h