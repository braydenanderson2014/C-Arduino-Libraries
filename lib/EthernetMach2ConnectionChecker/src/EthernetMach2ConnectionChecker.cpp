#include "EthernetMach2ConnectionChecker.h"

static String em2IpToString(const IPAddress& ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

static String em2JsonEscape(const String& src) {
    String out;
    out.reserve(src.length() + 8);

    for (size_t i = 0; i < src.length(); ++i) {
        char c = src[i];
        if (c == '\\') {
            out += "\\\\";
        } else if (c == '"') {
            out += "\\\"";
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c == '\t') {
            out += "\\t";
        } else {
            out += c;
        }
    }

    return out;
}

static String em2MacToString(const byte mac[6]) {
    const char* hex = "0123456789ABCDEF";
    String out;
    out.reserve(12);

    for (uint8_t i = 0; i < 6; ++i) {
        out += hex[(mac[i] >> 4) & 0x0F];
        out += hex[mac[i] & 0x0F];
    }

    return out;
}

EthernetMach2ConnectionChecker::EthernetMach2ConnectionChecker(uint8_t ethernetCSPin, uint8_t sdCSPin)
        : _ethernetCSPin(ethernetCSPin),
            _sdCSPin(sdCSPin),
      _ethernetReady(false),
      _autoChecks(true),
      _checkIntervalMs(60000UL),
      _heartbeatIntervalMs(30000UL),
    _controlPollIntervalMs(10000UL),
      _lastCheckAt(0),
      _lastHeartbeatAt(0),
    _lastControlPollAt(0),
      _serverTarget(),
      _agentId(""),
      _lastServerResponse(""),
      _lastError(""),
      _probes(ArrayList<EM2Probe>::DYNAMIC2, 4),
      _results(ArrayList<EM2ProbeResult>::DYNAMIC2, 4),
    _devices(ArrayList<EM2NetworkDevice>::DYNAMIC2, 4),
    _pathTrace(ArrayList<EM2TraceHop>::DYNAMIC2, 4),
    _eventLog(ArrayList<String>::DYNAMIC2, 8),
    _lastSpeedResult() {
}

bool EthernetMach2ConnectionChecker::begin(
    const byte macAddress[6], const EM2ServerTarget& server, unsigned long dhcpTimeoutMs) {
    prepareNetworkHardware();
    int dhcpResult = Ethernet.begin(const_cast<byte*>(macAddress), dhcpTimeoutMs);

    _ethernetReady = (dhcpResult != 0);
    _serverTarget = server;
    if (_agentId.length() == 0) {
        _agentId = em2MacToString(macAddress);
    }

    if (!_ethernetReady) {
        _lastError = String("Ethernet DHCP failed");
        recordEvent(_lastError);
        return false;
    }

    _lastError = "";
    recordEvent(String("Ethernet ready @ ") + em2IpToString(Ethernet.localIP()));
    recordEvent(String("Server target ") + _serverTarget.host + ":" + _serverTarget.port);
    return true;
}

bool EthernetMach2ConnectionChecker::beginStatic(
    const byte macAddress[6], const IPAddress& ip, const IPAddress& dns,
    const IPAddress& gateway, const IPAddress& subnet, const EM2ServerTarget& server) {
    prepareNetworkHardware();
    Ethernet.begin(const_cast<byte*>(macAddress), ip, dns, gateway, subnet);
    _ethernetReady = true;
    _serverTarget = server;

    if (_agentId.length() == 0) {
        _agentId = em2MacToString(macAddress);
    }

    _lastError = "";
    recordEvent(String("Static IP mode @ ") + em2IpToString(Ethernet.localIP()));
    recordEvent(String("Server target ") + _serverTarget.host + ":" + _serverTarget.port);
    return true;
}

void EthernetMach2ConnectionChecker::setAgentId(const String& agentId) {
    _agentId = agentId;
}

const String& EthernetMach2ConnectionChecker::getAgentId() const {
    return _agentId;
}

void EthernetMach2ConnectionChecker::setServerTarget(const EM2ServerTarget& server) {
    _serverTarget = server;
}

const EM2ServerTarget& EthernetMach2ConnectionChecker::getServerTarget() const {
    return _serverTarget;
}

void EthernetMach2ConnectionChecker::setCheckInterval(unsigned long intervalMs) {
    _checkIntervalMs = intervalMs;
}

void EthernetMach2ConnectionChecker::setHeartbeatInterval(unsigned long intervalMs) {
    _heartbeatIntervalMs = intervalMs;
}

void EthernetMach2ConnectionChecker::setControlPollInterval(unsigned long intervalMs) {
    _controlPollIntervalMs = intervalMs;
}

void EthernetMach2ConnectionChecker::enableAutoChecks(bool enabled) {
    _autoChecks = enabled;
}

void EthernetMach2ConnectionChecker::addProbe(const String& name, const String& host,
    const String& path, uint16_t port, uint16_t minAcceptedStatus,
    uint16_t maxAcceptedStatus) {
    _probes.add(EM2Probe(name, host, path, port, minAcceptedStatus, maxAcceptedStatus));
}

void EthernetMach2ConnectionChecker::clearProbes() {
    _probes.clear();
}

EM2SpeedResult EthernetMach2ConnectionChecker::runSpeedTest(
    const String& host, const String& path, uint16_t port) {
    EM2SpeedResult result;
    result.host = host;
    result.path = path;
    result.port = port;

    if (!_ethernetReady) {
        _lastError = String("Ethernet not ready");
        _lastSpeedResult = result;
        return result;
    }

    EthernetClient client;
    unsigned long connectStart = millis();
    if (!client.connect(host.c_str(), port)) {
        _lastError = String("Speed test connect failed");
        recordEvent(String("Speed test connect failed: ") + host);
        _lastSpeedResult = result;
        return result;
    }

    result.connectMs = millis() - connectStart;
    client.print(String("GET ") + path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + host + "\r\n");
    client.print("Connection: close\r\n");
    client.print("User-Agent: EM2-Agent/0.2\r\n\r\n");

    while (client.connected()) {
        String line = readLine(client, 2000UL);
        if (line.length() == 0) {
            break;
        }
    }

    uint8_t buffer[64];
    unsigned long byteCount = 0;
    unsigned long startedAt = millis();
    unsigned long deadline = startedAt + 20000UL;
    while (millis() < deadline && (client.connected() || client.available())) {
        int count = client.read(buffer, sizeof(buffer));
        if (count > 0) {
            byteCount += (unsigned long)count;
        }
    }

    result.durationMs = millis() - startedAt;
    client.stop();

    result.success = (byteCount > 0);
    result.bytesReceived = byteCount;
    result.durationMs = result.durationMs > 0 ? result.durationMs : 1;
    result.downloadBps = (byteCount * 1000UL) / result.durationMs;
    _lastSpeedResult = result;

    if (result.success) {
        _lastError = "";
        recordEvent(String("Speed test ") + result.downloadBps + " B/s");
    } else {
        _lastError = String("Speed test yielded no bytes");
        recordEvent(_lastError);
    }

    return result;
}

void EthernetMach2ConnectionChecker::scanSubnet(uint8_t startOctet, uint8_t endOctet, uint16_t port) {
    if (!_ethernetReady) {
        _lastError = String("scanSubnet before Ethernet ready");
        recordEvent(_lastError);
        return;
    }

    IPAddress myIp = Ethernet.localIP();
    uint8_t effectiveEnd = endOctet >= startOctet ? endOctet : startOctet;

    Ethernet.setRetransmissionTimeout(250);
    Ethernet.setRetransmissionCount(1);
    recordEvent(String("Scan ") + startOctet + "-" + effectiveEnd + " port=" + port);

    for (uint16_t octet = startOctet; octet <= effectiveEnd; ++octet) {
        if (octet == myIp[3] || octet == 0 || octet == 255) {
            continue;
        }

        IPAddress target(myIp[0], myIp[1], myIp[2], (uint8_t)octet);
        EthernetClient client;
        unsigned long startedAt = millis();
        bool connected = client.connect(target, port);
        unsigned long latencyMs = millis() - startedAt;
        if (connected) {
            client.stop();
            updateOrAddDevice(em2IpToString(target), port, true, latencyMs, millis());
        }
    }

    Ethernet.setRetransmissionTimeout(1000);
    Ethernet.setRetransmissionCount(3);
    _lastError = "";
}

void EthernetMach2ConnectionChecker::runPathTrace(
    const String& internetHost, const String& internetPath, uint16_t port) {
    _pathTrace.clear();

    IPAddress gateway = gatewayIP();
    if (!(gateway[0] == 0 && gateway[1] == 0 && gateway[2] == 0 && gateway[3] == 0)) {
        addTraceHop("gateway", em2IpToString(gateway), "/", 80, 100, 599);
    }

    addTraceHop("server", _serverTarget.host, "/health", _serverTarget.port, 200, 299);
    addTraceHop("internet", internetHost, internetPath, port, 200, 399);
    _lastError = "";
}

bool EthernetMach2ConnectionChecker::registerAgent() {
    return postJson(buildPath("/register"), registrationJson());
}

bool EthernetMach2ConnectionChecker::runChecks() {
    if (!_ethernetReady) {
        _lastError = String("runChecks called before Ethernet ready");
        recordEvent(_lastError);
        return false;
    }

    _results.clear();
    bool allAccepted = true;

    for (size_t i = 0; i < _probes.size(); ++i) {
        EM2ProbeResult result;
        bool accepted = runSingleProbe(_probes.getReference(i), result);
        _results.add(result);
        if (!accepted) {
            allAccepted = false;
        }
    }

    _lastCheckAt = millis();
    return allAccepted;
}

bool EthernetMach2ConnectionChecker::sendHeartbeat() {
    bool ok = postJson(buildPath("/heartbeat"), heartbeatJson());
    if (ok) {
        _lastHeartbeatAt = millis();
    }
    return ok;
}

bool EthernetMach2ConnectionChecker::sendResults() {
    return postJson(buildPath("/results/batch"), resultsJson());
}

bool EthernetMach2ConnectionChecker::sendNetworkSnapshot() {
    return postJson(buildPath("/network/snapshot"), networkSnapshotJson());
}

bool EthernetMach2ConnectionChecker::fetchConfig() {
    String body;
    if (!_ethernetReady) {
        return false;
    }

    String path = buildPath("/config/") + _agentId;
    bool ok = getText(path, body);
    if (ok) {
        _lastServerResponse = body;
        recordEvent(String("Config fetched for ") + _agentId);
    }
    return ok;
}

bool EthernetMach2ConnectionChecker::isServerReachable(unsigned long timeoutMs) {
    if (!_ethernetReady) {
        return false;
    }

    EthernetClient client;
    unsigned long startedAt = millis();
    bool connected = client.connect(_serverTarget.host.c_str(), _serverTarget.port);
    unsigned long elapsed = millis() - startedAt;

    if (connected) {
        client.stop();
        recordEvent(String("Server reachable in ") + elapsed + "ms");
        return true;
    }

    if (elapsed < timeoutMs) {
        _lastError = String("Server connect failed");
    }
    return false;
}

bool EthernetMach2ConnectionChecker::pollServerControl() {
    if (!_ethernetReady || _agentId.length() == 0) {
        return false;
    }

    String response;
    String path = buildPath("/commands/next/") + _agentId;
    if (!getText(path, response)) {
        return false;
    }

    _lastControlPollAt = millis();
    if (response.indexOf("\"available\":true") < 0) {
        return false;
    }

    long commandId = jsonLongValue(response, "commandId", 0);
    String action = jsonStringValue(response, "action");
    String host = jsonStringValue(response, "host");
    String targetPath = jsonStringValue(response, "path");
    uint16_t port = (uint16_t)jsonLongValue(response, "port", 80);
    uint8_t startOctet = (uint8_t)jsonLongValue(response, "startOctet", 1);
    uint8_t endOctet = (uint8_t)jsonLongValue(response, "endOctet", 30);

    bool ok = false;
    String summary;

    if (action == "run_checks") {
        size_t failedCount = 0;
        runChecks();
        for (size_t i = 0; i < _results.size(); ++i) {
            if (!_results.getReference(i).accepted) {
                ++failedCount;
            }
        }
        ok = sendResults();
        summary = String("checks=") + _results.size() + ",failed=" + failedCount;
    } else if (action == "run_speed_test") {
        EM2SpeedResult speedResult = runSpeedTest(
            host.length() > 0 ? host : String("speed.cloudflare.com"),
            targetPath.length() > 0 ? targetPath : String("/__down?bytes=51200"),
            port > 0 ? port : 80);
        ok = sendNetworkSnapshot();
        summary = String("speedBps=") + speedResult.downloadBps;
    } else if (action == "scan_subnet") {
        scanSubnet(startOctet, endOctet, port > 0 ? port : 80);
        ok = sendNetworkSnapshot();
        summary = String("devices=") + _devices.size();
    } else if (action == "run_path_trace") {
        runPathTrace(
            host.length() > 0 ? host : String("connectivitycheck.gstatic.com"),
            targetPath.length() > 0 ? targetPath : String("/generate_204"),
            port > 0 ? port : 80);
        ok = sendNetworkSnapshot();
        summary = String("hops=") + _pathTrace.size();
    } else if (action == "send_snapshot") {
        ok = sendNetworkSnapshot();
        summary = ok ? String("snapshot_uploaded") : String("snapshot_failed");
    } else if (action == "send_heartbeat") {
        ok = sendHeartbeat();
        summary = ok ? String("heartbeat_sent") : String("heartbeat_failed");
    } else if (action == "run_full_survey") {
        runChecks();
        sendResults();
        runSpeedTest();
        scanSubnet(startOctet, endOctet, port > 0 ? port : 80);
        runPathTrace();
        ok = sendNetworkSnapshot();
        summary = String("survey_devices=") + _devices.size() + ",hops=" + _pathTrace.size();
    } else {
        summary = String("unknown_action:") + action;
    }

    completeControlCommand(commandId, ok ? String("completed") : String("failed"), summary);
    return true;
}

void EthernetMach2ConnectionChecker::loop() {
    if (!_ethernetReady || !_autoChecks) {
        return;
    }

    unsigned long now = millis();
    if (_heartbeatIntervalMs > 0 && (now - _lastHeartbeatAt) >= _heartbeatIntervalMs) {
        sendHeartbeat();
    }

    if (_checkIntervalMs > 0 && (now - _lastCheckAt) >= _checkIntervalMs) {
        runChecks();
        sendResults();
    }

    if (_controlPollIntervalMs > 0 && (now - _lastControlPollAt) >= _controlPollIntervalMs) {
        pollServerControl();
    }
}

const ArrayList<EM2ProbeResult>& EthernetMach2ConnectionChecker::getResults() const {
    return _results;
}

const ArrayList<EM2NetworkDevice>& EthernetMach2ConnectionChecker::getDiscoveredDevices() const {
    return _devices;
}

const ArrayList<EM2TraceHop>& EthernetMach2ConnectionChecker::getPathTrace() const {
    return _pathTrace;
}

const ArrayList<String>& EthernetMach2ConnectionChecker::getEventLog() const {
    return _eventLog;
}

const EM2SpeedResult& EthernetMach2ConnectionChecker::getLastSpeedResult() const {
    return _lastSpeedResult;
}

IPAddress EthernetMach2ConnectionChecker::localIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.localIP();
}

