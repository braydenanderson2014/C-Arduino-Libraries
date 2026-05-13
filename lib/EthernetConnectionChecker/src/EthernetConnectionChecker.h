#ifndef ETHERNET_CONNECTION_CHECKER_H
#define ETHERNET_CONNECTION_CHECKER_H

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#include <ArrayList.h>
#include <Optional.h>
#include <Variant.h>

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
    bool connected;
    bool acceptedStatus;
    int statusCode;
    unsigned long latencyMs;
    unsigned long checkedAtMs;
    Optional<String> payloadPreview;
    Variant<String> notes;

    ECCProbeResult()
        : endpointName(""),
          host(""),
          path(""),
          connected(false),
          acceptedStatus(false),
          statusCode(-1),
          latencyMs(0),
          checkedAtMs(0),
          payloadPreview(),
          notes(String("")) {}
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

    bool runChecks();
    void loop();

    const ArrayList<ECCProbeResult>& getResults() const;
    const ArrayList<String>& getEventLog() const;
    Optional<String> getLastError() const;

    bool isEthernetReady() const;
    bool isSdReady() const;
    IPAddress localIP() const;

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

    Optional<String> _lastError;

    void flashCode(uint8_t count, uint16_t onMs = 120, uint16_t offMs = 120);
    void recordEvent(const String& message);

    bool runSingleCheck(const ECCEndpoint& endpoint, ECCProbeResult& resultOut);

    int parseStatusCode(const String& statusLine) const;
    String readLine(EthernetClient& client, unsigned long timeoutMs = 1500UL) const;
    String readBodyPreview(EthernetClient& client, size_t maxChars = 200, unsigned long timeoutMs = 2200UL) const;

    bool saveResultCsv(const ECCProbeResult& result);
    bool saveResultBinary(const ECCProbeResult& result);
    bool saveResultText(const ECCProbeResult& result);

    void handleClient(EthernetClient& client);
    String dashboardHtml() const;
    void sendHttp(EthernetClient& client, int code, const String& contentType, const String& body) const;
};

#endif
