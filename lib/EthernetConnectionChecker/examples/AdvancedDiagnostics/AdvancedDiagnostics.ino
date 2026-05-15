#include <Arduino.h>
#include <EthernetConnectionChecker.h>

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void printResultsToSerial() {
    const ArrayList<ECCProbeResult>& results = checker.getResults();

    Serial.println("---- Probe Results ----");
    for (size_t i = 0; i < results.size(); ++i) {
        const ECCProbeResult& r = results.get(i);

        Serial.print(r.endpointName);
        Serial.print(" status=");
        Serial.print(r.statusCode);
        Serial.print(" accepted=");
        Serial.print(r.acceptedStatus ? "yes" : "no");
        Serial.print(" range=");
        Serial.print(r.minAcceptedStatus);
        Serial.print("-");
        Serial.print(r.maxAcceptedStatus);
        Serial.print(" latency=");
        Serial.print(r.latencyMs);
        Serial.print("ms notes=");
        Serial.println(r.notes.getSingle());

        if (r.redirectLocation.hasValue()) {
            Serial.print("  redirect to: ");
            Serial.println(r.redirectLocation.getValue());
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(300);

    bool ok = checker.begin(mac);
    if (!ok) {
        Serial.println("Ethernet begin failed.");
        return;
    }

    if (!checker.isSdReady()) {
        Serial.println("SD not ready on first try, retrying...");
        checker.reinitSd(4);
    }

    // Router/gateway probe validates local network path.
    checker.addGatewayEndpoint("Gateway", "/", 80, 100, 599);

    // IANA commonly redirects, so 3xx should be accepted.
    checker.addEndpoint("IANA", "iana.org", "/", 80, 200, 399);

    // Reachability check treats any HTTP response as network success, including 403.
    checker.addReachabilityEndpoint("Arduino Reachability", "www.arduino.cc", "/", 80);

    // Strict endpoint requires 2xx only.
    checker.addEndpoint("Example Strict", "example.com", "/", 80, 200, 299);

    checker.setCheckInterval(90000UL);
    checker.enableAutoChecks(true);

    checker.runChecks();
    printResultsToSerial();

    Serial.print("Dashboard: http://");
    Serial.println(checker.localIP());
    Serial.println("Try: /api/diag and /api/results");
}

void loop() {
    checker.loop();
}
