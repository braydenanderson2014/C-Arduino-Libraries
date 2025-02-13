#include <Arduino.h>
#include "Variant.h"
#include <ArrayList.h> // Ensure you have this or SimpleVector included 
#include <SimpleVector.h> 
//#define useSimpleVector //Uncomment this line to use the SimpleVector library instead of ArrayList (can be uncommented within the Variant class)

void setup() {
    Serial.begin(9600);
    
    // Example with a single integer value
    Variant<int> singleVariant(42);
    Serial.print("Single Variant Value: ");
    Serial.println(singleVariant.getSingle());
    Serial.print("Is Single? ");
    Serial.println(singleVariant.isSingle() ? "Yes" : "No");

    // Example with an ArrayList of integers
    ArrayList<int> numList;
    numList.add(10);
    numList.add(20);
    numList.add(30);

    Variant<int> listVariant(numList);
    Serial.print("\nList Variant Values: ");
    for (int i = 0; i < listVariant.size(); i++) {
        Serial.print(listVariant.getList().get(i));
        Serial.print(" ");
    }
    Serial.println();
    Serial.print("Is Single? ");
    Serial.println(listVariant.isSingle() ? "Yes" : "No");

    // Converting a single value to a list and adding values
    singleVariant.addValue(100);
    singleVariant.addValue(200);

    Serial.print("\nSingle Variant converted to List Variant Values: ");
    for (int i = 0; i < singleVariant.size(); i++) {
        Serial.print(singleVariant.getList().get(i));
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
    // Nothing needed here for this example
}


/* EXPECTED OUTPUT (According to chatgpt which generated this example. Blame it not me ;)  )
Single Variant Value: 42
Is Single? Yes

List Variant Values: 10 20 30
Is Single? No

Single Variant converted to List Variant Values: 42 100 200

*/