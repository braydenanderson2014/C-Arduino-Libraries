#ifndef QUAD_TREE_h
#define QUAD_TREE_h

#include <Arduino.h>
#include <TypeTraits.h>
#include <SimpleVector.h>

template <typename T>
struct Point {
    static_assert(is_arithmetic<T>::value, "Point only supports number types");
    T x; // The x coordinate of the point
    T y; // The y coordinate of the point
};

template <typename T>
struct Rectangle {
    static_assert(is_arithmetic<T>::value, "Rectangle only supports number types");
    Point<T> bottomLeft;
    Point<T> topRight;

    bool contains(const Point<T>& point) const {
        return point.x >= bottomLeft.x && point.x <= topRight.x &&
               point.y >= bottomLeft.y && point.y <= topRight.y;
    }

    bool intersects(const Rectangle<T>& other) const {
        return !(topRight.x < other.bottomLeft.x || bottomLeft.x > other.topRight.x ||
                 topRight.y < other.bottomLeft.y || bottomLeft.y > other.topRight.y);
    }

    T width() const {
        return topRight.x - bottomLeft.x;
    }

    T height() const {
        return topRight.y - bottomLeft.y;
    }

    T area() const {
        return width() * height();
    }

    T perimeter() const {
        return 2 * (width() + height());
    }

    Point<T> center() const {
        return Point<T>{(bottomLeft.x + topRight.x) / 2, (bottomLeft.y + topRight.y) / 2};
    }

    T distance(const Point<T>& point) const {
        T dx = max(bottomLeft.x - point.x, max((T)0, point.x - topRight.x));
        T dy = max(bottomLeft.y - point.y, max((T)0, point.y - topRight.y));
        return sqrt(dx * dx + dy * dy);
    }

    T distance(const Rectangle<T>& other) const {
        T dx = max(bottomLeft.x - other.topRight.x, max((T)0, other.bottomLeft.x - topRight.x));
        T dy = max(bottomLeft.y - other.topRight.y, max((T)0, other.bottomLeft.y - topRight.y));
        return sqrt(dx * dx + dy * dy);
    }


};

template <typename T>
class QuadNode {
    private:
    static_assert(is_arithmetic<T>::value, "QuadNode only supports number types");
    public:
    Rectangle<T> boundary; // The boundary of the node
    SimpleVector<Point<T>> points; // The points contained in the node
    int capacity; // The maximum number of points a node can hold
    bool divided = false; // Whether the node has been divided
    QuadNode* children[4] = {nullptr}; // NW, NE, SW, SE

    QuadNode(const Rectangle<T>& boundary, int capacity)
        : boundary(boundary), capacity(capacity) {}

    ~QuadNode() {
        for (auto& child : children) {
            delete child;
        }
    }

    void subdivide() {
        T xMid = (boundary.bottomLeft.x + boundary.topRight.x) / 2;
        T yMid = (boundary.bottomLeft.y + boundary.topRight.y) / 2;
        Rectangle<T> nwRect = {{boundary.bottomLeft.x, yMid}, {xMid, boundary.topRight.y}};
        Rectangle<T> neRect = {{xMid, yMid}, {boundary.topRight.x, boundary.topRight.y}};
        Rectangle<T> swRect = {{boundary.bottomLeft.x, boundary.bottomLeft.y}, {xMid, yMid}};
        Rectangle<T> seRect = {{xMid, boundary.bottomLeft.y}, {boundary.topRight.x, yMid}};

        children[0] = new QuadNode<T>(nwRect, capacity);
        children[1] = new QuadNode<T>(neRect, capacity);
        children[2] = new QuadNode<T>(swRect, capacity);
        children[3] = new QuadNode<T>(seRect, capacity);

        divided = true;
    }

    bool insert(const Point<T>& point) {
        if (!boundary.contains(point)) return false;

        if (points.size() < capacity) {
            points.push_back(point);
            return true;
        }

        if (!divided) subdivide();

        for (auto& child : children) {
            if (child->insert(point)) return true;
        }

        return false; // Should not happen
    }

