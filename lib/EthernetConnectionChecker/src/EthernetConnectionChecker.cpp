#include "EthernetConnectionChecker.h"

struct ECCBinaryRecord {
    uint32_t checkedAtMs;
    uint32_t latencyMs;
    int16_t statusCode;
    uint8_t connected;
    uint8_t acceptedStatus;
    char endpoint[24];
    char host[40];
};

static void eccSafeCopy(char* dest, size_t destSize, const String& src) {
    if (destSize == 0) {
        return;
    }

    size_t maxLen = destSize - 1;
    size_t len = src.length();
    if (len > maxLen) {
        len = maxLen;
    }

    for (size_t i = 0; i < len; ++i) {
        dest[i] = src[i];
    }
    dest[len] = '\0';
}

static String eccIpToString(const IPAddress& ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

static String eccJsonEscape(const String& src) {
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

static String eccStatusClassName(int code) {
    if (code >= 100 && code <= 199) {
        return String("informational");
    }
    if (code >= 200 && code <= 299) {
        return String("success");
    }
    if (code >= 300 && code <= 399) {
        return String("redirect");
    }
    if (code >= 400 && code <= 499) {
        return String("client_error");
    }
    if (code >= 500 && code <= 599) {
        return String("server_error");
    }
    return String("unknown");
}

static String eccUrlDecode(const String& str) {
    String out;
    out.reserve(str.length());
    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i];
        if (c == '+') {
            out += ' ';
        } else if (c == '%' && i + 2 < str.length()) {
            char hi = str[++i];
            char lo = str[++i];
            uint8_t val = 0;
            if      (hi >= '0' && hi <= '9') val = (uint8_t)((hi - '0') << 4);
            else if (hi >= 'a' && hi <= 'f') val = (uint8_t)((hi - 'a' + 10) << 4);
            else if (hi >= 'A' && hi <= 'F') val = (uint8_t)((hi - 'A' + 10) << 4);
            if      (lo >= '0' && lo <= '9') val |= (uint8_t)(lo - '0');
            else if (lo >= 'a' && lo <= 'f') val |= (uint8_t)(lo - 'a' + 10);
            else if (lo >= 'A' && lo <= 'F') val |= (uint8_t)(lo - 'A' + 10);
            out += (char)val;
        } else {
            out += c;
        }
    }
    return out;
}

EthernetConnectionChecker::EthernetConnectionChecker(
    uint8_t ledPin,
    uint8_t ethernetCSPin,
    uint8_t sdCSPin,
    uint16_t webPort)
    : _ledPin(ledPin),
      _ethernetCSPin(ethernetCSPin),
      _sdCSPin(sdCSPin),
      _server(webPort),
      _ethernetReady(false),
      _sdReady(false),
      _autoChecks(true),
      _lastCheckAt(0),
      _checkIntervalMs(60000UL),
            _endpoints(ArrayList<ECCEndpoint>::DYNAMIC2, 4),
            _results(ArrayList<ECCProbeResult>::DYNAMIC2, 2),
            _eventLog(ArrayList<String>::DYNAMIC2, 4),
            _devices(ArrayList<ECCNetDevice>::DYNAMIC2, 2),
      _lastSpeedResult(),
      _serialBufLen(0),
      _lastError() {
    memset(_serialBuf, 0, sizeof(_serialBuf));
}

bool EthernetConnectionChecker::begin(const byte macAddress[6], unsigned long dhcpTimeoutMs) {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);

    pinMode(_ethernetCSPin, OUTPUT);
    pinMode(_sdCSPin, OUTPUT);
    digitalWrite(_ethernetCSPin, HIGH);
    digitalWrite(_sdCSPin, HIGH);

    SPI.begin();

    Ethernet.init(_ethernetCSPin);
    selectEthernetBus();
    int dhcpResult = Ethernet.begin(const_cast<byte*>(macAddress), dhcpTimeoutMs);
    _ethernetReady = (dhcpResult != 0);

    if (!_ethernetReady) {
        _lastError = String("Ethernet DHCP failed");
        recordEvent(_lastError.getValue());
        flashCode(4, 90, 110);
        return false;
    }

    delay(900);
    _server.begin();
    _ethernetReady = true;

    _sdReady = reinitSd(3);
    if (!_sdReady) {
        _lastError = String("SD init failed");
        recordEvent(_lastError.getValue());
        flashCode(3, 120, 120);
    } else {
        recordEvent(String("SD ready"));
    }

    recordEvent(String("Ethernet ready @ ") + eccIpToString(Ethernet.localIP()));
    flashCode(1, 220, 100);
    return true;
}

bool EthernetConnectionChecker::beginStatic(
    const byte macAddress[6],
    const IPAddress& ip,
    const IPAddress& dns,
    const IPAddress& gateway,
    const IPAddress& subnet) {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);

    pinMode(_ethernetCSPin, OUTPUT);
    pinMode(_sdCSPin, OUTPUT);
    digitalWrite(_ethernetCSPin, HIGH);
    digitalWrite(_sdCSPin, HIGH);

    SPI.begin();

    Ethernet.init(_ethernetCSPin);
    selectEthernetBus();
    Ethernet.begin(const_cast<byte*>(macAddress), ip, dns, gateway, subnet);
    delay(350);

    _server.begin();
    _ethernetReady = true;
    _sdReady = reinitSd(3);

    if (!_sdReady) {
        _lastError = String("SD init failed");
        recordEvent(_lastError.getValue());
    } else {
        recordEvent(String("SD ready"));
    }

    recordEvent(String("Static IP mode @ ") + eccIpToString(Ethernet.localIP()));
    flashCode(2, 120, 80);
    return true;
}

void EthernetConnectionChecker::setCheckInterval(unsigned long intervalMs) {
    _checkIntervalMs = intervalMs;
}

void EthernetConnectionChecker::enableAutoChecks(bool enabled) {
    _autoChecks = enabled;
}

void EthernetConnectionChecker::addEndpoint(
    const String& name,
    const String& host,
    const String& path,
    uint16_t port,
    uint16_t minAcceptedStatus,
    uint16_t maxAcceptedStatus) {
    _endpoints.add(ECCEndpoint(name, host, path, port, minAcceptedStatus, maxAcceptedStatus));
}

