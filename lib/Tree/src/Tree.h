#ifndef TREE_h
#define TREE_h
// ENUMS:

/**
 * @brief Enum for traversal order
 * 
*/
enum TraversalOrder {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    LEVEL_ORDER
};

/**
 * @brief Enum for tree type
 * 
*/
enum TreeType {
    BINARY,
    BINARY_SEARCH,
    AVL,
    RED_BLACK,
    HEAP,
    B_TREE,
    B_PLUS_TREE,
    TRIE,
    SUFFIX_TREE,
    SEGMANT_TREE,
    K_DIMENSIONAL_TREE,
    QUAD_TREE,
    FENWICK_TREE,
    INTERVAL_TREE,
    OCTREE,
    R_TREE
};

template <typename T>
class Tree {
private:
    /* data */

    //STRUCTS:

    /**
     * @brief Struct for binary node
    */
    struct binaryNode {
        T value;
        binaryNode *left;
        binaryNode *right;
        binaryNode(T value) : value(value), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for avl node
    */
    struct avlNode {
        T value;
        avlNode *left;
        avlNode *right;
        int height;
        avlNode(T value) : value(value), left(nullptr), right(nullptr), height(1) {}
    };

    /**
     * @brief Struct for binary search node
    */
    struct binarySearchNode {
        T value;
        binarySearchNode *left;
        binarySearchNode *right;
        binarySearchNode(T value) : value(value), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for red black node
    */
    struct redBlackNode {
        T value;
        redBlackNode *left;
        redBlackNode *right;
        redBlackNode *parent;
        bool isBlack;
        redBlackNode(T value) : value(value), left(nullptr), right(nullptr), parent(nullptr), isBlack(false) {}
    };

    /**
     * @brief Struct for heap node
    */
    struct heapNode {
        T value;
        heapNode *left;
        heapNode *right;
        heapNode(T value) : value(value), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for b tree node
    */
    struct bTreeNode {
        T *values;
        bTreeNode **children;
        int size;
        bool isLeaf;
        bTreeNode(int degree) : size(0), isLeaf(true) {
            values = new T[degree - 1];
            children = new bTreeNode*[degree];
            for(int i = 0; i < degree; i++){
                children[i] = nullptr;
            }
        }
    };

    /**
     * @brief Struct for b plus tree node
    */
    struct bPlusTreeNode {
        T *values;
        bPlusTreeNode **children;
        int size;
        bPlusTreeNode *next;
        bPlusTreeNode *prev;
        bPlusTreeNode(int degree) : size(0), next(nullptr), prev(nullptr) {
            values = new T[degree - 1];
            children = new bPlusTreeNode*[degree];
            for(int i = 0; i < degree; i++){
                children[i] = nullptr;
            }
        }
    };

    /**
     * @brief Struct for trie node
    */
    struct trieNode {
        trieNode *children[26];
        bool isEndOfWord;
        trieNode() : isEndOfWord(false) {
            for(int i = 0; i < 26; i++){
                children[i] = nullptr;
            }
        }
    };

    /**
     * @brief Struct for suffix tree node
    */
    struct suffixTreeNode {
        suffixTreeNode *children[26];
        suffixTreeNode *suffixLink;
        int start;
        int *end;
        int *end;
        suffixTreeNode() : suffixLink(nullptr) {
            for(int i = 0; i < 26; i++){
                children[i] = nullptr;
            }
        }
    };

    /**
     * @brief Struct for segment tree node
    */
    struct segmentTreeNode {
        int start;
        int end;
        int sum;
        segmentTreeNode *left;
        segmentTreeNode *right;
        segmentTreeNode(int start, int end) : start(start), end(end), sum(0), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for k dimensional tree node
    */
    struct kDimensionalTreeNode {
        T *point;
        kDimensionalTreeNode *left;
        kDimensionalTreeNode *right;
        kDimensionalTreeNode(T *point) : point(point), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for quad tree node
    */
    struct quadTreeNode {
        T value;
        quadTreeNode *topLeft;
        quadTreeNode *topRight;
        quadTreeNode *bottomLeft;
        quadTreeNode *bottomRight;
        quadTreeNode(T value) : value(value), topLeft(nullptr), topRight(nullptr), bottomLeft(nullptr), bottomRight(nullptr) {}
    };

    /**
     * @brief Struct for fenwick tree node
    */
    struct fenwickTreeNode {
        T value;
        fenwickTreeNode *parent;
        fenwickTreeNode(T value) : value(value), parent(nullptr) {}
    };

    /**
     * @brief Struct for interval tree node
    */
    struct intervalTreeNode {
        int start;
        int end;
        int max;
        intervalTreeNode *left;
        intervalTreeNode *right;
        intervalTreeNode(int start, int end) : start(start), end(end), max(end), left(nullptr), right(nullptr) {}
    };

    /**
     * @brief Struct for r tree node
    */
    struct rTreeNode {
        T value;
        rTreeNode *children;
        rTreeNode(T value) : value(value), children(nullptr) {}
    };

    /**
     * @brief Struct for b plus tree leaf node
    */
    struct bPlusTreeLeafNode {
        T *values;
        bPlusTreeLeafNode *next;
        bPlusTreeLeafNode *prev;
        int size;
        bPlusTreeLeafNode(int degree) : size(0), next(nullptr), prev(nullptr) {
            values = new T[degree - 1];
        }
    };  

    /**
     * @brief Struct for b plus tree internal node
    */
    struct bPlusTreeInternalNode {
        T *values;
        bPlusTreeInternalNode **children;
        int size;
        bPlusTreeInternalNode(int degree) : size(0) {
            values = new T[degree - 1];
            children = new bPlusTreeInternalNode*[degree];
            for(int i = 0; i < degree; i++){
                children[i] = nullptr;
            }
        }
    };

    /**
     * @brief Struct for b+ tree node
    */
    struct bPlusTree {
        bPlusTreeLeafNode *head;
        bPlusTreeLeafNode *tail;
        int degree;
        bPlusTree(int degree) : degree(degree), head(nullptr), tail(nullptr) {}
    };

    /**
     * @brief Struct for b tree
    */
    struct bTree {
        bTreeNode *root;
        int degree;
        bTree(int degree) : degree(degree), root(nullptr) {}
    };

