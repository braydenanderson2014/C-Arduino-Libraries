#ifndef RTREE_H
#define RTREE_H

#include <TypeTraits.h>
#include <Queue.h>

template <typename T>
struct Rectangle {
    static_assert(is_arithmetic<T>::value, "Rectangle requires a numerical type");
    T x1, y1, x2, y2;
};

template <typename T>
struct RTreeNode {
    static_assert(is_arithmetic<T>::value, "RTreeNode requires a numerical type");
    Rectangle <T> bounds;
    RTreeNode* children[4];
    int childCount;
};

enum TraversalOrder {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    LEVEL_ORDER
};

template <typename T>
class RTree {
private:
    static_assert(is_arithmetic<T>::value, "RTree requires a numerical type");
    RTreeNode<T>* root;
    
    TraversalOrder order;

    void splitNode(RTreeNode<T>* node, const Rectangle<T>& rect){
        RTreeNode<T>* newNode = new RTreeNode<T>();
        newNode->childCount = 0;
        newNode->children[newNode->childCount] = new RTreeNode<T>();
        newNode->children[newNode->childCount]->bounds = rect;
        newNode->childCount++;
        node->children[node->childCount] = newNode;
        node->childCount++;
    }

    RTreeNode<T>* findLeaf(RTreeNode<T>* node, const Rectangle<T>& rect){
        if(node->childCount == 0){
            return node;
        } else {
            for(int i = 0; i < node->childCount; i++){
                if(node->children[i]->bounds.x1 <= rect.x1 && node->children[i]->bounds.x2 >= rect.x2 && node->children[i]->bounds.y1 <= rect.y1 && node->children[i]->bounds.y2 >= rect.y2){
                    return findLeaf(node->children[i], rect);
                }
            }
        }
    }

    void condenseTree(RTreeNode<T>* node){
        if(node == root){
            return;
        }
        if(node->childCount < 2){
            RTreeNode<T>* parent = findParent(root, node);
            if(parent != nullptr){
                for(int i = 0; i < parent->childCount; i++){
                    if(parent->children[i] == node){
                        delete node;
                        for(int j = i; j < parent->childCount - 1; j++){
                            parent->children[j] = parent->children[j + 1];
                        }
                        parent->childCount--;
                        condenseTree(parent);
                    }
                }
            }
        }
    }

    RTreeNode<T>* findParent(RTreeNode<T>* node, RTreeNode<T>* child){
        if(node->childCount == 0){
            return nullptr;
        } else {
            for(int i = 0; i < node->childCount; i++){
                if(node->children[i] == child){
                    return node;
                } else {
                    RTreeNode<T>* parent = findParent(node->children[i], child);
                    if(parent != nullptr){
                        return parent;
                    }
                }
            }
        }
    }

    void clear(RTreeNode<T>* node){
        if(node->childCount == 0){
            delete node;
        } else {
            for(int i = 0; i < node->childCount; i++){
                clear(node->children[i]);
            }
            delete node;
        }
    }

    void print(TraversalOrder order, RTreeNode<T>* node){
        if(node == nullptr){
            return;
        }
        switch(order){
            case PRE_ORDER:
                Serial.print(node->bounds.x1);
                Serial.print(" ");
                Serial.print(node->bounds.y1);
                Serial.print(" ");
                Serial.print(node->bounds.x2);
                Serial.print(" ");
                Serial.println(node->bounds.y2);
                for (int i = 0; i < node->childCount; i++) {
                  print(order, node->children[i]);
                }
                break;
            case IN_ORDER:
                for (int i = 0; i < node->childCount; i++) {
                    print(order, node->children[i]);
                    Serial.print(node->bounds.x1);
                    Serial.print(" ");
                    Serial.print(node->bounds.y1);
                    Serial.print(" ");
                    Serial.print(node->bounds.x2);
                    Serial.print(" ");
                    Serial.println(node->bounds.y2);
                }
                break;
            case POST_ORDER:
                for (int i = 0; i < node->childCount; i++) {
                    print(order, node->children[i]);
                }
                Serial.print(node->bounds.x1);
                Serial.print(" ");
                Serial.print(node->bounds.y1);
                Serial.print(" ");
                Serial.print(node->bounds.x2);
                Serial.print(" ");
                Serial.println(node->bounds.y2);
                break;
            case LEVEL_ORDER:
                Queue<RTreeNode<T>*> q;
                q.enqueue(node);
                while(!q.isEmpty()){
                  RTreeNode<T>* current = q.dequeue();
                  Serial.print(current->bounds.x1);
                  Serial.print(" ");
                  Serial.print(current->bounds.y1);
                  Serial.print(" ");
                  Serial.print(current->bounds.x2);
                  Serial.print(" ");
                  Serial.println(current->bounds.y2);
                  for(int i = 0; i < current->childCount; i++){
                    q.enqueue(current->children[i]);
                  }
                }
                break;
        }
    }


public:
    RTree() : root(nullptr) {}

    void insert(const Rectangle<T>& rect){
        if(root == nullptr){
            root = new RTreeNode<T>();
            root->childCount = 0;
            root->children[root->childCount] = new RTreeNode<T>();
            root->children[root->childCount]->bounds = rect;
            root->childCount++;
        } else {
            RTreeNode<T>* leaf = findLeaf(root, rect);
            if(leaf->childCount < 4){
                leaf->children[leaf->childCount] = new RTreeNode<T>();
                leaf->children[leaf->childCount]->bounds = rect;
                leaf->childCount++;
            } else {
                splitNode(leaf, rect);
            }
        }
    }

    void remove(const Rectangle<T>& rect){
        RTreeNode<T>* leaf = findLeaf(root, rect);
        if (leaf == nullptr) {
        // The rectangle is not in the tree.
            return false;
        }

        // Find the rectangle in the leaf node.
        int index = -1;
        for (int i = 0; i < leaf->childCount; i++) {
            if (leaf->children[i]->bounds == rect) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            // The rectangle is not in the tree.
            return false;
        }

        // Remove the rectangle from the leaf node.
        delete leaf->children[index];
        for (int i = index; i < leaf->childCount - 1; i++) {
            leaf->children[i] = leaf->children[i + 1];
        }
        leaf->childCount--;

        // Reorganize the tree if necessary.
        condenseTree(leaf);

        return true;
    }

    bool search(const Rectangle<T>& rect){
        return false;
    }

    void clear(){
        clear(root);
        root = nullptr;
    }

    ~RTree(){
        clear();
    }

    void print(TraversalOrder order = IN_ORDER){
        print(order, root);
    }


};





#endif // RTREE_H