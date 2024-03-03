#ifndef BINARYSEARCHTREE_h
#define BINARYSEARCHTREE_h

template <typename T>
    struct BinarySearchNode {
        T data;
        BinarySearchNode *left;
        BinarySearchNode *right;
    };

template <typename T>
class BinarySearchTree {
private: 
    BinarySearchNode<T> *root;

    BinarySearchNode<T>* insert(const T &data, BinarySearchNode<T> * &node){
        if(node == NULL){
            node = new BinarySearchNode<T>;
            node->data = data;
            node->left = node->right = NULL;
        } else if(data < node->data){
            node->left = insert(data, node->left);
        } else if(data > node->data){
            node->right = insert(data, node->right);
        }
        return node;
    }

    BinarySearchNode<T>* deleteNode(BinarySearchNode<T>* root, const T &key){
        if(root == NULL){
            return root;
        }
        if(key < root->key){
            root->left = deleteNode(root->left, key);
        } else if(key > root->key){
            root->right = deleteNode(root->right, key);
        } else {
            if(root->left == NULL){
                BinarySearchNode<T> *temp = root->right;
                delete root;
                return temp;
            } else if(root->right == NULL){
                BinarySearchNode<T> *temp = root->left;
                delete root;
                return temp;
            }
            BinarySearchNode<T> *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
        return root;
    }

    BinarySearchNode<T>* minValueNode(BinarySearchNode<T>* node){
        BinarySearchNode<T>* current = node;
        while(current && current->left != NULL){
            current = current->left;
        }
        return current;
    }

    void clear(BinarySearchNode<T> * &node){
        if(node != NULL){
            clear(node->left);
            clear(node->right);
            delete node;
            node = NULL;
        }
    }



public:

    BinarySearchTree(){
        root = NULL;
    }

    ~BinarySearchTree(){
        clear();
    }

    void insert(const T &data){
        root = insert(data, root);
    }

    void deleteNode(const T &data){
        root = deleteNode(data, root);
    }

    void clear(){
        clear(root);
    }

    BinarySearchNode<T>* search(const T &data){
        BinarySearchNode<T> *current = root;
        while(current != NULL){
            if(data == current->data){
                return current;
            } else if(data < current->data){
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return NULL;
    }
};



#endif // BINARYSEARCHTREE_h