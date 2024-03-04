/*
    Example Name: Example For Suffix Tree Library

    Basic example for the Suffix Tree library. This example demonstrates the basic functionality of the Suffix Tree library.

    The circuit:
    This Library does not Require any Circuits to run.

    Created  month day year   // 03/01/2024
    By author's name braydenanderson2014 (Brayden Anderson)
    Modified day month year  // 03/01/2024
    By author's name  braydenanderson2014 (Brayden Anderson)

    *RECOMMENDED TO USE A TERMINAL SYMBOL IN THE STRING*
    *RECOMMENDED TO USE A BOARD WITH ADEQUATE MEMORY* (e.g. Arduino Mega, ESP32, etc.)

    https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/Release/README.md

*/

#include <Arduino.h>
#include <SuffixTree.h>


void setup() {
    Serial.begin(9600);
    // Ensure there's enough delay for the serial monitor to initialize
    delay(2000); 

    String text = "banana$"; // Example string with terminal symbol '$'
    SuffixTree tree(text);

    // Function to find and print the longest repeated substring
    tree.getLongestRepeatedSubstring();

    // Function to find and print the longest common substring
    tree.getLongestCommonSubstring("banana", "anana");

    // Function to find and print the longest palindrome
    Serial.println(tree.longestPalindrome());

    // Function to find and print all the palindromes
    SimpleVector<String> palindromes = tree.getAllPalindromes(); //getAllPalindromes returns a vector of all the palindromes
    for (int i = 0; i < palindromes.size(); i++) {
        Serial.println(palindromes[i]); // Print all the palindromes... prints the palindromes in the vector at index i
    }

}

void loop() {
    // Nothing to do here
}