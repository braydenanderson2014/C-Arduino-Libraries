/*
    Example Name: Example For Map Library

    Basic example for the Map library. This example demonstrates the basic functionality of the Map library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <Map.h>

// Create a Map object
Map<int, int> map;

void setup() {
    // Insert a key-value pair into the map
    map.insert(1, 2); // This will insert the key-value pair (1, 2) into the map
    map.insert(3, 4); // This will insert the key-value pair (3, 4) into the map

    map.add(5, 6); // This will insert the key-value pair (5, 6) into the map
    map.add(7, 8); // This will insert the key-value pair (7, 8) into the map

    map.put(9, 10); // This will insert the key-value pair (9, 10) into the map
    map.put(11, 12); // This will insert the key-value pair (11, 12) into the map
    
    // Search for a key in the map
    if(map.search(1)){
        Serial.println("Key found");
    } else {
        Serial.println("Key not found");
    }

    bool found = map.search(3); // This will search for the key 3 in the map
    if(found){
        Serial.println("Key found");
    } else {
        Serial.println("Key not found");
    }
    // Remove a key-value pair from the map
    map.remove(1);

    map.clear(); // This will clear the map
}
