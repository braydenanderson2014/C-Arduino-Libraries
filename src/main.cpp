#include <Arduino.h>
//#include "OrderedMap.h"
//#include "SDList.h"
#include "JSON.h"
//#include "DynamicStorageLibrary.h"
#include "MemoryManager.h"


#define MEMORY_POOL_SIZE 2048  // Preallocate 2KB
char memoryPool[MEMORY_POOL_SIZE];

extern char *__brkval;
extern char __bss_end;
int freeMemory() {
    return (int) &__bss_end - (int) __brkval;
}

void setup() {
    Serial.begin(115200);
    while(!Serial){}
    MemoryManager mm;
    Serial.println(freeMemory());
    void* ptr1 = mm.malloc(MEMORY_POOL_SIZE, __FILE__, __LINE__);
    Serial.println(freeMemory());
}



void loop(){

}
