#ifndef AVL_TREE_h
#define AVL_TREE_h

#include <Arduino.h>

template <typename T>
class AVLTree {
    private:
        struct AVLNode {
            T data;
            AVLNode *left;
            AVLNode *right;
            int height;
        };
        AVLNode *root;

        AVLNode *insert(AVLNode *node, T data){
            if(node == NULL){
                AVLNode *newNode = new AVLNode;
                newNode->data = data;
                newNode->left = NULL;
                newNode->right = NULL;
                newNode->height = 0;
                return newNode;
            }
            if(data < node->data){
                node->left = insert(node->left, data);
            } else if(data > node->data){
                node->right = insert(node->right, data);
            }
            node->height = 1 + max(height(node->left), height(node->right));
            int balance = getBalance(node);
            if(balance > 1 && data < node->left->data){
                return rotateRight(node);
            }
            if(balance < -1 && data > node->right->data){
                return rotateLeft(node);
            }
            if(balance > 1 && data > node->left->data){
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            if(balance < -1 && data < node->right->data){
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            return node;
        }

        AVLNode *remove(AVLNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    AVLNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    AVLNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            if(node == NULL){
                return node;
            }
            node->height = 1 + max(height(node->left), height(node->right));
            int balance = getBalance(node);
            if(balance > 1 && getBalance(node->left) >= 0){
                return rotateRight(node);
            }
            if(balance > 1 && getBalance(node->left) < 0){
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            if(balance < -1 && getBalance(node->right) <= 0){
                return rotateLeft(node);
            }
            if(balance < -1 && getBalance(node->right) > 0){
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            return node;
        }

        AVLNode *findMin(AVLNode *node){
            if(node == NULL){
                return node;
            }
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        
        }

        AVLNode *findMax(AVLNode *node){
            if(node == NULL){
                return node;
            }
            while(node->right != NULL){
                node = node->right;
            }
            return node;
        }

        AVLNode *find(AVLNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                return find(node->left, data);
            } else if(data > node->data){
                return find(node->right, data);
            } else {
                return node;
            }
        }

        int height(AVLNode *node){
            if(node == NULL){
                return -1;
            }
            return node->height;
        }

        int getBalance(AVLNode *node){
            if(node == NULL){
                return 0;
            }
            return height(node->left) - height(node->right);
        
        }

        AVLNode *rotateRight(AVLNode *node){
            AVLNode *newRoot = node->left;
            AVLNode *temp = newRoot->right;
            newRoot->right = node;
            node->left = temp;
            node->height = 1 + max(height(node->left), height(node->right));
            newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
            return newRoot;
        }

        AVLNode *rotateLeft(AVLNode *node){
            AVLNode *newRoot = node->right;
            AVLNode *temp = newRoot->left;
            newRoot->left = node;
            node->right = temp;
            node->height = 1 + max(height(node->left), height(node->right));
            newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
            return newRoot;
        }

        void inOrder(AVLNode *node){
            if(node == NULL){
                return;
            }
            inOrder(node->left);
            Serial.println(node->data);
            inOrder(node->right);
        }
        void preOrder(AVLNode *node){
            if(node == NULL){
                return;
            }
            Serial.println(node->data);
            preOrder(node->left);
            preOrder(node->right);
        }

        void postOrder(AVLNode *node){
            if(node == NULL){
                return;
            }
            postOrder(node->left);
            postOrder(node->right);
            Serial.println(node->data);
        }
    public:
        AVLTree(){
            root = NULL;
        }

        ~AVLTree(){
            delete root;
        }

        void insert(T data){
            root = insert(root, data);
        }

        void remove(T data){
            root = remove(root, data);
        
        }
        void inOrder(){
            inOrder(root);
        }
        void preOrder(){
            preOrder(root);
        }

        void postOrder(){
            postOrder(root);
        }
        int findMin(){
            AVLNode *temp = findMin(root);
            return temp->data;
        }

        int findMax(){
            AVLNode *temp = findMax(root);
            return temp->data;
        }
        int find(T data){
            AVLNode *temp = find(root, data);
            return temp->data;
        }

        int height(){
            return height(root);
        }

        int getBalance(){
            return getBalance(root);
        }
        void print(){
            inOrder();
        }
};
#endif // AVL_TREE_h