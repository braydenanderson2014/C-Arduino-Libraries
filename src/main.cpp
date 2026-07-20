
#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetMach2ConnectionChecker.h>
#include <ctype.h>
#include <string.h>

namespace {
// Update these values for your LAN before uploading to the Mega.
const byte MAC_ADDRESS[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
const char* SERVER_HOST = "192.168.1.187";
const uint16_t SERVER_PORT = 8080;
const char* SERVER_BASE_PATH = "/api/v1";
const char* SERVER_HEALTH_PATH = "/health";
const char* GATEWAY_HOST = "192.168.1.1";

const unsigned long CHECK_INTERVAL_MS = 60000UL;
const unsigned long HEARTBEAT_INTERVAL_MS = 30000UL;
const unsigned long CONTROL_POLL_INTERVAL_MS = 10000UL;
const unsigned long STATUS_PRINT_INTERVAL_MS = 30000UL;
const unsigned long RETRY_INTERVAL_MS = 10000UL;
const uint16_t DISCOVERY_RETRY_TIMEOUT_MS = 250;
const uint8_t DISCOVERY_RETRY_COUNT = 1;
const uint16_t NORMAL_RETRY_TIMEOUT_MS = 1000;
const uint8_t NORMAL_RETRY_COUNT = 3;
const uint8_t DEFAULT_SCAN_START_OCTET = 1;
const uint8_t DEFAULT_SCAN_END_OCTET = 30;
const uint16_t DEFAULT_SCAN_PORT = 80;

EthernetMach2ConnectionChecker agent(10);
EM2ServerTarget serverTarget(SERVER_HOST, SERVER_PORT, SERVER_BASE_PATH);

bool agentReady = false;
unsigned long lastStatusPrintAt = 0;
unsigned long lastRetryAt = 0;
char commandBuffer[64];
size_t commandLength = 0;

void printHelp() {
    Serial.println(F("[EM2] Commands: HELP STATUS RUN HEARTBEAT REGISTER CONFIG LOG DISCOVER SPEED SCAN TRACE SNAPSHOT STATS POLL DEVICES"));
}

void printLastError() {
    const String& lastError = agent.getLastError();
    if (lastError.length() == 0) {
        return;
    }

    Serial.print(F("[EM2] Last error: "));
    Serial.println(lastError);
}

void configureProbes() {
    agent.clearProbes();
    agent.addProbe("Mach2 Server", serverTarget.host, SERVER_HEALTH_PATH, SERVER_PORT, 200, 299);
    agent.addProbe("Gateway", GATEWAY_HOST, "/", 80, 100, 599);
    agent.addProbe("Google Connectivity", "connectivitycheck.gstatic.com", "/generate_204", 80, 204, 204);
}

String ipToString(const IPAddress& ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

String readHttpLine(EthernetClient& client, unsigned long timeoutMs) {
    String line = "";
    unsigned long startedAt = millis();

    while ((millis() - startedAt) < timeoutMs) {
        while (client.available()) {
            char c = (char)client.read();
            if (c == '\r') {
                continue;
            }
            if (c == '\n') {
                return line;
            }
            line += c;
        }
    }

    return line;
}

void adoptServerHost(const String& host) {
    serverTarget = EM2ServerTarget(host, SERVER_PORT, SERVER_BASE_PATH);
    agent.setServerTarget(serverTarget);
    configureProbes();
}

bool probeMach2ServerAtIp(const IPAddress& ip) {
    EthernetClient client;
    if (!client.connect(ip, SERVER_PORT)) {
        return false;
    }

    String host = ipToString(ip);
    client.print(String("GET ") + SERVER_HEALTH_PATH + " HTTP/1.1\r\n");
    client.print(String("Host: ") + host + "\r\n");
    client.print("Connection: close\r\n\r\n");

    String statusLine = readHttpLine(client, 900UL);
    if (statusLine.indexOf("200") < 0) {
        client.stop();
        return false;
    }

    while (client.connected()) {
        String headerLine = readHttpLine(client, 300UL);
        if (headerLine.length() == 0) {
            break;
        }
    }

    String body = "";
    unsigned long startedAt = millis();
    while ((millis() - startedAt) < 500UL && (client.connected() || client.available())) {
        while (client.available() && body.length() < 160) {
            body += (char)client.read();
        }
    }

    client.stop();
    return body.indexOf("\"status\":\"ok\"") >= 0;
}

bool scanRangeForServer(uint8_t start, uint8_t end, uint8_t ownHostOctet) {
    IPAddress localIp = agent.localIP();
    for (uint16_t hostOctet = start; hostOctet <= end; ++hostOctet) {
        if (hostOctet == ownHostOctet || hostOctet == 0 || hostOctet == 255) {
            continue;
        }

        IPAddress candidate(localIp[0], localIp[1], localIp[2], (uint8_t)hostOctet);
        if (probeMach2ServerAtIp(candidate)) {
            String discoveredHost = ipToString(candidate);
            Serial.print(F("[EM2] Discovered Mach2 server at "));
            Serial.println(discoveredHost);
            adoptServerHost(discoveredHost);
            return true;
        }
    }

    return false;
}

bool discoverServerOnSubnet() {
    IPAddress localIp = agent.localIP();
    if (localIp[0] == 0 && localIp[1] == 0 && localIp[2] == 0 && localIp[3] == 0) {
        Serial.println(F("[EM2] Cannot scan before Ethernet is ready."));
        return false;
    }

    Serial.print(F("[EM2] Scanning subnet for Mach2 server from "));
    Serial.print(localIp[0]);
    Serial.print(F("."));
    Serial.print(localIp[1]);
    Serial.print(F("."));
    Serial.print(localIp[2]);
    Serial.println(F(".x"));

    Ethernet.setRetransmissionTimeout(DISCOVERY_RETRY_TIMEOUT_MS);
    Ethernet.setRetransmissionCount(DISCOVERY_RETRY_COUNT);

    uint8_t ownHostOctet = localIp[3];
    bool found = scanRangeForServer(100, 199, ownHostOctet)
        || scanRangeForServer(2, 99, ownHostOctet)
        || scanRangeForServer(200, 254, ownHostOctet);

    Ethernet.setRetransmissionTimeout(NORMAL_RETRY_TIMEOUT_MS);
    Ethernet.setRetransmissionCount(NORMAL_RETRY_COUNT);

    if (!found) {
        Serial.println(F("[EM2] No Mach2 server signature found on the local subnet."));
    }
    return found;
}

void printResultsSummary() {
    const ArrayList<EM2ProbeResult>& results = agent.getResults();
    size_t acceptedCount = 0;
    size_t failedCount = 0;

    for (size_t i = 0; i < results.size(); ++i) {
        const EM2ProbeResult& result = results.getReference(i);
        if (result.accepted) {
            ++acceptedCount;
        } else {
            ++failedCount;
        }
    }

    Serial.print(F("[EM2] Results: "));
    Serial.print(results.size());
    Serial.print(F(" total, "));
    Serial.print(acceptedCount);
    Serial.print(F(" accepted, "));
    Serial.print(failedCount);
    Serial.println(F(" failed."));

    for (size_t i = 0; i < results.size(); ++i) {
        const EM2ProbeResult& result = results.getReference(i);
        Serial.print(F("  - "));
        Serial.print(result.name);
        Serial.print(F(": status="));
        Serial.print(result.statusCode);
        Serial.print(F(", latency="));
        Serial.print(result.latencyMs);
        Serial.print(F("ms, accepted="));
        Serial.println(result.accepted ? F("yes") : F("no"));
    }
}

void printDiscoveredDevices() {
    const ArrayList<EM2NetworkDevice>& devices = agent.getDiscoveredDevices();
    Serial.print(F("[EM2] Discovered devices: "));
    Serial.println(devices.size());

    for (size_t i = 0; i < devices.size(); ++i) {
        const EM2NetworkDevice& device = devices.getReference(i);
        Serial.print(F("  - "));
        Serial.print(device.host);
        Serial.print(F(":"));
        Serial.print(device.port);
        Serial.print(device.isUp ? F(" UP ") : F(" DOWN "));
        Serial.print(device.latencyMs);
        Serial.println(F("ms"));
    }
}

void printPathTrace() {
    const ArrayList<EM2TraceHop>& pathTrace = agent.getPathTrace();
    Serial.print(F("[EM2] Path trace hops: "));
    Serial.println(pathTrace.size());

    for (size_t i = 0; i < pathTrace.size(); ++i) {
        const EM2TraceHop& hop = pathTrace.getReference(i);
        Serial.print(F("  - "));
        Serial.print(hop.label);
        Serial.print(F(" -> "));
        Serial.print(hop.target);
        Serial.print(F(":"));
        Serial.print(hop.port);
        Serial.print(F(" status="));
        Serial.print(hop.statusCode);
        Serial.print(F(" latency="));
        Serial.print(hop.latencyMs);
        Serial.print(F("ms note="));
        Serial.println(hop.note);
    }
}

void printEventLog() {
    const ArrayList<String>& eventLog = agent.getEventLog();
    Serial.print(F("[EM2] Event log entries: "));
    Serial.println(eventLog.size());

    for (size_t i = 0; i < eventLog.size(); ++i) {
        Serial.print(F("  "));
        Serial.println(eventLog.getReference(i));
    }
}

void printStatus() {
    Serial.print(F("[EM2] Agent ready: "));
    Serial.println(agentReady ? F("yes") : F("no"));
    Serial.print(F("[EM2] Agent ID: "));
    Serial.println(agent.getAgentId());
    Serial.print(F("[EM2] Server: "));
    Serial.print(serverTarget.host);
    Serial.print(F(":"));
    Serial.println(serverTarget.port);
    Serial.print(F("[EM2] Local IP: "));
    Serial.println(agent.localIP());
    Serial.print(F("[EM2] Last response length: "));
    Serial.println(agent.getLastServerResponse().length());
    printLastError();
    printResultsSummary();
}

bool runManualCheck() {
    bool checksAccepted = agent.runChecks();
    bool resultsSent = agent.sendResults();
    bool snapshotSent = agent.sendNetworkSnapshot();

    Serial.println(checksAccepted
        ? F("[EM2] Checks finished with all probes accepted.")
        : F("[EM2] Checks finished with one or more failed probes."));
    Serial.println(resultsSent
        ? F("[EM2] Result batch uploaded.")
        : F("[EM2] Result batch upload failed."));
    Serial.println(snapshotSent
        ? F("[EM2] Snapshot uploaded.")
        : F("[EM2] Snapshot upload failed."));
    printLastError();
    printResultsSummary();

    return checksAccepted && resultsSent && snapshotSent;
}

void runSpeedTestAndUpload() {
    Serial.println(F("[EM2] Running speed test..."));
    EM2SpeedResult speedResult = agent.runSpeedTest();
    Serial.println(speedResult.success
        ? F("[EM2] Speed test completed.")
        : F("[EM2] Speed test failed."));
    Serial.print(F("[EM2] Download B/s: "));
    Serial.println(speedResult.downloadBps);
    Serial.println(agent.sendNetworkSnapshot()
        ? F("[EM2] Snapshot uploaded.")
        : F("[EM2] Snapshot upload failed."));
    printLastError();
}

void runScanAndUpload() {
    Serial.println(F("[EM2] Scanning subnet..."));
    agent.scanSubnet(DEFAULT_SCAN_START_OCTET, DEFAULT_SCAN_END_OCTET, DEFAULT_SCAN_PORT);
    printDiscoveredDevices();
    Serial.println(agent.sendNetworkSnapshot()
        ? F("[EM2] Snapshot uploaded.")
        : F("[EM2] Snapshot upload failed."));
    printLastError();
}

void runTraceAndUpload() {
    Serial.println(F("[EM2] Running path trace..."));
    agent.runPathTrace();
    printPathTrace();
    Serial.println(agent.sendNetworkSnapshot()
        ? F("[EM2] Snapshot uploaded.")
        : F("[EM2] Snapshot upload failed."));
    printLastError();
}

bool initializeAgent() {
    Serial.println(F("[EM2] Starting EthernetMach2ConnectionChecker..."));

    if (!agent.begin(MAC_ADDRESS, serverTarget)) {
        Serial.println(F("[EM2] DHCP or Ethernet startup failed."));
        printLastError();
        return false;
    }

    configureProbes();
    agent.setHeartbeatInterval(HEARTBEAT_INTERVAL_MS);
    agent.setCheckInterval(CHECK_INTERVAL_MS);
    agent.setControlPollInterval(CONTROL_POLL_INTERVAL_MS);
    agent.enableAutoChecks(true);

    Serial.print(F("[EM2] Agent IP: "));
    Serial.println(agent.localIP());

    bool registered = agent.registerAgent();
    if (!registered) {
        Serial.println(F("[EM2] Agent registration failed."));
        printLastError();
        if (discoverServerOnSubnet()) {
            registered = agent.registerAgent();
        }
    }
    Serial.println(registered
        ? F("[EM2] Agent registered with the server.")
        : F("[EM2] Agent registration failed."));
    printLastError();

    bool heartbeatSent = agent.sendHeartbeat();
    Serial.println(heartbeatSent
        ? F("[EM2] Heartbeat sent.")
        : F("[EM2] Heartbeat send failed."));
    printLastError();

    runManualCheck();

    Serial.println(F("[EM2] Mach2 agent ready. Type HELP for commands."));
    return true;
}

void fetchAndPrintConfig() {
    if (!agent.fetchConfig()) {
        Serial.println(F("[EM2] Config fetch failed."));
        printLastError();
        return;
    }

    Serial.println(F("[EM2] Raw config response:"));
    Serial.println(agent.getLastServerResponse());
}

void processCommand(char* command) {
    if (command[0] == '\0') {
        return;
    }

    for (size_t i = 0; command[i] != '\0'; ++i) {
        command[i] = (char)toupper((unsigned char)command[i]);
    }

    if (strcmp(command, "HELP") == 0) {
        printHelp();
        return;
    }

    if (!agentReady) {
        Serial.println(F("[EM2] Agent is not ready yet. Waiting for Ethernet startup."));
        return;
    }

    if (strcmp(command, "STATUS") == 0) {
        printStatus();
    } else if (strcmp(command, "RUN") == 0) {
        runManualCheck();
    } else if (strcmp(command, "SPEED") == 0) {
        runSpeedTestAndUpload();
    } else if (strcmp(command, "SCAN") == 0) {
        runScanAndUpload();
    } else if (strcmp(command, "TRACE") == 0) {
        runTraceAndUpload();
    } else if (strcmp(command, "SNAPSHOT") == 0) {
        Serial.println(agent.sendNetworkSnapshot()
            ? F("[EM2] Snapshot uploaded.")
            : F("[EM2] Snapshot upload failed."));
        printLastError();
    } else if (strcmp(command, "STATS") == 0) {
        Serial.println(agent.diagnosticsJson());
    } else if (strcmp(command, "POLL") == 0) {
        bool hadCommand = agent.pollServerControl();
        Serial.println(hadCommand
            ? F("[EM2] Control poll processed a command.")
            : F("[EM2] Control poll found no command."));
        printLastError();
    } else if (strcmp(command, "DEVICES") == 0) {
        printDiscoveredDevices();
    } else if (strcmp(command, "DISCOVER") == 0) {
        discoverServerOnSubnet();
    } else if (strcmp(command, "HEARTBEAT") == 0) {
        Serial.println(agent.sendHeartbeat()
            ? F("[EM2] Heartbeat sent.")
            : F("[EM2] Heartbeat send failed."));
        printLastError();
    } else if (strcmp(command, "REGISTER") == 0) {
        Serial.println(agent.registerAgent()
            ? F("[EM2] Agent registered.")
            : F("[EM2] Agent registration failed."));
        printLastError();
    } else if (strcmp(command, "CONFIG") == 0) {
        fetchAndPrintConfig();
    } else if (strcmp(command, "LOG") == 0) {
        printEventLog();
    } else {
        Serial.println(F("[EM2] Unknown command."));
        printHelp();
    }
}

void pollSerial() {
    while (Serial.available() > 0) {
        char incoming = (char)Serial.read();
        if (incoming == '\r') {
            continue;
        }

        if (incoming == '\n') {
            commandBuffer[commandLength] = '\0';
            processCommand(commandBuffer);
            commandLength = 0;
            continue;
        }

        if (commandLength < (sizeof(commandBuffer) - 1)) {
            commandBuffer[commandLength++] = incoming;
        }
    }
}

void maybeRetryStartup() {
    if (agentReady) {
        return;
    }

    unsigned long now = millis();
    if (lastRetryAt != 0 && (now - lastRetryAt) < RETRY_INTERVAL_MS) {
        return;
    }

    lastRetryAt = now;
    agentReady = initializeAgent();
}

void maybePrintStatus() {
    if (!agentReady) {
        return;
    }

    unsigned long now = millis();
    if (lastStatusPrintAt != 0 && (now - lastStatusPrintAt) < STATUS_PRINT_INTERVAL_MS) {
        return;
    }

    lastStatusPrintAt = now;
    Serial.print(F("[EM2] Online at "));
    Serial.print(agent.localIP());
    Serial.print(F(" -> server "));
    Serial.print(serverTarget.host);
    Serial.print(F(":"));
    Serial.println(serverTarget.port);
}
}

void setup() {
    Serial.begin(115200);
    delay(300);

    Serial.println(F("[EM2] Booting Mach2 agent sketch..."));
    printHelp();

    agentReady = initializeAgent();
    if (!agentReady) {
        lastRetryAt = millis();
        Serial.println(F("[EM2] Startup failed. Automatic retries are enabled."));
    }
}

void loop() {
    pollSerial();

    if (!agentReady) {
        maybeRetryStartup();
        return;
    }

    agent.loop();
    maybePrintStatus();
}

