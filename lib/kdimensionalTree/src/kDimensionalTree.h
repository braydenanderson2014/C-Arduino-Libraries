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

    // Stack entry for the iterative nearest-neighbor search.
    struct SearchEntry {
        KDimensionalNode* node;
        int depth;
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

    KDimensionalNode* nearestNeighbor(const SimpleVector<T>& point) {
        if (root == nullptr) return nullptr;

        // Iterative nearest-neighbor search with a heap-allocated explicit stack
        // to prevent call-stack overflow on deep or unbalanced trees.
        SimpleVector<SearchEntry> stack;

        // Pre-seed best with root so no sentinel value is needed in the loop.
        KDimensionalNode* best = root;
        double bestDistSq = distanceSq(point, root->point);

        SearchEntry first;
        first.node = root;
        first.depth = 0;
        stack.push_back(first);

        while (stack.size() > 0) {
            SearchEntry e = stack.get(stack.size() - 1);
            stack.erase(static_cast<int>(stack.size()) - 1);

            KDimensionalNode* node = e.node;
            if (node == nullptr) continue;

            int cd = e.depth % dimension;
            double dSq = distanceSq(point, node->point);

            if (dSq < bestDistSq) {
                bestDistSq = dSq;
                best = node;
            }

            double diff = static_cast<double>(point[cd]) - static_cast<double>(node->point[cd]);
            double splitDistSq = diff * diff;

            // Near child = same side as query point; explored first (pushed last in LIFO).
            KDimensionalNode* nearChild = diff < 0 ? node->left : node->right;
            // Far child = opposite side; only explore when the splitting-plane distance
            // is <= the current best, meaning a closer point could exist there.
            // When diff == 0 the query lies exactly on the plane (splitDistSq == 0),
            // so the condition is always true and both subtrees are searched.
            KDimensionalNode* farChild = diff < 0 ? node->right : node->left;

            // Push farChild first (lower priority in LIFO — explored second).
            if (farChild != nullptr && splitDistSq <= bestDistSq) {
                SearchEntry fe;
                fe.node = farChild;
                fe.depth = e.depth + 1;
                stack.push_back(fe);
            }

            // Push nearChild last (higher priority in LIFO — explored first).
            if (nearChild != nullptr) {
                SearchEntry ne;
                ne.node = nearChild;
                ne.depth = e.depth + 1;
                stack.push_back(ne);
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

    double distanceSq(const SimpleVector<T>& a, const SimpleVector<T>& b) const {
        double sum = 0;
        for (int i = 0; i < dimension; i++) {
            double diff = static_cast<double>(a[i]) - static_cast<double>(b[i]);
            sum += diff * diff;
        }
        return sum;
    }

    double distance(const SimpleVector<T>& a, const SimpleVector<T>& b) const {
        return Sqrt(distanceSq(a, b));
    }

    void freeTree(KDimensionalNode* node) {
        if (!node) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

public:
    KDimensionalTree(int k) : root(nullptr), dimension(k) {}

    ~KDimensionalTree() {
        freeTree(root);
    }

    // Prevent shallow copies that would cause double-free on destruction.
    KDimensionalTree(const KDimensionalTree&) = delete;
    KDimensionalTree& operator=(const KDimensionalTree&) = delete;

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

    SimpleVector<T> nearestNeighbour(const SimpleVector<T>& point) {
        KDimensionalNode* nearest = nearestNeighbor(point);
        return nearest ? nearest->point : SimpleVector<T>();
    }

    SimpleVector<SimpleVector<T>> rangeSearch(SimpleVector<T>& lower, SimpleVector<T>& upper) {
        SimpleVector<SimpleVector<T>> results;
        rangeSearch(root, lower, upper, 0, results);
        return results;
    }
};

#endif // K_DIMENSIONAL_TREE_h