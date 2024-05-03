#include <Arduino.h>
// #include <CustomString.h>
// #include <MemoryManager.h>
// #include <MathLib.h>
// #include <Numeric_Limits.h>
// #include <LittleFSProperties.h>
#include "SimpleArduinoTimer.h"

Timer timer = new Timer(false);
// SDList <String> strings(10, "pagefile.dat");
// ArrayList <String> list = ArrayList<String>(ArrayList<String>::DYNAMIC, 10);
void setup()
{
    /*
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

    */
    Serial.begin(9600);
    while (!Serial)
        ;
    timer.setTargetMinutes(5); // Set the target duration to 5 minutes
    unsigned long x = 1000;

    
    timer.start(); // Start the timer
    while(!timer.hasReachedTarget()){
        if(timer.checkTimer(x)){ // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
            Serial.println(String(timer.remainingTime())); //1 side effect of remainingTime() is that it will auto adjust based off of the time remaining. all without you knowing.
            x += 10000;
        }
    }
    Serial.println("Timer has reached target duration!");
    timer.reset(); // Reset the timer
    x = 1000; //sets the variable x back to 1 second (THIS IS NOT NECESSARY, JUST FOR DEMONSTRATION PURPOSES)


    
    timer.start();
    while (!timer.hasReachedTarget())
    {
        if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
        {
            timer.printTimeRemaining(); // Prints the remaining time on the timer
            x += 1000;
        }
    }

    timer.setTargetSeconds(10); // Set the target duration to 10 seconds
    timer.start(); // Start the timer
    while (!timer.hasReachedTarget())
    {
        if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
        {
            timer.printTimeRemaining(); // Prints the remaining time on the timer
            x += 1000;
        }
    }
    Serial.println("Timer has reached target duration!");

    timer.setTargetHours(2); // Set the target duration to 1 hour
    timer.start(); // Start the timer
    while (!timer.hasReachedTarget())
    {
        if (timer.checkTimer(x)) // checks if a second has passed (This is a non blocking way of only printing the timer every second.)
        {
            timer.printTimeRemaining(); // Prints the remaining time on the timer
            x += 1000;
        }
    }
    Serial.println("Timer has reached target duration!");


    //You can also call functions like 
    //timer.pause(); // Pauses the timer
    //timer.resume(); // Resumes the timer
    //timer.stop(); // Stops the timer
    //timer.clear(); // Clears the timer

    


}

void loop()
{
}
