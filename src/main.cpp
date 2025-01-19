#include <Arduino.h>
#include "OrderedMap.h"

OrderedMap<String, String> myMap;
void setup() {
    Serial.begin(9600); // Start Serial Connection
    myMap.setMode(Mode::MEMORY);
    myMap.begin();
    Serial.println(myMap.getMode());
    Serial.println(myMap.getKeyMode());
    Serial.println(myMap.getValueMode());

    for(size_t i = 0; i < 50; i++){
        myMap.put(String(i), String(i*2));
        Serial.println(i);
    }

    for(size_t i = 0; i < myMap.size(); i++){
        Serial.println("Key: " + String(i) + " Value: " + myMap.get(String(i)));
    }
}



void loop(){

}
