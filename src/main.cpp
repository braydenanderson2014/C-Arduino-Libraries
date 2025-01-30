#include <Arduino.h>
#include "OrderedMap.h"
#include "SDList.h"
#include "JSON.h"




void setup() {
    Serial.begin(9600);
    while(!Serial){}
    //OrderedMap<const char*, const char*> myMap;
    OrderedMap<String, String> myMap;

    while(!Serial);
    for (int i = 0; i < 5; i++) {
        char key[10];
        char value[10];

        // Format keys and values as strings (e.g., "key0", "value0").
        snprintf(key, sizeof(key), "key%d", i);
        snprintf(value, sizeof(value), "val%d", i);

        myMap.put(key, value);
    }
    myMap.serializeToJSON("test.json");
}

extern char *__brkval;
int freeMemory() {
    char top;
    return &top - __brkval;
}



/*
void setup(){
    Serial.begin(9600);
    while(!Serial){}
    Serial.println("Starting setup...");
    Serial.print("Free memory before SimpleVector: ");
    Serial.println(freeMemory());

    SimpleVector<int> testVector;

    Serial.print("Free memory after SimpleVector: ");
    Serial.println(freeMemory());

    //JSON json;

    testSimpleVectorWithNode();

    Serial.print("Free memory after JSON: ");
    Serial.println(freeMemory());

    Serial.println("Setup done.");

    keys.add("key1");
    keys.add("key2");    
    values.add("value1");
    values.add("value2");

    Serial.println("Size: " + String(keys.size()));
    Serial.println("Size: " + String(values.size()));

    keys.clear();
    values.clear();

    Serial.println("Starting loop...");
    for (int i = 0; i < 50; i++) {
        char key[10];
        char value[10];

        // Format keys and values as strings (e.g., "key0", "value0").
        snprintf(key, sizeof(key), "key%d", i);
        snprintf(value, sizeof(value), "val%d", i);

        keys.add(key);
        values.add(value);
        Serial.print("Added Key: ");
        Serial.print(key);
        Serial.print(", Value: ");
        Serial.println(value);
    }
}
*/



void loop(){

}