void EthernetConnectionChecker::addReachabilityEndpoint(
    const String& name,
    const String& host,
    const String& path,
    uint16_t port) {
    addEndpoint(name, host, path, port, 100, 599);
}

void EthernetConnectionChecker::addGatewayEndpoint(
    const String& name,
    const String& path,
    uint16_t port,
    uint16_t minAcceptedStatus,
    uint16_t maxAcceptedStatus) {
    IPAddress gw = Ethernet.gatewayIP();
    if (gw[0] == 0 && gw[1] == 0 && gw[2] == 0 && gw[3] == 0) {
        recordEvent(String("Gateway endpoint skipped (gateway unavailable)"));
        return;
    }

    addEndpoint(name, eccIpToString(gw), path, port, minAcceptedStatus, maxAcceptedStatus);
}

void EthernetConnectionChecker::addLocalDevice(
    const String& name, const String& hostOrIp, uint16_t port, const String& macAddress) {
    ECCNetDevice dev(name, hostOrIp, port, macAddress);
    _devices.add(dev);
    addEndpoint(name, hostOrIp, "/", port, 100, 599);
}

bool EthernetConnectionChecker::removeEndpoint(const String& name) {
    bool found = false;
    ArrayList<ECCEndpoint> newEndpoints(ArrayList<ECCEndpoint>::DYNAMIC2,
        _endpoints.size() > 0 ? _endpoints.size() : 4);
    for (size_t i = 0; i < _endpoints.size(); ++i) {
        const ECCEndpoint& ep = _endpoints.getReference(i);
        if (ep.name == name) {
            found = true;
        } else {
            newEndpoints.add(ep);
        }
    }
    if (found) _endpoints = newEndpoints;

    ArrayList<ECCNetDevice> newDevices(ArrayList<ECCNetDevice>::DYNAMIC2,
        _devices.size() > 0 ? _devices.size() : 4);
    for (size_t i = 0; i < _devices.size(); ++i) {
        const ECCNetDevice& dev = _devices.getReference(i);
        if (dev.name != name) newDevices.add(dev);
    }
    _devices = newDevices;
    return found;
}

const ArrayList<ECCNetDevice>& EthernetConnectionChecker::getDevices() const {
    return _devices;
}

ECCSpeedResult EthernetConnectionChecker::runSpeedTest(
    const String& host, const String& path, uint16_t port) {
    ECCSpeedResult result;
    if (!_ethernetReady) return result;

    EthernetClient client;
    selectEthernetBus();

    unsigned long connectStart = millis();
    if (!client.connect(host.c_str(), port)) {
        recordEvent(String("Speedtest: connect failed to ") + host);
        return result;
    }
    result.connectMs = millis() - connectStart;

    client.print(String("GET ") + path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + host + "\r\n");
    client.print("Connection: close\r\n");
    client.print("User-Agent: Mega-ECC/1.0\r\n\r\n");

    // Skip response headers
    while (client.connected()) {
        String line = readLine(client, 2000UL);
        if (line.length() == 0) break;
    }

    // Count body bytes
    uint8_t buf[64];
    unsigned long byteCount = 0;
    unsigned long start = millis();
    unsigned long deadline = start + 20000UL;

    while (millis() < deadline && (client.connected() || client.available())) {
        int n = client.read(buf, sizeof(buf));
        if (n > 0) byteCount += (unsigned long)n;
    }

    unsigned long duration = millis() - start;
    client.stop();

    result.success = (byteCount > 0);
    result.bytesReceived = byteCount;
    result.durationMs = (duration > 0) ? duration : 1;
    result.downloadBps = (byteCount * 1000UL) / result.durationMs;

    _lastSpeedResult = result;
    recordEvent(String("Speedtest: ") + byteCount + "B in " + duration + "ms = " + result.downloadBps + " B/s");
    return result;
}

void EthernetConnectionChecker::scanSubnet(
    uint8_t startOctet, uint8_t endOctet, uint16_t port) {
    if (!_ethernetReady) return;

    IPAddress myIp = Ethernet.localIP();

    // Reduce retransmission timeout for faster scan (100ms × 1 retry)
    Ethernet.setRetransmissionTimeout(1000);
    Ethernet.setRetransmissionCount(1);

    recordEvent(String("Scan: ") + myIp[0] + "." + myIp[1] + "." + myIp[2]
        + "." + startOctet + "-" + endOctet + " port=" + port);

    for (uint8_t octet = startOctet; octet <= endOctet; ++octet) {
        if (octet == myIp[3]) continue;

        IPAddress target(myIp[0], myIp[1], myIp[2], octet);
        EthernetClient client;
        selectEthernetBus();

        unsigned long t0 = millis();
        bool connected = client.connect(target, port);
        unsigned long lat = millis() - t0;

        if (connected) {
            client.stop();
            String ipStr = eccIpToString(target);

            bool found = false;
            for (size_t i = 0; i < _devices.size(); ++i) {
                ECCNetDevice& dev = _devices.getReference(i);
                if (dev.host == ipStr) {
                    dev.isUp = true;
                    dev.latencyMs = lat;
                    dev.lastSeenMs = millis();
                    found = true;
                    break;
                }
            }
            if (!found) {
                ECCNetDevice newDev(String("Device@") + ipStr, ipStr, port);
                newDev.isUp = true;
                newDev.latencyMs = lat;
                newDev.lastSeenMs = millis();
                _devices.add(newDev);
            }
            recordEvent(String("Scan found: ") + ipStr + ":" + port + " " + lat + "ms");
        }
    }

    // Restore defaults
    Ethernet.setRetransmissionTimeout(2000);
    Ethernet.setRetransmissionCount(8);
}

void EthernetConnectionChecker::syncDeviceStatus() {
    for (size_t di = 0; di < _devices.size(); ++di) {
        ECCNetDevice& dev = _devices.getReference(di);
        for (size_t ri = 0; ri < _results.size(); ++ri) {
            const ECCProbeResult& r = _results.getReference(ri);
            if (r.endpointName == dev.name) {
                dev.isUp = r.connected;
                dev.latencyMs = r.latencyMs;
                dev.lastSeenMs = r.checkedAtMs;
                break;
            }
        }
    }
}

