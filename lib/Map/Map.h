#ifndef MAP_h
#define MAP_h

#include <Arduino.h>

template <typename K, typename V>
class Map {
    private:
    int count = 0;
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
        MapNode** pp = &head;
        while (*pp != nullptr && (*pp)->key != key) {
            pp = &(*pp)->next;
        }
        if (*pp != nullptr) {
            (*pp)->value = value;
        } else {
            MapNode* newNode = new MapNode{key, value, nullptr};
            *pp = newNode;
        }
    }

        V get(K key) {
            MapNode* p = head;
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
            MapNode** pp = &head;
            while (*pp != nullptr && (*pp)->key != key) {
                pp = &(*pp)->next;
            }
            if (*pp != nullptr) {
                MapNode* temp = *pp;
                *pp = (*pp)->next;
                delete temp;
            }
        }
        bool containsKey(K key) {
            MapNode* p = head;
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
           return count == 0; 
        }
        void clear(){
            
        }
        K* keys(){
            
        }
        V* values(){
            
        }
        void print(){
            
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

                template<class First, class Second>
                struct pair{
                    First first;
                    Second second;
                };
                
                template <class First, class Second>
                pair<First, Second> make_pair(First first, Second second) {
                    pair<First, Second> p = {first, second};
                    return p;
                }
                pair<K, V> operator*() {
                    return make_pair(current->key, current->value);
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