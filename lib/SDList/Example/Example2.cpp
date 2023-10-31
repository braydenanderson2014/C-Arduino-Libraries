#include <Arduino.h>
#include "SDList.h"

// Initialize the list without specifying SD card parameters. (NO SD CARD EXAMPLE)
// The list will operate in memory-only mode.
SDList<int> myList(4, "pagefile.dat"); 

void setup() {
  Serial.begin(9600);

  myList.append(10);
  myList.append(20);
  myList.append(30);

  for (uint16_t i = 0; i < myList.size(); ++i) {
    Serial.println(myList.get(i));
  }
}

void loop() {
  // Nothing in the loop for this example.
}