String EthernetConnectionChecker::parseQueryParam(
    const String& request, const String& key) const {
    int qmark = request.indexOf('?');
    if (qmark < 0) return String();

    int httpSpace = request.lastIndexOf(' ');
    if (httpSpace <= qmark) httpSpace = (int)request.length();

    String query = request.substring(qmark + 1, httpSpace);
    String searchKey = key + "=";
    int keyIdx = query.indexOf(searchKey);
    if (keyIdx < 0) return String();

    int valueStart = keyIdx + (int)searchKey.length();
    int ampIdx = query.indexOf('&', valueStart);
    String value = (ampIdx < 0)
        ? query.substring(valueStart)
        : query.substring(valueStart, ampIdx);
    return eccUrlDecode(value);
}

void EthernetConnectionChecker::handleSerialCommand(const String& rawCmd) {
    String cmd = rawCmd;
    cmd.trim();
    if (cmd.length() == 0) return;

    // Tokenize on spaces (up to 8 tokens)
    String tokens[8];
    uint8_t tc = 0;
    int start = 0;
    for (int i = 0; i <= (int)cmd.length() && tc < 8; ++i) {
        if (i == (int)cmd.length() || cmd[i] == ' ') {
            if (i > start) tokens[tc++] = cmd.substring(start, i);
            start = i + 1;
        }
    }
    if (tc == 0) return;

    String verb = tokens[0];
    verb.toUpperCase();

    if (verb == "HELP") {
        Serial.println(F("Commands:"));
        Serial.println(F("  RUN              Run all endpoint checks"));
        Serial.println(F("  LIST             List all endpoints"));
        Serial.println(F("  DEVICES          List local devices"));
        Serial.println(F("  CLEAR            Clear results and event log"));
        Serial.println(F("  SPEED            Run download speed test"));
        Serial.println(F("  SCAN s e [port]  TCP scan subnet octets s-e"));
        Serial.println(F("  ADD n h p pt mn mx  Add endpoint"));
        Serial.println(F("  DEL name         Remove endpoint by name"));
        Serial.println(F("  ADDDEV n h [pt] [mac]  Add local device"));
        Serial.println(F("  DIAG             Print diagnostics JSON"));
    } else if (verb == "RUN") {
        Serial.println(F("Running checks..."));
        runChecks();
        Serial.println(F("Done."));
    } else if (verb == "LIST") {
        Serial.print(F("Endpoints ("));
        Serial.print(_endpoints.size());
        Serial.println(F("):"));
        for (size_t i = 0; i < _endpoints.size(); ++i) {
            const ECCEndpoint& ep = _endpoints.getReference(i);
            Serial.print("  "); Serial.print(ep.name);
            Serial.print(" -> "); Serial.print(ep.host); Serial.print(ep.path);
            Serial.print(" :"); Serial.print(ep.port);
            Serial.print(" ["); Serial.print(ep.minAcceptedStatus);
            Serial.print("-"); Serial.print(ep.maxAcceptedStatus);
            Serial.println("]");
        }
    } else if (verb == "DEVICES") {
        Serial.print(F("Devices ("));
        Serial.print(_devices.size());
        Serial.println(F("):"));
        for (size_t i = 0; i < _devices.size(); ++i) {
            const ECCNetDevice& dev = _devices.getReference(i);
            Serial.print("  "); Serial.print(dev.name);
            Serial.print(" @ "); Serial.print(dev.host);
            Serial.print(":"); Serial.print(dev.port);
            Serial.print(dev.isUp ? F(" UP") : F(" DOWN"));
            Serial.print(F(" lat=")); Serial.print(dev.latencyMs); Serial.print(F("ms"));
            if (dev.macAddress.length() > 0) {
                Serial.print(F(" mac=")); Serial.print(dev.macAddress);
            }
            Serial.println();
        }
    } else if (verb == "CLEAR") {
        _results.clear();
        _eventLog.clear();
        Serial.println(F("Cleared."));
    } else if (verb == "SPEED") {
        Serial.println(F("Running speed test..."));
        ECCSpeedResult r = runSpeedTest();
        if (r.success) {
            Serial.print(r.bytesReceived); Serial.print(F(" bytes in "));
            Serial.print(r.durationMs); Serial.print(F("ms = "));
            Serial.print(r.downloadBps); Serial.println(F(" B/s"));
        } else {
            Serial.println(F("Speed test failed."));
        }
    } else if (verb == "SCAN" && tc >= 3) {
        uint8_t s = (uint8_t)tokens[1].toInt();
        uint8_t e = (uint8_t)tokens[2].toInt();
        uint16_t p = (tc >= 4) ? (uint16_t)tokens[3].toInt() : 80;
        Serial.print(F("Scanning .")); Serial.print(s);
        Serial.print(F("-.")); Serial.println(e);
        scanSubnet(s, e, p);
        Serial.println(F("Scan done."));
    } else if (verb == "ADD" && tc >= 7) {
        addEndpoint(tokens[1], tokens[2], tokens[3],
            (uint16_t)tokens[4].toInt(),
            (uint16_t)tokens[5].toInt(),
            (uint16_t)tokens[6].toInt());
        Serial.print(F("Added: ")); Serial.println(tokens[1]);
    } else if (verb == "DEL" && tc >= 2) {
        Serial.println(removeEndpoint(tokens[1]) ? F("Removed.") : F("Not found."));
    } else if (verb == "ADDDEV" && tc >= 3) {
        uint16_t p = (tc >= 4) ? (uint16_t)tokens[3].toInt() : 80;
        String mac = (tc >= 5) ? tokens[4] : String("");
        addLocalDevice(tokens[1], tokens[2], p, mac);
        Serial.print(F("Added device: ")); Serial.println(tokens[1]);
    } else if (verb == "DIAG") {
        Serial.println(diagnosticsJson());
    } else {
        Serial.print(F("Unknown: ")); Serial.println(verb);
        Serial.println(F("Type HELP for commands."));
    }
}

