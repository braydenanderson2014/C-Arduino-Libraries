#include "ExtremeVariant.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) {} // Ensure Serial is ready

    Serial.println("--- ExtremeVariant Example ---");

    // Create an ExtremeVariant instance for int values with String keys
    ExtremeVariant<String, int> variant; //MUST DEFINE BOTH TEMPLATE PARAMETERS EVEN IF YOU DO NOT USE BOTH!!!
    //String represents the key type, int represents the value type
    //For single items or list items, the value is used as the type. 

    // Demonstrate setting a single value
    Serial.println("\nSetting a Single Value:");
    variant.setSingle(42); // Set a single value
    if (variant.getType() == SINGLE_ELEMENT) { // Check if the variant contains a single value
        Serial.print("Single Value: ");
        Serial.println(variant.getSingle().orElse(-1)); // Output: 42//Get the single value, or else return -1 (Error/Default value)
    }

    // Demonstrate setting a list of values
    Serial.println("\nSetting a List of Values:");
    SimpleVector<int> values; // Create a list of values
    values.push_back(10); // Add values to the list
    values.push_back(20);
    values.push_back(30);
    variant.setList(values); //Set the Vector as the list of values
    if (variant.getType() == SINGLE_LIST) { // Check if the variant contains a list of values
        Optional<SimpleVector<int>> listOpt = variant.getList(); // Get the list of values and store in an Optional of type SimpleVector<int>
        if (listOpt.hasValue()) { // Check if the list is valid (not empty)
            Serial.print("List Values: ");
            // Iterate over the list and print values
            for (size_t i = 0; i < listOpt.getValue().size(); i++) { //getValue() returns the actual item instead of a pointer
                Serial.print(listOpt.getValue()[i]); // Output: 10 20 30 //getValue() returns the actual item instead of a pointer
                Serial.print(" ");
            }
            Serial.println();
        }
    }

    // Demonstrate setting key-value pairs
    Serial.println("\nSetting Key-Value Pairs:");
    Hashtable<String, int> table; // Create a Hashtable with String keys and int values
    table.put("one", 1); // Add key-value pairs to the Hashtable
    table.put("two", 2);
    table.put("three", 3);
    variant.setKeyValuePairs(table); // Set the variant to contain the key-value pairs
    if (variant.getType() == KEY_VALUE_PAIRS) { // Check if the variant contains key-value pairs
        Optional<Hashtable<String, int>> mapOpt = variant.getKeyValuePairs(); // Get the key-value pairs and store in an Optional of type Hashtable<String, int>
        if (mapOpt.hasValue()) { // Check if the key-value pairs are valid (not empty)
            Serial.println("Key-Value Pairs:");
            // Retrieve all keys and iterate to print key-value pairs
            SimpleVector<String> keys = mapOpt.getValue().keys(); // Get all keys from the Hashtable and store in a SimpleVector<String>
            for (size_t i = 0; i < keys.size(); i++) { // Iterate over the keys
                Serial.print(keys[i]); // Output: one two three
                Serial.print(": ");
                // Optional of type int to handle potential missing values
                //getValue() [HASHTABLE]: Returns the actual item instead of a pointer
                //getElement() [SIMPLEVECTOR]: Returns the item at the specified index
                //keys[i] is the key at the current index.
                Optional<int> valueOpt = Optional<int>(mapOpt.getValue().getElement(keys[i])); // Get the value associated with the key
                Serial.println(valueOpt.orElse(-1)); // Output: 1 2 3 //orElse() returns -1 if the value is missing
            }
        }
    }

    // Demonstrate clearing the variant
    Serial.println("\nClearing Variant:");
    variant.clear(); // If I need to explain this one, you should probably not be coding. :)
    Serial.println(variant.getType() == NONE ? "Variant cleared" : "Clear failed"); // Check if the variant is cleared
}

void loop() {
    // Nothing to do in loop
}