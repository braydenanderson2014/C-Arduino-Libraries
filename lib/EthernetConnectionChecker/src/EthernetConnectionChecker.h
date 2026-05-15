#ifndef ETHERNET_CONNECTION_CHECKER_H
#define ETHERNET_CONNECTION_CHECKER_H

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#ifndef ECC_ENABLE_LOCAL_SKINNY_ARRAYLIST
#define ECC_ENABLE_LOCAL_SKINNY_ARRAYLIST 1
#endif

#if ECC_ENABLE_LOCAL_SKINNY_ARRAYLIST
#define ECC_DEFINED_SKINNYARRAY_FOR_ECC
#define SkinnyArray
#endif

#include <ArrayList.h>

#ifdef ECC_DEFINED_SKINNYARRAY_FOR_ECC
#undef SkinnyArray
#undef ECC_DEFINED_SKINNYARRAY_FOR_ECC
#endif
#include <Optional.h>

struct ECCNoteField {
    String value;

    ECCNoteField()
        : value("") {}

    ECCNoteField(const String& initialValue)
        : value(initialValue) {}

    void setSingle(const String& noteValue) {
        value = noteValue;
    }

    const String& getSingle() const {
        return value;
    }
};

struct ECCEndpoint {
    String name;
    String host;
    String path;
    uint16_t port;
    uint16_t minAcceptedStatus;
    uint16_t maxAcceptedStatus;

    ECCEndpoint()
        : name(""), host(""), path("/"), port(80), minAcceptedStatus(200), maxAcceptedStatus(399) {}

    ECCEndpoint(
        const String& endpointName,
        const String& endpointHost,
        const String& endpointPath,
        uint16_t endpointPort,
        uint16_t minStatus,
        uint16_t maxStatus)
        : name(endpointName),
          host(endpointHost),
          path(endpointPath),
          port(endpointPort),
          minAcceptedStatus(minStatus),
          maxAcceptedStatus(maxStatus) {}
};

struct ECCProbeResult {
    String endpointName;
    String host;
    String path;
    uint16_t minAcceptedStatus;
    uint16_t maxAcceptedStatus;
    bool connected;
    bool acceptedStatus;
    int statusCode;
    unsigned long latencyMs;
    unsigned long checkedAtMs;
    Optional<String> payloadPreview;
    Optional<String> redirectLocation;
    ECCNoteField notes;

    ECCProbeResult()
        : endpointName(""),
          host(""),
          path(""),
                    minAcceptedStatus(200),
                    maxAcceptedStatus(399),
          connected(false),
          acceptedStatus(false),
          statusCode(-1),
          latencyMs(0),
          checkedAtMs(0),
          payloadPreview(),
                    redirectLocation(),
          notes(String("")) {}
};

struct ECCNetDevice {
    String name;
    String host;
    String macAddress;
    uint16_t port;
    bool isUp;
    unsigned long latencyMs;
    unsigned long lastSeenMs;

    ECCNetDevice()
        : name(""), host(""), macAddress(""), port(80),
          isUp(false), latencyMs(0), lastSeenMs(0) {}

    ECCNetDevice(const String& n, const String& h, uint16_t p, const String& mac = "")
        : name(n), host(h), macAddress(mac), port(p),
          isUp(false), latencyMs(0), lastSeenMs(0) {}

    bool operator==(const ECCNetDevice& o) const {
        return name == o.name && host == o.host;
    }
};

struct ECCSpeedResult {
    bool success;
    unsigned long connectMs;
    unsigned long durationMs;
    unsigned long bytesReceived;
    unsigned long downloadBps;

    ECCSpeedResult()
        : success(false), connectMs(0), durationMs(0),
          bytesReceived(0), downloadBps(0) {}
};

class EthernetConnectionChecker {
public:
    EthernetConnectionChecker(
        uint8_t ledPin = LED_BUILTIN,
        uint8_t ethernetCSPin = 10,
        uint8_t sdCSPin = 4,
        uint16_t webPort = 80);