void EthernetConnectionChecker::processSerial() {
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\r') continue;
        if (c == '\n' || _serialBufLen >= 63) {
            _serialBuf[_serialBufLen] = '\0';
            if (_serialBufLen > 0) {
                handleSerialCommand(String(_serialBuf));
            }
            _serialBufLen = 0;
        } else {
            _serialBuf[_serialBufLen++] = c;
        }
    }
}

String EthernetConnectionChecker::devicesJson() const {
    String json;
    json.reserve(512);
    json += "{\"devices\":[";
    for (size_t i = 0; i < _devices.size(); ++i) {
        const ECCNetDevice& dev = _devices.getReference(i);
        if (i > 0) json += ",";
        json += "{\"name\":\"" + eccJsonEscape(dev.name) + "\",";
        json += "\"host\":\"" + eccJsonEscape(dev.host) + "\",";
        json += "\"mac\":\"" + eccJsonEscape(dev.macAddress) + "\",";
        json += "\"port\":" + String(dev.port) + ",";
        json += "\"isUp\":" + String(dev.isUp ? "true" : "false") + ",";
        json += "\"latencyMs\":" + String(dev.latencyMs) + ",";
        json += "\"lastSeenMs\":" + String(dev.lastSeenMs) + "}";
    }
    json += "]}";
    return json;
}

String EthernetConnectionChecker::speedJson() const {
    String json;
    json.reserve(160);
    json += "{\"success\":" + String(_lastSpeedResult.success ? "true" : "false") + ",";
    json += "\"connectMs\":" + String(_lastSpeedResult.connectMs) + ",";
    json += "\"durationMs\":" + String(_lastSpeedResult.durationMs) + ",";
    json += "\"bytesReceived\":" + String(_lastSpeedResult.bytesReceived) + ",";
    json += "\"downloadBps\":" + String(_lastSpeedResult.downloadBps) + "}";
    return json;
}

bool EthernetConnectionChecker::runChecks() {
    if (!_ethernetReady) {
        _lastError = String("runChecks called before Ethernet ready");
        recordEvent(_lastError.getValue());
        flashCode(5, 80, 80);
        return false;
    }

    _results.clear();

    bool allHealthy = true;
    for (size_t i = 0; i < _endpoints.size(); ++i) {
        ECCProbeResult result;
        bool ok = runSingleCheck(_endpoints.getReference(i), result);
        if (!ok) {
            allHealthy = false;
        }

        _results.add(result);
        saveResultCsv(result);
        saveResultBinary(result);
        saveResultText(result);
    }

    _lastCheckAt = millis();
    syncDeviceStatus();

    if (allHealthy) {
        flashCode(1, 180, 80);
    } else {
        flashCode(2, 80, 120);
    }

    return allHealthy;
}

void EthernetConnectionChecker::loop() {
    selectEthernetBus();
    EthernetClient client = _server.available();
    if (client) {
        handleClient(client);
    }

    if (_autoChecks && (_checkIntervalMs > 0)) {
        unsigned long now = millis();
        if ((now - _lastCheckAt) >= _checkIntervalMs) {
            runChecks();
        }
    }

    processSerial();
}

const ArrayList<ECCProbeResult>& EthernetConnectionChecker::getResults() const {
    return _results;
}

const ArrayList<String>& EthernetConnectionChecker::getEventLog() const {
    return _eventLog;
}

Optional<String> EthernetConnectionChecker::getLastError() const {
    return _lastError;
}

bool EthernetConnectionChecker::isEthernetReady() const {
    return _ethernetReady;
}

bool EthernetConnectionChecker::isSdReady() const {
    return _sdReady;
}

IPAddress EthernetConnectionChecker::localIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.localIP();
}

IPAddress EthernetConnectionChecker::gatewayIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.gatewayIP();
}

IPAddress EthernetConnectionChecker::dnsIP() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.dnsServerIP();
}

IPAddress EthernetConnectionChecker::subnetMask() const {
    if (!_ethernetReady) {
        return IPAddress(0, 0, 0, 0);
    }
    return Ethernet.subnetMask();
}

bool EthernetConnectionChecker::reinitSd(uint8_t attempts) {
    if (attempts == 0) {
        attempts = 1;
    }

    bool ready = false;
    for (uint8_t i = 0; i < attempts; ++i) {
        selectSdBus();
        if (SD.begin(_sdCSPin)) {
            ready = true;
            break;
        }

        delay(20);
    }

    selectEthernetBus();
    _sdReady = ready;
    return ready;
}

void EthernetConnectionChecker::flashCode(uint8_t count, uint16_t onMs, uint16_t offMs) {
    for (uint8_t i = 0; i < count; ++i) {
        digitalWrite(_ledPin, HIGH);
        delay(onMs);
        digitalWrite(_ledPin, LOW);
        if (i + 1 < count) {
            delay(offMs);
        }
    }
}

void EthernetConnectionChecker::recordEvent(const String& message) {
    String stamped = String("[") + millis() + String(" ms] ") + message;
    _eventLog.add(stamped);

    if (_eventLog.size() > 15) {
        ArrayList<String> trimmed(ArrayList<String>::DYNAMIC2, 15);
        for (size_t i = _eventLog.size() - 15; i < _eventLog.size(); ++i) {
            trimmed.add(_eventLog.getReference(i));
        }
        _eventLog = trimmed;
    }
}

void EthernetConnectionChecker::selectEthernetBus() const {
    digitalWrite(_sdCSPin, HIGH);
    digitalWrite(_ethernetCSPin, LOW);
}

void EthernetConnectionChecker::selectSdBus() const {
    digitalWrite(_ethernetCSPin, HIGH);
    digitalWrite(_sdCSPin, LOW);
}

