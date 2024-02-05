#include <Arduino.h>

#include "SDList.h"
#include "ArrayList.h"


SDList <String> list(SDCARD, 8);

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){}
  list.begin(4); // This is the CS pin (THIS FUNCTION IS REQUIRED TO RUN in SD MODE but not Required for Memory Mode!!!)
  list.append("This Library is crazy");

  //Serial.println(list.size());
  Serial.println(list.get(0));
  //list.clear();
}

void loop(){
  
}


//--------------------------------------------------------------------------------

#include <Arduino.h>

#include "SDList.h"
#include "ArrayList.h"


SDList <String> list(MEMORY, 8);

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){}
  list.begin(4); // This is the CS pin (THIS FUNCTION IS REQUIRED TO RUN in SD MODE but not Required for Memory Mode!!!)
  list.append("This Library is crazy");

  //Serial.println(list.size());
  Serial.println(list.get(0));
  //list.clear();
}

void loop(){
  
}
