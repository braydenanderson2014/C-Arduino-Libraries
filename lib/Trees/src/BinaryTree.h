#ifndef BINARYTREE_h
#define BINARYTREE_h

template <typename T>
class BinaryTree {
private:
    struct BinaryNode {
        T data;
        BinaryNode *left;
        BinaryNode *right;
    };
    BinaryNode *root;

    void insert(BinaryNode *&node, T value){
        if(node == nullptr){
            node = new BinaryNode;
            node->data = value;
            node->left = nullptr;
            node->right = nullptr;
        } else if(value < node->data){
            insert(node->left, value);
        } else {
            insert(node->right, value);
        }
    }

    void print(BinaryNode *node){
        if(node != nullptr){
            print(node->left);
            Serial.println(node->data);
            print(node->right);
        }
    }

    void destroy(BinaryNode *node){
        if(node != nullptr){
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:

    BinaryTree(){
        root = nullptr;
    }

    ~BinaryTree(){
        destroy(root);
    }

    void insert(T value){
        insert(root, value);
    }

    void print(){
        print(root);
    }

    bool search(T value){
        BinaryNode *node = root;
        while(node != nullptr){
            if(value == node->data){
                return true;
            } else if(value < node->data){
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return false;
    }

    void remove(T value){
        BinaryNode *node = root;
        BinaryNode *parent = nullptr;
        while(node != nullptr){
            if(value == node->data){
                break;
            } else if(value < node->data){
                parent = node;
                node = node->left;
            } else {
                parent = node;
                node = node->right;
            }
        }
        if(node == nullptr){
            return;
        }
        if(node->left == nullptr && node->right == nullptr){
            if(parent == nullptr){
                root = nullptr;
            } else if(parent->left == node){
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete node;
        } else if(node->left == nullptr){
            if(parent == nullptr){
                root = node->right;
            } else if(parent->left == node){
                parent->left = node->right;
            } else {
                parent->right = node->right;
            }
            delete node;
        } else if(node->right == nullptr){
            if(parent == nullptr){
                root = node->left;
            } else if(parent->left == node){
                parent->left = node->left;
            } else {
                parent->right = node->left;
            }
            delete node;
        } else {
            BinaryNode *temp = node->right;
            parent = nullptr;
            while(temp->left != nullptr){
                parent = temp;
                temp = temp->left;
            }
            node->data = temp->data;
            if(parent == nullptr){
                node->right = temp->right;
            } else {
                parent->left = temp->right;
            }
            delete temp;
        }
    }

    T findMin(){
        BinaryNode *node = root;
        while(node->left != nullptr){
            node = node->left;
        }
        return node->data;
    }

    T findMax(){
        BinaryNode *node = root;
        while(node->right != nullptr){
            node = node->right;
        }
        return node->data;
    }

    int height(BinaryNode *node){
        if(node == nullptr){
            return -1;
        }
        int left = height(node->left);
        int right = height(node->right);
        return left > right ? left + 1 : right + 1;
    }

    int height(){
        return height(root);
    }

    int size(BinaryNode *node){
        if(node == nullptr){
            return 0;
        }
        return size(node->left) + size(node->right) + 1;
    }

    int size(){
        return size(root);
    }

    bool isBalanced(BinaryNode *node){
        if(node == nullptr){
            return true;
        }
        int left = height(node->left);
        int right = height(node->right);
        return abs(left - right) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }

    bool isBalanced(){
        return isBalanced(root);
    }

    bool isBST(BinaryNode *node, T min, T max){
        if(node == nullptr){
            return true;
        }
        if(node->data < min || node->data > max){
            return false;
        }
        return isBST(node->left, min, node->data) && isBST(node->right, node->data, max);
    }

    bool isBST(){
        return isBST(root, findMin(), findMax());
    }

    void levelOrder(){
        Queue<BinaryNode *> queue;
        queue.enqueue(root);
        while(!queue.isEmpty()){
            BinaryNode *node = queue.dequeue();
            Serial.println(node->data);
            if(node->left != nullptr){
                queue.enqueue(node->left);
            }
            if(node->right != nullptr){
                queue.enqueue(node->right);
            }
        }
    }

    void inOrder(){
        Stack<BinaryNode *> stack;
        BinaryNode *node = root;
        while(node != nullptr || !stack.isEmpty()){
            while(node != nullptr){
                stack.push(node);
                node = node->left;
            }
            node = stack.pop();
            Serial.println(node->data);
            node = node->right;
        }
    }

    void preOrder(){
        Stack<BinaryNode *> stack;
        stack.push(root);
        while(!stack.isEmpty()){
            BinaryNode *node = stack.pop();
            Serial.println(node->data);
            if(node->right != nullptr){
                stack.push(node->right);
            }
            if(node->left != nullptr){
                stack.push(node->left);
            }
        }
    }

    void postOrder(){
        Stack<BinaryNode *> stack;
        BinaryNode *node = root;
        BinaryNode *last = nullptr;
        while(node != nullptr || !stack.isEmpty()){
            while(node != nullptr){
                stack.push(node);
                node = node->left;
            }
            while(node == nullptr && !stack.isEmpty()){
                node = stack.peek();
                if(node->right == nullptr || node->right == last){
                    Serial.println(node->data);
                    stack.pop();
                    last = node;
                    node = nullptr;
                } else {
                    node = node->right;
                }
            }
        }
    }

    void morrisInOrder(){
        BinaryNode *node = root;
        while(node != nullptr){
            if(node->left == nullptr){
                Serial.println(node->data);
                node = node->right;
            } else {
                BinaryNode *pre = node->left;
                while(pre->right != nullptr && pre->right != node){
                    pre = pre->right;
                }
                if(pre->right == nullptr){
                    pre->right = node;
                    node = node->left;
                } else {
                    pre->right = nullptr;
                    Serial.println(node->data);
                    node = node->right;
                }
            }
        }
    }

    void morrisPreOrder(){
        BinaryNode *node = root;
        while(node != nullptr){
            if(node->left == nullptr){
                Serial.println(node->data);
                node = node->right;
            } else {
                BinaryNode *pre = node->left;
                while(pre->right != nullptr && pre->right != node){
                    pre = pre->right;
                }
                if(pre->right == nullptr){
                    Serial.println(node->data);
                    pre->right = node;
                    node = node->left;
                } else {
                    pre->right = nullptr;
                    node = node->right;
                }
            }
        }
    }

    void morrisPostOrder(){
        BinaryNode *dummy = new BinaryNode;
        dummy->left = root;
        BinaryNode *node = dummy;
        while(node != nullptr){
            if(node->left == nullptr){
                node = node->right;
            } else {
                BinaryNode *pre = node->left;
                while(pre->right != nullptr && pre->right != node){
                    pre = pre->right;
                }
                if(pre->right == nullptr){
                    pre->right = node;
                    node = node->left;
                } else {
                    reverse(node->left, pre);
                    BinaryNode *temp = pre;
                    while(temp != node->left){
                        Serial.println(temp->data);
                        temp = temp->right;
                    }
                    Serial.println(temp->data);
                    reverse(pre, node->left);
                    pre->right = nullptr;
                    node = node->right;
                }
            }
        }
        delete dummy;
    }

    void reverse(BinaryNode *from, BinaryNode *to){
        if(from == to){
            return;
        }
        BinaryNode *x = from;
        BinaryNode *y = from->right;
        BinaryNode *z;
        while(x != to){
            z = y->right;
            y->right = x;
            x = y;
            y = z;
        }
    }

    void printPaths(){
        int path[1000];
        printPaths(root, path, 0);
    }

    void printPaths(BinaryNode *node, int path[], int length){
        if(node == nullptr){
            return;
        }
        path[length++] = node->data;
        if(node->left == nullptr && node->right == nullptr){
            for(int i = 0; i < length; i++){
                Serial.println(path[i]);
            }
            Serial.println();
        } else {
            printPaths(node->left, path, length);
            printPaths(node->right, path, length);
        }
    }

    void mirror(){
        mirror(root);
    }

    void mirror(BinaryNode *node){
        if(node == nullptr){
            return;
        }
        BinaryNode *temp = node->left;
        node->left = node->right;
        node->right = temp;
        mirror(node->left);
        mirror(node->right);
    }

    bool isMirror(BinaryNode *node1, BinaryNode *node2){
        if(node1 == nullptr && node2 == nullptr){
            return true;
        }
        if(node1 == nullptr || node2 == nullptr){
            return false;
        }
        return node1->data == node2->data && isMirror(node1->left, node2->right) && isMirror(node1->right, node2->left);
    }

    bool isSymmetric(){
        return isMirror(root, root);
    }

    bool hasPathSum(int sum){
        return hasPathSum(root, sum);
    }

    bool hasPathSum(BinaryNode *node, int sum){
        if(node == nullptr){
            return sum == 0;
        }
        return hasPathSum(node->left, sum - node->data) || hasPathSum(node->right, sum - node->data);
    }

    void doubleTree(){
        doubleTree(root);
    }

    void doubleTree(BinaryNode *node){
        if(node == nullptr){
            return;
        }
        doubleTree(node->left);
        doubleTree(node->right);
        BinaryNode *temp = new BinaryNode;
        temp->data = node->data;
        temp->left = node->left;
        node->left = temp;
    }

    bool sameTree(BinaryTree<T> *tree){
        return sameTree(root, tree->root);
    }

    bool sameTree(BinaryNode *node1, BinaryNode *node2){
        if(node1 == nullptr && node2 == nullptr){
            return true;
        }
        if(node1 == nullptr || node2 == nullptr){
            return false;
        }
        return node1->data == node2->data && sameTree(node1->left, node2->left) && sameTree(node1->right, node2->right);
    }

    bool isBST2(){
        return isBST2(root, nullptr, nullptr);
    }

    bool isBST2(BinaryNode *node, T min, T max){
        if(node == nullptr){
            return true;
        }
        if((min != nullptr && node->data <= min) || (max != nullptr && node->data > max)){
            return false;
        }
        return isBST2(node->left, min, node->data) && isBST2(node->right, node->data, max);
    }

    void trimOutsideRange(T min, T max){
        trimOutsideRange(root, min, max);
    }

    void trimOutsideRange(BinaryNode *node, T min, T max){
        if(node == nullptr){
            return;
        }
        node->left = trimOutsideRange(node->left, min, max);
        node->right = trimOutsideRange(node->right, min, max);
        if(node->data < min){
            BinaryNode *temp = node->right;
            delete node;
            return temp;
        }
        if(node->data > max){
            BinaryNode *temp = node->left;
            delete node;
            return temp;
        }
        return node;
    }

    void printAncestors(T value){
        int ancestors[1000];
        printAncestors(root, value, ancestors, 0);
    }

    bool printAncestors(BinaryNode *node, T value, int ancestors[], int length){
        if(node == nullptr){
            return false;
        }
        if(node->data == value){
            for(int i = 0; i < length; i++){
                Serial.println(ancestors[i]);
            }
            return true;
        }
        ancestors[length++] = node->data;
        if(printAncestors(node->left, value, ancestors, length) || printAncestors(node->right, value, ancestors, length)){
            return true;
        }
        return false;
    }

    void printLeaves(){
        printLeaves(root);
    }

    void printLeaves(BinaryNode *node){
        if(node == nullptr){
            return;
        }
        if(node->left == nullptr && node->right == nullptr){
            Serial.println(node->data);
        }
        printLeaves(node->left);
        printLeaves(node->right);
    }

    void printLeftView(){
        printLeftView(root, 1);
    }

    void printLeftView(BinaryNode *node, int level){
        static int maxLevel = 0;
        if(node == nullptr){
            return;
        }
        if(level > maxLevel){
            Serial.println(node->data);
            maxLevel = level;
        }
        printLeftView(node->left, level + 1);
        printLeftView(node->right, level + 1);
    }

    void printRightView(){
        printRightView(root, 1);
    }

    void printRightView(BinaryNode *node, int level){
        static int maxLevel = 0;
        if(node == nullptr){
            return;
        }
        if(level > maxLevel){
            Serial.println(node->data);
            maxLevel = level;
        }
        printRightView(node->right, level + 1);
        printRightView(node->left, level + 1);
    }

    void printTopView(){
        map<int, T> topView;
        queue<pair<BinaryNode *, int>> queue;
        queue.push(make_pair(root, 0));
        while(!queue.empty()){
            pair<BinaryNode *, int> front = queue.front();
            queue.pop();
            if(topView.find(front.second) == topView.end()){
                topView[front.second] = front.first->data;
            }
            if(front.first->left != nullptr){
                queue.push(make_pair(front.first->left, front.second - 1));
            }
            if(front.first->right != nullptr){
                queue.push(make_pair(front.first->right, front.second + 1));
            }
        }
        for(auto it = topView.begin(); it != topView.end(); it++){
            Serial.println(it->second);
        }
    }

    void printBottomView(){
        map<int, T> bottomView;
        queue<pair<BinaryNode *, int>> queue;
        queue.push(make_pair(root, 0));
        while(!queue.empty()){
            pair<BinaryNode *, int> front = queue.front();
            queue.pop();
            bottomView[front.second] = front.first->data;
            if(front.first->left != nullptr){
                queue.push(make_pair(front.first->left, front.second - 1));
            }
            if(front.first->right != nullptr){
                queue.push(make_pair(front.first->right, front.second + 1));
            }
        }
        for(auto it = bottomView.begin(); it != bottomView.end(); it++){
            Serial.println(it->second);
        }
    }

    void printVerticalOrder(){
        map<int, vector<T>> verticalOrder;
        queue<pair<BinaryNode *, int>> queue;
        queue.push(make_pair(root, 0));
        while(!queue.empty()){
            pair<BinaryNode *, int> front = queue.front();
            queue.pop();
            verticalOrder[front.second].push_back(front.first->data);
            if(front.first->left != nullptr){
                queue.push(make_pair(front.first->left, front.second - 1));
            }
            if(front.first->right != nullptr){
                queue.push(make_pair(front.first->right, front.second + 1));
            }
        }
        for(auto it = verticalOrder.begin(); it != verticalOrder.end(); it++){
            for(int i = 0; i < it->second.size(); i++){
                Serial.println(it->second[i]);
            }
        }
    }

    void printZigZag(){
        stack<BinaryNode *> currentLevel;
        stack<BinaryNode *> nextLevel;
        bool leftToRight = true;
        currentLevel.push(root);
        while(!currentLevel.empty()){
            BinaryNode *node = currentLevel.top();
            currentLevel.pop();
            if(node != nullptr){
                Serial.println(node->data);
                if(leftToRight){
                    nextLevel.push(node->left);
                    nextLevel.push(node->right);
                } else {
                    nextLevel.push(node->right);
                    nextLevel.push(node->left);
                }
            }
            if(currentLevel.empty()){
                leftToRight = !leftToRight;
                swap(currentLevel, nextLevel);
            }
        }
    }

    void printSpiral(){
        stack<BinaryNode *> currentLevel;
        stack<BinaryNode *> nextLevel;
        bool leftToRight = true;
        currentLevel.push(root);
        while(!currentLevel.empty()){
            BinaryNode *node = currentLevel.top();
            currentLevel.pop();
            if(node != nullptr){
                Serial.println(node->data);
                if(leftToRight){
                    nextLevel.push(node->left);
                    nextLevel.push(node->right);
                } else {
                    nextLevel.push(node->right);
                    nextLevel.push(node->left);
                }
            }
            if(currentLevel.empty()){
                leftToRight = !leftToRight;
                swap(currentLevel, nextLevel);
            }
        }
    }

    void printDiagonal(){
        map<int, vector<T>> diagonal;
        queue<pair<BinaryNode *, int>> queue;
        queue.push(make_pair(root, 0));
        while(!queue.empty()){
            pair<BinaryNode *, int> front = queue.front();
            queue.pop();
            diagonal[front.second].push_back(front.first->data);
            if(front.first->left != nullptr){
                queue.push(make_pair(front.first->left, front.second + 1));
            }
            if(front.first->right != nullptr){
                queue.push(make_pair(front.first->right, front.second));
            }
        }
        for(auto it = diagonal.begin(); it != diagonal.end(); it++){
            for(int i = 0; i < it->second.size(); i++){
                Serial.println(it->second[i]);
            }
        }
    }

    void printBoundary(){
        if(root != nullptr){
            Serial.println(root->data);
            printLeftBoundary(root->left);
            printLeaves(root->left);
            printLeaves(root->right);
            printRightBoundary(root->right);
        }
    }

    void printLeftBoundary(BinaryNode *node){
        if(node == nullptr){
            return;
        }
        if(node->left != nullptr){
            Serial.println(node->data);
            printLeftBoundary(node->left);
        } else if(node->right != nullptr){
            Serial.println(node->data);
            printLeftBoundary(node->right);
        }
    }

    void printRightBoundary(BinaryNode *node){
        if(node == nullptr){
            return;
        }
        if(node->right != nullptr){
            printRightBoundary(node->right);
            Serial.println(node->data);
        } else if(node->left != nullptr){
            printRightBoundary(node->left);
            Serial.println(node->data);
        }
    }

    void printCousins(T value){
        printCousins(root, value);
    }

    void printCousins(BinaryNode *node, T value){
        if(node == nullptr){
            return;
        }
        if(node->data == value){
            return;
        }
        if(height(node, value) == 1){
            Serial.println(node->data);
        } else {
            printCousins(node->left, value);
            printCousins(node->right, value);
        }
    }

    int height(BinaryNode *node, T value){
        if(node == nullptr){
            return -1;
        }
        if(node->data == value){
            return 0;
        }
        int left = height(node->left, value);
        int right = height(node->right, value);
        return left > right ? left + 1 : right + 1;
    }

    void printNodesAtDistance(int distance){
        printNodesAtDistance(root, distance);
    }

    void printNodesAtDistance(BinaryNode *node, int distance){
        if(node == nullptr){
            return;
        }
        if(distance == 0){
            Serial.println(node->data);
        } else {
            printNodesAtDistance(node->left, distance - 1);
            printNodesAtDistance(node->right, distance - 1);
        }
    }
};
#endif // BINARYTREE_h