IPAddress EthernetMach2ConnectionChecker::gatewayIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.gatewayIP();
}

IPAddress EthernetMach2ConnectionChecker::dnsIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.dnsServerIP();
}

IPAddress EthernetMach2ConnectionChecker::subnetMask() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.subnetMask();
}

bool EthernetMach2ConnectionChecker::isEthernetReady() const {
    return _ethernetReady;
}

String EthernetMach2ConnectionChecker::diagnosticsJson() const {
    String json;
    json.reserve(720);
    json += "{";
    json += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    json += "\"ip\":\"" + em2IpToString(localIP()) + "\",";
    json += "\"gateway\":\"" + em2IpToString(gatewayIP()) + "\",";
    json += "\"dns\":\"" + em2IpToString(dnsIP()) + "\",";
    json += "\"subnet\":\"" + em2IpToString(subnetMask()) + "\",";
    json += "\"ethernetReady\":" + String(_ethernetReady ? "true" : "false") + ",";
    json += "\"hardware\":\"" + hardwareStatusString() + "\",";
    json += "\"link\":\"" + linkStatusString() + "\",";
    json += "\"serverHost\":\"" + em2JsonEscape(_serverTarget.host) + "\",";
    json += "\"serverPort\":" + String(_serverTarget.port) + ",";
    json += "\"probeCount\":" + String(_probes.size()) + ",";
    json += "\"resultCount\":" + String(_results.size()) + ",";
    json += "\"deviceCount\":" + String(_devices.size()) + ",";
    json += "\"pathTraceCount\":" + String(_pathTrace.size()) + ",";
    json += "\"lastCheckAtMs\":" + String(_lastCheckAt) + ",";
    json += "\"lastControlPollAtMs\":" + String(_lastControlPollAt) + ",";
    json += "\"lastError\":\"" + em2JsonEscape(_lastError) + "\"";
    json += "}";
    return json;
}

