#include <Dictionary.h>

int main() {
    Dictionary<String, int> myDictionary;

    // Adding key-value pairs to the dictionary
    myDictionary.put("apple", 5);
    myDictionary.put("banana", 3);
    myDictionary.put("cherry", 8);

    // Retrieving values by key
    int appleCount = myDictionary.get("apple");
    int bananaCount = myDictionary.get("banana");

}