    void query(const Rectangle<T>& range, SimpleVector<Point<T>>& found) const {
        if (!boundary.intersects(range)) return;

        for (const auto& point : points) {
            if (range.contains(point)) {
                found.push_back(point);
            }
        }

        if (!divided) return;

        for (const auto& child : children) {
            child->query(range, found);
        }
    }

    bool deleteNode(const Point<T>& point) {
        if (!boundary.contains(point)) {
            return false;
        }

        if (points.size() > 0) {
            for (int i = 0; i < points.size(); i++) {
                if (points[i].x == point.x && points[i].y == point.y) {
                    points.erase(i);
                    return true;
                }
            }
        }

        if (divided) {
            for (int i = 0; i < 4; i++) {
                if (children[i]->deleteNode(point)) {
                    return true;
                }
            }
        }

        return false;
    }
    // Additional functions like deletePoint could be implemented here
    bool contains(const Point<T>& point) {
        if (!boundary.contains(point)) {
            return false;
        }

        if (points.size() > 0) {
            for (int i = 0; i < points.size(); i++) {
                if (points[i].x == point.x && points[i].y == point.y) {
                    return true;
                }
            }
        }

        if (divided) {
            for (int i = 0; i < 4; i++) {
                if (children[i]->contains(point)) {
                    return true;
                }
            }
        }

        return false;
    }

    uint16_t depth() {
        if (!divided) {
            return 1;
        }

        uint16_t maxDepth = 0;
        for (int i = 0; i < 4; i++) {
            maxDepth = max(maxDepth, children[i]->depth());
        }

        return maxDepth + 1;
    }

    SimpleVector<Point<T>> query(const Rectangle<T>& range) {
        SimpleVector<Point<T>> found;
        query(range, found);
        return found;
    }

    bool deletePoint(const Point<T>& point) {
        if (!boundary.contains(point)) {
            return false;
        }
        return deleteNode(point);
    }

};

template <typename T>
class QuadTree {
    private:
    QuadNode<T>* root;
    Rectangle<T> boundary;
    int capacity;

    void print(const QuadNode<T>* node) {
        if (node->isLeaf()) {
            Serial.println("Leaf");
            for (int i = 0; i < node->points.size(); i++) {
                Serial.print("Point: ");
                Serial.print(node->points[i].x);
                Serial.print(", ");
                Serial.println(node->points[i].y);
            }
        } else {
            Serial.println("Divided");
            print(node->NW);
            print(node->NE);
            print(node->SW);
            print(node->SouthEast);
        }
    }

    public:
    QuadTree(const Rectangle<T>& boundary, const int& capacity) : boundary(boundary), capacity(capacity) {
        root = new QuadNode<T>(boundary, capacity);
    }

    ~QuadTree() {
        delete root;
    }

    bool insert(const Point<T>& point) {
        return root->insert(point);
    }

    SimpleVector<Point<T>> query(const Rectangle<T>& range) const {
        SimpleVector<Point<T>> found;
        root->query(range, found);
        return found;
    }

    void clear() {
        delete root;
        root = new QuadNode<T>(boundary, capacity);
    }

    bool deletePoint(const Point<T>& point) {
        if (!boundary.contains(point)) {
            return false;
        }
        return deletePoint(root, point);
    }

    void print() {
        print(root);
    }

    uint16_t size() {
        return root->points.size();
    }

    uint16_t depth() {
        return depth(root);
    }

    bool contains(const Point<T>& point) {
        return boundary.contains(point);
    }

    bool movePoint(const Point<T>& oldPoint, const Point<T>& newPoint) {
        // Check if the old point exists in the quadtree and if the new point is within the boundary
        if (!root->contains(oldPoint) || !boundary.contains(newPoint)) {
            return false;
        }

        // Remove the old point and insert the new point
        root->deletePoint(oldPoint);
        root->insert(newPoint);

        return true;
    }
};



#endif // QUAD_TREE_h