bool EthernetConnectionChecker::runSingleCheck(const ECCEndpoint& endpoint, ECCProbeResult& resultOut) {
    EthernetClient client;
    selectEthernetBus();

    resultOut.endpointName = endpoint.name;
    resultOut.host = endpoint.host;
    resultOut.path = endpoint.path;
    resultOut.minAcceptedStatus = endpoint.minAcceptedStatus;
    resultOut.maxAcceptedStatus = endpoint.maxAcceptedStatus;
    resultOut.checkedAtMs = millis();

    unsigned long startMs = millis();
    bool connected = client.connect(endpoint.host.c_str(), endpoint.port);
    resultOut.connected = connected;

    if (!connected) {
        resultOut.statusCode = -1;
        resultOut.acceptedStatus = false;
        resultOut.latencyMs = millis() - startMs;
        resultOut.notes.setSingle(String("connect_failed"));
        recordEvent(String("FAIL connect ") + endpoint.host + endpoint.path);
        return false;
    }

    client.print(String("GET ") + endpoint.path + " HTTP/1.1\r\n");
    client.print(String("Host: ") + endpoint.host + "\r\n");
    client.print("Connection: close\r\n");
    client.print("User-Agent: Mega-ECC/1.0\r\n\r\n");

    String statusLine = readLine(client, 1800UL);
    int code = parseStatusCode(statusLine);

    String redirectLocation;

    while (client.connected()) {
        String headerLine = readLine(client, 1200UL);
        if (headerLine.length() == 0) {
            break;
        }

        if (headerLine.startsWith("Location:")) {
            redirectLocation = headerLine.substring(9);
            redirectLocation.trim();
        }
    }

    String payloadPreview = readBodyPreview(client, 180, 2400UL);

    resultOut.statusCode = code;
    resultOut.latencyMs = millis() - startMs;
    resultOut.acceptedStatus = (code >= static_cast<int>(endpoint.minAcceptedStatus) && code <= static_cast<int>(endpoint.maxAcceptedStatus));

    if (redirectLocation.length() > 0) {
        resultOut.redirectLocation = redirectLocation;
    }

    String note = eccStatusClassName(code);
    if (code == 301 || code == 302 || code == 307 || code == 308) {
        note += String(" redirect");
    } else if (code == 403) {
        note += String(" forbidden");
    }
    if (!resultOut.acceptedStatus) {
        note += String(" out_of_range[") + endpoint.minAcceptedStatus + String("-") + endpoint.maxAcceptedStatus + String("]");
    }
    resultOut.notes.setSingle(note);

    if (payloadPreview.length() > 0) {
        resultOut.payloadPreview = payloadPreview;
    }

    client.stop();

    if (resultOut.acceptedStatus) {
        recordEvent(String("OK ") + endpoint.name + " status=" + resultOut.statusCode);
    } else {
        recordEvent(String("WARN ") + endpoint.name + " status=" + resultOut.statusCode);
    }

    return resultOut.acceptedStatus;
}

int EthernetConnectionChecker::parseStatusCode(const String& statusLine) const {
    int firstSpace = statusLine.indexOf(' ');
    if (firstSpace < 0) {
        return -1;
    }

    int secondSpace = statusLine.indexOf(' ', firstSpace + 1);
    if (secondSpace < 0) {
        secondSpace = statusLine.length();
    }

    String code = statusLine.substring(firstSpace + 1, secondSpace);
    return code.toInt();
}

