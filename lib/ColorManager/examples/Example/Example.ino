/*
    Example Name: Example For ColorManager Library

    Basic example for the ColorManager library. This example demonstrates the basic functionality of the ColorManager library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/



#include "ColorManager.h"

ColorManager colorManager; // Create a ColorManager object

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600); // Initialize the serial monitor
    Serial.println("Hello World"); // Prints "Hello World" to the serial monitor

    colorManager.addColor(Color(255, 0, 0)); // Red
    colorManager.addColor(Color(0, 255, 0)); // Green
    colorManager.addColor(Color(0, 0, 255)); // Blue

    Serial.println("Color 1: " + (String)colorManager.getColor(0).getR() + ", " + (String)colorManager.getColor(0).getG() + ", " + (String)colorManager.getColor(0).getB()); // Prints "Color 1: 255, 0, 0" to the serial monitor
    Serial.println("Color 2: " + (String)colorManager.getColor(1).getR() + ", " + (String)colorManager.getColor(1).getG() + ", " + (String)colorManager.getColor(1).getB()); // Prints "Color 2: 0, 255, 0" to the serial monitor
    Serial.println("Color 3: " + (String)colorManager.getColor(2).getR() + ", " + (String)colorManager.getColor(2).getG() + ", " + (String)colorManager.getColor(2).getB()); // Prints "Color 3: 0, 0, 255" to the serial monitor

    colorManager.removeColor(1); // Removes the color at index 1 (Green)
    colorManager.colorToHex(Color(colorManager.getColor(0).getR(), colorManager.getColor(0).getG(), colorManager.getColor(0).getB())); // Prints "FF0000" to the serial monitor
    colorManager.colorToHex(Color(255,255,255)); // Prints "FFFFFF" to the serial monitor

    colorManager.clear(); // Clears the color list



}



void loop() {
  // put your main code here, to run repeatedly:
}