#include <Arduino.h>
#include "Properties.h"
#include "MyDictionary.h"
#include "LinkedList.h"
#include "UnorderedMap.h"
#include "SDList.h"


Properties prop;
Hashtable<String, int> hashtable;
SimpleVector<String> keys = hashtable.keys();
// put function declarations here:
int myFunction(int, int);

void setup() {
  //prop.loadFromSD("test.txt");
  prop.setProperty("test", "test");
  prop.setProperty("test2", "test2");
  prop.setProperty("test3", "test3");
  //prop.saveToSD("test.txt");

  //prop.setProperty("item1", "test2", "test.txt");
  hashtable.put("apple", 5);
  hashtable.put("banana", 3);
  hashtable.put("cherry", 8);
  
  int* applePointer = hashtable.get("apple");
  int appleCount = applePointer ? *applePointer : 0; // checks if applePointer is not null before dereferencing

  int* bananaPointer = hashtable.get("banana");
  int bananaCount = bananaPointer ? *bananaPointer : 0; // checks if bananaPointer is not null before dereferencing

  int* cherryPointer = hashtable.get("cherry");
  int cherryCount = cherryPointer ? *cherryPointer : 0; // checks if cherryPointer is not null before dereferencing

  Serial.println(appleCount);
  Serial.println(bananaCount);
  Serial.println(cherryCount);

  Serial.println("Property key 1: " + prop.getProperty("test"));
  Serial.println("Property key 2: " + prop.getProperty("test2"));
  Serial.println("Property key 3: " + prop.getProperty("test3"));

  Properties::KeyIterator keyIterator = prop.keysIterator();
    while (keyIterator.hasNext()) {
        String key = keyIterator.next();
        Serial.println("Key: " + key);
    }

    // Iterate through values
    Properties::ValueIterator valueIterator = prop.valuesIterator();
    while (valueIterator.hasNext()) {
        String value = valueIterator.next();
        Serial.println("Value: " + value);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}