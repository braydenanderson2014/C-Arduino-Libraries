# OrderedMap Class

## Description
Using the SDList Library which uses ArrayList in the background, Ordered List is a non hashed, in order, saveable map. It uses Key and Value Pairs, You can save the map to a file.

## Memory Optimization
**Version 1.0.1-ALPHA (Memory Optimized)**
- JSON object is now only created when serialization/deserialization is needed
- Significantly reduced memory footprint for OrderedMap instances
- Each OrderedMap instance no longer carries the overhead of a JSON object with SdFat and SimpleVector allocations
- Memory is only used for serialization when `serializeToJSON()` or `deserializeFromJSON()` is explicitly called

This optimization allows Arduino Mega to create many more OrderedMap instances without running out of memory.

## STATUS:
* Testing: So far testing is successful!
* Memory: Optimized for minimal memory usage - suitable for resource-constrained devices

## Change Log:
### Version 1.0.1-ALPHA: [CURRENT]
* Memory optimization: Removed persistent JSON member variable
* JSON object now created only during serialization/deserialization operations
* Significantly reduced memory footprint per OrderedMap instance

### Version 1.0.0-BETA: [UNRELEASED], [FUTURE-RELEASE]
* Initial Release


## Example:

```cpp
#include <Arduino.h>
#include "OrderedMap.h"

OrderedMap<String, int> map;

void setup() {
    Serial.begin(115200);
    
    // Add key-value pairs
    map.put("apple", 5);
    map.put("banana", 3);
    map.put("cherry", 8);
    
    // Get values
    Serial.println(map.get("banana"));  // Output: 3
    
    // Update values
    map.put("banana", 10);
    Serial.println(map.get("banana"));  // Output: 10
    
    // Check size
    Serial.println(map.size());  // Output: 3
    
    // Serialize to file (JSON object created only during this call)
    map.serializeToJSON("data.json");
    
    // Deserialize from file (JSON object created only during this call)
    OrderedMap<String, int> map2;
    map2.deserializeFromJSON("data.json");
}
```

See the `Example/Example.cpp` file for more detailed examples.

## API

### Basic Operations
- `void put(const K& key, const V& value)` - Add or update a key-value pair
- `V get(const K& key)` - Get value by key (returns default value if not found)
- `size_t size()` - Get number of entries
- `void clear()` - Remove all entries

### Serialization (Memory Efficient)
- `void serializeToJSON(const String& filename)` - Save map to JSON file (creates JSON object temporarily)
- `void deserializeFromJSON(const String& filename)` - Load map from JSON file (creates JSON object temporarily)

**Note:** JSON objects are only created during serialization/deserialization operations, minimizing memory usage.