String EthernetMach2ConnectionChecker::networkSnapshotJson() const {
    String json;
    json.reserve(2200);

    size_t acceptedCount = 0;
    size_t failedCount = 0;
    for (size_t i = 0; i < _results.size(); ++i) {
        if (_results.getReference(i).accepted) {
            ++acceptedCount;
        } else {
            ++failedCount;
        }
    }

    json += "{";
    json += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    json += "\"ip\":\"" + em2IpToString(localIP()) + "\",";
    json += "\"gateway\":\"" + em2IpToString(gatewayIP()) + "\",";
    json += "\"dns\":\"" + em2IpToString(dnsIP()) + "\",";
    json += "\"subnet\":\"" + em2IpToString(subnetMask()) + "\",";
    json += "\"link\":\"" + linkStatusString() + "\",";
    json += "\"hardware\":\"" + hardwareStatusString() + "\",";
    json += "\"serverHost\":\"" + em2JsonEscape(_serverTarget.host) + "\",";
    json += "\"serverPort\":" + String(_serverTarget.port) + ",";
    json += "\"lastCheckAtMs\":" + String(_lastCheckAt) + ",";
    json += "\"resultsSummary\":{";
    json += "\"totalCount\":" + String(_results.size()) + ",";
    json += "\"acceptedCount\":" + String(acceptedCount) + ",";
    json += "\"failedCount\":" + String(failedCount) + "},";
    json += "\"speedTest\":{";
    json += "\"success\":" + String(_lastSpeedResult.success ? "true" : "false") + ",";
    json += "\"connectMs\":" + String(_lastSpeedResult.connectMs) + ",";
    json += "\"durationMs\":" + String(_lastSpeedResult.durationMs) + ",";
    json += "\"bytesReceived\":" + String(_lastSpeedResult.bytesReceived) + ",";
    json += "\"downloadBps\":" + String(_lastSpeedResult.downloadBps) + ",";
    json += "\"host\":\"" + em2JsonEscape(_lastSpeedResult.host) + "\",";
    json += "\"path\":\"" + em2JsonEscape(_lastSpeedResult.path) + "\",";
    json += "\"port\":" + String(_lastSpeedResult.port) + "},";
    json += "\"devices\":[";
    for (size_t i = 0; i < _devices.size(); ++i) {
        const EM2NetworkDevice& device = _devices.getReference(i);
        if (i > 0) {
            json += ",";
        }
        json += "{";
        json += "\"host\":\"" + em2JsonEscape(device.host) + "\",";
        json += "\"port\":" + String(device.port) + ",";
        json += "\"isUp\":" + String(device.isUp ? "true" : "false") + ",";
        json += "\"latencyMs\":" + String(device.latencyMs) + ",";
        json += "\"lastSeenMs\":" + String(device.lastSeenMs) + "}";
    }
    json += "],\"pathTrace\":[";
    for (size_t i = 0; i < _pathTrace.size(); ++i) {
        const EM2TraceHop& hop = _pathTrace.getReference(i);
        if (i > 0) {
            json += ",";
        }
        json += "{";
        json += "\"label\":\"" + em2JsonEscape(hop.label) + "\",";
        json += "\"target\":\"" + em2JsonEscape(hop.target) + "\",";
        json += "\"port\":" + String(hop.port) + ",";
        json += "\"status\":" + String(hop.statusCode) + ",";
        json += "\"connected\":" + String(hop.connected ? "true" : "false") + ",";
        json += "\"accepted\":" + String(hop.accepted ? "true" : "false") + ",";
        json += "\"latencyMs\":" + String(hop.latencyMs) + ",";
        json += "\"note\":\"" + em2JsonEscape(hop.note) + "\"}";
    }
    json += "]}";
    return json;
}

