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
      _endpoints(ArrayList<ECCEndpoint>::DYNAMIC2, 8),
      _results(ArrayList<ECCProbeResult>::DYNAMIC2, 8),
      _eventLog(ArrayList<String>::DYNAMIC2, 16),
      _lastError() {}

bool EthernetConnectionChecker::begin(const byte macAddress[6], unsigned long dhcpTimeoutMs) {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);

    pinMode(_ethernetCSPin, OUTPUT);
    pinMode(_sdCSPin, OUTPUT);
    digitalWrite(_sdCSPin, HIGH);

    SPI.begin();

    Ethernet.init(_ethernetCSPin);
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

    _sdReady = SD.begin(_sdCSPin);
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
    digitalWrite(_sdCSPin, HIGH);

    SPI.begin();

    Ethernet.init(_ethernetCSPin);
    Ethernet.begin(const_cast<byte*>(macAddress), ip, dns, gateway, subnet);
    delay(350);

    _server.begin();
    _ethernetReady = true;
    _sdReady = SD.begin(_sdCSPin);

    if (!_sdReady) {
        _lastError = String("SD init failed");
        recordEvent(_lastError.getValue());
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
        bool ok = runSingleCheck(_endpoints.get(i), result);
        if (!ok) {
            allHealthy = false;
        }

        _results.add(result);
        saveResultCsv(result);
        saveResultBinary(result);
        saveResultText(result);
    }

    _lastCheckAt = millis();

    if (allHealthy) {
        flashCode(1, 180, 80);
    } else {
        flashCode(2, 80, 120);
    }

    return allHealthy;
}

void EthernetConnectionChecker::loop() {
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
    return Ethernet.localIP();
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

    if (_eventLog.size() > 50) {
        ArrayList<String> trimmed(ArrayList<String>::DYNAMIC2, 50);
        for (size_t i = _eventLog.size() - 50; i < _eventLog.size(); ++i) {
            trimmed.add(_eventLog.get(i));
        }
        _eventLog = trimmed;
    }
}

bool EthernetConnectionChecker::runSingleCheck(const ECCEndpoint& endpoint, ECCProbeResult& resultOut) {
    EthernetClient client;

    resultOut.endpointName = endpoint.name;
    resultOut.host = endpoint.host;
    resultOut.path = endpoint.path;
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

    while (client.connected()) {
        String headerLine = readLine(client, 1200UL);
        if (headerLine.length() == 0) {
            break;
        }
    }

    String payloadPreview = readBodyPreview(client, 180, 2400UL);

    resultOut.statusCode = code;
    resultOut.latencyMs = millis() - startMs;
    resultOut.acceptedStatus = (code >= static_cast<int>(endpoint.minAcceptedStatus) && code <= static_cast<int>(endpoint.maxAcceptedStatus));
    resultOut.notes.setSingle(resultOut.acceptedStatus ? String("ok") : String("status_out_of_range"));

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

    File file = SD.open("/netlog.csv", FILE_WRITE);
    if (!file) {
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
    file.print(result.connected ? 1 : 0);
    file.print(',');
    file.println(result.acceptedStatus ? 1 : 0);
    file.close();
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

    File file = SD.open("/netmetric.bin", FILE_WRITE);
    if (!file) {
        return false;
    }

    size_t written = file.write(reinterpret_cast<const uint8_t*>(&rec), sizeof(ECCBinaryRecord));
    file.close();
    return written == sizeof(ECCBinaryRecord);
}

bool EthernetConnectionChecker::saveResultText(const ECCProbeResult& result) {
    if (!_sdReady || !result.payloadPreview.hasValue()) {
        return false;
    }

    File file = SD.open("/payloads.txt", FILE_WRITE);
    if (!file) {
        return false;
    }

    file.print('#');
    file.print(result.checkedAtMs);
    file.print(' ');
    file.print(result.endpointName);
    file.print(" => ");
    file.println(result.payloadPreview.getValue());
    file.close();
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
        sendHttp(client, 200, "text/html", dashboardHtml());
    } else if (requestLine.startsWith("GET /clear")) {
        _eventLog.clear();
        _results.clear();
        sendHttp(client, 200, "text/html", dashboardHtml());
    } else {
        sendHttp(client, 200, "text/html", dashboardHtml());
    }

    delay(1);
    client.stop();
}

String EthernetConnectionChecker::dashboardHtml() const {
    String html;
    html.reserve(3000);

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
    html += eccIpToString(Ethernet.localIP());
    html += " | <strong>SD:</strong> ";
    html += (_sdReady ? "Ready" : "Missing");
    html += " | <strong>Endpoints:</strong> ";
    html += _endpoints.size();
    html += "</p>";
    html += "<a class='btn' href='/run'>Run Checks</a>";
    html += "<a class='btn secondary' href='/clear'>Clear Runtime Data</a></div>";

    html += "<div class='card'><h3>Latest Results</h3><table><tr><th>Name</th><th>Status</th><th>Latency(ms)</th><th>Host</th><th>Path</th></tr>";
    for (size_t i = 0; i < _results.size(); ++i) {
        const ECCProbeResult& r = _results.get(i);
        html += "<tr><td>" + r.endpointName + "</td><td>";
        html += r.statusCode;
        html += r.acceptedStatus ? " OK" : " FAIL";
        html += "</td><td>";
        html += r.latencyMs;
        html += "</td><td>" + r.host + "</td><td>" + r.path + "</td></tr>";
    }
    html += "</table></div>";

    html += "<div class='card'><h3>Event Log</h3><ul>";
    for (size_t i = 0; i < _eventLog.size(); ++i) {
        html += "<li>" + _eventLog.get(i) + "</li>";
    }
    html += "</ul></div>";

    html += "</body></html>";
    return html;
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
