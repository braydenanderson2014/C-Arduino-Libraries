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
    Point<T> bottomLeft; // The bottom-left point of the rectangle
    Point<T> topRight; // The top-right point of the rectangle
    Point<T> topLeft; // The top-left point of the rectangle
    Point<T> bottomRight; // The bottom-right point of the rectangle

    bool contains(const Point<T>& point) {
        return point.x >= bottomLeft.x && point.x <= topRight.x && point.y >= bottomLeft.y && point.y <= topRight.y;
    }

    bool intersects(const Rectangle<T>& other) {
        return !(topLeft.x > other.bottomRight.x || bottomRight.x < other.topLeft.x || topLeft.y > other.bottomRight.y || bottomRight.y < other.topLeft.y);
    }

};

template <typename T>
struct QuadNode {
    static_assert(is_arithmetic<T>::value, "QuadNode only supports number types");
    Point<T> point; // The point stored in the node
    Rectangle<T> boundary; // The boundary of the node
    SimpleVector<Point<T>> points; // The points stored in the node
    int capacity; // The maximum number of points that can be stored in the node
    bool divided; // Whether the node has been divided
    QuadNode* NW; // The north-west quadrant
    QuadNode* NE; // The north-east quadrant
    QuadNode* SW; // The south-west quadrant
    QuadNode* SouthEast; // The south-east quadrant

    QuadNode(const Rectangle<T>& boundary, const int& capacity) : boundary(boundary), capacity(capacity), divided(false) {
        NW = NE = SW = SouthEast = nullptr;
    }

    bool isLeaf() {
        return NW == nullptr && NE == nullptr && SW == nullptr && SouthEast == nullptr;
    }

    void subdivide() {
        T x = boundary.bottomLeft.x + (boundary.topRight.x - boundary.bottomLeft.x) / 2;
        T y = boundary.bottomLeft.y + (boundary.topRight.y - boundary.bottomLeft.y) / 2;
        Rectangle<T> ne = Rectangle<T>{Point<T>{x, y}, boundary.topRight};
        NE = new QuadNode(ne, capacity);
        Rectangle<T> nw = Rectangle<T>{Point<T>{boundary.bottomLeft.x, y}, Point<T>{x, boundary.topRight.y}};
        NW = new QuadNode(nw, capacity);
        Rectangle<T> se = Rectangle<T>{Point<T>{x, boundary.bottomLeft.y}, Point<T>{boundary.topRight.x, y}};
        SouthEast = new QuadNode(se, capacity);
        Rectangle<T> sw = Rectangle<T>{boundary.bottomLeft, Point<T>{x, y}};
        SW = new QuadNode(sw, capacity);
        divided = true;
    }

    bool insert(const Point<T>& point) {
        if (!boundary.contains(point)) {
            return false;
        }
        if (points.size() < capacity) {
            points.push_back(point);
            return true;
        }
        if (!divided) {
            subdivide();
        }
        return (NW->insert(point) || NE->insert(point) || SW->insert(point) || SouthEast->insert(point));
    }

    SimpleVector<Point<T>> query(const Rectangle<T>& range) {
        SimpleVector<Point<T>> found;
        if (!boundary.intersects(range)) {
            return found;
        }
        for (int i = 0; i < points.size(); i++) {
            if (range.contains(points[i])) {
                found.push_back(points[i]);
            }
        }
        if (divided) {
            SimpleVector<Point<T>> nwPoints = NW->query(range);
            SimpleVector<Point<T>> nePoints = NE->query(range);
            SimpleVector<Point<T>> swPoints = SW->query(range);
            SimpleVector<Point<T>> sePoints = SouthEast->query(range);

            found.insert(found.end(), nwPoints.begin(), nwPoints.end());
            found.insert(found.end(), nePoints.begin(), nePoints.end());
            found.insert(found.end(), swPoints.begin(), swPoints.end());
            found.insert(found.end(), sePoints.begin(), sePoints.end());
        }
        return found;
    }

    bool deletePoint(const Point<T>& point) {
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
            return (NW->deletePoint(point) || NE->deletePoint(point) || SW->deletePoint(point) || SouthEast->deletePoint(point));
        }
        return false;
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

    SimpleVector<Point<T>> query(const Rectangle<T>& range) {
        return root->query(range);
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