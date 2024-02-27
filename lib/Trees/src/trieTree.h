#ifndef TRIE_TREE_h
#define TRIE_TREE_h

#include <Arduino.h>

template <typename T>
class TrieTree {
    private:
        struct TrieNode {
            T data;
            TrieNode *left;
            TrieNode *right;
        };

        TrieNode *root;

        TrieNode *insert(TrieNode *node, T data){
            if(node == NULL){
                TrieNode *newNode = new TrieNode;
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

        TrieNode *remove(TrieNode *node, T data){
            if(node == NULL){
                return node;
            }
            if(data < node->data){
                node->left = remove(node->left, data);
            } else if(data > node->data){
                node->right = remove(node->right, data);
            } else {
                if(node->left == NULL || node->right == NULL){
                    TrieNode *temp = node->left ? node->left : node->right;
                    if(temp == NULL){
                        temp = node;
                        node = NULL;
                    } else {
                        *node = *temp;
                    }
                    delete temp;
                } else {
                    TrieNode *temp = findMin(node->right);
                    node->data = temp->data;
                    node->right = remove(node->right, temp->data);
                }
            }
            return node;
        }

        TrieNode *findMin(TrieNode *node){
            while(node->left != NULL){
                node = node->left;
            }
            return node;
        }

    public:
        TrieTree(){
            root = NULL;
        }

        void insert(T data){
            root = insert(root, data);
        }

        void remove(T data){
            root = remove(root, data);
        }

        T findMin(){
            return findMin(root)->data;
        }
};

#endif
