#include <Arduino.h>
#include "Properties.h"
#include "MyDictionary.h"
#include "LinkedList.h"
#include "UnorderedMap.h"
#include "SDList.h"
#include "DoubleLinkedList.h"

SDList <String> strings(10, "pagefile.dat");
SDList <int> ints(10, "pagefile.dat");
void setup(){
  Serial.begin(115200);
  if(strings.sdAvailable()){
    strings.append("Hello");
    strings.append("World!");
    strings.append("!");
  }

  if(ints.sdAvailable()){
    ints.append(1);
    ints.append(2);
    ints.append(3);
  }

}

void loop(){

}