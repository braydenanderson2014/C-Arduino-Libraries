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
    StringMap.put("Hello", "World");
    StringMap.put("Hello1", "World1");
    StringMap.put("Hello2", "World2");
    StringMap.put("Hello3", "World3");
    StringMap.put("Hello4", "World4");

    StringIntMap.put("Hello", 1);
    StringIntMap.put("Hello1", 2);
    StringIntMap.put("Hello2", 3);
    StringIntMap.put("Hello3", 4);
    StringIntMap.put("Hello4", 5);

    IntMap.put(1, 1);
    IntMap.put(2, 2);
    IntMap.put(3, 3);
    IntMap.put(4, 4);
    IntMap.put(5, 5);

    IntStringMap.put(1, "Hello");
    IntStringMap.put(2, "Hello1");
    IntStringMap.put(3, "Hello2");
    IntStringMap.put(4, "Hello3");
    IntStringMap.put(5, "Hello4");

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