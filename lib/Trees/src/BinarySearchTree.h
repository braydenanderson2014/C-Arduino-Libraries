#ifndef BINARYSEARCHTREE_h
#define BINARYSEARCHTREE_h

template <typename T>
class BinarySearchTree {
private: 
    struct BinarySearchNode {
        T data;
        BinarySearchNode *left;
        BinarySearchNode *right;
    };
    BinarySearchNode *root;

    BinarySearchNode *insert(T data, BinarySearchNode *node){
        if(node == nullptr){
            BinarySearchNode *newNode = new BinarySearchNode;
            newNode->data = data;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }
        if(data < node->data){
            node->left = insert(data, node->left);
        } else {
            node->right = insert(data, node->right);
        }
        return node;
    }

    void inOrder(BinarySearchNode *node){
        if(node == nullptr){
            return;
        }
        inOrder(node->left);
        Serial.println(node->data);
        inOrder(node->right);
    }

    void preOrder(BinarySearchNode *node){
        if(node == nullptr){
            return;
        }
        Serial.println(node->data);
        preOrder(node->left);
        preOrder(node->right);
    }

    void postOrder(BinarySearchNode *node){
        if(node == nullptr){
            return;
        }
        postOrder(node->left);
        postOrder(node->right);
        Serial.println(node->data);
    }

    BinarySearchNode *findMin(BinarySearchNode *node){
        if(node == nullptr){
            return nullptr;
        }
        if(node->left == nullptr){
            return node;
        }
        return findMin(node->left);
    }

    BinarySearchNode *findMax(BinarySearchNode *node){
        if(node == nullptr){
            return nullptr;
        }
        if(node->right == nullptr){
            return node;
        }
        return findMax(node->right);
    }

    BinarySearchNode *remove(T data, BinarySearchNode *node){
        if(node == nullptr){
            return nullptr;
        }
        if(data < node->data){
            node->left = remove(data, node->left);
        } else if(data > node->data){
            node->right = remove(data, node->right);
        } else {
            if(node->left == nullptr && node->right == nullptr){
                delete node;
                return nullptr;
            } else if(node->left == nullptr){
                BinarySearchNode *temp = node->right;
                delete node;
                return temp;
            } else if(node->right == nullptr){
                BinarySearchNode *temp = node->left;
                delete node;
                return temp;
            } else {
                BinarySearchNode *temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(temp->data, node->right);
            }
        }
        return node;
    }