    /**
     * @brief Struct for avl tree
    */
    struct avlTree {
        avlNode *root;
        avlTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for binary search tree
    */
    struct binarySearchTree {
        binarySearchNode *root;
        binarySearchTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for binary tree
    */
    struct binaryTree {
        binaryNode *root;
        binaryTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for red black tree
    */
    struct redBlackTree {
        redBlackNode *root;
        redBlackTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for heap tree
    */
    struct heapTree {
        heapNode *root;
        heapTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for trie tree
    */
    struct trieTree {
        trieNode *root;
        trieTree() : root(new trieNode()) {}
    };

    /**
     * @brief Struct for suffix tree
    */
    struct suffixTree {
        suffixTreeNode *root;
        suffixTree() : root(new suffixTreeNode()) {}
    };

    /**
     * @brief Struct for segment tree
    */
    struct segmentTree {
        segmentTreeNode *root;
        segmentTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for k dimensional tree
    */
    struct kDimensionalTree {
        kDimensionalTreeNode *root;
        kDimensionalTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for quad tree
    */
    struct quadTree {
        quadTreeNode *root;
        quadTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for fenwick tree
    */
    struct fenwickTree {
        fenwickTreeNode *root;
        fenwickTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for interval tree
    */
    struct intervalTree {
        intervalTreeNode *root;
        intervalTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for r tree
    */
    struct rTree {
        rTreeNode *root;
        rTree() : root(nullptr) {}
    };

    /**
     * @brief Struct for b plus tree
    */
    struct bPlusTree {
        bPlusTreeInternalNode *root;
        int degree;
        bPlusTree(int degree) : degree(degree), root(nullptr) {}
    };

    /**
     * @brief Struct for b tree
    */
    struct bTree {
        bTreeNode *root;
        int degree;
        bTree(int degree) : degree(degree), root(nullptr) {}
    };

    /**
     * @brief Struct for avl tree
    */
    struct avlTree {
        avlNode *root;
        avlTree() : root(nullptr) {}
    };


    TreeType type;  // Type of tree

    Tree(TreeType type) : type(type) {} // Constructor
    TraversalOrder order;   // Order of traversal
    int nodeCount; // Number of nodes
    int height; // Height of tree
    int degree; // Degree of tree
    int size; // Size of tree
    int *values; // Values of tree
    T *keys; // Keys of tree
    binaryNode *root; // Root of binary tree
    avlNode *root; // Root of avl tree
    binarySearchNode *root; // Root of binary search tree
    redBlackNode *root; // Root of red black tree
    heapNode *root; // Root of heap tree
    bTreeNode *root; // Root of b tree
    bPlusTreeNode *root; // Root of b plus tree
    trieNode *root; // Root of trie tree
    suffixTreeNode *root; // Root of suffix tree
    segmentTreeNode *root; // Root of segment tree
    kDimensionalTreeNode *root; // Root of k dimensional tree
    quadTreeNode *root; // Root of quad tree
    fenwickTreeNode *root; // Root of fenwick tree
    intervalTreeNode *root; // Root of interval tree
    rTreeNode *root; // Root of r tree
    bPlusTreeLeafNode *root; // Root of b plus tree leaf node
    bPlusTreeInternalNode *root; // Root of b plus tree internal node
    bPlusTree *root; // Root of b plus tree
    bTree *root; // Root of b tree
    avlTree *root; // Root of avl tree
    binarySearchTree *root; // Root of binary search tree
    binaryTree *root; // Root of binary tree
    redBlackTree *root; // Root of red black tree
    heapTree *root; // Root of heap tree
    trieTree *root; // Root of trie tree
    suffixTree *root; // Root of suffix tree
    segmentTree *root; // Root of segment tree
    kDimensionalTree *root; // Root of k dimensional tree
    quadTree *root; // Root of quad tree
    fenwickTree *root; // Root of fenwick tree
    intervalTree *root; // Root of interval tree
    rTree *root; // Root of r tree
    

    /**
     * @brief insert a value into binary tree
    TODO: Implement queue, push, pop
    */
    void insertBinary(T value){
        if(root == nullptr){
            root = new binaryNode(value);
            return;
        }
        queue<binaryNode*> q;
        q.push(root);
        while(!q.empty()){
            binaryNode *temp = q.front();
            q.pop();
            if(temp->left == nullptr){
                temp->left = new binaryNode(value);
                return;
            }
            if(temp->right == nullptr){
                temp->right = new binaryNode(value);
                return;
            }
            q.push(temp->left);
            q.push(temp->right);
        }
    }

    void insertBinarySearch(T value){
        if(root == nullptr){
            root = new binarySearchNode(value);
            return;
        }
        binarySearchNode *temp = root;
        while(temp != nullptr){
            if(value < temp->value){
                if(temp->left == nullptr){
                    temp->left = new binarySearchNode(value);
                    return;
                }
                temp = temp->left;
            } else {
                if(temp->right == nullptr){
                    temp->right = new binarySearchNode(value);
                    return;
                }
                temp = temp->right;
            }
        }
    }

    void insertAVL(T value){
        root = insertAVL(root, value);
    }