    bool begin(const byte macAddress[6], unsigned long dhcpTimeoutMs = 12000UL);
    bool beginStatic(const byte macAddress[6], const IPAddress& ip, const IPAddress& dns, const IPAddress& gateway, const IPAddress& subnet);

    void setCheckInterval(unsigned long intervalMs);
    void enableAutoChecks(bool enabled);

    void addEndpoint(
        const String& name,
        const String& host,
        const String& path = "/",
        uint16_t port = 80,
        uint16_t minAcceptedStatus = 200,
        uint16_t maxAcceptedStatus = 399);

    void addReachabilityEndpoint(
        const String& name,
        const String& host,
        const String& path = "/",
        uint16_t port = 80);

    void addGatewayEndpoint(
        const String& name = "Gateway",
        const String& path = "/",
        uint16_t port = 80,
        uint16_t minAcceptedStatus = 100,
        uint16_t maxAcceptedStatus = 599);

    bool runChecks();
    void loop();
    bool reinitSd(uint8_t attempts = 3);

    const ArrayList<ECCProbeResult>& getResults() const;
    const ArrayList<String>& getEventLog() const;
    Optional<String> getLastError() const;

    bool isEthernetReady() const;
    bool isSdReady() const;
    IPAddress localIP() const;
    IPAddress gatewayIP() const;
    IPAddress dnsIP() const;
    IPAddress subnetMask() const;

    String diagnosticsJson() const;
    String resultsJson() const;
    String dashboardHtmlSnapshot() const;

    // Local device tracking
    void addLocalDevice(
        const String& name,
        const String& hostOrIp,
        uint16_t port = 80,
        const String& macAddress = "");
    bool removeEndpoint(const String& name);
    const ArrayList<ECCNetDevice>& getDevices() const;
    String devicesJson() const;

    // Speed test
    ECCSpeedResult runSpeedTest(
        const String& host = "speed.cloudflare.com",
        const String& path = "/__down?bytes=51200",
        uint16_t port = 80);
    String speedJson() const;

    // Subnet scanner (TCP-based, blocks during scan)
    void scanSubnet(
        uint8_t startOctet = 1,
        uint8_t endOctet = 50,
        uint16_t port = 80);

    // Serial console: call from loop(); type HELP for commands
    void processSerial();

private:
    uint8_t _ledPin;
    uint8_t _ethernetCSPin;
    uint8_t _sdCSPin;

    EthernetServer _server;

    bool _ethernetReady;
    bool _sdReady;
    bool _autoChecks;

    unsigned long _lastCheckAt;
    unsigned long _checkIntervalMs;

    ArrayList<ECCEndpoint> _endpoints;
    ArrayList<ECCProbeResult> _results;
    ArrayList<String> _eventLog;
    ArrayList<ECCNetDevice> _devices;

    ECCSpeedResult _lastSpeedResult;

    char _serialBuf[64];
    uint8_t _serialBufLen;

    Optional<String> _lastError;

    void flashCode(uint8_t count, uint16_t onMs = 120, uint16_t offMs = 120);
    void recordEvent(const String& message);
    void selectEthernetBus() const;
    void selectSdBus() const;

    bool runSingleCheck(const ECCEndpoint& endpoint, ECCProbeResult& resultOut);

    int parseStatusCode(const String& statusLine) const;
    String readLine(EthernetClient& client, unsigned long timeoutMs = 1500UL) const;
    String readBodyPreview(EthernetClient& client, size_t maxChars = 200, unsigned long timeoutMs = 2200UL) const;

    bool saveResultCsv(const ECCProbeResult& result);
    bool saveResultBinary(const ECCProbeResult& result);
    bool saveResultText(const ECCProbeResult& result);
    String linkStatusString() const;
    String hardwareStatusString() const;

    void handleClient(EthernetClient& client);
    void sendDashboard(EthernetClient& client) const;
    String dashboardHtml() const;
    void sendHttp(EthernetClient& client, int code, const String& contentType, const String& body) const;

    String parseQueryParam(const String& request, const String& key) const;
    void handleSerialCommand(const String& cmd);
    void syncDeviceStatus();
};

#endif