String EthernetMach2ConnectionChecker::registrationJson() const {
    String json;
    json.reserve(192);
    json += "{";
    json += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    json += "\"ip\":\"" + em2IpToString(localIP()) + "\",";
    json += "\"agentName\":\"EthernetMach2ConnectionChecker\",";
    json += "\"firmware\":\"scaffold-0.1.0\"";
    json += "}";
    return json;
}

String EthernetMach2ConnectionChecker::heartbeatJson() const {
    String json;
    json.reserve(192);
    json += "{";
    json += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    json += "\"ip\":\"" + em2IpToString(localIP()) + "\",";
    json += "\"heartbeatAtMs\":" + String(millis()) + ",";
    json += "\"resultCount\":" + String(_results.size()) + ",";
    json += "\"eventCount\":" + String(_eventLog.size());
    json += "}";
    return json;
}

String EthernetMach2ConnectionChecker::resultsJson() const {
    String json;
    json.reserve(512);
    json += "{";
    json += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    json += "\"ip\":\"" + em2IpToString(localIP()) + "\",";
    json += "\"checkedAtMs\":" + String(_lastCheckAt) + ",";
    json += "\"results\":[";

    for (size_t i = 0; i < _results.size(); ++i) {
        const EM2ProbeResult& result = _results.getReference(i);
        if (i > 0) {
            json += ",";
        }

        json += "{";
        json += "\"name\":\"" + em2JsonEscape(result.name) + "\",";
        json += "\"host\":\"" + em2JsonEscape(result.host) + "\",";
        json += "\"path\":\"" + em2JsonEscape(result.path) + "\",";
        json += "\"status\":" + String(result.statusCode) + ",";
        json += "\"connected\":" + String(result.connected ? "true" : "false") + ",";
        json += "\"accepted\":" + String(result.accepted ? "true" : "false") + ",";
        json += "\"latencyMs\":" + String(result.latencyMs) + ",";
        json += "\"checkedAtMs\":" + String(result.checkedAtMs) + ",";
        json += "\"note\":\"" + em2JsonEscape(result.note) + "\"";
        json += "}";
    }

    json += "]}";
    return json;
}

