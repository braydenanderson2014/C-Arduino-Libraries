#ifndef BINARYTREE_h
#define BINARYTREE_h

template <typename T>
struct BinaryNode {
        T data;
        BinaryNode *left;
        BinaryNode *right;
};
template <typename T>
class BinaryTree {
private:
    BinaryNode<T> *root;

    BinaryNode<T>* insert(BinaryNode<T>* node, T data) {
      // If the tree is empty, assign a new node address
      if (node == nullptr) {
        node = createNode(data);
      }
      // Else, do nothing.
      return node;
    }

    void inorder(BinaryNode<T>* node) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left);
        Serial.println(node->data);
        inorder(node->right);
    }

    BinaryNode<T>* deleteNode(BinaryNode<T>* node, T data) {
        if (node == nullptr) {
            return node;
        }
        if (data < node->data) {
            node->left = deleteNode(node->left, data);
        } else if (data > node->data) {
            node->right = deleteNode(node->right, data);
        } else {
            if (node->left == nullptr) {
                BinaryNode<T>* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                BinaryNode<T>* temp = node->left;
                delete node;
                return temp;
            }
            BinaryNode<T>* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }
        return node;
    }

    int height(BinaryNode<T>* node) {
        if (node == nullptr) {
            return 0;
        } else {
            int lheight = height(node->left);
            int rheight = height(node->right);
            if (lheight > rheight) {
                return lheight + 1;
            } else {
                return rheight + 1;
            }
        }
    }

    int leafcount(BinaryNode<T>* node) {
        if (node == nullptr) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return 1;
        } else {
            return leafcount(node->left) + leafcount(node->right);
        }
    }

    int nonleafcount(BinaryNode<T>* node) {
        if (node == nullptr || (node->left == nullptr && node->right == nullptr)) {
            return 0;
        } else {
            return 1 + nonleafcount(node->left) + nonleafcount(node->right);
        }
    }

    int nodecount(BinaryNode<T>* node) {
        if (node == nullptr) {
            return 0;
        } else {
            return 1 + nodecount(node->left) + nodecount(node->right);
        }
    }

    int depth(BinaryNode<T>* node, T data) {
        if (node == nullptr) {
            return -1;
        }
        if (node->data == data) {
            return 0;
        }
        int left = depth(node->left, data);
        int right = depth(node->right, data);
        if (left >= 0) {
            return left + 1;
        }
        if (right >= 0) {
            return right + 1;
        }
        return -1;
    }

    int level(BinaryNode<T>* node, T data) {
        return depth(node, data);
    }

    int width(BinaryNode<T>* node) {
        int h = height(node);
        int maxwidth = 0;
        for (int i = 1; i <= h; i++) {
            int width = getWidth(node, i);
            if (width > maxwidth) {
                maxwidth = width;
            }
        }
        return maxwidth;
    }

    int getWidth(BinaryNode<T>* node, int level) {
        if (node == nullptr) {
            return 0;
        }
        if (level == 1) {
            return 1;
        } else if (level > 1) {
            return getWidth(node->left, level - 1) + getWidth(node->right, level - 1);
        }
    }

    int findMax(BinaryNode<T>* node) {
        if (node == nullptr) {
            return -1;
        }
        while (node->right != nullptr) {
            node = node->right;
        }
        return node->data;
    }

    int findMin(BinaryNode<T>* node) {
        if (node == nullptr) {
            return -1;
        }
        while (node->left != nullptr) {
            node = node->left;
        }
        return node->data;
    }

    void mirror(BinaryNode<T>* node) {
        if (node == nullptr) {
            return;
        } else {
            BinaryNode<T>* temp;
            mirror(node->left);
            mirror(node->right);
            temp = node->left;
            node->left = node->right;
            node->right = temp;
        }
    }

    void preorder(BinaryNode<T>* node) {
        if (node == nullptr) {
            return;
        }
        Serial.println(node->data);
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(BinaryNode<T>* node) {
        if (node == nullptr) {
            return;
        }
        postorder(node->left);
        postorder(node->right);
        Serial.println(node->data);
    }

    void levelorder(BinaryNode<T>* node) {
        int h = height(node);
        for (int i = 1; i <= h; i++) {
            printGivenLevel(node, i);
        }
    }

    void printGivenLevel(BinaryNode<T>* node, int level) {
        if (node == nullptr) {
            return;
        }
        if (level == 1) {
            Serial.println(node->data);
        } else if (level > 1) {
            printGivenLevel(node->left, level - 1);
            printGivenLevel(node->right, level - 1);
        }
    }

    BinaryNode<T>* minValueNode(BinaryNode<T>* node) {
        BinaryNode<T>* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    BinaryNode<T>* search(BinaryNode<T>* node, T data) {
        // Base Cases: root is null or key is present at root
        if (node == nullptr || node->data == data)
           return node;
        
        // Key is greater than root's key
        if (node->data < data)
           return search(node->right, data);
    
        // Key is smaller than root's key
        return search(node->left, data);
    }
public:
    BinaryTree() : root(nullptr) {}

    BinaryNode<T>* createNode(T data) {
        BinaryNode<T> *newNode = new BinaryNode<T>();
        if(!newNode) {
            Serial.println("Memory Error");
            return nullptr;
        }
        newNode->data = data;
        newNode->left = newNode->right = nullptr;
        return newNode;
    }

    void insert(T data) {
        root = insert(root, data);
    }

    void inorder(){
        inorder(root);
    }

    void deleteNode(T data) {
        root = deleteNode(root, data);
    }

    int height() {
        return height(root);
    }

    int leafcount() {
        return leafcount(root);
    }

    int nonleafcount() {
        return nonleafcount(root);
    }

    int nodecount() {
        return nodecount(root);
    }

    int depth(T data) {
        return depth(root, data);
    }

    int level(T data) {
        return level(root, data);
    }

    int width() {
        return width(root);
    }

    int findMax() {
        return findMax(root);
    }

    int findMin() {
        return findMin(root);
    }

    void mirror() {
        mirror(root);
    }

    //preorder
    void preorder() {
        preorder(root);
    }

    //postorder
    void postorder() {
        postorder(root);
    }

    //levelorder
    void levelorder() {
        levelorder(root);
    }

    // Function to be called by the user, it uses the private search function
    BinaryNode<T>* search(T data) {
        return search(root, data);
    }

    void clear() {
        while (root != nullptr) {
            deleteNode(root->data);
        }
    }

};
#endif // BINARYTREE_h