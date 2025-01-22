#include <Arduino.h>
#include "OrderedMap.h"
#include "SDList.h"

//OrderedMap<String, String> myMap;
SDList<String> list;

void setup() {
    Serial.begin(9600); // Start Serial Connection
    /*
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
    */

/*

    ArrayList<String> myArray;
    
    for(size_t i = 0; i < 50; i++){
        myArray.add("Array: " + String(i));
    }
    for(size_t i = 0; i < myArray.size(); i++){
        Serial.println("Array: " + myArray.get(i));
    }
    myArray.clear();
*/
    list.setMode(Mode::SDCARD);
    //ArrayList<String> list;
    for(size_t i = 0; i < 50; i++){
        delay(100);
        list.append(String(i));
        Serial.println(i);
    }
    delay(1000);
        Serial.println("List Size: " + list.size());
        Serial.println("List Capacity: " + list.capacity());
    delay(1000);
    for(size_t i = 0; i < list.size(); i++){
        Serial.println(list.get(i));
    }
    Serial.println(list.getMode());
}


void loop(){

}
