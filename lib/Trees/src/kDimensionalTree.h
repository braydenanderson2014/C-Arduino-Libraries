#ifndef K_DIMENSIONAL_TREE_h
#define K_DIMENSIONAL_TREE_h

#include <Arduino.h>
#include <SimpleVector.h>
#include <Math.h>

template <typename T>

class KDimensionalTree {
    private:
    struct KDimensionalNode {
        SimpleVector<T> point;
        KDimensionalNode *left, *right;
    };
    KDimensionalNode *root;
    int dimension;

    KDimensionalNode *insert(KDimensionalNode *node, const SimpleVector<T>& point, int depth){
        if(node == nullptr){
            KDimensionalNode *newNode = new KDimensionalNode;
            newNode->point = point;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        int currentDimension = depth % dimension;
        if(point[currentDimension] < node->point[currentDimension]){
            node->left = insert(node->left, point, depth + 1);
        } else {
            node->right = insert(node->right, point, depth + 1);
        }
        return node;
    }

    bool search(KDimensionalNode* node, const SimpleVector<int>& point, int depth) {
        // If node is null, the point was not found
        if (node == nullptr) {
            return false;
        }

        // If the point is found at node
        if (node->point == point) {
            return true;
        }

        // Calculate current dimension (cd)
        int cd = depth % dimension;

        // If the point to be searched has smaller value than the node in cd dimension,
        // it goes to left of the node, else right of the node
        if (point[cd] < node->point[cd]) {
            return search(node->left, point, depth + 1);
        } else {
            return search(node->right, point, depth + 1);
        }
    }

    KDimensionalNode *remove(KDimensionalNode *node, const SimpleVector<T>& point, int depth){
        if(node == nullptr){
            return node;
        }
        int currentDimension = depth % dimension;
        if(point[currentDimension] < node->point[currentDimension]){
            node->left = remove(node->left, point, depth + 1);
        } else if(point[currentDimension] > node->point[currentDimension]){
            node->right = remove(node->right, point, depth + 1);
        } else {
            if(node->right == nullptr){
                KDimensionalNode *temp = node->left;
                delete node;
                return temp;
            } else if(node->left == nullptr){
                KDimensionalNode *temp = node->right;
                delete node;
                return temp;
            }
            KDimensionalNode *temp = findMin(node->right);
            node->point = temp->point;
            node->right = remove(node->right, temp->point, depth + 1);
        }
        return node;
    }

    KDimensionalNode* nearestNeighbor(KDimensionalNode* node, const SimpleVector<T>& point, int depth){
        if(node == nullptr){
            return nullptr;
        }
        int currentDimension = depth % dimension;

        KDimensionalNode* nextNode = nullptr;
        KDimensionalNode* otherNode = nullptr;

        if(point[currentDimension] < node->point[currentDimension]){
            nextNode = node->left;
            otherNode = node->right;
        } else {
            nextNode = node->right;
            otherNode = node->left;
        }

        KDimensionalNode* temp = nearestNeighbor(nextNode, point, depth + 1);

        KDimensionalNode* best = nullptr;
        if(!temp){
            best = node;
        } else if(distance(point, node->point) < distance(point, best->point)){
            best = temp;
        } else {
            best = node;
        }

        if(distance(best ->point,point) > abs(point[currentDimension] - node->point[currentDimension])){
            temp = nearestNeighbor(otherNode, point, depth + 1);
            if(temp && distance(temp->point, point) < distance(best->point, point)){
                best = temp;
            }
        }
        return best;
    }

    void rangeSearch(KDimensionalNode* node, const SimpleVector<T>& lower, const SimpleVector<T>& upper, int depth, SimpleVector<SimpleVector<T>>& points){
        if(node == nullptr){
            return;
        }
        int currentDimension = depth % dimension;

        if(inRange(node ->point, lower, upper)){
            points.push_back(node->point);
        }

        if(node ->left && lower[currentDimension] <= node->point[currentDimension]){
            rangeSearch(node->left, lower, upper, depth + 1, points);
        }

        if(node ->right && upper[currentDimension] >= node->point[currentDimension]){
            rangeSearch(node->right, lower, upper, depth + 1, points);
        }
    }

    bool inRange(const SimpleVector<T>& point, const SimpleVector<T>& lower, const SimpleVector<T>& upper){
        for(int i = 0; i < dimension; i++){
            if(point[i] < lower[i] || point[i] > upper[i]){
                return false;
            }
        }
        return true;
    }

    double distance(const SimpleVector<T>& point1, const SimpleVector<T>& point2){
        double sum = 0;
        for(int i = 0; i < dimension; i++){
            sum += (point1[i] - point2[i]) * (point1[i] - point2[i]);
        }
        return Sqrt(sum);
    }

    public:
    KDimensionalTree(int k) : dimension(k), root(nullptr) {}

    void insert(SimpleVector<T>& point){
        root = insert(root, point, 0);
    }

    bool search(SimpleVector<T>& point){
        return search(root, point, 0);
    }

    void remove(SimpleVector<T>& point){
        root = remove(root, point, 0);
    }

    SimpleVector<T>& nearestNeighbour(SimpleVector<T>& point){
        KDimensionalNode* nearest = nearestNeighbour(root, point, 0);
        return nearest ? nearest->point : SimpleVector<T>();
    }

    SimpleVector<SimpleVector<T>> rangeSearch(SimpleVector<T>& lower, SimpleVector<T>& upper){
        SimpleVector<SimpleVector<T>> points;
        rangeSearch(root, lower, upper, 0, points);
        return points;
    }

};




#endif //   K_DIMENSIONAL_TREE_h