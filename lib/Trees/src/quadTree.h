#ifndef QUAD_TREE_h
#define QUAD_TREE_h
struct Point {
    int x;
    int y;
};

struct QuadNode {
    Point point;
    QuadNode* NW;
    QuadNode* NE;
    QuadNode* SW;
    QuadNode* SE;

    bool isLeaf() {
        return NW == nullptr && NE == nullptr && SW == nullptr && SE == nullptr;
    }
};
class QuadTree {
    public:
    QuadTree() : root(nullptr) {}
    
    ~QuadTree(){
        clear(root);
    }

    void insert(QuadNode* node, Point point) {
    // Determine in which quadrant the point belongs and insert it there
        if (point.x < node->point.x) { // west
            if (point.y < node->point.y) { // south
                insert(node->SW, point);
            } else { // north
                insert(node->NW, point);
            }
        } else { // east
            if (point.y < node->point.y) { // south
                insert(node->SE, point);
            } else { // north
                insert(node->NE, point);
            }
        }
    }

    void split(QuadNode* node) {
        // Create four child nodes
        node->NW = new QuadNode();
        node->NE = new QuadNode();
        node->SW = new QuadNode();
        node->SE = new QuadNode();

        // Reassign the point in the original node to the appropriate child node
        insert(node, node->point);
        node->point = Point(); // Reset the point in the original node
    }

    bool search(Point point) {
        return search(root, point);
    }

    void clear(QuadNode* node){
        if(node == nullptr){
            return;
        }
        clear(node->NW);
        clear(node->NE);
        clear(node->SW);
        clear(node->SE);
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
        return node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr;
    }

    void insert(Point point) {
        if (root == nullptr) {
            root = new QuadNode{point, nullptr, nullptr, nullptr, nullptr};
        } else {
            insert(root, point);
        }
    }

    bool search(QuadNode* node, Point point) {
        if (node == nullptr) {
            return false;
        }

        if (node->point.x == point.x && node->point.y == point.y) {
            return true;
        }

        if (point.x < node->point.x) { // west
            if (point.y < node->point.y) { // south
                return search(node->SW, point);
            } else { // north
                return search(node->NW, point);
            }
        } else { // east
            if (point.y < node->point.y) { // south
                return search(node->SE, point);
            } else { // north
                return search(node->NE, point);
            }
        }
    }

    QuadNode* remove(QuadNode* node, Point point){
        if(node == nullptr){
            return nullptr;
        }

        if(node->point.x == point.x && node->point.y == point.y){
            if(node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr){
                delete node;
                return nullptr;
            }
            if(node->NW == nullptr){
                if(node->NE == nullptr){
                    if(node->SW == nullptr){
                        return node->SE;
                    }
                    if(node->SE == nullptr){
                        return node->SW;
                    }
                }
                if(node->SW == nullptr){
                    if(node->SE == nullptr){
                        return node->NE;
                    }
                }
            }
            if(node->NE == nullptr){
                if(node->SW == nullptr){
                    if(node->SE == nullptr){
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