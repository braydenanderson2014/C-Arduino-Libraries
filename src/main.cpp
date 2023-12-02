#include <Arduino.h>
#include "Properties.h"
#include "MyDictionary.h"
#include "LinkedList.h"
#include "UnorderedMap.h"
#include "SDList.h"
#include "DoubleLinkedList.h"
#include "ArrayList.h"
#include "Operators.h"
#include "Predicates.h"

ArrayList <String> theStrings;

//SDList <String> strings(10, "pagefile.dat");
void setup(){
  Serial.begin(9600);
  Serial.println("Hello World");
  
  theStrings.add("Hello");
  theStrings.add("World");
  theStrings.add("!");

  for(int i = 0; i < theStrings.size(); i++){
    Serial.println(theStrings.get(i));
  }

  theStrings.remove(1);
  theStrings.remove(0);

  
  for(int i = 0; i < theStrings.size(); i++){
    Serial.println(theStrings.get(i));
  }
}

void loop(){

}