#include <Arduino.h>
#include "OrderedMap.h"

// Example demonstrating OrderedMap with reduced memory usage
// After optimization, OrderedMap no longer keeps a JSON object in memory
// unless serialization/deserialization is explicitly called

OrderedMap<String, int> map1;
OrderedMap<int, String> map2;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    
    Serial.println("OrderedMap Example - Memory Optimized");
    Serial.println("=====================================\n");
    
    // Example 1: Basic usage with String keys and int values
    Serial.println("Example 1: String -> int");
    map1.put("apple", 5);
    map1.put("banana", 3);
    map1.put("cherry", 8);
    map1.put("date", 2);
    
    Serial.print("Size: ");
    Serial.println(map1.size());
    
    Serial.print("Value for 'banana': ");
    Serial.println(map1.get("banana"));
    
    Serial.print("Value for 'cherry': ");
    Serial.println(map1.get("cherry"));
    
    Serial.println();
    
    // Example 2: Basic usage with int keys and String values
    Serial.println("Example 2: int -> String");
    map2.put(1, "one");
    map2.put(2, "two");
    map2.put(3, "three");
    map2.put(4, "four");
    
    Serial.print("Size: ");
    Serial.println(map2.size());
    
    Serial.print("Value for key 2: ");
    Serial.println(map2.get(2));
    
    Serial.print("Value for key 4: ");
    Serial.println(map2.get(4));
    
    Serial.println();
    
    // Example 3: Update existing value
    Serial.println("Example 3: Updating values");
    map1.put("banana", 10);  // Update existing value
    Serial.print("Updated value for 'banana': ");
    Serial.println(map1.get("banana"));
    
    Serial.println();
    
    // Example 4: Demonstrate serialization (JSON only created when needed)
    Serial.println("Example 4: Serialization to file");
    Serial.println("Note: JSON object is only created during this call");
    map1.serializeToJSON("test_map.json");
    
    Serial.println();
    
    // Example 5: Clear and deserialize
    Serial.println("Example 5: Deserialization from file");
    OrderedMap<String, int> map3;
    map3.deserializeFromJSON("test_map.json");
    Serial.print("Deserialized map size: ");
    Serial.println(map3.size());
    Serial.print("Value for 'apple': ");
    Serial.println(map3.get("apple"));
    
    Serial.println();
    Serial.println("Memory optimization: Each OrderedMap now uses significantly");
    Serial.println("less memory as JSON is not kept in memory permanently.");
}

void loop() {
    // Nothing to do in loop
}
