#include <Arduino.h>

#include "SDList.h"
#include "ArrayList.h"


SDList <String> list(SDCARD, 8);

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){}
  list.begin(4);
  list.append("Gracen is crazy");

  Serial.println(list.size());
  Serial.println(list.get(0));
  //list.clear();
}

void loop(){
  
}
