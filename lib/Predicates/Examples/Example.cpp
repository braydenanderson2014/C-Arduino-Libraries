#include <Arduino.h>
#include <ArrayList.h>
#include <LinkedList.h>
#include "Operators.h"
#include "Predicates.h"

ArrayList<int> list(ArrayList<int>::DYNAMIC, 10);
LinkedList<int> linkedList;
void setup(){
    Serial.begin(9600);
    while(!Serial){}

    for(int i = 0; i < 10; i++){
        list.add(i);
        linkedList.append(i);
    }

    for(int i = 0; i < list.size(); i++){
        Serial.println(list.get(i) + "\n");
        Serial.println(String(linkedList.getAsString(i)) + "\n");
    }

    for(int i = 0; i < list.size(); i++){
        list.set(Operators<int>::add(list.get(i)), i);
        linkedList.insert(Operators<int>::add(linkedList.getAsString(i).toInt()), i);
    }

    for(int i = 0; i < list.size(); i++){
        Serial.println(list.get(i) + "\n");
        Serial.println(String(linkedList.getAsString(i)) + "\n");
    }

    for(int i = 0; i < list.size(); i++){
        list.set(Operators<int>::subtract(list.get(i)), i);
        linkedList.insert(Operators<int>::subtract(linkedList.getAsString(i).toInt()), i);

        if(Predicates<int>::isEven(list.get(i))){
            list.remove(i);
        }

        if(Predicates<int>::isOdd(linkedList.getAsString(i).toInt())){
            linkedList.remove(i);
        }
    }
    for(int i = 0; i < list.size(); i++){
        Serial.println(list.get(i) + "\n");
        Serial.println(String(linkedList.getAsString(i)) + "\n");
    }
}

void loop(){

}