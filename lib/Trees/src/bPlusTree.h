#ifndef B_PLUS_TREE_h
#define B_PLUS_TREE_h

#include <Arduino.h>
template <typename T>
class BPlusTree {
    private:
        struct BPlusNode {
            T data
            BPlusNode *left;
            BPlusNode *right;
        };

        BPlusNode *root;

        BPlusNode *insert(BPlusNode *node, T data){
            if(node == NULL){
                BPlusNode *newNode = new BPlusNode;
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

        BPlusNode *remove(BPlusNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    BPlusNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    BPlusNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            return node;
        }

        BPlusNode *findMin(BPlusNode *node){
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

    public:


        BPlusTree(){
            root = NULL;
        }

        void insert(T data){
            root = insert(root, data);
        }

        void remove(T data){
            root = remove(root, data);
        }

        void inOrder(BPlusNode *node){
            if(node == NULL){
                return;
            }
            inOrder(node->left);
            Serial.println(node->data);
            inOrder(node->right);
        }

        void inOrder(){
            inOrder(root);
        }
};
#endif // B_PLUS_TREE_h