const String& EthernetMach2ConnectionChecker::getLastServerResponse() const {
    return _lastServerResponse;
}

const String& EthernetMach2ConnectionChecker::getLastError() const {
    return _lastError;
}

bool EthernetMach2ConnectionChecker::runSingleProbe(const EM2Probe& probe, EM2ProbeResult& resultOut) {
    EthernetClient client;
    resultOut.name = probe.name;
    resultOut.host = probe.host;
    resultOut.path = probe.path;
    resultOut.checkedAtMs = millis();

    bool connected = false;
    uint8_t attempts = 0;
    for (; attempts < 2 && !connected; ++attempts) {
        unsigned long startedAt = millis();
        connected = client.connect(probe.host.c_str(), probe.port);
        resultOut.latencyMs = millis() - startedAt;
        if (!connected) {
            client.stop();
        }
    }

    resultOut.connected = connected;

    if (!connected) {
        resultOut.statusCode = -1;
        resultOut.accepted = false;
        resultOut.note = attempts > 1 ? "connect_failed_retry" : "connect_failed";
        recordEvent(String("FAIL ") + probe.name + " connect");
        return false;
    }

    client.print(String("GET ") + probe.path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + probe.host + "\r\n");
    client.print("Connection: close\r\n");
    client.print("User-Agent: EM2-Agent/0.1\r\n\r\n");

    String statusLine = readLine(client, 1800UL);
    int statusCode = parseStatusCode(statusLine);
    resultOut.statusCode = statusCode;
    resultOut.accepted = (statusCode >= (int)probe.minAcceptedStatus
        && statusCode <= (int)probe.maxAcceptedStatus);
    if (resultOut.accepted) {
        resultOut.note = attempts > 1 ? "accepted_retry" : "accepted";
    } else {
        resultOut.note = "out_of_range";
    }

    while (client.connected()) {
        String line = readLine(client, 500UL);
        if (line.length() == 0) {
            break;
        }
    }

    client.stop();
    _lastError = "";
    recordEvent(String(resultOut.accepted ? "OK " : "WARN ") + probe.name + " status=" + statusCode);
    return resultOut.accepted;
}

