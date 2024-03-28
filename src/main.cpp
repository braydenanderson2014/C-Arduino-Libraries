#include <Arduino.h>
#include <CustomString.h>
//#include <MemoryManager.h>
#include <MathLib.h>
#include <Numeric_Limits.h>
#include <LittleFSProperties.h> 



//SDList <String> strings(10, "pagefile.dat");
//ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup(){
    Custom_String::String str = "Hello, World!"; // Create a string object with the value "Hello, World!"
    Serial.begin(9600); // Start the serial communication
    Serial.println(str.C_STR()); // Hello, World!

    str.append(" This is a test!"); // Append the string " This is a test!" to the end of the string
    Serial.println(str.C_STR()); // Hello, World! This is a test!

    Serial.println(str.Capacity()); // Strings capacity

    int i = 0;
    Serial.println(str.indexOf('o', i) + i); // 4

    Custom_String::String str2 = "9999";
    Serial.println(Random(str.toInt(), 5));

    float f = 3.14159*str.toFloat(); // Converts String to float
    Serial.println(f); // 9.42477

    Serial.println(Round(f)*str.toInt()*str.toFloat()/2.25); // 18

}

void loop(){
  
}
