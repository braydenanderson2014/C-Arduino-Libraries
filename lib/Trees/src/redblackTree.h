#ifndef REDBLACK_TREE_h
#define REDBLACK_TREE_h

#include <Arduino.h>

template <typename T>
class RedBlackTree{
    private:
        struct RedBlackNode{
            T data;
            RedBlackNode *left;
            RedBlackNode *right;
            RedBlackNode *parent;
            bool isBlack;
        };
        RedBlackNode *root;

        RedBlackNode *insert(RedBlackNode *node, T data){
            if(node == NULL){
                RedBlackNode *newNode = new RedBlackNode;
                newNode->data = data;
                newNode->left = NULL;
                newNode->right = NULL;
                newNode->parent = NULL;
                newNode->isBlack = false;
                return newNode;
            }
            if(data < node->data){
                node->left = insert(node->left, data);
                node->left->parent = node;
            } else if(data > node->data){
                node->right = insert(node->right, data);
                node->right->parent = node;
            }
            return node;
        }

        RedBlackNode *remove(RedBlackNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    RedBlackNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    RedBlackNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            return node;
        }

        RedBlackNode *findMin(RedBlackNode *node){
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

        RedBlackNode *rotateLeft(RedBlackNode *node){
            RedBlackNode *temp = node->right;
            node->right = temp->left;
            if(temp->left != NULL){
                temp->left->parent = node;
            }
            temp->parent = node->parent;
            if(node->parent == NULL){
                root = temp;
            } else if(node == node->parent->left){
                node->parent->left = temp;
            } else {
                node->parent->right = temp;
            }
            temp->left = node;
            node->parent = temp;
            return temp;
        }

        RedBlackNode *rotateRight(RedBlackNode *node){
            RedBlackNode *temp = node->left;
            node->left = temp->right;
            if(temp->right != NULL){
                temp->right->parent = node;
            }
            temp->parent = node->parent;
            if(node->parent == NULL){
                root = temp;
            } else if(node == node->parent->right){
                node->parent->right = temp;
            } else {
                node->parent->left = temp;
            }
            temp->right = node;
            node->parent = temp;
            return temp;
        }

        void fixInsert(RedBlackNode *node){
            RedBlackNode *parent = NULL;
            RedBlackNode *grandparent = NULL;
            while((node != root) && (node->isBlack != false) && (node->parent->isBlack == false)){
                parent = node->parent;
                grandparent = node->parent->parent;
                if(parent == grandparent->left){
                    RedBlackNode *uncle = grandparent->right;
                    if(uncle != NULL && uncle->isBlack == false){
                        grandparent->isBlack = false;
                        parent->isBlack = true;
                        uncle->isBlack = true;
                        node = grandparent;
                    } else {
                        if(node == parent->right){
                            node = parent;
                            rotateLeft(node);
                        }
                        rotateRight(grandparent);
                        bool temp = parent->isBlack;
                        parent->isBlack = grandparent->isBlack;
                        grandparent->isBlack = temp;
                        node = parent;
                    }
                } else {
                    RedBlackNode *uncle = grandparent->left;
                    if(uncle != NULL && uncle->isBlack == false){
                        grandparent->isBlack = false;
                        parent->isBlack = true;
                        uncle->isBlack = true;
                        node = grandparent;
                    } else {
                        if(node == parent->left){
                            node = parent;
                            rotateRight(node);
                        }
                        rotateLeft(grandparent);
                        bool temp = parent->isBlack;
                        parent->isBlack = grandparent->isBlack;
                        grandparent->isBlack = temp;
                        node = parent;
                    }
                }
            }
            root->isBlack = true;
        }

        void fixRemove(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            if(node == root){
                root = NULL;
                return;
            }
            if(node->isBlack == false || (node->left != NULL && node->left->isBlack == false) || (node->right != NULL && node->right->isBlack == false)){
                RedBlackNode *temp = node->left != NULL ? node->left : node->right;
                if(node == node->parent->left){
                    node->parent->left = temp;
                    if(temp != NULL){
                        temp->parent = node->parent;
                    }
                    temp->isBlack = true;
                } else {
                    node->parent->right = temp;
                    if(temp != NULL){
                        temp->parent = node->parent;
                    }
                    temp->isBlack = true;
                }
                delete node;
            } else {
                RedBlackNode *sibling = NULL;
                RedBlackNode *parent = NULL;
                RedBlackNode *temp = node;
                temp->isBlack = true;
                while(temp != root && temp->isBlack == true){
                    parent = temp->parent;
                    if(temp == parent->left){
                        sibling = parent->right;
                        if(sibling->isBlack == false){
                            sibling->isBlack = true;
                            parent->isBlack = false;
                            rotateLeft(parent);
                            sibling = parent->right;
                        }
                        if((sibling->left == NULL || sibling->left->isBlack == true) && (sibling->right == NULL || sibling->right->isBlack == true)){
                            sibling->isBlack = false;
                            temp = parent;
                        } else {
                            if(sibling->right == NULL || sibling->right->isBlack == true){
                                sibling->left->isBlack = true;
                                sibling->isBlack = false;
                                rotateRight(sibling);
                                sibling = parent->right;
                            }
                            sibling->isBlack = parent->isBlack;
                            parent->isBlack = true;
                            sibling->right->isBlack = true;
                            rotateLeft(parent);
                            temp = root;
                        }
                    } else {
                        sibling = parent->left;
                        if(sibling->isBlack == false){
                            sibling->isBlack = true;
                            parent->isBlack = false;
                            rotateRight(parent);
                            sibling = parent->left;

                        }
                        if((sibling->left == NULL || sibling->left->isBlack == true) && (sibling->right == NULL || sibling->right->isBlack == true)){
                            sibling->isBlack = false;
                            temp = parent;
                        } else {
                            if(sibling->left == NULL || sibling->left->isBlack == true){
                                sibling->right->isBlack = true;
                                sibling->isBlack = false;
                                rotateLeft(sibling);
                                sibling = parent->left;
                            }
                            sibling->isBlack = parent->isBlack;
                            parent->isBlack = true;
                            sibling->left->isBlack = true;
                            rotateRight(parent);
                            temp = root;
                        }
                    }
                }
                if(temp != NULL){
                    temp->isBlack = true;
                }
            }
        }

        void clear(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            clear(node->left);
            clear(node->right);
            delete node;
        }


    public:
        RedBlackTree(){
            root = NULL;
        }

        ~RedBlackTree(){
            delete root;
        }

        void insert(T data){
            root = insert(root, data);
            fixInsert(root);
        }

        void remove(T data){
            root = remove(root, data);
            fixRemove(root);
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

        void inOrder(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            inOrder(node->left);
            Serial.println(node->data);
            inOrder(node->right);
        }

        void preOrder(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            Serial.println(node->data);
            preOrder(node->left);
            preOrder(node->right);
        }

        void postOrder(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            postOrder(node->left);
            postOrder(node->right);
            Serial.println(node->data);
        }

        int findMin(){
            RedBlackNode *temp = findMin(root);
            return temp->data;
        }

        int findMax(){
            RedBlackNode *temp = root;
            while(temp->right != NULL){
                temp = temp->right;
            }
            return temp->data;
        }

        int findHeight(){
            return findHeight(root);
        }

        int findHeight(RedBlackNode *node){
            if(node == NULL){
                return -1;
            }
            int left = findHeight(node->left);
            int right = findHeight(node->right);
            if(left > right){
                return left + 1;
            } else {
                return right + 1;
            }
        }

        bool find(T data){
            return find(root, data);
        }

        bool find(RedBlackNode *node, T data){
            if(node == NULL){
                return false;
            }
            if(data < node->data){
                return find(node->left, data);
            } else if(data > node->data){
                return find(node->right, data);
            } else {
                return true;
            }
        }

        void print(){
            print(root, 0);
        }

        void print(RedBlackNode *node, int space){
            if(node == NULL){
                return;
            }
            space += 10;
            print(node->right, space);
            Serial.println();
            for(int i = 10; i < space; i++){
                Serial.print(" ");
            }
            Serial.println(node->data);
            print(node->left, space);
        }

        void clear(){
            clear(root);
        }

        void clear(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            clear(node->left);
            clear(node->right);
            delete node;
        }

        void clearTree(){
            clear(root);
            root = NULL;
        }

        void clearTree(RedBlackNode *node){
            if(node == NULL){
                return;
            }
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }



};

#endif //   REDBLACK_TREE_h