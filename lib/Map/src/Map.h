#ifndef MAP_h
#define MAP_h

#include <Arduino.h>
#include <SimpleVector.h>

template <typename K, typename V>
class Map {
    private:
    int Count = 0; // Count of the number of nodes in the Map

    /**
     * A node in the linked list that represents the map.
    */
    struct MapNode {
        K key;
        V value;
        MapNode *next;
    };

    MapNode* head; // The head of the linked list that represents the map

    public:
        friend class MapIterator; // Allow the MapIterator class to access the private members of the Map class

        /**
         * Constructor for the Map class. Initializes the head of the linked list to nullptr.
        */
        Map() : head(nullptr){}

        /**
         * Destructor for the Map class. Deletes all the nodes in the linked list.
        */
        ~Map(){
            while(head != nullptr){
                MapNode* temp = head;
                head = head->next;
                delete temp;
            }
        }

        /**
         * @brief Inserts a key-value pair into the map.
         * 
         * @param key The key of the key-value pair to be inserted.
         * @param value The value of the key-value pair to be inserted.
         * @param index The index at which the key-value pair should be inserted.
         *  
         * @return true if the key-value pair was successfully inserted, false otherwise.
         * 
         * @attention The index should be between 0 and Count (inclusive).
         * If the index is out of bounds, the function should return false.
         * 
         * @date Fixed Function to insert a key-value pair into the map at a specific index. 05/20/2024
         * @version Fixed in Version 1.0.0-BETA
         */ 
        bool insert(K key, V value, int index) {
            if (index < 0 || index > Count) {
                return false; // Return false if the index is out of bounds
            }
            MapNode** pp = &head; 
            for (int i = 0; i < index; i++) {
                pp = &(*pp)->next; // Traverse the linked list to the node before the index
            }
            MapNode* newNode = new MapNode{key, value, *pp}; // Create a new node with the key and value
            *pp = newNode; // Insert the new node at the index
            Count++;
            return true;
        }

        /**
         * @brief Adds a key-value pair to the map.
         * 
         * @param key The key of the key-value pair to be added.
         * @param value The value of the key-value pair to be added.
         * 
         * @return true if the key-value pair was successfully added, false otherwise.
         */
        bool add(K key, V value) {
            MapNode** pp = &head;
            while (*pp != nullptr && (*pp)->key != key) {
                pp = &(*pp)->next;
            }
            if (*pp != nullptr) {
                return false;
            } else {
                MapNode* newNode = new MapNode{key, value, nullptr};
                *pp = newNode;
                Count++;
                return true;
            }
        }

        /**
         * @brief Puts a key-value pair into the map.
         * 
         * @param key The key of the key-value pair to be put.
         * @param value The value of the key-value pair to be put.
         * 
         * @return void
         * @note This Function is exactly the same as the add Function.
         */
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

        /**
         * @brief Gets the value associated with a key in the map.
         * 
         * @param key The key whose value is to be retrieved.
         * 
         * @return The value associated with the key if the key is in the map, otherwise an empty value.
         * 
        */
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

        /**
         * @brief Removes a key-value pair from the map.
         * 
         * @param key The key of the key-value pair to be removed.
         * 
         * @return void
         */
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

        /**
         * @brief Checks if the map contains a key.
         * 
         * @param key The key to be checked.
         * 
         * @return true if the map contains the key, false otherwise.
         */
        bool containsKey(K key) {
            MapNode* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            return p != nullptr;
        }

        /**
         * @brief Checks if the map contains a value.
         * 
         * @param value The value to be checked.
         * 
         * @return true if the map contains the value, false otherwise.
         */
        bool containsValue(V value){
            MapNode* p = head;
            while (p != nullptr && p->value != value) {
                p = p->next;
            }
            return p != nullptr;
        }

        /**
         * @brief Checks the Size of the Map
         * 
         * @return the number of nodes in the map
         */
        int size(){
            return Count;    
        }

        /**
         * @brief Checks if the map is empty.
         * 
         * @return true if the map is empty, false otherwise.
         */
        bool empty(){
           return Count == 0; 
        }

        /**
         * @brief Clears the map.
         * 
         * @return void
         */
        void clear(){
            while(head != nullptr){
                MapNode* temp = head;
                head = head->next;
                delete temp;
            }    
        }

        /**
         * @brief Searches for a key in the map.
         * 
         * @param key The key to be searched for.
         * 
         * @return true if the key is found, false otherwise.
         */
        bool search(K key){
            MapNode* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            return p != nullptr;
        }

        /**
         * @brief Checks if the map contains a key-value pair.
         * 
         * @param key The key to be checked.
         * @param value The value to be checked.
         * 
         * @return true if the map contains the key-value pair, false otherwise.
         */
        bool contains(K key, V value){
            MapNode* p = head;
            while (p != nullptr && (p->key != key || p->value != value)) {
                p = p->next;
            }
            return p != nullptr;
        }
        
        
        /**
         * Returns a vector containing all the keys in the map.
         *
         * @return A vector containing all the keys in the map.
         */
        SimpleVector<K> keys() {
            SimpleVector<K> keyVector;
            for (MapNode* p = head; p != nullptr; p = p->next) {
                keyVector.push_back(p->key);
            }
            return keyVector;
        }

