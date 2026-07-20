#ifndef K_DIMENSIONAL_TREE_h
#define K_DIMENSIONAL_TREE_h

#include <Arduino.h>
#include <SimpleVector.h>
#include <MathLib.h>

template <typename T>
class KDimensionalTree {
private:
    struct KDimensionalNode {
        SimpleVector<T> point;
        KDimensionalNode *left, *right;
    };

    KDimensionalNode *root;
    int dimension;

    KDimensionalNode *insert(KDimensionalNode *node, const SimpleVector<T>& point, int depth) {
        if (node == nullptr) {
            KDimensionalNode *newNode = new KDimensionalNode;
            newNode->point = point;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        int currentDimension = depth % dimension;
        if (point[currentDimension] < node->point[currentDimension]) {
            node->left = insert(node->left, point, depth + 1);
        } else {
            node->right = insert(node->right, point, depth + 1);
        }

        return node;
    }

    bool search(KDimensionalNode* node, const SimpleVector<T>& point, int depth) {
        if (node == nullptr) {
            return false;
        }

        if (node->point == point) {
            return true;
        }

        int cd = depth % dimension;
        if (point[cd] < node->point[cd]) {
            return search(node->left, point, depth + 1);
        } else {
            return search(node->right, point, depth + 1);
        }
    }

    KDimensionalNode* findMin(KDimensionalNode* node, int d, int depth = 0) {
        if (node == nullptr) return nullptr;

        int cd = depth % dimension;

        if (cd == d) {
            if (node->left == nullptr)
                return node;
            return findMin(node->left, d, depth + 1);
        }

        KDimensionalNode* leftMin = findMin(node->left, d, depth + 1);
        KDimensionalNode* rightMin = findMin(node->right, d, depth + 1);
        KDimensionalNode* minNode = node;

        if (leftMin && leftMin->point[d] < minNode->point[d]) minNode = leftMin;
        if (rightMin && rightMin->point[d] < minNode->point[d]) minNode = rightMin;

        return minNode;
    }

    KDimensionalNode *remove(KDimensionalNode *node, const SimpleVector<T>& point, int depth) {
        if (node == nullptr) {
            return nullptr;
        }

        int cd = depth % dimension;

        if (node->point == point) {
            if (node->right != nullptr) {
                KDimensionalNode *minNode = findMin(node->right, cd, depth + 1);
                node->point = minNode->point;
                node->right = remove(node->right, minNode->point, depth + 1);
            } else if (node->left != nullptr) {
                KDimensionalNode *minNode = findMin(node->left, cd, depth + 1);
                node->point = minNode->point;
                node->right = remove(node->left, minNode->point, depth + 1);
                node->left = nullptr;
            } else {
                delete node;
                return nullptr;
            }
        } else if (point[cd] < node->point[cd]) {
            node->left = remove(node->left, point, depth + 1);
        } else {
            node->right = remove(node->right, point, depth + 1);
        }

        return node;
    }

    KDimensionalNode* nearestNeighbor(KDimensionalNode* node, const SimpleVector<T>& point, int depth) {
        if (node == nullptr) return nullptr;

        int cd = depth % dimension;

        KDimensionalNode* nextNode = point[cd] < node->point[cd] ? node->left : node->right;
        KDimensionalNode* otherNode = point[cd] < node->point[cd] ? node->right : node->left;

        KDimensionalNode* best = node;
        KDimensionalNode* temp = nearestNeighbor(nextNode, point, depth + 1);

        if (temp && distance(point, temp->point) < distance(point, best->point)) {
            best = temp;
        }

        if (abs(point[cd] - node->point[cd]) < distance(point, best->point)) {
            temp = nearestNeighbor(otherNode, point, depth + 1);
            if (temp && distance(point, temp->point) < distance(point, best->point)) {
                best = temp;
            }
        }

        return best;
    }

    void rangeSearch(KDimensionalNode* node, const SimpleVector<T>& lower, const SimpleVector<T>& upper, int depth, SimpleVector<SimpleVector<T>>& results) {
        if (node == nullptr) return;

        int cd = depth % dimension;

        if (inRange(node->point, lower, upper)) {
            results.push_back(node->point);
        }

        if (node->left && lower[cd] <= node->point[cd]) {
            rangeSearch(node->left, lower, upper, depth + 1, results);
        }

        if (node->right && upper[cd] >= node->point[cd]) {
            rangeSearch(node->right, lower, upper, depth + 1, results);
        }
    }

    bool inRange(const SimpleVector<T>& point, const SimpleVector<T>& lower, const SimpleVector<T>& upper) {
        for (int i = 0; i < dimension; i++) {
            if (point[i] < lower[i] || point[i] > upper[i]) return false;
        }
        return true;
    }

    double distance(const SimpleVector<T>& a, const SimpleVector<T>& b) {
        double sum = 0;
        for (int i = 0; i < dimension; i++) {
            sum += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return Sqrt(sum); // assuming MathLib.h defines Sqrt()
    }

    void freeTree(KDimensionalNode* node) {
        if (!node) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

public:
    KDimensionalTree(int k) : root(nullptr), dimension(k) {}

    void insert(const SimpleVector<T>& point) {
        root = insert(root, point, 0);
    }

    bool search(const SimpleVector<T>& point) {
        return search(root, point, 0);
    }

    void remove(const SimpleVector<T>& point) {
        root = remove(root, point, 0);
    }

    void clear() {
        freeTree(root);
        root = nullptr;
    }

    SimpleVector<T> nearestNeighbour(SimpleVector<T>& point) {
        KDimensionalNode* nearest = nearestNeighbor(root, point, 0);
        return nearest ? nearest->point : SimpleVector<T>();
    }

    SimpleVector<SimpleVector<T>> rangeSearch(SimpleVector<T>& lower, SimpleVector<T>& upper) {
        SimpleVector<SimpleVector<T>> results;
        rangeSearch(root, lower, upper, 0, results);
        return results;
    }
};

#endif // K_DIMENSIONAL_TREE_h