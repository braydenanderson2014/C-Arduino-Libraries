#include "ArrayList.h"

ArrayList<String> list;
void setup() {
    Serial.begin(9600); // Start Serial Connection
    for(int i = 0; i < 50; i++){
        list.add(String(i));
        Serial.println(list.capacity());
    }

    for(int i = 0; i < list.size(); i++){
        Serial.println(list.get(i));
    }
}



void loop(){

}
