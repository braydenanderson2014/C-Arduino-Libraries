#ifndef MAP_h
#define MAP_h

#include <Arduino.h>


template <typename K, typename V>
class Map {
    private:
    struct MapNode {
        K key;
        V value;
        MapNode *next;
    };

    MapNode* head;

    public:
        Map() : head(nullptr){}

        ~Map(){
            while(head != nullptr){
                MapNode* temp = head;
                head = head->next;
                delete temp;
            }
        }

       void put(K key, V value) {
        Node** pp = &head;
        while (*pp != nullptr && (*pp)->key != key) {
            pp = &(*pp)->next;
        }
        if (*pp != nullptr) {
            (*pp)->value = value;
        } else {
            Node* newNode = new Node{key, value, nullptr};
            *pp = newNode;
        }
    }

        V get(K key) {
            Node* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            if (p != nullptr) {
                return p->value;
            } else {
                return V();
            }
        }

        void remove(K key){
            Node** pp = &head;
            while (*pp != nullptr && (*pp)->key != key) {
                pp = &(*pp)->next;
            }
            if (*pp != nullptr) {
                Node* temp = *pp;
                *pp = (*pp)->next;
                delete temp;
            }
        }
        bool containsKey(K key) {
            Node* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            return p != nullptr;
        }

        bool containsValue(V value){
            
        }
        int size(){
            return count;    
        }
        bool isEmpty(){
            
        }
        void clear(){
            
        }
        K* keys(){
            
        }
        V* values(){
            
        }
        void print(){
            
        }

        ~Map(){
            
        }

        V& operator[](const K& key) {
            MapNode** pp = &head;
            while (*pp != nullptr && (*pp)->key != key) {
                pp = &(*pp)->next;
            }
            if (*pp != nullptr) {
                return (*pp)->value;
            } else {
                MapNode* newNode = new MapNode{key, V(), nullptr};
                *pp = newNode;
                return newNode->value;
            }
        }

        class Iterator {
            private:
                MapNode* current;
            public:
                Iterator(MapNode* start) : current(start) {}
                bool operator!=(const Iterator& other) const {
                    return current != other.current;
                }
                void operator++() {
                    current = current->next;
                }
                std::pair<K, V> operator*() {
                    return std::make_pair(current->key, current->value);
                }
        };

        Iterator begin() {
            return Iterator(head);
        }

        Iterator end() {
            return Iterator(nullptr);
        }


};






#endif // MAP_h