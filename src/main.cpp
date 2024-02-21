#include <Arduino.h>

#include "DoubleLinkedList.h"
#include "BasicLinkedList.h"


DoubleLinkedList <String> list;
LinkedList <String> list2;

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){}
  for(int i = 0; i < 30; i++){
    list.append(String(i * 10));
    list2.append(String(i * 10));
  }

  for(int i = 0; i < list.size(); i++){
    Serial.println("DoubleLinkedList" + list.getElement(i));
    Serial.println("LinkedList" + list2.getElement(i));
  }
  //list.clear();
}

void loop(){
  
}
