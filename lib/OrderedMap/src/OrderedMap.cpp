#include <Arduino.h>
#include "OrderedMap.h"
#include <SDList.h>

template<typename K, typename V>
OrderedMap<K, V>::OrderedMap(){
    internalKeys = SDList<K>(MEMORY, 8);
    internalValues = SDList<V>(MEMORY, 8);
}

template<typename K, typename V>
OrderedMap<K, V>::OrderedMap(size_t initialCapacity, typename OrderedMap<K, V>::Mode mode){
    internalKeys = SDList<K>(mode, initialCapacity);
    internalValues = SDList<V>(mode, initialCapacity);
}

template<typename K, typename V>
void OrderedMap<K, V>::put(K key, V value){
    internalKeys.append(key);
    internalValues.append(value);
    count++;
}
template<typename K, typename V>
V OrderedMap<K, V>::get(K key){
    for(int i = 0; i < internalKeys.size(); i++){
        if(internalKeys.get(i) == key){
            return internalValues.get(i);
        }
    }
    return NULL;
}

template<typename K, typename V>
void OrderedMap<K, V>::remove(K key){
    for(int i = 0; i < internalKeys.size(); i++){
        if(internalKeys.get(i) == key){
            internalKeys.remove(i);
            internalValues.remove(i);
            count--;
        }
    }
}

template<typename K, typename V>
bool OrderedMap<K, V>::containsKey(K key){
    for(int i = 0; i < internalKeys.size(); i++){
        if(internalKeys.get(i) == key){
            return true;
        }
    }
    return false;
}

template<typename K, typename V>
size_t OrderedMap<K, V>::size(){
    return internalKeys.size();
}

template<typename K, typename V>
size_t OrderedMap<K, V>::capacity(){
    return internalKeys.capacity();
}

template<typename K, typename V>
bool OrderedMap<K, V>::isEmpty(){
    return internalKeys.isEmpty();
}

template<typename K, typename V>
void OrderedMap<K, V>::clear(){
    internalKeys.clear();
    internalValues.clear();
}

template<typename K, typename V>
SDList<K> OrderedMap<K, V>::keys(){
    return internalKeys;
}

template<typename K, typename V>
SDList<V> OrderedMap<K, V>::values(){
    return internalValues;
}

template<typename K, typename V>
bool OrderedMap<K, V>::save(const String& filename){
    internalKeys.dumpMemoryToFile();
    internalValues.dumpMemoryToFile();
    return true;
}

template<typename K, typename V>
bool OrderedMap<K, V>::load(const String& filename){
    internalKeys.readFileIntoMemory();
    internalValues.readFileIntoMemory();
    return true;
}

template<typename K, typename V>
bool OrderedMap<K, V>::store(const String& filename, const String& comments){
    internalKeys.dumpMemoryToFile();
    internalValues.dumpMemoryToFile();
    return true;
}

template<typename K, typename V>
bool OrderedMap<K, V>::exists(const String& key){
    return internalKeys.contains(key);
}

template<typename K, typename V>
bool OrderedMap<K, V>::exists(const String& key, const String& value){
    for(int i = 0; i < internalKeys.size(); i++){
        if(internalKeys.get(i) == key && internalValues.get(i) == value){
            return true;
        }
    }
    return false;
}

template<typename K, typename V>
bool OrderedMap<K, V>::fileExists(const String& filename){
    return SD.exists(filename);
}

template<typename K, typename V>
bool OrderedMap<K, V>::deleteFile(const String& filename){
    return SD.remove(filename);
}

template<typename K, typename V>
bool OrderedMap<K, V>::renameFile(const String& oldFilename, const String& newFilename){
    return;
}

template<typename K, typename V>
bool OrderedMap<K, V>::swapMode(){
    if(mode == MEMORY){
        mode = SD;
    } else {
        mode = MEMORY;
    }
    setMode(mode);
    return true;
}

template<typename K, typename V>
typename OrderedMap<K,V>::Mode OrderedMap<K, V>::getMode() const {
    return internalKeys.getMode();
}

template<typename K, typename V>
void OrderedMap<K, V>::setMode(typename OrderedMap<K, V>::Mode mode){
    internalKeys.setMode(mode);
    internalValues.setMode(mode);
    mode = this -> mode;
}






