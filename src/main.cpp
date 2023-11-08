#include <Arduino.h>
#include "Properties.h"
#include "MyDictionary.h"
#include "LinkedList.h"
#include "UnorderedMap.h"
#include "SDList.h"


UnorderedMap <String, String> StringMap;
UnorderedMap <String, int> StringIntMap;
UnorderedMap <int, int> IntMap;
UnorderedMap <int, String> IntStringMap;
void setup() {
    Serial.begin(9600);
    StringMap.insert("Hello", "World");
    StringMap.insert("Hello1", "World1");
    StringMap.insert("Hello2", "World2");
    StringMap.insert("Hello3", "World3");
    StringMap.insert("Hello4", "World4");

    StringIntMap.insert("Hello", 1);
    StringIntMap.insert("Hello1", 2);
    StringIntMap.insert("Hello2", 3);
    StringIntMap.insert("Hello3", 4);
    StringIntMap.insert("Hello4", 5);

    IntMap.insert(1, 1);
    IntMap.insert(2, 2);
    IntMap.insert(3, 3);
    IntMap.insert(4, 4);
    IntMap.insert(5, 5);

    IntStringMap.insert(1, "Hello");
    IntStringMap.insert(2, "Hello1");
    IntStringMap.insert(3, "Hello2");
    IntStringMap.insert(4, "Hello3");
    IntStringMap.insert(5, "Hello4");

    Serial.println(StringMap.get("Hello"));

    Serial.println(StringIntMap.get("Hello"));

    Serial.println(IntMap.get(1));

    Serial.println(IntStringMap.get(1));

  //Capacity
  Serial.println("Capacity" + String(StringMap.getCapacity()));
  Serial.println("Size" + String(StringMap.getSize()));

  Serial.println("Capacity" + String(StringIntMap.getCapacity()));
  Serial.println("Size" + String(StringIntMap.getSize()));

  Serial.println("Capacity" + String(IntMap.getCapacity()));
  Serial.println("Size" + String(IntMap.getSize()));

  Serial.println("Capacity" + String(IntStringMap.getCapacity()));
  Serial.println("Size" + String(IntStringMap.getSize()));
}
    



void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}