        /**
         * Returns a vector containing all the values stored in the map.
         *
         * @return A vector containing all the values stored in the map.
         */
        SimpleVector<V> values() {
            SimpleVector<V> valueVector;
            for (MapNode* p = head; p != nullptr; p = p->next) {
                valueVector.push_back(p->value);
            }
            return valueVector;
        }
        
        /**
         * Prints the key-value pairs in the map.
         * This function iterates through the map and prints each key-value pair to the Serial monitor.
         */
        void print(){
            MapNode* p = head;
            while (p != nullptr) {
                Serial.print(p->key);
                Serial.print(" : ");
                Serial.println(p->value);
                p = p->next;
            }      
        }

        /**
         * Checks if a given key exists in the map.
         * 
         * @param key The key to search for.
         * @return True if the key exists in the map, false otherwise.
         */
        bool count(K key){
            MapNode* p = head;
            while (p != nullptr && p->key != key) {
                p = p->next;
            }
            return p != nullptr;
        }


        /**
         * @brief Overloaded subscript operator for accessing elements in the map.
         * 
         * This operator allows you to access or modify the value associated with a given key in the map.
         * If the key is found in the map, the corresponding value is returned.
         * If the key is not found, a new node is created with the given key and a default-constructed value is assigned to it.
         * The count of nodes in the map is incremented when a new node is added.
         * 
         * @param key The key to be accessed or modified.
         * @return A reference to the value associated with the given key.
         * 
         * @date fixed known issue in this function 05/20/2024
         * @version fixed in Version 1.0.0-BETA
         */
        V& operator[](const K& key) {
            MapNode** pp = &head;
            while (*pp != nullptr && (*pp)->key != key) {
              pp = &(*pp)->next;
            }
            if (*pp != nullptr) {
              return (*pp)->value;
            } else {
              MapNode* newNode = new MapNode{ key, V(), nullptr };
              *pp = newNode;
              Count++; // Increment the count when a new node is added
              return newNode->value;
            }
        }

        class MapIterator {
            private:
                MapNode* current; // The current node in the linked list
                
                
                /**
                 * @brief A template struct representing a pair of values.
                 * 
                 * This struct is used to store a pair of values, where the first value is of type 'First' and the second value is of type 'Second'.
                 */
                template<class First, class Second>
                struct pair{
                    First first;    /**< The first value of the pair. */
                    Second second;  /**< The second value of the pair. */
                };
                pair<const K, V> pr; // The pair of key and value
            public:

                /**
                 * @brief Constructs a MapIterator object.
                 *
                 * This constructor initializes a MapIterator object with the given start node.
                 *
                 * @param start A pointer to the start node of the map.
                 */
                MapIterator(MapNode* start) : current(start), pr{start != nullptr ? start->key : K(), start != nullptr ? start->value : V()}  {}
                /**
                 * @brief Overloaded inequality operator for comparing two MapIterator objects.
                 *
                 * This operator compares the current iterator with another iterator and returns true if they are not equal, and false otherwise.
                 *
                 * @param other The MapIterator object to compare with.
                 * @return True if the iterators are not equal, false otherwise.
                 */
                bool operator!=(const MapIterator& other) const {
                    return current != other.current;
                }

                /**
                 * @brief Advances the iterator to the next element in the map.
                 * 
                 * This operator overloads the pre-increment operator (++). It advances the iterator to the next element in the map.
                 * 
                 * @note If the iterator is already pointing to the last element, it will become invalid after calling this operator.
                 */
                void operator++() {
                    current = current->next;
                }
                
                /**
                 * @brief Creates a pair object with the given values.
                 * 
                 * This function creates a pair object with the specified values for the first and second elements.
                 * The pair object is then returned.
                 * 
                 * @tparam First The type of the first element in the pair.
                 * @tparam Second The type of the second element in the pair.
                 * @param first The value for the first element.
                 * @param second The value for the second element.
                 * @return A pair object with the specified values.
                 */
                template <class First, class Second>
                pair<First, Second> make_pair(First first, Second second) {
                    pair<First, Second> p = {first, second};
                    return p;
                }

                /**
                 * @brief Overloads the arrow operator (->) to create a new pair object and return a pointer to it.
                 * 
                 * This operator is used to access the members of the current pair in the Map class.
                 * It creates a new pair object with the same key and value as the current pair, and returns a pointer to it.
                 * 
                 * @tparam K The type of the key in the pair.
                 * @tparam V The type of the value in the pair.
                 * @return A pointer to a new pair object with the same key and value as the current pair.
                 */
                pair<K, V>* operator->() {
                    pair<K, V>* temp = new pair<K, V>{current->key, current->value};
                    return temp;
                }

                /**
                 * @brief Dereference operator for the Map iterator.
                 * 
                 * This operator allows you to dereference the Map iterator and obtain a reference to the key-value pair it is pointing to.
                 * 
                 * @return A reference to the key-value pair.
                 */
                pair<const K, V>& operator*() {
                    return pr;
                }
        };

        /**
         * Returns an iterator pointing to the first element in the map.
         *
         * @return An iterator pointing to the first element in the map.
         */
        MapIterator begin() {
            return MapIterator(head);
        }

        /**
         * Returns an iterator pointing to the past-the-end element in the map.
         * The past-the-end element is the theoretical element that would follow the last element in the map.
         * It does not point to any element, and thus shall not be dereferenced.
         *
         * @return An iterator pointing to the past-the-end element in the map.
         */
        MapIterator end() {
            return MapIterator(nullptr);
        }
};

#endif // MAP_h