String EthernetConnectionChecker::readLine(EthernetClient& client, unsigned long timeoutMs) const {
    String line = "";
    unsigned long start = millis();

    while ((millis() - start) < timeoutMs) {
        while (client.available()) {
            char c = static_cast<char>(client.read());
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

String EthernetConnectionChecker::readBodyPreview(EthernetClient& client, size_t maxChars, unsigned long timeoutMs) const {
    String data = "";
    unsigned long start = millis();

    while ((millis() - start) < timeoutMs && data.length() < maxChars) {
        while (client.available() && data.length() < maxChars) {
            char c = static_cast<char>(client.read());
            if (c >= 32 || c == '\n' || c == '\t') {
                data += c;
            }
        }

        if (!client.connected() && !client.available()) {
            break;
        }
    }

    data.trim();
    return data;
}

bool EthernetConnectionChecker::saveResultCsv(const ECCProbeResult& result) {
    if (!_sdReady) {
        return false;
    }

    selectSdBus();
    File file = SD.open("/netlog.csv", FILE_WRITE);
    if (!file) {
        selectEthernetBus();
        return false;
    }

    file.print(result.checkedAtMs);
    file.print(',');
    file.print(result.endpointName);
    file.print(',');
    file.print(result.host);
    file.print(',');
    file.print(result.path);
    file.print(',');
    file.print(result.statusCode);
    file.print(',');
    file.print(result.latencyMs);
    file.print(',');
    file.print(result.minAcceptedStatus);
    file.print('-');
    file.print(result.maxAcceptedStatus);
    file.print(',');
    file.print(result.connected ? 1 : 0);
    file.print(',');
    file.println(result.acceptedStatus ? 1 : 0);
    file.close();
    selectEthernetBus();
    return true;
}

bool EthernetConnectionChecker::saveResultBinary(const ECCProbeResult& result) {
    if (!_sdReady) {
        return false;
    }

    ECCBinaryRecord rec;
    rec.checkedAtMs = static_cast<uint32_t>(result.checkedAtMs);
    rec.latencyMs = static_cast<uint32_t>(result.latencyMs);
    rec.statusCode = static_cast<int16_t>(result.statusCode);
    rec.connected = result.connected ? 1U : 0U;
    rec.acceptedStatus = result.acceptedStatus ? 1U : 0U;

    eccSafeCopy(rec.endpoint, sizeof(rec.endpoint), result.endpointName);
    eccSafeCopy(rec.host, sizeof(rec.host), result.host);

    selectSdBus();
    File file = SD.open("/netmetric.bin", FILE_WRITE);
    if (!file) {
        selectEthernetBus();
        return false;
    }

    size_t written = file.write(reinterpret_cast<const uint8_t*>(&rec), sizeof(ECCBinaryRecord));
    file.close();
    selectEthernetBus();
    return written == sizeof(ECCBinaryRecord);
}

bool EthernetConnectionChecker::saveResultText(const ECCProbeResult& result) {
    if (!_sdReady || !result.payloadPreview.hasValue()) {
        return false;
    }

    selectSdBus();
    File file = SD.open("/payloads.txt", FILE_WRITE);
    if (!file) {
        selectEthernetBus();
        return false;
    }

    file.print('#');
    file.print(result.checkedAtMs);
    file.print(' ');
    file.print(result.endpointName);
    file.print(" => ");
    file.println(result.payloadPreview.getValue());

    if (result.redirectLocation.hasValue()) {
        file.print("  location: ");
        file.println(result.redirectLocation.getValue());
    }

    file.close();
    selectEthernetBus();
    return true;
}

void EthernetConnectionChecker::handleClient(EthernetClient& client) {
    String requestLine = readLine(client, 1500UL);

    while (client.connected()) {
        String line = readLine(client, 1000UL);
        if (line.length() == 0) {
            break;
        }
    }

    if (requestLine.startsWith("GET /run")) {
        runChecks();
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /api/diag")) {
        sendHttp(client, 200, "application/json", diagnosticsJson());
    } else if (requestLine.startsWith("GET /api/results")) {
        sendHttp(client, 200, "application/json", resultsJson());
    } else if (requestLine.startsWith("GET /api/speed")) {
        sendHttp(client, 200, "application/json", speedJson());
    } else if (requestLine.startsWith("GET /api/devices")) {
        sendHttp(client, 200, "application/json", devicesJson());
    } else if (requestLine.startsWith("GET /add-endpoint")) {
        String n  = parseQueryParam(requestLine, "n");
        String h  = parseQueryParam(requestLine, "h");
        String p  = parseQueryParam(requestLine, "p");
        String pt = parseQueryParam(requestLine, "pt");
        String mn = parseQueryParam(requestLine, "mn");
        String mx = parseQueryParam(requestLine, "mx");
        if (n.length() > 0 && h.length() > 0) {
            addEndpoint(n, h,
                p.length() > 0 ? p : String("/"),
                pt.length() > 0 ? (uint16_t)pt.toInt() : 80,
                mn.length() > 0 ? (uint16_t)mn.toInt() : 200,
                mx.length() > 0 ? (uint16_t)mx.toInt() : 399);
        }
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /remove-endpoint")) {
        String n = parseQueryParam(requestLine, "n");
        if (n.length() > 0) removeEndpoint(n);
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /speed")) {
        runSpeedTest();
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /scan")) {
        String s  = parseQueryParam(requestLine, "s");
        String e  = parseQueryParam(requestLine, "e");
        String pt = parseQueryParam(requestLine, "pt");
        uint8_t startOct = s.length() > 0 ? (uint8_t)s.toInt() : 1;
        uint8_t endOct   = e.length() > 0 ? (uint8_t)e.toInt() : 30;
        uint16_t scanPort = pt.length() > 0 ? (uint16_t)pt.toInt() : 80;
        // Cap scan at 50 hosts per request to limit blocking time
        if ((int)endOct - (int)startOct > 50) endOct = startOct + 50;
        scanSubnet(startOct, endOct, scanPort);
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /reinit-sd")) {
        reinitSd(3);
        sendDashboard(client);
    } else if (requestLine.startsWith("GET /clear")) {
        _eventLog.clear();
        _results.clear();
        sendDashboard(client);
    } else {
        sendDashboard(client);
    }

    delay(1);
    client.stop();
}

String EthernetConnectionChecker::dashboardHtml() const {
    String html;
    html.reserve(5200);

    IPAddress localIp = localIP();
    IPAddress gateway = gatewayIP();
    IPAddress subnet = subnetMask();
    IPAddress dns = dnsIP();

    html += "<!doctype html><html><head><meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Ethernet Checker</title>";
    html += "<style>body{font-family:Verdana,sans-serif;margin:0;padding:16px;background:#f2f5f7;color:#1f2c34;}";
    html += ".card{background:#fff;padding:12px;border-radius:10px;box-shadow:0 2px 8px rgba(0,0,0,.08);margin-bottom:12px;}";
    html += "table{width:100%;border-collapse:collapse;font-size:13px;}th,td{padding:6px;border-bottom:1px solid #e7edf2;text-align:left;}";
    html += "a.btn{display:inline-block;margin-right:8px;padding:8px 12px;background:#0a6;color:#fff;text-decoration:none;border-radius:8px;}";
    html += "a.btn.secondary{background:#555;}";
    html += "</style></head><body>";

    html += "<div class='card'><h2>EthernetConnectionChecker</h2>";
    html += "<p><strong>IP:</strong> ";
    html += eccIpToString(localIp);
    html += " | <strong>SD:</strong> ";
    html += (_sdReady ? "Ready" : "Missing");
    html += " | <strong>Endpoints:</strong> ";
    html += _endpoints.size();
    html += "</p>";
    html += "<a class='btn' href='/run'>Run Checks</a>";
    html += "<a class='btn secondary' href='/reinit-sd'>Reinit SD</a>";
    html += "<a class='btn secondary' href='/clear'>Clear Runtime Data</a></div>";

    html += "<div class='card'><h3>Network Diagnostics</h3><table>";
    html += "<tr><th>Local IP</th><td>" + eccIpToString(localIp) + "</td></tr>";
    html += "<tr><th>Gateway</th><td>" + eccIpToString(gateway) + "</td></tr>";
    html += "<tr><th>Subnet</th><td>" + eccIpToString(subnet) + "</td></tr>";
    html += "<tr><th>DNS</th><td>" + eccIpToString(dns) + "</td></tr>";
    html += "<tr><th>Hardware</th><td>" + hardwareStatusString() + "</td></tr>";
    html += "<tr><th>Link</th><td>" + linkStatusString() + "</td></tr>";
    html += "</table><p><a class='btn secondary' href='/api/diag'>/api/diag</a> <a class='btn secondary' href='/api/results'>/api/results</a></p></div>";

    html += "<div class='card'><h3>Latest Results</h3><table><tr><th>Name</th><th>Status</th><th>Accepted</th><th>Latency(ms)</th><th>Host</th><th>Path</th><th>Notes</th><th>Location</th></tr>";
    for (size_t i = 0; i < _results.size(); ++i) {
        const ECCProbeResult& r = _results.getReference(i);
        html += "<tr><td>" + r.endpointName + "</td><td>";
        html += r.statusCode;
        html += r.acceptedStatus ? " OK" : " FAIL";
        html += "</td><td>";
        html += r.minAcceptedStatus;
        html += "-";
        html += r.maxAcceptedStatus;
        html += "</td><td>";
        html += r.latencyMs;
        html += "</td><td>" + r.host + "</td><td>" + r.path + "</td><td>";
        html += r.notes.getSingle();
        html += "</td><td>";
        if (r.redirectLocation.hasValue()) {
            html += r.redirectLocation.getValue();
        } else {
            html += "-";
        }
        html += "</td></tr>";
    }
    html += "</table></div>";

    html += "<div class='card'><h3>Event Log</h3><ul>";
    for (size_t i = 0; i < _eventLog.size(); ++i) {
        html += "<li>" + _eventLog.getReference(i) + "</li>";
    }
    html += "</ul></div>";

    html += "</body></html>";
    return html;
}

void EthernetConnectionChecker::sendDashboard(EthernetClient& client) const {
    IPAddress localIp = localIP();
    IPAddress gateway = gatewayIP();
    IPAddress subnet = subnetMask();
    IPAddress dns = dnsIP();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Connection: close");
    client.println();

    client.print("<!doctype html><html><head><meta charset='utf-8'>");
    client.print("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.print("<title>Ethernet Checker</title>");
    client.print("<style>body{font-family:Verdana,sans-serif;margin:0;padding:16px;background:#f2f5f7;color:#1f2c34;}");
    client.print(".card{background:#fff;padding:12px;border-radius:10px;box-shadow:0 2px 8px rgba(0,0,0,.08);margin-bottom:12px;}");
    client.print("table{width:100%;border-collapse:collapse;font-size:13px;}th,td{padding:6px;border-bottom:1px solid #e7edf2;text-align:left;}");
    client.print("a.btn{display:inline-block;margin-right:8px;padding:8px 12px;background:#0a6;color:#fff;text-decoration:none;border-radius:8px;}");
    client.print("a.btn.secondary{background:#555;}");
    client.print("</style></head><body>");

    client.print("<div class='card'><h2>EthernetConnectionChecker</h2><p><strong>IP:</strong> ");
    client.print(eccIpToString(localIp));
    client.print(" | <strong>SD:</strong> ");
    client.print(_sdReady ? "Ready" : "Missing");
    client.print(" | <strong>Endpoints:</strong> ");
    client.print(_endpoints.size());
    client.print(" | <strong>Devices:</strong> ");
    client.print(_devices.size());
    client.print("</p>");
    client.print("<a class='btn' href='/run'>Run Checks</a>");
    client.print("<a class='btn' href='/speed'>Speed Test</a>");
    client.print("<a class='btn secondary' href='/reinit-sd'>Reinit SD</a>");
    client.print("<a class='btn secondary' href='/clear'>Clear Data</a></div>");

    client.print("<div class='card'><h3>Network Diagnostics</h3><table>");
    client.print("<tr><th>Local IP</th><td>");
    client.print(eccIpToString(localIp));
    client.print("</td></tr><tr><th>Gateway</th><td>");
    client.print(eccIpToString(gateway));
    client.print("</td></tr><tr><th>Subnet</th><td>");
    client.print(eccIpToString(subnet));
    client.print("</td></tr><tr><th>DNS</th><td>");
    client.print(eccIpToString(dns));
    client.print("</td></tr><tr><th>Hardware</th><td>");
    client.print(hardwareStatusString());
    client.print("</td></tr><tr><th>Link</th><td>");
    client.print(linkStatusString());
    client.print("</td></tr></table><p>");
    client.print("<a class='btn secondary' href='/api/diag'>/api/diag</a> ");
    client.print("<a class='btn secondary' href='/api/results'>/api/results</a> ");
    client.print("<a class='btn secondary' href='/api/speed'>/api/speed</a> ");
    client.print("<a class='btn secondary' href='/api/devices'>/api/devices</a>");
    client.print("</p></div>");

    client.print("<div class='card'><h3>Latest Results</h3><table><tr><th>Name</th><th>Status</th><th>Accepted</th><th>Latency(ms)</th><th>Host</th><th>Path</th><th>Notes</th><th>Location</th><th></th></tr>");
    for (size_t i = 0; i < _results.size(); ++i) {
        const ECCProbeResult& r = _results.getReference(i);
        client.print("<tr><td>");
        client.print(r.endpointName);
        client.print("</td><td>");
        client.print(r.statusCode);
        client.print(r.acceptedStatus ? " OK" : " FAIL");
        client.print("</td><td>");
        client.print(r.minAcceptedStatus);
        client.print("-");
        client.print(r.maxAcceptedStatus);
        client.print("</td><td>");
        client.print(r.latencyMs);
        client.print("</td><td>");
        client.print(r.host);
        client.print("</td><td>");
        client.print(r.path);
        client.print("</td><td>");
        client.print(r.notes.getSingle());
        client.print("</td><td>");
        if (r.redirectLocation.hasValue()) {
            client.print(r.redirectLocation.getValue());
        } else {
            client.print("-");
        }
        client.print("</td><td><a href='/remove-endpoint?n=");
        client.print(r.endpointName);
        client.print("'>&#10005;</a></td></tr>");
    }
    client.print("</table></div>");

    // Speed Test card
    client.print("<div class='card'><h3>Speed Test</h3>");
    client.print("<a class='btn' href='/speed'>Run Speed Test</a> ");
    client.print("<a class='btn secondary' href='/api/speed'>JSON</a>");
    if (_lastSpeedResult.success) {
        client.print("<p>Last: <strong>");
        client.print(_lastSpeedResult.downloadBps);
        client.print(" B/s</strong> &mdash; ");
        client.print(_lastSpeedResult.bytesReceived);
        client.print(" bytes in ");
        client.print(_lastSpeedResult.durationMs);
        client.print("ms (connect ");
        client.print(_lastSpeedResult.connectMs);
        client.print("ms)</p>");
    }
    client.print("</div>");

    // Local Devices card
    if (_devices.size() > 0) {
        client.print("<div class='card'><h3>Local Devices (");
        client.print(_devices.size());
        client.print(")</h3><table><tr><th>Name</th><th>Host</th><th>MAC</th><th>Port</th><th>Status</th><th>Latency(ms)</th></tr>");
        for (size_t i = 0; i < _devices.size(); ++i) {
            const ECCNetDevice& dev = _devices.getReference(i);
            client.print("<tr><td>"); client.print(dev.name);
            client.print("</td><td>"); client.print(dev.host);
            client.print("</td><td>"); client.print(dev.macAddress.length() > 0 ? dev.macAddress : "-");
            client.print("</td><td>"); client.print(dev.port);
            client.print("</td><td>"); client.print(dev.isUp ? "UP" : "DOWN");
            client.print("</td><td>"); client.print(dev.latencyMs);
            client.print("</td></tr>");
        }
        client.print("</table></div>");
    }

    // Add Endpoint form card
    client.print("<div class='card'><h3>Add Endpoint</h3>");
    client.print("<form action='/add-endpoint'>");
    client.print("<table><tr>");
    client.print("<td>Name</td><td><input name='n' size='10'></td>");
    client.print("<td>Host/IP</td><td><input name='h' size='18'></td>");
    client.print("<td>Path</td><td><input name='p' value='/' size='8'></td></tr><tr>");
    client.print("<td>Port</td><td><input name='pt' value='80' size='5'></td>");
    client.print("<td>Min Status</td><td><input name='mn' value='200' size='5'></td>");
    client.print("<td>Max Status</td><td><input name='mx' value='399' size='5'></td>");
    client.print("<td><input type='submit' value='Add'></td></tr></table></form></div>");

    // Subnet Scanner card
    client.print("<div class='card'><h3>Subnet Scanner</h3>");
    client.print("<p>TCP-connects to each IP in range to find active hosts. Capped at 50 hosts.</p>");
    client.print("<form action='/scan'>");
    client.print("Start octet: <input name='s' value='1' size='3'> ");
    client.print("End octet: <input name='e' value='30' size='3'> ");
    client.print("Port: <input name='pt' value='80' size='5'> ");
    client.print("<input type='submit' value='Scan (may take ~30s)'></form></div>");

    client.print("<div class='card'><h3>Event Log</h3><ul>");
    for (size_t i = 0; i < _eventLog.size(); ++i) {
        client.print("<li>");
        client.print(_eventLog.getReference(i));
        client.print("</li>");
    }
    client.print("</ul></div></body></html>");
}

void EthernetConnectionChecker::sendHttp(EthernetClient& client, int code, const String& contentType, const String& body) const {
    client.print("HTTP/1.1 ");
    client.print(code);
    client.println(" OK");
    client.print("Content-Type: ");
    client.println(contentType);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(body.length());
    client.println();
    client.print(body);
}

String EthernetConnectionChecker::linkStatusString() const {
    if (!_ethernetReady) {
        return String("not_initialized");
    }
    EthernetLinkStatus link = Ethernet.linkStatus();
    if (link == LinkON) {
        return String("up");
    }
    if (link == LinkOFF) {
        return String("down");
    }
    return String("unknown");
}

String EthernetConnectionChecker::hardwareStatusString() const {
    if (!_ethernetReady) {
        return String("not_initialized");
    }
    EthernetHardwareStatus hw = Ethernet.hardwareStatus();
    if (hw == EthernetNoHardware) {
        return String("no_hardware");
    }
    if (hw == EthernetW5100) {
        return String("W5100");
    }
    if (hw == EthernetW5200) {
        return String("W5200");
    }
    if (hw == EthernetW5500) {
        return String("W5500");
    }
    return String("unknown");
}

String EthernetConnectionChecker::diagnosticsJson() const {
    String json;
    json.reserve(720);

    IPAddress localIp = localIP();
    IPAddress gateway = gatewayIP();
    IPAddress subnet = subnetMask();
    IPAddress dns = dnsIP();

    json += "{";
    json += "\"ip\":\"" + eccIpToString(localIp) + "\",";
    json += "\"gateway\":\"" + eccIpToString(gateway) + "\",";
    json += "\"subnet\":\"" + eccIpToString(subnet) + "\",";
    json += "\"dns\":\"" + eccIpToString(dns) + "\",";
    json += "\"ethernetReady\":" + String(_ethernetReady ? "true" : "false") + ",";
    json += "\"sdReady\":" + String(_sdReady ? "true" : "false") + ",";
    json += "\"hardware\":\"" + hardwareStatusString() + "\",";
    json += "\"link\":\"" + linkStatusString() + "\",";
    json += "\"endpointCount\":" + String(_endpoints.size()) + ",";
    json += "\"resultCount\":" + String(_results.size()) + ",";
    json += "\"lastCheckAtMs\":" + String(_lastCheckAt) + ",";
    json += "\"lastError\":\"" + eccJsonEscape(_lastError.hasValue() ? _lastError.getValue() : String("")) + "\"";
    json += "}";

    return json;
}

String EthernetConnectionChecker::resultsJson() const {
    String json;
    json.reserve(2200);

    json += "{\"results\":[";
    for (size_t i = 0; i < _results.size(); ++i) {
        const ECCProbeResult& r = _results.getReference(i);

        if (i > 0) {
            json += ",";
        }

        json += "{";
        json += "\"name\":\"" + eccJsonEscape(r.endpointName) + "\",";
        json += "\"host\":\"" + eccJsonEscape(r.host) + "\",";
        json += "\"path\":\"" + eccJsonEscape(r.path) + "\",";
        json += "\"status\":" + String(r.statusCode) + ",";
        json += "\"accepted\":" + String(r.acceptedStatus ? "true" : "false") + ",";
        json += "\"acceptedMin\":" + String(r.minAcceptedStatus) + ",";
        json += "\"acceptedMax\":" + String(r.maxAcceptedStatus) + ",";
        json += "\"connected\":" + String(r.connected ? "true" : "false") + ",";
        json += "\"latencyMs\":" + String(r.latencyMs) + ",";
        json += "\"checkedAtMs\":" + String(r.checkedAtMs) + ",";
        json += "\"notes\":\"" + eccJsonEscape(r.notes.getSingle()) + "\",";
        json += "\"redirectLocation\":\"" + eccJsonEscape(r.redirectLocation.hasValue() ? r.redirectLocation.getValue() : String("")) + "\"";
        json += "}";
    }
    json += "]}";

    return json;
}

String EthernetConnectionChecker::dashboardHtmlSnapshot() const {
    return dashboardHtml();
}
