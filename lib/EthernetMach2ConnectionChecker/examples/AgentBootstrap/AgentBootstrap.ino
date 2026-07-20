#include <Arduino.h>
#include <EthernetMach2ConnectionChecker.h>

byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
EM2ServerTarget server("192.168.1.20", 8080, "/api/v1");
EthernetMach2ConnectionChecker agent(10);

void setup() {
    Serial.begin(115200);
    delay(300);

    if (!agent.begin(mac, server)) {
        Serial.println("Agent start failed.");
        return;
    }

    agent.addProbe("Google Connectivity", "connectivitycheck.gstatic.com",
        "/generate_204", 80, 204, 204);
    agent.addProbe("Gateway", "192.168.1.1", "/", 80, 100, 599);

    agent.setHeartbeatInterval(30000UL);
    agent.setCheckInterval(60000UL);
    agent.enableAutoChecks(true);

    agent.registerAgent();
    agent.sendHeartbeat();
    agent.runChecks();
    agent.sendResults();

    Serial.print("Agent IP: ");
    Serial.println(agent.localIP());
    Serial.println("Mach2 agent ready.");
}

void loop() {
    agent.loop();
}
