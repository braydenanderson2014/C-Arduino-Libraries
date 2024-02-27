#ifndef HEAP_TREE_h
#define HEAP_TREE_h

#include <Arduino.h>
template <typename T>
class HeapTree{
    private:
        struct HeapNode{
            T data;
            HeapNode *left;
            HeapNode *right;
        };

        HeapNode *root;

        HeapNode *insert(HeapNode *node, T data){
            if(node == NULL){
                HeapNode *newNode = new HeapNode;
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

        HeapNode *remove(HeapNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    HeapNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    HeapNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            return node;
        }

        HeapNode *findMin(HeapNode *node){
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

        void print(HeapNode *node){
            if(node != NULL){
                print(node->left);
                Serial.println(node->data);
                print(node->right);
            }
        }

    public:
        HeapTree(){
            root = NULL;
        }

        ~HeapTree(){
            delete root;
        }

        void insert(T data){
            root = insert(root, data);
        }

        void remove(T data){
            root = remove(root, data);
        }

        void print(){
            print(root);
        }
        
        int findMin(){
            HeapNode *temp = findMin(root);
            return temp->data;
        }



};


#endif // HEAP_TREE_h
