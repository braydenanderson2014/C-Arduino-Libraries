#ifndef MAP_h
#define MAP_h

#include <Arduino.h>
#include <SimpleVector.h>

template <typename K, typename V>
class Map {
    private:
    int Count = 0;
    struct MapNode {
        K key;
        V value;
        MapNode *next;
    };

    MapNode* head;

    public:
        friend class Iterator;

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
            Count++;
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
                Count--;
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
            MapNode* p = head;
            while (p != nullptr && p->value != value) {
                p = p->next;
            }
            return p != nullptr;
        }

        int size(){
            return Count;    
        }

        bool empty(){
           return Count == 0; 
        }

        void clear(){
            while(head != nullptr){
                MapNode* temp = head;
                head = head->next;
                delete temp;
            }    
        }
        SimpleVector<K> keys() {
            SimpleVector<K> keyVector;
            for (MapNode* p = head; p != nullptr; p = p->next) {
                keyVector.push_back(p->key);
            }
            return keyVector;
        }

        SimpleVector<V> values() {
            SimpleVector<V> valueVector;
            for (MapNode* p = head; p != nullptr; p = p->next) {
                valueVector.push_back(p->value);
            }
            return valueVector;
        }
        
        void print(){
            MapNode* p = head;
            while (p != nullptr) {
                Serial.print(p->key);
                Serial.print(" : ");
                Serial.println(p->value);
                p = p->next;
            }      
        }

        bool count(K key){
            MapNode* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            return p != nullptr;
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
                template<class First, class Second>
                struct pair{
                    First first;
                    Second second;
                };
                pair<const K, V> pr;
            public:
                Iterator(MapNode* start) : current(start), pr{start != nullptr ? start->key : K(), start != nullptr ? start->value : V()}  {}
                bool operator!=(const Iterator& other) const {
                    return current != other.current;
                }
                void operator++() {
                    current = current->next;
                }

                
                
                template <class First, class Second>
                pair<First, Second> make_pair(First first, Second second) {
                    pair<First, Second> p = {first, second};
                    return p;
                }

                pair<K, V>* operator->() {
                    pair<K, V>* temp = new pair<K, V>{current->key, current->value};
                    return temp;
                }

                pair<const K, V>& operator*() {
                    return pr;
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