bool EthernetMach2ConnectionChecker::postJson(const String& path, const String& body) {
    if (!_ethernetReady) {
        _lastError = String("Ethernet not ready");
        return false;
    }

    EthernetClient client;
    if (!client.connect(_serverTarget.host.c_str(), _serverTarget.port)) {
        _lastError = String("Server connect failed");
        recordEvent(_lastError);
        return false;
    }

    client.print(String("POST ") + path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + _serverTarget.host + "\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n");
    client.print(String("Content-Length: ") + body.length() + "\r\n\r\n");
    client.print(body);

    String statusLine = readLine(client, 2500UL);
    int statusCode = parseStatusCode(statusLine);

    while (client.connected()) {
        String headerLine = readLine(client, 1200UL);
        if (headerLine.length() == 0) {
            break;
        }
    }

    String responseBody = "";
    unsigned long startedAt = millis();
    while ((millis() - startedAt) < 1200UL && (client.connected() || client.available())) {
        while (client.available() && responseBody.length() < 200) {
            responseBody += (char)client.read();
        }
    }

    client.stop();
    _lastServerResponse = responseBody;
    bool ok = statusCode >= 200 && statusCode <= 299;
    if (ok) {
        _lastError = "";
    } else {
        _lastError = String("Server status ") + statusCode;
    }
    return ok;
}

