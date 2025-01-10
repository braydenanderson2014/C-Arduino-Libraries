#include "Hashtable.h"
Hashtable<String, String> table;


void debugTable();
void setup() {
    Serial.begin(9600); // Start Serial Connection

    //ADD ELEMENTS TO THE TABLE
    table.put("key1", "value1");
    table.put("key2", "value2");
    table.put("key3", "value3");
    table.put("key4", "value4");
    table.put("key5", "value5");
    table.put("key6", "value6");
    table.put("key7", "value7");
    table.put("key8", "value8");
    table.put("key9", "value9");
    table.put("key10", "value10");
    table.put("key11", "value11");
    table.put("key12", "value12");
    table.put("key13", "value13");
    table.put("key14", "value14");
    table.put("key15", "value15");
    table.put("key16", "value16");
    table.put("key17", "value17");
    table.put("key18", "value18");
    table.put("key19", "value19");
    table.put("key20", "value20");
    

    Serial.println("Size: " + String(table.size()));
    Serial.println("Elements: " + String(table.elements()));
    Serial.println("Is Empty: " + String(table.isEmpty()));

    /*
    for (auto it = table.begin(); it != table.end(); ++it) {
        auto kv = *it;
        Serial.print("Iterator output: Key: ");
        Serial.print(kv.key);
        Serial.print(", Value: ");
        Serial.println(kv.value);
    }
    */
    //debugTable();
    Serial.println("Iterator complete");




    String* value = table.get("key1");
    if (value != nullptr) {
        Serial.println("Value of key1: " + *value);
    } else {
        Serial.println("Key1 not found");
    }

    String value2 = table.getElement("key2");
    Serial.println("Value of key2: " + value2);

}

void debugTable() {
    for (int i = 0; i < table.bucketCount(); ++i) {
        Serial.print("Bucket ");
        Serial.print(i);
        Serial.print(": ");
        auto entry = table.getBucket(i); // Use a method to get the bucket by index
        while (entry != nullptr) {
            Serial.print(entry->key + " -> " + entry->value + ", ");
            entry = entry->next;
        }
        Serial.println();
    }
}



void loop(){

}
