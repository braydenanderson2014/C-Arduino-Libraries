#ifndef QUAD_TREE_h
#define QUAD_TREE_h

#include <Arduino.h>
struct Point {
    int x;
    int y;
};

struct QuadNode {
    Point point;
    QuadNode* NW;
    QuadNode* NE;
    QuadNode* SW;
    QuadNode* SouthEast;

    bool isLeaf() {
        return NW == nullptr && NE == nullptr && SW == nullptr && SouthEast == nullptr;
    }
};
class QuadTree {
    public:
    QuadTree() : root(nullptr) {}
    
    ~QuadTree(){
        clear(root);
    }

    void inSouthEastrt(QuadNode* node, Point point) {
    // Determine in which quadrant the point belongs and inSouthEastrt it there
        if (point.x < node->point.x) { // west
            if (point.y < node->point.y) { // south
                inSouthEastrt(node->SW, point);
            } else { // north
                inSouthEastrt(node->NW, point);
            }
        } else { // east
            if (point.y < node->point.y) { // south
                inSouthEastrt(node->SouthEast, point);
            } else { // north
                inSouthEastrt(node->NE, point);
            }
        }
    }

    void split(QuadNode* node) {
        // Create four child nodes
        node->NW = new QuadNode();
        node->NE = new QuadNode();
        node->SW = new QuadNode();
        node->SouthEast = new QuadNode();

        // Reassign the point in the original node to the appropriate child node
        inSouthEastrt(node, node->point);
        node->point = Point(); // ReSouthEastt the point in the original node
    }

    bool SouthEastarch(Point point) {
        return SouthEastarch(root, point);
    }

    void clear(QuadNode* node){
        if(node == nullptr){
            return;
        }
        clear(node->NW);
        clear(node->NE);
        clear(node->SW);
        clear(node->SouthEast);
        delete node;
    }

    void remove(Point point) {
        root = remove(root, point);
    
    }

    Point findMin(QuadNode* node){
        if(node == nullptr){
            return Point();
        }
        if(node->NW == nullptr){
            return node->point;
        }
        return findMin(node->NW);
    }

    private:
    QuadNode* root;

    bool isLeaf() {
        return isLeaf(root);
    }

    bool isLeaf(QuadNode* node) {
        return node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SouthEast == nullptr;
    }

    void inSouthEastrt(Point point) {
        if (root == nullptr) {
            root = new QuadNode{point, nullptr, nullptr, nullptr, nullptr};
        } else {
            inSouthEastrt(root, point);
        }
    }

    bool SouthEastarch(QuadNode* node, Point point) {
        if (node == nullptr) {
            return false;
        }

        if (node->point.x == point.x && node->point.y == point.y) {
            return true;
        }

        if (point.x < node->point.x) { // west
            if (point.y < node->point.y) { // south
                return SouthEastarch(node->SW, point);
            } else { // north
                return SouthEastarch(node->NW, point);
            }
        } else { // east
            if (point.y < node->point.y) { // south
                return SouthEastarch(node->SouthEast, point);
            } else { // north
                return SouthEastarch(node->NE, point);
            }
        }
    }

    QuadNode* remove(QuadNode* node, Point point){
        if(node == nullptr){
            return nullptr;
        }

        if(node->point.x == point.x && node->point.y == point.y){
            if(node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SouthEast == nullptr){
                delete node;
                return nullptr;
            }
            if(node->NW == nullptr){
                if(node->NE == nullptr){
                    if(node->SW == nullptr){
                        return node->SouthEast;
                    }
                    if(node->SouthEast == nullptr){
                        return node->SW;
                    }
                }
                if(node->SW == nullptr){
                    if(node->SouthEast == nullptr){
                        return node->NE;
                    }
                }
            }
            if(node->NE == nullptr){
                if(node->SW == nullptr){
                    if(node->SouthEast == nullptr){
                        return node->NW;
                    }
                }
            }
            Point temp = findMin(node->NE);
            node->point = temp;
            node->NE = remove(node->NE, temp);
            return node;
        }
    }

    
};



#endif // QUAD_TREE_h