bool EthernetMach2ConnectionChecker::completeControlCommand(
    long commandId, const String& status, const String& summary) {
    if (commandId <= 0) {
        return false;
    }

    String body;
    body.reserve(192);
    body += "{";
    body += "\"deviceId\":\"" + em2JsonEscape(_agentId) + "\",";
    body += "\"status\":\"" + em2JsonEscape(status) + "\",";
    body += "\"summary\":\"" + em2JsonEscape(summary) + "\"";
    body += "}";
    return postJson(buildPath("/commands/") + String(commandId) + "/complete", body);
}

bool EthernetMach2ConnectionChecker::getText(const String& path, String& bodyOut) {
    if (!_ethernetReady) {
        _lastError = String("Ethernet not ready");
        return false;
    }

    EthernetClient client;
    if (!client.connect(_serverTarget.host.c_str(), _serverTarget.port)) {
        _lastError = String("Server connect failed");
        return false;
    }

    client.print(String("GET ") + path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + _serverTarget.host + "\r\n");
    client.print("Connection: close\r\n\r\n");

    String statusLine = readLine(client, 2500UL);
    int statusCode = parseStatusCode(statusLine);

    while (client.connected()) {
        String headerLine = readLine(client, 1200UL);
        if (headerLine.length() == 0) {
            break;
        }
    }

    bodyOut = "";
    unsigned long startedAt = millis();
    while ((millis() - startedAt) < 1200UL && (client.connected() || client.available())) {
        while (client.available() && bodyOut.length() < 256) {
            bodyOut += (char)client.read();
        }
    }

    client.stop();
    bool ok = statusCode >= 200 && statusCode <= 299;
    if (ok) {
        _lastError = "";
    } else {
        _lastError = String("Server status ") + statusCode;
    }
    return ok;
}

String EthernetMach2ConnectionChecker::buildPath(const String& suffix) const {
    String path = _serverTarget.basePath;
    if (!path.startsWith("/")) {
        path = "/" + path;
    }
    if (path.endsWith("/") && suffix.startsWith("/")) {
        return path.substring(0, path.length() - 1) + suffix;
    }
    if (!path.endsWith("/") && !suffix.startsWith("/")) {
        return path + "/" + suffix;
    }
    return path + suffix;
}

void EthernetMach2ConnectionChecker::recordEvent(const String& message) {
    String stamped = String("[") + millis() + String(" ms] ") + message;
    _eventLog.add(stamped);

    if (_eventLog.size() > 20) {
        ArrayList<String> trimmed(ArrayList<String>::DYNAMIC2, 20);
        for (size_t i = _eventLog.size() - 20; i < _eventLog.size(); ++i) {
            trimmed.add(_eventLog.getReference(i));
        }
        _eventLog = trimmed;
    }
}

void EthernetMach2ConnectionChecker::prepareNetworkHardware() {
    pinMode(_ethernetCSPin, OUTPUT);
    pinMode(_sdCSPin, OUTPUT);
    digitalWrite(_ethernetCSPin, HIGH);
    digitalWrite(_sdCSPin, HIGH);

    SPI.begin();
    Ethernet.init(_ethernetCSPin);
}

