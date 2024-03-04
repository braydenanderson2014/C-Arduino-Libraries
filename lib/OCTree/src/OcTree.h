#ifndef OCTREE_H
#define OCTREE_H

#define MAX_NODES 256

#include <TypeTraits.h>
#include <Arduino.h>
#include <SimpleVector.h>

template <typename T>
struct OctreeNode {
    static_assert(is_arithmetic<T>::value, "OctreeNode requires a numerical type");
    T data;
    OctreeNode<T>* children[8]; // Pointers to the 8 children
    OctreeNode<T>* nextFree; // Pointer to the next free node in the pool
};

template <typename T>
class Octree {
    private:
    static_assert(is_arithmetic<T>::value, "Octree requires a numerical type");
    OctreeNode<T>* root;
    OctreeNode<T> nodePool[MAX_NODES];
    OctreeNode<T>* freeListHead = nullptr; // Points to the head of the free list
    int nodePoolIndex = 0;
    
    OctreeNode<T>* allocateNode() {
        if (freeListHead != nullptr) {
            // Allocate from the free list
            OctreeNode<T>* node = freeListHead;
            freeListHead = freeListHead->nextFree; // Move to the next free node
            node->nextFree = nullptr; // Clear the nextFree pointer
            return node;
        } else if (nodePoolIndex < MAX_NODES) {
            // Allocate from the pool array
            return &nodePool[nodePoolIndex++];
        } else {
            // Out of memory
            return nullptr;
        }
    }


    void deallocateNode(OctreeNode<T>* node) {
        if (!node) return;
        // Reset the node's state if necessary, e.g., nullify children pointers
        for (int i = 0; i < 8; ++i) {
            node->children[i] = nullptr;
        }
        // Add the node to the free list
        node->nextFree = freeListHead;
        freeListHead = node;
    }


    void splitNode(OctreeNode<T>* node, const T& data){
        OctreeNode<T>* newNode = new OctreeNode<T>();
        newNode->data = data;
        for(int i = 0; i < 8; i++){
            newNode->children[i] = nullptr;
        }
        node->children[node->childCount] = newNode;
        node->childCount++;
    }

    OctreeNode<T>* findLeaf(OctreeNode<T>* node, const T& data){
        if(node->childCount == 0){
            return node;
        } else {
            for(int i = 0; i < node->childCount; i++){
                if(node->children[i]->data == data){
                    return findLeaf(node->children[i], data);
                }
            }
        }
    }

    void condenseTree(OctreeNode<T>* node){
        if(node == root){
            return;
        }
        if(node->childCount < 8){
            for(int i = 0; i < node->childCount; i++){
                if(node->children[i]->childCount == 0){
                    delete node->children[i];
                    node->children[i] = nullptr;
                }
            }
        }
    }

void radiusSearchHelper(OctreeNode<T>* node, const T& centerX, const T& centerY, const T& centerZ, const T& radius, SimpleVector<T>& results) {
    if (!node) return;
    // Assuming data is a point at (x, y, z)
    T dx = node->data - centerX;
    T dy = node->data - centerY; // Placeholder for actual y coordinate
    T dz = node->data - centerZ; // Placeholder for actual z coordinate
    T distance = sqrt(dx*dx + dy*dy + dz*dz);
    if (distance <= radius) {
        results.push_back(node->data);
    }
    for (int i = 0; i < 8; ++i) {
        if (node->children[i]) {
            radiusSearchHelper(node->children[i], centerX, centerY, centerZ, radius, results);
        }
    }
}


    public:
    Octree() : root(nullptr) {}

    ~Octree(){
        delete root;
    }

    void insert(const T& data){
        allocateNode();
        if(root == nullptr){
            root = new OctreeNode<T>();
            root->data = data;
            for(int i = 0; i < 8; i++){
                root->children[i] = nullptr;
            }
        } else {
            insert(data, root, 0, 0, 0, 0, 0, 0);
        }
    }

