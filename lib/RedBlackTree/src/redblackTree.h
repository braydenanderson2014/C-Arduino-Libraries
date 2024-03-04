#ifndef REDBLACK_TREE_h
#define REDBLACK_TREE_h

#include <Arduino.h>


enum DualColor {RED, BLACK};

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

        void leftRotate(RedBlackNode<T>* &root, RedBlackNode<T>* &node){
            RedBlackNode<T> *rightChild = node->right;
            node->right = rightChild->left;
            if(node->right != NULL){
                node->right->parent = node;
            }
            rightChild->parent = node->parent;
            if(node->parent == NULL){
                root = rightChild;
            }else if(node == node->parent->left){
                node->parent->left = rightChild;
            }else{
                node->parent->right = rightChild;
            }
            rightChild->left = node;
            node->parent = rightChild;
        }

        void rightRotate(RedBlackNode<T>* &root, RedBlackNode<T>* &node){
            RedBlackNode<T> *leftChild = node->left;
            node->left = leftChild->right;
            if(node->left != NULL){
                node->left->parent = node;
            }
            leftChild->parent = node->parent;
            if(node->parent == NULL){
                root = leftChild;
            }else if(node == node->parent->left){
                node->parent->left = leftChild;
            }else{
                node->parent->right = leftChild;
            }
            leftChild->right = node;
            node->parent = leftChild;
        }

        void deleteNodeHelper(RedBlackNode<T>* &root, T &data){
            RedBlackNode<T> *node = root;
            RedBlackNode<T> *parent = NULL;
            RedBlackNode<T> *child = NULL;
            RedBlackNode<T> *successor = NULL;
            RedBlackNode<T> *toDelete = NULL;
            while(node != NULL){
                if(node->data == data){
                    toDelete = node;
                }
                if(node->data <= data){
                    node = node->right;
                }else{
                    node = node->left;
                }
            }
            if(toDelete == NULL){
                return;
            }
            if(toDelete->left == NULL || toDelete->right == NULL){
                successor = toDelete;
            }else{
                successor = toDelete->right;
                while(successor->left != NULL){
                    successor = successor->left;
                }
            }
            if(successor->left != NULL){
                child = successor->left;
            }else{
                child = successor->right;
            }
            parent = successor->parent;
            if(child != NULL){
                child->parent = parent;
            }
            if(parent == NULL){
                root = child;
            }else if(successor == parent->left){
                parent->left = child;
            }else{
                parent->right = child;
            }
            if(successor != toDelete){
                toDelete->data = successor->data;
            }
            if(successor->color == BLACK){
                fixDoubleBlack(root, child, parent);
            }
            delete successor;
        }

        void fixDoubleBlack(RedBlackNode<T>* &root, RedBlackNode<T>* &node, RedBlackNode<T>* &parent){
            RedBlackNode<T> *sibling = NULL;
            while((node == NULL || node->color == BLACK) && node != root){
                if(node == parent->left){
                    sibling = parent->right;
                    if(sibling->color == RED){
                        sibling->color = BLACK;
                        parent->color = RED;
                        leftRotate(root, parent);
                        sibling = parent->right;
                    }
                    if((sibling->left == NULL || sibling->left->color == BLACK) && (sibling->right == NULL || sibling->right->color == BLACK)){
                        sibling->color = RED;
                        node = parent;
                        parent = node->parent;
                    }else{
                        if(sibling->right == NULL || sibling->right->color == BLACK){
                            sibling->left->color = BLACK;
                            sibling->color = RED;
                            rightRotate(root, sibling);
                            sibling = parent->right;
                        }
                        sibling->color = parent->color;
                        parent->color = BLACK;
                        sibling->right->color = BLACK;
                        leftRotate(root, parent);
                        node = root;
                        break;
                    }
                }else{
                    sibling = parent->left;
                    if(sibling->color == RED){
                        sibling->color = BLACK;
                        parent->color = RED;
                        rightRotate(root, parent);
                        sibling = parent->left;
                    }
                    if((sibling->left == NULL || sibling->left->color == BLACK) && (sibling->right == NULL || sibling->right->color == BLACK)){
                        sibling->color = RED;
                        node = parent;
                        parent = node->parent;
                    }else{
                        if(sibling->left == NULL || sibling->left->color == BLACK){
                            sibling->right->color = BLACK;
                            sibling->color = RED;
                            leftRotate(root, sibling);
                            sibling = parent->left;
                        }
                        sibling->color = parent->color;
                        parent->color = BLACK;
                        sibling->left->color = BLACK;
                        rightRotate(root, parent);
                        node = root;
                        break;
                    }
                }
            }
            if(node != NULL){
                node->color = BLACK;
            }
        }

        RedBlackNode<T>* deleteNode(RedBlackNode<T>* &root, T &data){
            deleteNodeHelper(root, data);
            return root;
        }

        RedBlackNode<T>* search(RedBlackNode<T>* root, const T &data){
            if(root == NULL || root->data == data){
                return root;
            }
            if(root->data < data){
                return search(root->right, data);
            }
            return search(root->left, data);
        }

        void inOrderHelper(RedBlackNode<T>* root){
            if(root == NULL){
                return;
            }
            inOrderHelper(root->left);
            Serial.print(root->data);
            Serial.print(" ");
            inOrderHelper(root->right);
        }

        void preOrderHelper(RedBlackNode<T>* root){
            if(root == NULL){
                return;
            }
            Serial.print(root->data);
            Serial.print(" ");
            preOrderHelper(root->left);
            preOrderHelper(root->right);
        }

        void postOrderHelper(RedBlackNode<T>* root){
            if(root == NULL){
                return;
            }
            postOrderHelper(root->left);
            postOrderHelper(root->right);
            Serial.print(root->data);
            Serial.print(" ");
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
        
        void deleteNode(const T &data){
            root = deleteNode(root, data);
        }

        RedBlackNode<T>* search(const T &data){
            return search(root, data);
        }
    
        void inOrder(){
            inOrderHelper(root);
        }

        void preOrder(){
            preOrderHelper(root);
        }

        void postOrder(){
            postOrderHelper(root);
        }

        void clear(){
            clear(root);
        }

        void clear(RedBlackNode<T>* &root){
            if(root == NULL){
                return;
            }
            clear(root->left);
            clear(root->right);
            delete root;
        }

        void rotateLeft(RedBlackNode<T>* &node){
            leftRotate(root, node);
        }

        void rotateRight(RedBlackNode<T>* &node){
            rightRotate(root, node);
        }

        RedBlackNode<T>* getRoot(){
            return root;
        }

        RedBlackNode<T>* getMin(){
            RedBlackNode<T> *node = root;
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

        RedBlackNode<T>* getMax(){
            RedBlackNode<T> *node = root;
            while(node->right != NULL){
                node = node->right;
            }
            return node;
        }
};

#endif //   REDBLACK_TREE_h