void EthernetMach2ConnectionChecker::updateOrAddDevice(
    const String& host, uint16_t port, bool isUp, unsigned long latencyMs, unsigned long lastSeenMs) {
    for (size_t i = 0; i < _devices.size(); ++i) {
        EM2NetworkDevice& device = _devices.getReference(i);
        if (device.host == host && device.port == port) {
            device.isUp = isUp;
            device.latencyMs = latencyMs;
            device.lastSeenMs = lastSeenMs;
            return;
        }
    }

    if (_devices.size() >= 24) {
        return;
    }

    EM2NetworkDevice device;
    device.host = host;
    device.port = port;
    device.isUp = isUp;
    device.latencyMs = latencyMs;
    device.lastSeenMs = lastSeenMs;
    _devices.add(device);
}

void EthernetMach2ConnectionChecker::addTraceHop(
    const String& label, const String& host, const String& path,
    uint16_t port, uint16_t minAcceptedStatus, uint16_t maxAcceptedStatus) {
    EM2Probe probe(label, host, path, port, minAcceptedStatus, maxAcceptedStatus);
    EM2ProbeResult probeResult;
    runSingleProbe(probe, probeResult);

    EM2TraceHop hop;
    hop.label = label;
    hop.target = host;
    hop.port = port;
    hop.statusCode = probeResult.statusCode;
    hop.connected = probeResult.connected;
    hop.accepted = probeResult.accepted;
    hop.latencyMs = probeResult.latencyMs;
    hop.note = probeResult.note;
    _pathTrace.add(hop);
}

String EthernetMach2ConnectionChecker::linkStatusString() const {
    if (!_ethernetReady) {
        return String("not_initialized");
    }

    EthernetLinkStatus linkStatus = Ethernet.linkStatus();
    if (linkStatus == LinkON) {
        return String("up");
    }
    if (linkStatus == LinkOFF) {
        return String("down");
    }
    return String("unknown");
}

String EthernetMach2ConnectionChecker::hardwareStatusString() const {
    if (!_ethernetReady) {
        return String("not_initialized");
    }

    EthernetHardwareStatus hardwareStatus = Ethernet.hardwareStatus();
    if (hardwareStatus == EthernetNoHardware) {
        return String("no_hardware");
    }
    if (hardwareStatus == EthernetW5100) {
        return String("W5100");
    }
    if (hardwareStatus == EthernetW5200) {
        return String("W5200");
    }
    if (hardwareStatus == EthernetW5500) {
        return String("W5500");
    }
    return String("unknown");
}

String EthernetMach2ConnectionChecker::jsonStringValue(const String& json, const char* key) const {
    String pattern = String("\"") + key + "\":\"";
    int index = json.indexOf(pattern);
    if (index < 0) {
        return String("");
    }

    int cursor = index + pattern.length();
    String value;
    bool escaped = false;
    while (cursor < (int)json.length()) {
        char c = json[cursor++];
        if (escaped) {
            value += c;
            escaped = false;
            continue;
        }
        if (c == '\\') {
            escaped = true;
            continue;
        }
        if (c == '"') {
            break;
        }
        value += c;
    }

    return value;
}

long EthernetMach2ConnectionChecker::jsonLongValue(
    const String& json, const char* key, long defaultValue) const {
    String pattern = String("\"") + key + "\":";
    int index = json.indexOf(pattern);
    if (index < 0) {
        return defaultValue;
    }

    int cursor = index + pattern.length();
    while (cursor < (int)json.length() && json[cursor] == ' ') {
        ++cursor;
    }

    int end = cursor;
    while (end < (int)json.length()) {
        char c = json[end];
        if ((c >= '0' && c <= '9') || c == '-') {
            ++end;
            continue;
        }
        break;
    }

    if (end <= cursor) {
        return defaultValue;
    }

    return json.substring(cursor, end).toInt();
}

int EthernetMach2ConnectionChecker::parseStatusCode(const String& statusLine) const {
    int firstSpace = statusLine.indexOf(' ');
    if (firstSpace < 0) {
        return -1;
    }

    int secondSpace = statusLine.indexOf(' ', firstSpace + 1);
    if (secondSpace < 0) {
        secondSpace = statusLine.length();
    }

    return statusLine.substring(firstSpace + 1, secondSpace).toInt();
}

String EthernetMach2ConnectionChecker::readLine(EthernetClient& client, unsigned long timeoutMs) const {
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