    avlNode* insertAVL(avlNode *node, T value){
        if(node == nullptr){
            return new avlNode(value);
        }
        if(value < node->value){
            node->left = insertAVL(node->left, value);
        } else {
            node->right = insertAVL(node->right, value);
        }
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if(balance > 1 && value < node->left->value){
            return rightRotate(node);
        }
        if(balance < -1 && value > node->right->value){
            return leftRotate(node);
        }
        if(balance > 1 && value > node->left->value){
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if(balance < -1 && value < node->right->value){
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void insertRedBlack(T value){
        root = insertRedBlack(root, value);
    }

    redBlackNode* insertRedBlack(redBlackNode *node, T value){
        if(node == nullptr){
            return new redBlackNode(value);
        }
        if(value < node->value){
            node->left = insertRedBlack(node->left, value);
            node->left->parent = node;
        } else {
            node->right = insertRedBlack(node->right, value);
            node->right->parent = node;
        }
        if(isRed(node->right) && !isRed(node->left)){
            node = leftRotate(node);
        }
        if(isRed(node->left) && isRed(node->left->left)){
            node = rightRotate(node);
        }
        if(isRed(node->left) && isRed(node->right)){
            flipColors(node);
        }
        return node;
    }

    void insertHeap(T value){
        if(root == nullptr){
            root = new heapNode(value);
            return;
        }
        queue<heapNode*> q;
        q.push(root);
        while(!q.empty()){
            heapNode *temp = q.front();
            q.pop();
            if(temp->left == nullptr){
                temp->left = new heapNode(value);
                return;
            }
            if(temp->right == nullptr){
                temp->right = new heapNode(value);
                return;
            }
            q.push(temp->left);
            q.push(temp->right);
        }
    }

    void insertBTree(T value){
        if(root == nullptr){
            root = new bTreeNode(degree);
            root->values[root->size++] = value;
            return;
        }
        if(root->size == degree - 1){
            bTreeNode *newNode = new bTreeNode(degree);
            newNode->children[0] = root;
            splitChild(newNode, 0);
            int i = 0;
            if(newNode->values[0] < value){
                i++;
            }
            insertNonFull(newNode->children[i], value);
            root = newNode;
        } else {
            insertNonFull(root, value);
        }
    }

    void insertNonFull(bTreeNode *node, T value){
        int i = node->size - 1;
        if(node->isLeaf){
            while(i >= 0 && node->values[i] > value){
                node->values[i + 1] = node->values[i];
                i--;
            }
            node->values[i + 1] = value;
            node->size++;
        } else {
            while(i >= 0 && node->values[i] > value){
                i--;
            }
            i++;
            if(node->children[i]->size == degree - 1){
                splitChild(node, i);
                if(node->values[i] < value){
                    i++;
                }
            }
            insertNonFull(node->children[i], value);
        }
    }

    void insertBPlusTree(T value){
        if(root == nullptr){
            root = new bPlusTreeLeafNode(degree);
            root->values[root->size++] = value;
            return;
        }
        bPlusTreeLeafNode *node = findLeafNode(root, value);
        if(node->size < degree - 1){
            int i = 0;
            while(i < node->size && node->values[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            node->values[i] = value;
            node->size++;
        } else {
            bPlusTreeLeafNode *newNode = new bPlusTreeLeafNode(degree);
            int *temp = new int[degree];
            for(int i = 0; i < node->size; i++){
                temp[i] = node->values[i];
            }
            int i = 0;
            while(i < node->size && temp[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            node->values[i] = value;
            node->size = (degree + 1) / 2;
            newNode->size = degree + 1 - node->size;
            for(int i = 0; i < node->size; i++){
                node->values[i] = temp[i];
            }
            for(int i = 0; i < newNode->size; i++){
                newNode->values[i] = temp[node->size + i];
            }
            newNode->next = node->next;
            node->next = newNode;
            newNode->prev = node;
            if(newNode->next != nullptr){
                newNode->next->prev = newNode;
            }
            if(node == root){
                bPlusTreeInternalNode *newRoot = new bPlusTreeInternalNode(degree);
                newRoot->values[newRoot->size++] = newNode->values[0];
                newRoot->children[0] = node;
                newRoot->children[1] = newNode;
                root = newRoot;
            } else {
                insertInternalNode(root, node, newNode->values[0]);
            }
        }
    }

    void insertInternalNode(bPlusTreeInternalNode *node, bPlusTreeLeafNode *child, T value){
        if(node->size < degree - 1){
            int i = 0;
            while(i < node->size && node->values[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            for(int j = node->size + 1; j > i + 1; j--){
                node->children[j] = node->children[j - 1];
            }
            node->values[i] = value;
            node->children[i + 1] = child;
            node->size++;
        } else {
            bPlusTreeInternalNode *newNode = new bPlusTreeInternalNode(degree);
            int *temp = new int[degree];
            bPlusTreeLeafNode **tempChildren = new bPlusTreeLeafNode*[degree + 1];
            for(int i = 0; i < node->size; i++){
                temp[i] = node->values[i];
            }
            for(int i = 0; i < node->size + 1; i++){
                tempChildren[i] = node->children[i];
            }
            int i = 0;
            while(i < node->size && temp[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            for(int j = node->size + 1; j > i + 1; j--){
                node->children[j] = node->children[j - 1];
            }
            node->values[i] = value;
            node->children[i + 1] = child;
            node->size = (degree + 1) / 2;
            newNode->size = degree - 1 - node->size;
            for(int i = 0; i < node->size; i++){
                node->values[i] = temp[i];
            }
            for(int i = 0; i < newNode->size; i++){
                newNode->values[i] = temp[node->size + i];
            }
            for(int i = 0; i < node->size + 1; i++){
                node->children[i] = tempChildren[i];
            }
            for(int i = 0; i < newNode->size + 1; i++){
                newNode->children[i] = tempChildren[node->size + i];
            }
            if(node == root){
                bPlusTreeInternalNode *newRoot = new bPlusTreeInternalNode(degree);
                newRoot->values[newRoot->size++] = newNode->values[0];
                newRoot->children[0] = node;
                newRoot->children[1] = newNode;
                root = newRoot;
            } else {
                insertInternalNode(root, node, newNode->values[0]);
            }
        }
    }

    void insertTrie(T value){
        trieNode *temp = root;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                temp->children[index] = new trieNode();
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = true;
    }

    void insertSuffixTree(T value){
        suffixTreeNode *temp = root;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                temp->children[index] = new suffixTreeNode();
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = true;
    }

    void insertSegmentTree(T value){
        root = insertSegmentTree(root, 0, size - 1, value);
    }

    segmentTreeNode* insertSegmentTree(segmentTreeNode *node, int start, int end, T value){
        if(node == nullptr){
            return new segmentTreeNode(start, end);
        }
        if(start == end){
            node->sum = value;
            return node;
        }
        int mid = start + (end - start) / 2;
        node->left = insertSegmentTree(node->left, start, mid, value);
        node->right = insertSegmentTree(node->right, mid + 1, end, value);
        node->sum = node->left->sum + node->right->sum;
        return node;
    }

    void insertKDimensionalTree(T value){
        root = insertKDimensionalTree(root, value);
    }

    kDimensionalTreeNode* insertKDimensionalTree(kDimensionalTreeNode *node, T *value){
        if(node == nullptr){
            return new kDimensionalTreeNode(value);
        }
        int i = 0;
        if(i % 2 == 0){
            if(value[i] < node->point[i]){
                node->left = insertKDimensionalTree(node->left, value);
            } else {
                node->right = insertKDimensionalTree(node->right, value);
            }
        } else {
            if(value[i] < node->point[i]){
                node->left = insertKDimensionalTree(node->left, value);
            } else {
                node->right = insertKDimensionalTree(node->right, value);
            }
        }
        return node;
    }

    void insertQuadTree(T value){
        root = insertQuadTree(root, value);
    }

    quadTreeNode* insertQuadTree(quadTreeNode *node, T value){
        if(node == nullptr){
            return new quadTreeNode(value);
        }
        if(value.x < node->value.x && value.y < node->value.y){
            node->topLeft = insertQuadTree(node->topLeft, value);
        } else if(value.x < node->value.x && value.y >= node->value.y){
            node->bottomLeft = insertQuadTree(node->bottomLeft, value);
        } else if(value.x >= node->value.x && value.y < node->value.y){
            node->topRight = insertQuadTree(node->topRight, value);
        } else {
            node->bottomRight = insertQuadTree(node->bottomRight, value);
        }
        return node;
    }

    void insertFenwickTree(T value){
        if(root == nullptr){
            root = new fenwickTreeNode(value);
            return;
        }
        fenwickTreeNode *temp = root;
        while(temp != nullptr){
            temp->value += value;
            temp = temp->parent;
        }
    }

    void insertIntervalTree(T value){
        root = insertIntervalTree(root, value);
    }

    intervalTreeNode* insertIntervalTree(intervalTreeNode *node, T value){
        if(node == nullptr){
            return new intervalTreeNode(value.start, value.end);
        }
        if(value.start < node->start){
            node->start = value.start;
        }
        if(value.end > node->end){
            node->end = value.end;
        }
        node->max = max(node->max, value.end);
        if(value.start < node->start){
            node->left = insertIntervalTree(node->left, value);
        } else {
            node->right = insertIntervalTree(node->right, value);
        }
        return node;
    }

    void insertRTree(T value){
        root = insertRTree(root, value);
    }

    rTreeNode* insertRTree(rTreeNode *node, T value){
        if(node == nullptr){
            return new rTreeNode(value);
        }
        if(node->children == nullptr){
            node->children = new rTreeNode(value);
            return node;
        }
        int minIndex = 0;
        int minArea = INT_MAX;
        for(int i = 0; i < node->children.size(); i++){
            int area = calculateArea(node->children[i]->value, value);
            if(area < minArea){
                minArea = area;
                minIndex = i;
            }
        }
        node->children[minIndex] = insertRTree(node->children[minIndex], value);
        return node;
    }

    void insertBPlusTreeLeafNode(bPlusTreeLeafNode *node, T value){
        if(node->size < degree - 1){
            int i = 0;
            while(i < node->size && node->values[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            node->values[i] = value;
            node->size++;
        } else {
            bPlusTreeLeafNode *newNode = new bPlusTreeLeafNode(degree);
            int *temp = new int[degree];
            for(int i = 0; i < node->size; i++){
                temp[i] = node->values[i];
            }
            int i = 0;
            while(i < node->size && temp[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            node->values[i] = value;
            node->size = (degree + 1) / 2;
            newNode->size = degree + 1 - node->size;
            for(int i = 0; i < node->size; i++){
                node->values[i] = temp[i];
            }
            for(int i = 0; i < newNode->size; i++){
                newNode->values[i] = temp[node->size + i];
            }
            newNode->next = node->next;
            node->next = newNode;
            newNode->prev = node;
            if(newNode->next != nullptr){
                newNode->next->prev = newNode;
            }
            if(node == root){
                bPlusTreeInternalNode *newRoot = new bPlusTreeInternalNode(degree);
                newRoot->values[newRoot->size++] = newNode->values[0];
                newRoot->children[0] = node;
                newRoot->children[1] = newNode;
                root = newRoot;
            } else {
                insertInternalNode(root, node, newNode->values[0]);
            }
        }
    }   

    void insertBPlusTreeInternalNode(bPlusTreeInternalNode *node, T value){
        if(node->size < degree - 1){
            int i = 0;
            while(i < node->size && node->values[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            for(int j = node->size + 1; j > i + 1; j--){
                node->children[j] = node->children[j - 1];
            }
            node->values[i] = value;
            node->children[i + 1] = child;
            node->size++;
        } else {
            bPlusTreeInternalNode *newNode = new bPlusTreeInternalNode(degree);
            int *temp = new int[degree];
            bPlusTreeLeafNode **tempChildren = new bPlusTreeLeafNode*[degree + 1];
            for(int i = 0; i < node->size; i++){
                temp[i] = node->values[i];
            }
            for(int i = 0; i < node->size + 1; i++){
                tempChildren[i] = node->children[i];
            }
            int i = 0;
            while(i < node->size && temp[i] < value){
                i++;
            }
            for(int j = node->size; j > i; j--){
                node->values[j] = node->values[j - 1];
            }
            for(int j = node->size + 1; j > i + 1; j--){
                node->children[j] = node->children[j - 1];
            }
            node->values[i] = value;
            node->children[i + 1] = child;
            node->size = (degree + 1) / 2;
            newNode->size = degree - 1 - node->size;
            for(int i = 0; i < node->size; i++){
                node->values[i] = temp[i];
            }
            for(int i = 0; i < newNode->size; i++){
                newNode->values[i] = temp[node->size + i];
            }
            for(int i = 0; i < node->size + 1; i++){
                node->children[i]
            }
            for(int i = 0; i < newNode->size + 1; i++){
                newNode->children[i] = tempChildren[node->size + i];
            }
            if(node == root){
                bPlusTreeInternalNode *newRoot = new bPlusTreeInternalNode(degree);
                newRoot->values[newRoot->size++] = newNode->values[0];
                newRoot->children[0] = node;
                newRoot->children[1] = newNode;
                root = newRoot;
            } else {
                insertInternalNode(root, node, newNode->values[0]);
            }
        }
    }

    void removeBinary(T value){
        if(root == nullptr){
            return;
        }
        queue<binaryNode*> q;
        q.push(root);
        binaryNode *keyNode = nullptr;
        binaryNode *temp = nullptr;
        while(!q.empty()){
            temp = q.front();
            q.pop();
            if(temp->value == value){
                keyNode = temp;
            }
            if(temp->left != nullptr){
                q.push(temp->left);
            }
            if(temp->right != nullptr){
                q.push(temp->right);
            }
        }
        if(keyNode != nullptr){
            keyNode->value = temp->value;
            deleteDeepestNode(temp);
        }
    }

    void removeBinarySearch(T value){
        root = removeBinarySearch(root, value);
    }

    binarySearchNode* removeBinarySearch(binarySearchNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(value < node->value){
            node->left = removeBinarySearch(node->left, value);
        } else if(value > node->value){
            node->right = removeBinarySearch(node->right, value);
        } else {
            if(node->left == nullptr){
                binarySearchNode *temp = node->right;
                delete node;
                return temp;
            } else if(node->right == nullptr){
                binarySearchNode *temp = node->left;
                delete node;
                return temp;
            }
            binarySearchNode *temp = minValueNode(node->right);
            node->value = temp->value;
            node->right = removeBinarySearch(node->right, temp->value);
        }
        return node;
    }

    binarySearchNode* minValueNode(binarySearchNode *node){
        binarySearchNode *current = node;
        while(current->left != nullptr){
            current = current->left;
        }
        return current;
    }

    void removeAVL(T value){
        root = removeAVL(root, value);
    }

    avlNode* removeAVL(avlNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(value < node->value){
            node->left = removeAVL(node->left, value);
        } else if(value > node->value){
            node->right = removeAVL(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                avlNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                avlNode *temp = minValueNode(node->right);
                node->value = temp->value;
                node->right = removeAVL(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return node;
        }
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if(balance > 1 && getBalance(node->left) >= 0){
            return rightRotate(node);
        }
        if(balance > 1 && getBalance(node->left) < 0){
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if(balance < -1 && getBalance(node->right) <= 0){
            return leftRotate(node);
        }
        if(balance < -1 && getBalance(node->right) > 0){
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void removeRedBlack(T value){
        root = removeRedBlack(root, value);
    }

    redBlackNode* removeRedBlack(redBlackNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(value < node->value){
            node->left = removeRedBlack(node->left, value);
        } else if(value > node->value){
            node->right = removeRedBlack(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                redBlackNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                redBlackNode *temp = minValueNode(node->right);
                node->value = temp->value;
                node->right = removeRedBlack(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return node;
        }
        if(isRed(node->right) && !isRed(node->left)){
            node = leftRotate(node);
        }
        if(isRed(node->left) && isRed(node->left->left)){
            node = rightRotate(node);
        }
        if(isRed(node->left) && isRed(node->right)){
            flipColors(node);
        }
        return node;
    }

    void removeHeap(T value){
        if(root == nullptr){
            return;
        }
        queue<heapNode*> q;
        q.push(root);
        heapNode *keyNode = nullptr;
        heapNode *temp = nullptr;
        while(!q.empty()){
            temp = q.front();
            q.pop();
            if(temp->value == value){
                keyNode = temp;
            }
            if(temp->left != nullptr){
                q.push(temp->left);
            }
            if(temp->right != nullptr){
                q.push(temp->right);
            }
        }
        if(keyNode != nullptr){
            keyNode->value = temp->value;
            deleteDeepestNode(temp);
        }
    }

    void removeBTree(T value){
        if(root == nullptr){
            return;
        }
        removeBTree(root, value);
        if(root->size == 0){
            bTreeNode *temp = root;
            if(root->isLeaf){
                root = nullptr;
            } else {
                root = root->children[0];
            }
            delete temp;
        }
    }

    void removeBTree(bTreeNode *node, T value){
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            if(node->isLeaf){
                for(int j = i + 1; j < node->size; j++){
                    node->values[j - 1] = node->values[j];
                }
                node->size--;
            } else {
                T temp = node->values[i];
                if(node->children[i]->size >= degree){
                    bTreeNode *pred = getPredecessor(node, i);
                    node->values[i] = pred->values[pred->size - 1];
                    removeBTree(pred, pred->values[pred->size - 1]);
                } else if(node->children[i + 1]->size >= degree){
                    bTreeNode *succ = getSuccessor(node, i);
                    node->values[i] = succ->values[0];
                    removeBTree(succ, succ->values[0]);
                } else {
                    merge(node, i);
                    removeBTree(node->children[i], temp);
                }
            }
        } else {
            if(node->isLeaf){
                return;
            }
            bool flag = (i == node->size);
            if(node->children[i]->size < degree){
                fill(node, i);
            }
            if(flag && i > node->size){
                removeBTree(node->children[i - 1], value);
            } else {
                removeBTree(node->children[i], value);
            }
        }
    }

    bTreeNode* getPredecessor(bTreeNode *node, int index){
        bTreeNode *temp = node->children[index];
        while(!temp->isLeaf){
            temp = temp->children[temp->size];
        }
        return temp;
    }

    bTreeNode* getSuccessor(bTreeNode *node, int index){
        bTreeNode *temp = node->children[index + 1];
        while(!temp->isLeaf){
            temp = temp->children[0];
        }
        return temp;
    }

    void merge(bTreeNode *node, int index){
        bTreeNode *child = node->children[index];
        bTreeNode *sibling = node->children[index + 1];
        child->values[degree - 1] = node->values[index];
        for(int i = 0; i < degree - 1; i++){
            child->values[degree + i] = sibling->values[i];
        }
        if(!child->isLeaf){
            for(int i = 0; i < degree; i++){
                child->children[degree + i] = sibling->children[i];
            }
        }
        for(int i = index + 1; i < node->size; i++){
            node->values[i - 1] = node->values[i];
        }
        for(int i = index + 2; i <= node->size; i++){
            node->children[i - 1] = node->children[i];
        }
        child->size += sibling->size + 1;
        node->size--;
        delete sibling;
    }

    void fill(bTreeNode *node, int index){
        if(index != 0 && node->children[index - 1]->size >= degree){
            borrowFromPrev(node, index);
        } else if(index != node->size && node->children[index + 1]->size >= degree){
            borrowFromNext(node, index);
        } else {
            if(index != node->size){
                merge(node, index);
            } else {
                merge(node, index - 1);
            }
        }
    }

    void borrowFromPrev(bTreeNode *node, int index){
        bTreeNode *child = node->children[index];
        bTreeNode *sibling = node->children[index - 1];
        for(int i = child->size - 1; i >= 0; i--){
            child->values[i + 1] = child->values[i];
        }
        if(!child->isLeaf){
            for(int i = child->size; i >= 0; i--){
                child->children[i + 1] = child->children[i];
            }
        }
        child->values[0] = node->values[index - 1];
        if(!child->isLeaf){
            child->children[0] = sibling->children[sibling->size];
        }
        node->values[index - 1] = sibling->values[sibling->size - 1];
        child->size++;
        sibling->size--;
    }

    void borrowFromNext(bTreeNode *node, int index){
        bTreeNode *child = node->children[index];
        bTreeNode *sibling = node->children[index + 1];
        child->values[child->size] = node->values[index];
        if(!child->isLeaf){
            child->children[child->size + 1] = sibling->children[0];
        }
        node->values[index] = sibling->values[0];
        for(int i = 1; i < sibling->size; i++){
            sibling->values[i - 1] = sibling->values[i];
        }
        if(!sibling->isLeaf){
            for(int i = 1; i <= sibling->size; i++){
                sibling->children[i - 1] = sibling->children[i];
            }
        }
        child->size++;
        sibling->size--;
    }

    void removeBPlusTree(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTree(node->next, value);
        }
    }

    void removeTrie(T value){
        trieNode *temp = root;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSuffixTree(T value){
        suffixTreeNode *temp = root;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSegmentTree(T value){
        root = removeSegmentTree(root, 0, size - 1, value);
    }

    segmentTreeNode* removeSegmentTree(segmentTreeNode *node, int start, int end, T value){
        if(node == nullptr){
            return node;
        }
        if(start == end){
            node->sum = 0;
            return node;
        }
        int mid = start + (end - start) / 2;
        if(value <= mid){
            node->left = removeSegmentTree(node->left, start, mid, value);
        } else {
            node->right = removeSegmentTree(node->right, mid + 1, end, value);
        }
        node->sum = node->left->sum + node->right->sum;
        return node;
    }

    void removeKDimensionalTree(T value){
        root = removeKDimensionalTree(root, value);
    }

    kDimensionalTreeNode* removeKDimensionalTree(kDimensionalTreeNode *node, T *value){
        if(node == nullptr){
            return node;
        }
        int i = 0;
        if(i % 2 == 0){
            if(value[i] < node->point[i]){
                node->left = removeKDimensionalTree(node->left, value);
            } else {
                node->right = removeKDimensionalTree(node->right, value);
            }
        } else {
            if(value[i] < node->point[i]){
                node->left = removeKDimensionalTree(node->left, value);
            } else {
                node->right = removeKDimensionalTree(node->right, value);
            }
        }
        return node;
    }

    void removeQuadTree(T value){
        root = removeQuadTree(root, value);
    }

    quadTreeNode* removeQuadTree(quadTreeNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(value.x < node->value.x && value.y < node->value.y){
            node->topLeft = removeQuadTree(node->topLeft, value);
        } else if(value.x < node->value.x && value.y >= node->value.y){
            node->bottomLeft = removeQuadTree(node->bottomLeft, value);
        } else if(value.x >= node->value.x && value.y < node->value.y){
            node->topRight = removeQuadTree(node->topRight, value);
        } else {
            node->bottomRight = removeQuadTree(node->bottomRight, value);
        }
        return node;
    }

    void removeFenwickTree(T value){
        if(root == nullptr){
            return;
        }
        fenwickTreeNode *temp = root;
        while(temp != nullptr){
            temp->value -= value;
            temp = temp->parent;
        }
    }

    void removeIntervalTree(T value){
        root = removeIntervalTree(root, value);
    }

    intervalTreeNode* removeIntervalTree(intervalTreeNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(value.start < node->start){
            node->start = value.start;
        }
        if(value.end > node->end){
            node->end = value.end;
        }
        node->max = max(node->max, value.end);
        if(value.start < node->start){
            node->left = removeIntervalTree(node->left, value);
        } else {
            node->right = removeIntervalTree(node->right, value);
        }
        return node;
    }

    void removeRTree(T value){
        root = removeRTree(root, value);
    }

    rTreeNode* removeRTree(rTreeNode *node, T value){
        if(node == nullptr){
            return node;
        }
        if(node->children == nullptr){
            return node;
        }
        int minIndex = 0;
        int minArea = INT_MAX;
        for(int i = 0; i < node->children.size(); i++){
            int area = calculateArea(node->children[i]->value, value);
            if(area < minArea){
                minArea = area;
                minIndex = i;
            }
        }
        node->children[minIndex] = removeRTree(node->children[minIndex], value);
        return node;
    }

    void removeBPlusTreeLeafNode(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTreeLeafNode(node->next, value);
        }
    }

    void removeBPlusTreeInternalNode(bPlusTreeInternalNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            for(int j = i + 2; j <= node->size; j++){
                node->children[j - 1] = node->children[j];
            }
            node->size--;
        } else {
            removeBPlusTreeInternalNode(node->children[i], value);
        }
    }

    void removeAVL(avlNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value < node->value){
            removeAVL(node->left, value);
        } else if(value > node->value){
            removeAVL(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                avlNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                avlNode *temp = minValueNode(node->right);
                node->value = temp->value;
                removeAVL(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return;
        }
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if(balance > 1 && getBalance(node->left) >= 0){
            node = rightRotate(node);
        }
        if(balance > 1 && getBalance(node->left) < 0){
            node->left = leftRotate(node->left);
            node = rightRotate(node);
        }
        if(balance < -1 && getBalance(node->right) <= 0){
            node = leftRotate(node);
        }
        if(balance < -1 && getBalance(node->right) > 0){
            node->right = rightRotate(node->right);
            node = leftRotate(node);
        }
    }

    void removeRedBlack(redBlackNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value < node->value){
            removeRedBlack(node->left, value);
        } else if(value > node->value){
            removeRedBlack(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                redBlackNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                redBlackNode *temp = minValueNode(node->right);
                node->value = temp->value;
                removeRedBlack(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return;
        }
        if(isRed(node->right) && !isRed(node->left)){
            node = leftRotate(node);
        }
        if(isRed(node->left) && isRed(node->left->left)){
            node = rightRotate(node);
        }
        if(isRed(node->left) && isRed(node->right)){
            flipColors(node);
        }
    }

    void removeHeap(heapNode *node, T value){
        if(node == nullptr){
            return;
        }
        queue<heapNode*> q;
        q.push(node);
        heapNode *keyNode = nullptr;
        heapNode *temp = nullptr;
        while(!q.empty()){
            temp = q.front();
            q.pop();
            if(temp->value == value){
                keyNode = temp;
            }
            if(temp->left != nullptr){
                q.push(temp->left);
            }
            if(temp->right != nullptr){
                q.push(temp->right);
            }
        }
        if(keyNode != nullptr){
            keyNode->value = temp->value;
            deleteDeepestNode(temp);
        }
    }

    void removeBTree(bTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        removeBTree(node, value);
        if(node->size == 0){
            bTreeNode *temp = node;
            if(node->isLeaf){
                node = nullptr;
            } else {
                node = node->children[0];
            }
            delete temp;
        }
    }

    void removeBPlusTree(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTree(node->next, value);
        }
    }

    void removeTrie(trieNode *node, T value){
        if(node == nullptr){
            return;
        }
        trieNode *temp = node;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSuffixTree(suffixTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        suffixTreeNode *temp = node;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSegmentTree(segmentTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        removeSegmentTree(node, 0, size - 1, value);
    }

    void removeSegmentTree(segmentTreeNode *node, int start, int end, T value){
        if(node == nullptr){
            return;
        }
        if(start == end){
            node->sum = 0;
            return;
        }
        int mid = start + (end - start) / 2;
        if(value <= mid){
            removeSegmentTree(node->left, start, mid, value);
        } else {
            removeSegmentTree(node->right, mid + 1, end, value);
        }
        node->sum = node->left->sum + node->right->sum;
    }

    void removeKDimensionalTree(kDimensionalTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        if(i % 2 == 0){
            if(value[i] < node->point[i]){
                removeKDimensionalTree(node->left, value);
            } else {
                removeKDimensionalTree(node->right, value);
            }
        } else {
            if(value[i] < node->point[i]){
                removeKDimensionalTree(node->left, value);
            } else {
                removeKDimensionalTree(node->right, value);
            }
        }
    }

    void removeQuadTree(quadTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value.x < node->value.x && value.y < node->value.y){
            removeQuadTree(node->topLeft, value);
        } else if(value.x < node->value.x && value.y >= node->value.y){
            removeQuadTree(node->bottomLeft, value);
        } else if(value.x >= node->value.x && value.y < node->value.y){
            removeQuadTree(node->topRight, value);
        } else {
            removeQuadTree(node->bottomRight, value);
        }
    }

    void removeFenwickTree(fenwickTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        fenwickTreeNode *temp = node;
        while(temp != nullptr){
            temp->value -= value;
            temp = temp->parent;
        }
    }

    void removeIntervalTree(intervalTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value.start < node->start){
            node->start = value.start;
        }
        if(value.end > node->end){
            node->end = value.end;
        }
        node->max = max(node->max, value.end);
        if(value.start < node->start){
            removeIntervalTree(node->left, value);
        } else {
            removeIntervalTree(node->right, value);
        }
    }

    void removeRTree(rTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(node->children == nullptr){
            return;
        }
        int minIndex = 0;
        int minArea = INT_MAX;
        for(int i = 0; i < node->children.size(); i++){
            int area = calculateArea(node->children[i]->value, value);
            if(area < minArea){
                minArea = area;
                minIndex = i;
            }
        }
        removeRTree(node->children[minIndex], value);
    }

    void removeBPlusTreeLeafNode(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTreeLeafNode(node->next, value);
        }
    }

    void removeBPlusTreeInternalNode(bPlusTreeInternalNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            for(int j = i + 2; j <= node->size; j++){
                node->children[j - 1] = node->children[j];
            }
            node->size--;
        } else {
            removeBPlusTreeInternalNode(node->children[i], value);
        }
    }

    void removeAVL(avlNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value < node->value){
            removeAVL(node->left, value);
        } else if(value > node->value){
            removeAVL(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                avlNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                avlNode *temp = minValueNode(node->right);
                node->value = temp->value;
                removeAVL(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return;
        }
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if(balance > 1 && getBalance(node->left) >= 0){
            node = rightRotate(node);
        }
        if(balance > 1 && getBalance(node->left) < 0){
            node->left = leftRotate(node->left);
            node = rightRotate(node);
        }
        if(balance < -1 && getBalance(node->right) <= 0){
            node = leftRotate(node);
        }
        if(balance < -1 && getBalance(node->right) > 0){
            node->right = rightRotate(node->right);
            node = leftRotate(node);
        }
    }

    void removeRedBlack(redBlackNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value < node->value){
            removeRedBlack(node->left, value);
        } else if(value > node->value){
            removeRedBlack(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                redBlackNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                redBlackNode *temp = minValueNode(node->right);
                node->value = temp->value;
                removeRedBlack(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return;
        }
        if(isRed(node->right) && !isRed(node->left)){
            node = leftRotate(node);
        }
        if(isRed(node->left) && isRed(node->left->left)){
            node = rightRotate(node);
        }
        if(isRed(node->left) && isRed(node->right)){
            flipColors(node);
        }
    }

    void removeHeap(heapNode *node, T value){
        if(node == nullptr){
            return;
        }
        queue<heapNode*> q;
        q.push(node);
        heapNode *keyNode = nullptr;
        heapNode *temp = nullptr;
        while(!q.empty()){
            temp = q.front();
            q.pop();
            if(temp->value == value){
                keyNode = temp;
            }
            if(temp->left != nullptr){
                q.push(temp->left);
            }
            if(temp->right != nullptr){
                q.push(temp->right);
            }
        }
        if(keyNode != nullptr){
            keyNode->value = temp->value;
            deleteDeepestNode(temp);
        }
    }

    void removeBTree(bTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        removeBTree(node, value);
        if(node->size == 0){
            bTreeNode *temp = node;
            if(node->isLeaf){
                node = nullptr;
            } else {
                node = node->children[0];
            }
            delete temp;
        }
    }

    void removeBPlusTree(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTree(node->next, value);
        }
    }

    void removeTrie(trieNode *node, T value){
        if(node == nullptr){
            return;
        }
        trieNode *temp = node;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSuffixTree(suffixTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        suffixTreeNode *temp = node;
        for(int i = 0; i < value.size(); i++){
            int index = value[i] - 'a';
            if(temp->children[index] == nullptr){
                return;
            }
            temp = temp->children[index];
        }
        temp->isEndOfWord = false;
    }

    void removeSegmentTree(segmentTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        removeSegmentTree(node, 0, size - 1, value);
    }

    void removeSegmentTree(segmentTreeNode *node, int start, int end, T value){
        if(node == nullptr){
            return;
        }
        if(start == end){
            node->sum = 0;
            return;
        }
        int mid = start + (end - start) / 2;
        if(value <= mid){
            removeSegmentTree(node->left, start, mid, value);
        } else {
            removeSegmentTree(node->right, mid + 1, end, value);
        }
        node->sum = node->left->sum + node->right->sum;
    }

    void removeKDimensionalTree(kDimensionalTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        if(i % 2 == 0){
            if(value[i] < node->point[i]){
                removeKDimensionalTree(node->left, value);
            } else {
                removeKDimensionalTree(node->right, value);
            }
        } else {
            if(value[i] < node->point[i]){
                removeKDimensionalTree(node->left, value);
            } else {
                removeKDimensionalTree(node->right, value);
            }
        }
    }

    void removeQuadTree(quadTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value.x < node->value.x && value.y < node->value.y){
            removeQuadTree(node->topLeft, value);
        } else if(value.x < node->value.x && value.y >= node->value.y){
            removeQuadTree(node->bottomLeft, value);
        } else if(value.x >= node->value.x && value.y < node->value.y){
            removeQuadTree(node->topRight, value);
        } else {
            removeQuadTree(node->bottomRight, value);
        }
    }

    void removeFenwickTree(fenwickTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        fenwickTreeNode *temp = node;
        while(temp != nullptr){
            temp->value -= value;
            temp = temp->parent;
        }
    }

    void removeIntervalTree(intervalTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value.start < node->start){
            node->start = value.start;
        }
        if(value.end > node->end){
            node->end = value.end;
        }
        node->max = max(node->max, value.end);
        if(value.start < node->start){
            removeIntervalTree(node->left, value);
        } else {
            removeIntervalTree(node->right, value);
        }
    }

    void removeRTree(rTreeNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(node->children == nullptr){
            return;
        }
        int minIndex = 0;
        int minArea = INT_MAX;
        for(int i = 0; i < node->children.size(); i++){
            int area = calculateArea(node->children[i]->value, value);
            if(area < minArea){
                minArea = area;
                minIndex = i;
            }
        }
        removeRTree(node->children[minIndex], value);
    }

    void removeBPlusTreeLeafNode(bPlusTreeLeafNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            node->size--;
        } else {
            removeBPlusTreeLeafNode(node->next, value);
        }
    }

    void removeBPlusTreeInternalNode(bPlusTreeInternalNode *node, T value){
        if(node == nullptr){
            return;
        }
        int i = 0;
        while(i < node->size && node->values[i] < value){
            i++;
        }
        if(i < node->size && node->values[i] == value){
            for(int j = i + 1; j < node->size; j++){
                node->values[j - 1] = node->values[j];
            }
            for(int j = i + 2; j <= node->size; j++){
                node->children[j - 1] = node->children[j];
            }
            node->size--;
        } else {
            removeBPlusTreeInternalNode(node->children[i], value);
        }
    }

    void removeAVL(avlNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(value < node->value){
            removeAVL(node->left, value);
        } else if(value > node->value){
            removeAVL(node->right, value);
        } else {
            if(node->left == nullptr || node->right == nullptr){
                avlNode *temp = node->left ? node->left : node->right;
                if(temp == nullptr){
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                avlNode *temp = minValueNode(node->right);
                node->value = temp->value;
                removeAVL(node->right, temp->value);
            }
        }
        if(node == nullptr){
            return;
        }
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        if(balance > 1 && getBalance(node->left) >= 0){
            node = rightRotate(node);
        }
        if(balance > 1 && getBalance(node->left) < 0){
            node->left = leftRotate(node->left);
            node = rightRotate(node);
        }
        if(balance < -1 && getBalance(node->right) <= 0){
            node = leftRotate(node);
        }
        if(balance < -1 && getBalance(node->right) > 0){
            node->right = rightRotate(node->right);
            node = leftRotate(node);
        }
    }

    //leftRotate
    avlNode* leftRotate(avlNode *node){
        avlNode *temp = node->right;
        avlNode *temp2 = temp->left;
        temp->left = node;
        node->right = temp2;
        node->height = 1 + max(height(node->left), height(node->right));
        temp->height = 1 + max(height(temp->left), height(temp->right));
        return temp;
    }

    //rightRotate
    avlNode* rightRotate(avlNode *node){
        avlNode *temp = node->left;
        avlNode *temp2 = temp->right;
        temp->right = node;
        node->left = temp2;
        node->height = 1 + max(height(node->left), height(node->right));
        temp->height = 1 + max(height(temp->left), height(temp->right));
        return temp;
    }

    //getBalance
    int getBalance(avlNode *node){
        if(node == nullptr){
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    //height
    int height(avlNode *node){
        if(node == nullptr){
            return 0;
        }
        return node->height;
    }

    //minValueNode
    avlNode* minValueNode(avlNode *node){
        avlNode *temp = node;
        while(temp->left != nullptr){
            temp = temp->left;
        }
        return temp;
    }

    //isRed
    bool isRed(redBlackNode *node){
        if(node == nullptr){
            return false;
        }
        return node->color == RED;
    }

    

public:
    Tree(){
        type = BINARY;
    }

    void insert(T value){
        switch (type) {
            case BINARY:
                insertBinary(value);
                break;
            case BINARY_SEARCH:
                insertBinarySearch(value);
                break;
            case AVL:
                insertAVL(value);
                break;
            case RED_BLACK:
                insertRedBlack(value);
                break;
            case HEAP:
                insertHeap(value);
                break;
            case B_TREE:
                insertBTree(value);
                break;
            case B_PLUS_TREE:
                insertBPlusTree(value);
                break;
            case TRIE:
                insertTrie(value);
                break;
            case SUFFIX_TREE:
                insertSuffixTree(value);
                break;
            case SEGMANT_TREE:
                insertSegmentTree(value);
                break;
            case K_DIMENSIONAL_TREE:
                insertKDimensionalTree(value);
                break;
            case QUAD_TREE:
                insertQuadTree(value);
                break;
            case FENWICK_TREE:
                insertFenwickTree(value);
                break;
            case INTERVAL_TREE:
                insertIntervalTree(value);
                break;
            case R_TREE:
                insertRTree(value);
                break;
            default:
                break;
        }
    }

    void remove(T value){
        switch (type) {
            case BINARY:
                removeBinary(value);
                break;
            case BINARY_SEARCH:
                removeBinarySearch(value);
                break;
            case AVL:
                removeAVL(value);
                break;
            case RED_BLACK:
                removeRedBlack(value);
                break;
            case HEAP:
                removeHeap(value);
                break;
            case B_TREE:
                removeBTree(value);
                break;
            case B_PLUS_TREE:
                removeBPlusTree(value);
                break;
            case TRIE:
                removeTrie(value);
                break;
            case SUFFIX_TREE:
                removeSuffixTree(value);
                break;
            case SEGMANT_TREE:
                removeSegmentTree(value);
                break;
            case K_DIMENSIONAL_TREE:
                removeKDimensionalTree(value);
                break;
            case QUAD_TREE:
                removeQuadTree(value);
                break;
            case FENWICK_TREE:
                removeFenwickTree(value);
                break;
            case INTERVAL_TREE:
                removeIntervalTree(value);
                break;
            case R_TREE:
                removeRTree(value);
                break;
            default:
                break;
        }
    }
};

#endif // TREE_h