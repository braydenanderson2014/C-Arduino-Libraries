#ifndef BTREE_H
#define BTREE_H

#include <Arduino.h>

class BTree {
    private:
        struct BNode {
            int data;
            BNode *left;
            BNode *right;
        };
        BNode *root;

        BNode *insert(BNode *node, int data){
            if(node == NULL){
                BNode *newNode = new BNode;
                newNode->data = data;
                newNode->left = NULL;
                newNode->right = NULL;
                return newNode;
            }
            if(data < node->data){
                node->left = insert(node->left, data);
            } else if(data > node->data){
                node->right = insert(node->right, data);
            }
            return node;
        }

        BNode *remove(BNode *node, int data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    BNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    BNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            return node;
        }

        BNode *findMin(BNode *node){
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

        
        void inOrder(BNode *node){
            if(node == NULL){
                return;
            }
            inOrder(node->left);
            Serial.println(node->data);
            inOrder(node->right);
        }
        void preOrder(BNode *node){
            if(node == NULL){
                return;
            }
            Serial.println(node->data);
            preOrder(node->left);
            preOrder(node->right);
        }

        void postOrder(BNode *node){
            if(node == NULL){
                return;
            }
            postOrder(node->left);
            postOrder(node->right);
            Serial.println(node->data);
        }

    public:

        BTree(){
            root = NULL;
        }

        void insert(int data){
            root = insert(root, data);
        }

        void remove(int data){
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

};
#endif // BTREE_H
