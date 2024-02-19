/*
    Example Name: Example For SDList Library

    Basic example for the SDList library. This example demonstrates the basic functionality of the SDList library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 02/18/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 02/18/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/
#include <Arduino.h>

#include <SDList.h>


SDList <String> list(SDCARD, 8);
SDList <String> sdList(SDCARD, 8);
SDList <int> intList(MEMORY, 8);

//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
  Serial.begin(9600);
  while(!Serial){} // Wait for Serial to be ready
  list.begin(4); // This is the CS pin (THIS FUNCTION IS REQUIRED TO RUN in SD MODE but not Required for Memory Mode!!!)
  sdList.begin(4, "list.txt"); // This is the CS pin (THIS FUNCTION IS REQUIRED TO RUN in SD MODE but not Required for Memory Mode!!!) you can also specify the file name
  
  list.append("This Library is crazy"); //Add an element to the list with value "This Library is crazy"
  sdList.append("Like really crazy"); //Add an element to the list with value "Like really crazy"
  for(int i = 0; i < 10; i++){
    intList.append(i); //Add an element to the list with value i
  }

  Serial.println(list.get(0)); //Print the element at index 0
  Serial.println(sdList.get(0));  //Print the element at index 0
  Serial.println(intList.get(5));  //Print the element at index 0

  Serial.println(String(list.size())); //Print the size of the list
  Serial.println(String(sdList.size())); //Print the size of the list
  Serial.println(String(intList.size())); //Print the size of the list

  list.remove(0); //Remove the element at index 0
  sdList.remove(0); //Remove the element at index 0

  intList.clear(); //Clear the list
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
