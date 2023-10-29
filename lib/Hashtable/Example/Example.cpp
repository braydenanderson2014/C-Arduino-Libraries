#include <Arduino>
#include <Hashtable.h>

int main() {
    Hashtable<std::string, int> myHashtable;

    // Adding key-value pairs to the hashtable
    myHashtable.put("apple", 5);
    myHashtable.put("banana", 3);
    myHashtable.put("cherry", 8);

    // Retrieving values by key
    int appleCount = myHashtable.get("apple");
    int bananaCount = myHashtable.get("banana");

//    std::cout << "Apple count: " << appleCount << std::endl;
    Serial.println("Apple count: " + appleCount);
//    std::cout << "Banana count: " << bananaCount << std::endl;
    Serial.println("Banana count: " + bananaCount);

    // Removing a key-value pair
    myHashtable.remove("cherry");

    // Checking if a key exists
    if (myHashtable.containsKey("cherry")) {
        //std::cout << "Cherry exists in the hashtable." << std::endl;
        Serial.println("Cherry exists in the hashtable.");
    } else {
        Serial.println("Cherry does not exist in the hashtable.");
    }

    // Iterating through keys
    SimpleVector<std::string> keys = myHashtable.keys();
    for (const std::string& key : keys) {
        Serial.print("Key: ");
        Serial.print(key.c_str());
        Serial.print(", Value: ");
        Serial.println(myHashtable.get(key).c_str());
    }

    return 0;
}
