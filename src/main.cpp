#include <Arduino.h>
#include <ArrayList.h>

ArrayList<String> list;
unsigned long lastMillis = 0;
void setup(){
    Serial.begin(9600);
    while(!Serial);
    delay(5000);
    list.add("1234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 0: " + list.get(0));
    delay(1000);

    list.add("2234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 1: " + list.get(1));
    delay(1000);

    list.add("3234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 2: " + list.get(2));
    delay(1000);

    list.add("4234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 3: " + list.get(3));
    delay(1000);

    list.add("5234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 4: " + list.get(4));
    delay(1000);

    list.add("6234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 5: " + list.get(5));
    delay(1000);

    list.add("7234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 6: " + list.get(6));
    delay(1000);

    list.add("8234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 7: " + list.get(7));
    delay(1000);

    list.add("9234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 8: " + list.get(8));
    delay(1000);

    list.add("A234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 9: " + list.get(9));
    delay(1000);

    list.add("B234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 10: " + list.get(10));
    delay(1000);

    list.add("C234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 11: " + list.get(11));
    delay(1000);

    list.add("D234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 12: " + list.get(12));
    delay(1000);

    list.add("E234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 13: " + list.get(13));
    delay(1000);

    list.add("F234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 14: " + list.get(14));
    delay(1000);

    list.add("G234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 15: " + list.get(15));
    delay(1000);
    
    list.add("H234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 16: " + list.get(16));
    delay(1000);

    list.add("I234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 17: " + list.get(17));
    delay(1000);

    list.add("J234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 18: " + list.get(18));
    delay(1000);

    list.add("K234567890ABCDEF12345678");
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
    Serial.println("Element at index 19: " + list.get(19));
    delay(1000);

    Serial.println("Removing Now Printing");
    for(int i = 0; i < list.size(); i++){
        Serial.println(list.get(i));
    }
    Serial.println("Size: " + String(list.size()));
    Serial.println("Capacity: " + String(list.capacity()));
}

void loop(){
    
}