    void clear(BinarySearchNode *node){
        if(node == nullptr){
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:

    BinarySearchTree(){
        root = nullptr;
    }

    ~BinarySearchTree(){
        clear();
    }

    void insert(T data){
        root = insert(data, root);
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

    T findMin(){
        BinarySearchNode *node = findMin(root);
        if(node == nullptr){
            return T();
        }
        return node->data;
    }

    T findMax(){
        BinarySearchNode *node = findMax(root);
        if(node == nullptr){
            return T();
        }
        return node->data;
    }

    void remove(T data){
        root = remove(data, root);
    }

    void clear(){
        clear(root);
        root = nullptr;
    }

    bool isEmpty(){
        return root == nullptr;
    }

    bool contains(T data){
        BinarySearchNode *node = root;
        while(node != nullptr){
            if(data == node->data){
                return true;
            } else if(data < node->data){
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return false;
    }

    T find(T data){
        BinarySearchNode *node = root;
        while(node != nullptr){
            if(data == node->data){
                return node->data;
            } else if(data < node->data){
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return T();
    }

    int size(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        return 1 + size(node->left) + size(node->right);
    }

    int size(){
        return size(root);
    }

    int height(BinarySearchNode *node){
        if(node == nullptr){
            return -1;
        }
        return 1 + max(height(node->left), height(node->right));
    }

    int height(){
        return height(root);
    }

    int leafNodes(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        if(node->left == nullptr && node->right == nullptr){
            return 1;
        }
        return leafNodes(node->left) + leafNodes(node->right);
    }

    int leafNodes(){
        return leafNodes(root);
    }

    int nonLeafNodes(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        if(node->left == nullptr && node->right == nullptr){
            return 0;
        }
        return 1 + nonLeafNodes(node->left) + nonLeafNodes(node->right);
    }

    int nonLeafNodes(){
        return nonLeafNodes(root);
    }

    int fullNodes(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        if(node->left != nullptr && node->right != nullptr){
            return 1 + fullNodes(node->left) + fullNodes(node->right);
        }
        return fullNodes(node->left) + fullNodes(node->right);
    }

    int fullNodes(){
        return fullNodes(root);
    }

    int halfNodes(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        if((node->left != nullptr && node->right == nullptr) || (node->left == nullptr && node->right != nullptr)){
            return 1 + halfNodes(node->left) + halfNodes(node->right);
        }
        return halfNodes(node->left) + halfNodes(node->right);
    }

    int halfNodes(){
        return halfNodes(root);
    }

    int level(BinarySearchNode *node, T data, int level){
        if(node == nullptr){
            return -1;
        }
        if(data == node->data){
            return level;
        }
        int left = level(node->left, data, level + 1);
        if(left != -1){
            return left;
        }
        return level(node->right, data, level + 1);
    }

    int level(T data){
        return level(root, data, 0);
    }

    int levelSum(BinarySearchNode *node, int level){
        if(node == nullptr){
            return 0;
        }
        if(level == 0){
            return node->data;
        }
        return levelSum(node->left, level - 1) + levelSum(node->right, level - 1);
    }

    int levelSum(int level){
        return levelSum(root, level);
    }

    int sum(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        return node->data + sum(node->left) + sum(node->right);
    }

    int sum(){
        return sum(root);
    }

    int maxSumPath(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        return node->data + max(maxSumPath(node->left), maxSumPath(node->right));
    }

    int maxSumPath(){
        return maxSumPath(root);
    }

    int diameter(BinarySearchNode *node){
        if(node == nullptr){
            return 0;
        }
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        int leftDiameter = diameter(node->left);
        int rightDiameter = diameter(node->right);
        return max(leftHeight + rightHeight + 1, max(leftDiameter, rightDiameter));
    }

    int diameter(){
        return diameter(root);
    }

    bool isBalanced(BinarySearchNode *node){
        if(node == nullptr){
            return true;
        }
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return abs(leftHeight - rightHeight) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }

    bool isBalanced(){
        return isBalanced(root);
    }

    bool isFull(BinarySearchNode *node){
        if(node == nullptr){
            return true;
        }
        if(node->left == nullptr && node->right == nullptr){
            return true;
        }
        if(node->left != nullptr && node->right != nullptr){
            return isFull(node->left) && isFull(node->right);
        }
        return false;
    }

    bool isFull(){
        return isFull(root);
    }

    bool isComplete(BinarySearchNode *node, int index, int size){
        if(node == nullptr){
            return true;
        }
        if(index >= size){
            return false;
        }
        return isComplete(node->left, 2 * index + 1, size) && isComplete(node->right, 2 * index + 2, size);
    }

    bool isComplete(){
        return isComplete(root, 0, size());
    }

    bool isPerfect(BinarySearchNode *node, int depth, int level){
        if(node == nullptr){
            return true;
        }
        if(node->left == nullptr && node->right == nullptr){
            return depth == level;
        }
        if(node->left == nullptr || node->right == nullptr){
            return false;
        }
        return isPerfect(node->left, depth + 1, level) && isPerfect(node->right, depth + 1, level);
    }

    bool isPerfect(){
        return isPerfect(root, 0, height());
    }

    bool isDegenerate(BinarySearchNode *node){
        if(node == nullptr){
            return true;
        }
        if(node->left != nullptr && node->right != nullptr){
            return false;
        }
        return isDegenerate(node->left) && isDegenerate(node->right);
    }

    bool isDegenerate(){
        return isDegenerate(root);
    }

    bool isSkewedLeft(BinarySearchNode *node){
        if(node == nullptr){
            return true;
        }
        if(node->right != nullptr){
            return false;
        }
        return isSkewedLeft(node->left);
    }

    bool isSkewedLeft(){
        return isSkewedLeft(root);
    }

    bool isSkewedRight(BinarySearchNode *node){
        if(node == nullptr){
            return true;
        }
        if(node->left != nullptr){
            return false;
        }
        return isSkewedRight(node->right);
    }

    bool isSkewedRight(){
        return isSkewedRight(root);
    }

    bool isSymmetric(BinarySearchNode *left, BinarySearchNode *right){
        if(left == nullptr && right == nullptr){
            return true;
        }
        if(left == nullptr || right == nullptr){
            return false;
        }
        return left->data == right->data && isSymmetric(left->left, right->right) && isSymmetric(left->right, right->left);
    }

    bool isSymmetric(){
        return isSymmetric(root, root);
    }

    bool isMirror(BinarySearchNode *left, BinarySearchNode *right){
        if(left == nullptr && right == nullptr){
            return true;
        }
        if(left == nullptr || right == nullptr){
            return false;
        }
        return left->data == right->data && isMirror(left->left, right->right) && isMirror(left->right, right->left);
    }

    bool isMirror(){
        return isMirror(root, root);
    }

    bool isIdentical(BinarySearchNode *left, BinarySearchNode *right){
        if(left == nullptr && right == nullptr){
            return true;
        }
        if(left == nullptr || right == nullptr){
            return false;
        }
        return left->data == right->data && isIdentical(left->left, right->left) && isIdentical(left->right, right->right);
    }

    bool isIdentical(BinarySearchTree<T> *tree){
        return isIdentical(root, tree->root);
    }

    bool isSubtree(BinarySearchNode *node, BinarySearchNode *subtree){
        if(node == nullptr){
            return false;
        }
        if(node->data == subtree->data && isIdentical(node, subtree)){
            return true;
        }
        return isSubtree(node->left, subtree) || isSubtree(node->right, subtree);
    }

    bool isSubtree(BinarySearchTree<T> *tree){
        return isSubtree(root, tree->root);
    }

    bool isCousin(BinarySearchNode *node, BinarySearchNode *cousin, int level){
        if(node == nullptr){
            return false;
        }
        if(level == 0){
            return node->data == cousin->data;
        }
        return isCousin(node->left, cousin, level - 1) || isCousin(node->right, cousin, level - 1);
    }

    bool isCousin(BinarySearchNode *node, BinarySearchNode *cousin, BinarySearchNode *parent, int level){
        if(node == nullptr){
            return false;
        }
        if(level == 0){
            return node->data == cousin->data && node->data != parent->data;
        }
        return isCousin(node->left, cousin, parent, level - 1) || isCousin(node->right, cousin, parent, level - 1);
    }

    bool isCousin(BinarySearchNode *cousin, int level){
        return isCousin(root, cousin, level);
    }

    bool isCousin(BinarySearchNode *cousin, BinarySearchNode *parent, int level){
        return isCousin(root, cousin, parent, level);
    }

    bool isCousin(BinarySearchTree<T> *tree, int level){
        return isCousin(tree->root, level);
    }

    bool isCousin(BinarySearchTree<T> *tree, BinarySearchNode *parent, int level){
        return isCousin(tree->root, parent, level);
    }

    bool isSibling(BinarySearchNode *node, BinarySearchNode *sibling){
        if(node == nullptr){
            return false;
        }
        if(node->left == nullptr && node->right == nullptr){
            return false;
        }
        if(node->left != nullptr && node->right != nullptr){
            return node->left->data == sibling->data || node->right->data == sibling->data;
        }
        return isSibling(node->left, sibling) || isSibling(node->right, sibling);
    }

    bool isSibling(BinarySearchNode *node, BinarySearchNode *sibling, BinarySearchNode *parent){
        if(node == nullptr){
            return false;
        }
        if(node->left == nullptr && node->right == nullptr){
            return false;
        }
        if(node->left != nullptr && node->right != nullptr){
            return (node->left->data == sibling->data || node->right->data == sibling->data) && node->data != parent->data;
        }
        return isSibling(node->left, sibling, parent) || isSibling(node->right, sibling, parent);
    }   

    bool isSibling(BinarySearchNode *sibling){
        return isSibling(root, sibling);
    }

    bool isSibling(BinarySearchNode *sibling, BinarySearchNode *parent){
        return isSibling(root, sibling, parent);
    }

    bool isSibling(BinarySearchTree<T> *tree){
        return isSibling(tree->root);
    }

    bool isSibling(BinarySearchTree<T> *tree, BinarySearchNode *parent){
        return isSibling(tree->root, parent);
    }

    bool isAncestor(BinarySearchNode *node, BinarySearchNode *descendant){
        if(node == nullptr){
            return false;
        }
        if(node->data == descendant->data){
            return true;
        }
        return isAncestor(node->left, descendant) || isAncestor(node->right, descendant);
    }

    bool isAncestor(BinarySearchNode *node, BinarySearchNode *descendant, BinarySearchNode *parent){
        if(node == nullptr){
            return false;
        }
        if(node->data == descendant->data){
            return node->data != parent->data;
        }
        return isAncestor(node->left, descendant, parent) || isAncestor(node->right, descendant, parent);
    }

    bool isAncestor(BinarySearchNode *descendant){
        return isAncestor(root, descendant);
    }

    bool isAncestor(BinarySearchNode *descendant, BinarySearchNode *parent){
        return isAncestor(root, descendant, parent);
    }

    bool isAncestor(BinarySearchTree<T> *tree){
        return isAncestor(tree->root);
    }

    bool isAncestor(BinarySearchTree<T> *tree, BinarySearchNode *parent){
        return isAncestor(tree->root, parent);
    }

    bool isDescendant(BinarySearchNode *node, BinarySearchNode *ancestor){
        if(node == nullptr){
            return false;
        }
        if(node->data == ancestor->data){
            return true;
        }
        return isDescendant(node->left, ancestor) || isDescendant(node->right, ancestor);
    }

    bool isDescendant(BinarySearchNode *node, BinarySearchNode *ancestor, BinarySearchNode *parent){
        if(node == nullptr){
            return false;
        }
        if(node->data == ancestor->data){
            return node->data != parent->data;
        }
        return isDescendant(node->left, ancestor, parent) || isDescendant(node->right, ancestor, parent);
    }

    bool isDescendant(BinarySearchNode *ancestor){
        return isDescendant(root, ancestor);
    }

    bool isDescendant(BinarySearchNode *ancestor, BinarySearchNode *parent){
        return isDescendant(root, ancestor, parent);
    }

    bool isDescendant(BinarySearchTree<T> *tree){
        return isDescendant(tree->root);
    }

    bool isDescendant(BinarySearchTree<T> *tree, BinarySearchNode *parent){
        return isDescendant(tree->root, parent);
    }

    bool isLeaf(BinarySearchNode *node){
        return node != nullptr && node->left == nullptr && node->right == nullptr;
    }

    bool isLeaf(){
        return isLeaf(root);
    }

    bool isRoot(BinarySearchNode *node){
        return node != nullptr && node == root;
    }

    bool isRoot(){
        return isRoot(root);
    }

    bool isInternal(BinarySearchNode *node){
        return node != nullptr && (node->left != nullptr || node->right != nullptr);
    }

    bool isInternal(){
        return isInternal(root);
    }

    bool isExternal(BinarySearchNode *node){
        return node != nullptr && node->left == nullptr && node->right == nullptr;
    }

    bool isExternal(){
        return isExternal(root);
    }

    bool isLeftChild(BinarySearchNode *node, BinarySearchNode *parent){
        return parent != nullptr && parent->left != nullptr && parent->left == node;
    }

    bool isLeftChild(BinarySearchNode *node){
        return isLeftChild(node, node->parent);
    }

    bool isRightChild(BinarySearchNode *node, BinarySearchNode *parent){
        return parent != nullptr && parent->right != nullptr && parent->right == node;
    }

    bool isRightChild(BinarySearchNode *node){
        return isRightChild(node, node->parent);
    }

};



#endif // BINARYSEARCHTREE_h