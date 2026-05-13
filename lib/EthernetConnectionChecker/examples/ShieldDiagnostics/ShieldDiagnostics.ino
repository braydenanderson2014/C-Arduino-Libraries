#include <Arduino.h>
#include <EthernetConnectionChecker.h>

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };

EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void setup() {
    Serial.begin(115200);
    delay(200);

    bool ok = checker.begin(mac);
    if (!ok) {
        Serial.println("Ethernet init failed. LED flash code shows failures.");
        return;
    }

    checker.addEndpoint("Google", "example.com", "/", 80, 200, 399);
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
