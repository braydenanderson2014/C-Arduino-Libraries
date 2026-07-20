#include <Arduino.h>
#include <EthernetConnectionChecker.h>

byte eccMac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void setup() {
    Serial.begin(115200);
    delay(1200);
    Serial.println(F("[ECC] Booting..."));

    bool ok = checker.begin(eccMac);
    if (!ok) {
        Serial.println(F("Ethernet init failed. Check LED flash codes."));
        return;
    }

    Serial.println(F("[ECC] Ethernet initialized."));

    // Keep the startup endpoint set small until the sketch is stable on-device.
    checker.addEndpoint("Google Connectivity", "connectivitycheck.gstatic.com",
        "/generate_204", 80, 204, 204);
    checker.addReachabilityEndpoint("Cloudflare", "1.1.1.1", "/", 80);
    checker.addGatewayEndpoint("Gateway", "/", 80, 100, 599);

    // Re-enable additional endpoints after startup stability is confirmed:
    // checker.addEndpoint("IANA", "iana.org", "/", 80, 200, 399);
    // checker.addEndpoint("Example", "example.com", "/", 80, 200, 399);
    // checker.addReachabilityEndpoint("Arduino CC", "www.arduino.cc", "/", 80);
    // checker.addEndpoint("Google", "google.com", "/", 80, 200, 399);

    checker.setCheckInterval(120000UL);
    checker.enableAutoChecks(true);

    Serial.print(F("Dashboard: http://"));
    Serial.println(checker.localIP());
    Serial.println(F("Type HELP in Serial Monitor for console commands."));
    Serial.println(F("Use RUN or /run to start the first check."));
}

void loop() {
    checker.loop();
}