    void insert(const T& data, OctreeNode<T>* node, int x1, int x2, int y1, int y2, int z1, int z2){
        allocateNode();
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int midZ = (z1 + z2) / 2;

        if(data <= midX && data <= midY && data <= midZ){
            if(node->children[0] == nullptr){
                node->children[0] = new OctreeNode<T>();
                node->children[0]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[0]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[0], x1, midX, y1, midY, z1, midZ);
            }
        } else if(data <= midX && data <= midY && data > midZ){
            if(node->children[1] == nullptr){
                node->children[1] = new OctreeNode<T>();
                node->children[1]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[1]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[1], x1, midX, y1, midY, midZ, z2);
            }
        } else if(data <= midX && data > midY && data <= midZ){
            if(node->children[2] == nullptr){
                node->children[2] = new OctreeNode<T>();
                node->children[2]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[2]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[2], x1, midX, midY, y2, z1, midZ);
            }
        } else if(data <= midX && data > midY && data > midZ){
            if(node->children[3] == nullptr){
                node->children[3] = new OctreeNode<T>();
                node->children[3]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[3]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[3], x1, midX, midY, y2, midZ, z2);
            }
        } else if(data > midX && data <= midY && data <= midZ){
            if(node->children[4] == nullptr){
                node->children[4] = new OctreeNode<T>();
                node->children[4]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[4]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[4], midX, x2, y1, midY, z1, midZ);
            }
        } else if(data > midX && data <= midY && data > midZ){
            if(node->children[5] == nullptr){
                node->children[5] = new OctreeNode<T>();
                node->children[5]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[5]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[5], midX, x2, y1, midY, midZ, z2);
            }
        } else if(data > midX && data > midY && data <= midZ){
            if(node->children[6] == nullptr){
                node->children[6] = new OctreeNode<T>();
                node->children[6]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[6]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[6], midX, x2, midY, y2, z1, midZ);
            }
        } else if(data > midX && data > midY && data > midZ){
            if(node->children[7] == nullptr){
                node->children[7] = new OctreeNode<T>();
                node->children[7]->data = data;
                for(int i = 0; i < 8; i++){
                    node->children[7]->children[i] = nullptr;
                }
            } else {
                insert(data, node->children[7], midX, x2, midY, y2, midZ, z2);
            }
        }
    }

    void insertBulk(const SimpleVector<T>& data) {
        for (size_t i = 0; i < data.size(); ++i) {
            insert(data[i]);
        }
    }

    void removeBulk(const SimpleVector<T>& data) {
        for (size_t i = 0; i < data.size(); ++i) {
            remove(data[i]);
        }
    }

    void remove(const T& data){
        deallocateNode();
        if(root != nullptr){
            remove(data, root, 0, 0, 0, 0, 0, 0);
        }
    }

    void remove(const T& data, OctreeNode<T>* node, int x1, int x2, int y1, int y2, int z1, int z2){
        deallocateNode();
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int midZ = (z1 + z2) / 2;

        if(data <= midX && data <= midY && data <= midZ){
            if(node->children[0] != nullptr){
                remove(data, node->children[0], x1, midX, y1, midY, z1, midZ);
            }
        } else if(data <= midX && data <= midY && data > midZ){
            if(node->children[1] != nullptr){
                remove(data, node->children[1], x1, midX, y1, midY, midZ, z2);
            }
        } else if(data <= midX && data > midY && data <= midZ){
            if(node->children[2] != nullptr){
                remove(data, node->children[2], x1, midX, midY, y2, z1, midZ);
            }
        } else if(data <= midX && data > midY && data > midZ){
            if(node->children[3] != nullptr){
                remove(data, node->children[3], x1, midX, midY, y2, midZ, z2);
            }
        } else if(data > midX && data <= midY && data <= midZ){
            if(node->children[4] != nullptr){
                remove(data, node->children[4], midX, x2, y1, midY, z1, midZ);
            }
        } else if(data > midX && data <= midY && data > midZ){
            if(node->children[5] != nullptr){
                remove(data, node->children[5], midX, x2, y1, midY, midZ, z2);
            }
        } else if(data > midX && data > midY && data <= midZ){
            if(node->children[6] != nullptr){
                remove(data, node->children[6], midX, x2, midY, y2, z1, midZ);
            }
        } else if(data > midX && data > midY && data > midZ){
            if(node->children[7] != nullptr){
                remove(data, node->children[7], midX, x2, midY, y2, midZ, z2);
            }
        }
    }

    bool search(const T& data){
        if(root != nullptr){
            return search(data, root, 0, 0, 0, 0, 0, 0);
        }
        return false;
    }

    bool search(const T& data, OctreeNode<T>* node, int x1, int x2, int y1, int y2, int z1, int z2){
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int midZ = (z1 + z2) / 2;

        if(data <= midX && data <= midY && data <= midZ){
            if(node->children[0] != nullptr){
                return search(data, node->children[0], x1, midX, y1, midY, z1, midZ);
            }
        } else if(data <= midX && data <= midY && data > midZ){
            if(node->children[1] != nullptr){
                return search(data, node->children[1], x1, midX, y1, midY, midZ, z2);
            }
        } else if(data <= midX && data > midY && data <= midZ){
            if(node->children[2] != nullptr){
                return search(data, node->children[2], x1, midX, midY, y2, z1, midZ);
            }
        } else if(data <= midX && data > midY && data > midZ){
            if(node->children[3] != nullptr){
                return search(data, node->children[3], x1, midX, midY, y2, midZ, z2);
            }
        } else if(data > midX && data <= midY && data <= midZ){
            if(node->children[4] != nullptr){
                return search(data, node->children[4], midX, x2, y1, midY, z1, midZ);
            }
        } else if(data > midX && data <= midY && data > midZ){
            if(node->children[5] != nullptr){
                return search(data, node->children[5], midX, x2, y1, midY, midZ, z2);
            }
        } else if(data > midX && data > midY && data <= midZ){
            if(node->children[6] != nullptr){
                return search(data, node->children[6], midX, x2, midY, y2, z1, midZ);
            }
        } else if(data > midX && data > midY && data > midZ){
            if(node->children[7] != nullptr){
                return search(data, node->children[7], midX, x2, midY, y2, midZ, z2);
            }
        }
        return false;
    }

    void clear(){
        if(root != nullptr){
            clear(root);
            root = nullptr;
            nodePoolIndex = 0; // Reset node pool for reuse
        }
    }

    void clear(OctreeNode<T>* node){
        for(int i = 0; i < 8; i++){
            if(node->children[i] != nullptr){
                clear(node->children[i]);
            }
        }
        delete node;
    }


    T nearestNeighbor(const T& data){
        if(root != nullptr){
            return nearestNeighbor(data, root, 0, 0, 0, 0, 0, 0);
        }
        return T();
    }

    T nearestNeighbor(const T& data, OctreeNode<T>* node, int x1, int x2, int y1, int y2, int z1, int z2){
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        int midZ = (z1 + z2) / 2;

        if(data <= midX && data <= midY && data <= midZ){
            if(node->children[0] != nullptr){
                return nearestNeighbor(data, node->children[0], x1, midX, y1, midY, z1, midZ);
            }
        } else if(data <= midX && data <= midY && data > midZ){
            if(node->children[1] != nullptr){
                return nearestNeighbor(data, node->children[1], x1, midX, y1, midY, midZ, z2);
            }
        } else if(data <= midX && data > midY && data <= midZ){
            if(node->children[2] != nullptr){
                return nearestNeighbor(data, node->children[2], x1, midX, midY, y2, z1, midZ);
            }
        } else if(data <= midX && data > midY && data > midZ){
            if(node->children[3] != nullptr){
                return nearestNeighbor(data, node->children[3], x1, midX, midY, y2, midZ, z2);
            }
        } else if(data > midX && data <= midY && data <= midZ){
            if(node->children[4] != nullptr){
                return nearestNeighbor(data, node->children[4], midX, x2, y1, midY, z1, midZ);
            }
        } else if(data > midX && data <= midY && data > midZ){
            if(node->children[5] != nullptr){
                return nearestNeighbor(data, node->children[5], midX, x2, y1, midY, midZ, z2);
            }
        } else if(data > midX && data > midY && data <= midZ){
            if(node->children[6] != nullptr){
                return nearestNeighbor(data, node->children[6], midX, x2, midY, y2, z1, midZ);
            }
        } else if(data > midX && data > midY && data > midZ){
            if(node->children[7] != nullptr){
                return nearestNeighbor(data, node->children[7], midX, x2, midY, y2, midZ, z2);
            }
        }
        return node->data;
    }

    SimpleVector<T> inOrderTraversal(){
        SimpleVector<T> result;
        inOrderTraversal(root, result);
        return result;
    }

    void inOrderTraversal(OctreeNode<T>* node, SimpleVector<T>& result){
        if(node != nullptr){
            inOrderTraversal(node->children[0], result);
            result.push_back(node->data);
            inOrderTraversal(node->children[1], result);
            inOrderTraversal(node->children[2], result);
            result.push_back(node->data);
            inOrderTraversal(node->children[3], result);
            inOrderTraversal(node->children[4], result);
            result.push_back(node->data);
            inOrderTraversal(node->children[5], result);
            inOrderTraversal(node->children[6], result);
            result.push_back(node->data);
            inOrderTraversal(node->children[7], result);
        }
    }

    SimpleVector<T> preOrderTraversal(){
        SimpleVector<T> result;
        preOrderTraversal(root, result);
        return result;
    }

    void preOrderTraversal(OctreeNode<T>* node, SimpleVector<T>& result){
        if(node != nullptr){
            result.push_back(node->data);
            preOrderTraversal(node->children[0], result);
            preOrderTraversal(node->children[1], result);
            preOrderTraversal(node->children[2], result);
            preOrderTraversal(node->children[3], result);
            preOrderTraversal(node->children[4], result);
            preOrderTraversal(node->children[5], result);
            preOrderTraversal(node->children[6], result);
            preOrderTraversal(node->children[7], result);
        }
    }

    SimpleVector<T> postOrderTraversal(){
        SimpleVector<T> result;
        postOrderTraversal(root, result);
        return result;
    }

    void postOrderTraversal(OctreeNode<T>* node, SimpleVector<T>& result){
        if(node != nullptr){
            postOrderTraversal(node->children[0], result);
            postOrderTraversal(node->children[1], result);
            postOrderTraversal(node->children[2], result);
            postOrderTraversal(node->children[3], result);
            postOrderTraversal(node->children[4], result);
            postOrderTraversal(node->children[5], result);
            postOrderTraversal(node->children[6], result);
            postOrderTraversal(node->children[7], result);
            result.push_back(node->data);
        }
    }

    SimpleVector<T> levelOrderTraversal(){
        SimpleVector<T> result;
        for(int i = 0; i < 8; i++){
            levelOrderTraversal(root, result, i);
        }
        return result;
    }

    void levelOrderTraversal(OctreeNode<T>* node, SimpleVector<T>& result, int level){
        if(node != nullptr){
            if(level == 0){
                result.push_back(node->data);
            } else {
                levelOrderTraversal(node->children[0], result, level - 1);
                levelOrderTraversal(node->children[1], result, level - 1);
                levelOrderTraversal(node->children[2], result, level - 1);
                levelOrderTraversal(node->children[3], result, level - 1);
                levelOrderTraversal(node->children[4], result, level - 1);
                levelOrderTraversal(node->children[5], result, level - 1);
                levelOrderTraversal(node->children[6], result, level - 1);
                levelOrderTraversal(node->children[7], result, level - 1);
            }
        }
    }

    OctreeNode<T>* getRoot(){
        return root;
    }

    SimpleVector<T> rangeSearch(const T& x1, const T& x2, const T& y1, const T& y2, const T& z1, const T& z2){
        SimpleVector<T> result;
        rangeSearch(root, result, x1, x2, y1, y2, z1, z2);
        return result;
    }

    void radiusSearch(const T& centerX, const T& centerY, const T& centerZ, const T& radius, SimpleVector<T>& results) {
        radiusSearchHelper(root, centerX, centerY, centerZ, radius, results);
    }
};
#endif // OCTREE_H