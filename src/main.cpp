
#include <Arduino.h>
#include <EthernetConnectionChecker.h>

byte eccMac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void setup() {
    Serial.begin(115200);
    delay(200);

    bool ok = checker.begin(eccMac);
    if (!ok) {
        Serial.println("Ethernet init failed. Check LED flash codes.");
        return;
    }

    checker.addEndpoint("Example", "example.com", "/", 80, 200, 399);
    checker.addEndpoint("IANA", "iana.org", "/", 80, 200, 399);
    checker.addEndpoint("Arduino", "www.arduino.cc", "/", 80, 200, 399);

    checker.setCheckInterval(120000UL);
    checker.enableAutoChecks(true);
    checker.runChecks();

    Serial.print("Dashboard: http://");
    Serial.println(checker.localIP());
}

void loop() {
    checker.loop();
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

