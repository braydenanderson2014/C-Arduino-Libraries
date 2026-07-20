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

    checker.addGatewayEndpoint("Router", "/", 80, 100, 599);
    checker.addEndpoint("IANA", "iana.org", "/", 80, 200, 399);
    checker.addReachabilityEndpoint("Arduino Reachability", "www.arduino.cc", "/", 80);
    checker.addEndpoint("Example", "example.com", "/", 80, 200, 399);

    checker.setCheckInterval(120000UL);
    checker.enableAutoChecks(true);

    checker.runChecks();

    Serial.print("Dashboard: http://");
    Serial.println(checker.localIP());
    Serial.println("API /api/diag and /api/results available on dashboard host");
}

void loop() {
    checker.loop();
}
