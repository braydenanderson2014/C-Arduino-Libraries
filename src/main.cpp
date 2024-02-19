#include <Arduino.h>

#include "DoubleLinkedList.h"


DoubleLinkedList <String> list;

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){}
  for(int i = 0; i < 5; i++){
    list.append(String(i * 10));
  }

  for(int i = 0; i < list.size(); i++){
    Serial.println(list.getElement(i));
  }
  //list.clear();
}

void loop(){
  
}
