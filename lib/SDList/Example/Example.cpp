#include <Arduino.h>
#include "SDList.h"
//EXAMPLE WITH SD CARD
SDList<int> myList(4, "pagefile.dat");  // CS pin is 4, page file name is "pagefile.dat"

void setup() {
  Serial.begin(9600);

  // Check if SD card is available
  if (myList.sdAvailable()) {
    Serial.println("SD card is available!");

    myList.append(42);
    myList.append(100);
    myList.append(500);

    for (uint16_t i = 0; i < myList.size(); ++i) {
      Serial.println(myList.get(i));
    }
  } else {
    Serial.println("SD card not detected.");
  }
}

void loop() {
  // Nothing in the loop for this example.
}
