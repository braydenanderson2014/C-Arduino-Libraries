



#include <Arduino.h>
//#include "OrderedMap.h"
//#include "SDList.h"
#include "JSON.h"
//#include "DynamicStorageLibrary.h"
#include <LZ4.h>
#include "Optional.h"

//#include "MemoryManager.h"
String jsonStr = "{\"hello\":\"world\"}";  // Example JSON string
//uint8_t compressed[128];  // Buffer for compressed data
//uint8_t decompressed[128];

void setup() {
    Serial.begin(115200);
    while (!Serial) {}  // Ensure Serial is ready
    JSON json;

    const char* jsonKey = "Number";
    //const char* jsonStr = "{\"hello\":\"world\"}";
    //json.setString(jsonKey, jsonStr);
    json.setNumber(jsonKey, 42);


    String jsonOutput = json.writeToString();
    Serial.print("Serialized JSON: ");
    Serial.println(jsonOutput);

    bool success = json.writeToFile("test.json");
    Serial.println(success ? "Yes" : "No");

    
}

void loop() {
    // put your main code here, to run repeatedly:
}

/*
#define MEMORY_POOL_SIZE 2048  // Preallocate 2KB
char memoryPool[MEMORY_POOL_SIZE];

extern unsigned int __bss_end;
extern void *__brkval;

int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    } else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }
    return free_memory;
}



*/
//DynamicStorage<String, String> storage(DynamicStorage<String, String>::AUTO);

