#ifndef ETHERNET_MACH2_CONNECTION_CHECKER_H
#define ETHERNET_MACH2_CONNECTION_CHECKER_H

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
#include "../../UnoQFileTransferClient.h"

// Define EM2_USE_BRIDGE before including this header to enable Bridge-backed
// networking and logging. Boards without Arduino_RouterBridge are unaffected.
#ifdef EM2_USE_BRIDGE
  #include <Arduino_RouterBridge.h>
  #ifndef EM2_BRIDGE_CHUNK_SIZE
    #define EM2_BRIDGE_CHUNK_SIZE 128
  #endif
#endif

#if defined(__has_include) && __has_include(<FS.h>)
    #include <FS.h>
#else
    namespace fs {
        class FS {
        public:
            virtual ~FS() = default;
            virtual File open(const char* path, const char* mode) = 0;
            virtual bool exists(const char* path) = 0;
            virtual bool remove(const char* path) = 0;
        };
    }
#endif

#ifndef EM2_ENABLE_LOCAL_SKINNY_ARRAYLIST
#define EM2_ENABLE_LOCAL_SKINNY_ARRAYLIST 1
#endif

#if EM2_ENABLE_LOCAL_SKINNY_ARRAYLIST
#define EM2_DEFINED_SKINNYARRAY
#define SkinnyArray
#endif

#include <ArrayList.h>

#ifdef EM2_DEFINED_SKINNYARRAY
#undef SkinnyArray
#undef EM2_DEFINED_SKINNYARRAY
#endif

struct EM2ServerTarget {
    String host;
    uint16_t port;
    String basePath;

    EM2ServerTarget()
        : host(""), port(8080), basePath("/api/v1") {}

    EM2ServerTarget(const String& serverHost, uint16_t serverPort,
        const String& serverBasePath = "/api/v1")
        : host(serverHost), port(serverPort), basePath(serverBasePath) {}
};

struct EM2Probe {
    String name;
    String host;
    String path;
    uint16_t port;
    uint16_t minAcceptedStatus;
    uint16_t maxAcceptedStatus;

    EM2Probe()
        : name(""), host(""), path("/"), port(80),
          minAcceptedStatus(200), maxAcceptedStatus(399) {}

    EM2Probe(const String& probeName, const String& probeHost, const String& probePath,
        uint16_t probePort, uint16_t minStatus, uint16_t maxStatus)
        : name(probeName), host(probeHost), path(probePath), port(probePort),
          minAcceptedStatus(minStatus), maxAcceptedStatus(maxStatus) {}
};

struct EM2ProbeResult {
    String name;
    String host;
    String path;
    int statusCode;
    bool connected;
    bool accepted;
    unsigned long latencyMs;
    unsigned long checkedAtMs;
    String note;

    EM2ProbeResult()
        : name(""), host(""), path("/"), statusCode(-1), connected(false),
          accepted(false), latencyMs(0), checkedAtMs(0), note("") {}
};

struct EM2SpeedResult {
    bool success;
    unsigned long connectMs;
    unsigned long durationMs;
    unsigned long bytesReceived;
    unsigned long downloadBps;
    String host;
    String path;
    uint16_t port;

    EM2SpeedResult()
        : success(false), connectMs(0), durationMs(0), bytesReceived(0),
          downloadBps(0), host(""), path(""), port(80) {}
};

struct EM2NetworkDevice {
    String host;
    uint16_t port;
    bool isUp;
    unsigned long latencyMs;
    unsigned long lastSeenMs;

    EM2NetworkDevice()
        : host(""), port(80), isUp(false), latencyMs(0), lastSeenMs(0) {}
};

struct EM2TraceHop {
    String label;
    String target;
    uint16_t port;
    int statusCode;
    bool connected;
    bool accepted;
    unsigned long latencyMs;
    String note;

    EM2TraceHop()
        : label(""), target(""), port(80), statusCode(-1), connected(false),
          accepted(false), latencyMs(0), note("") {}
};

class EthernetMach2ConnectionChecker {
public:
    explicit EthernetMach2ConnectionChecker(uint8_t ethernetCSPin = 10, uint8_t sdCSPin = 4);

    bool begin(const byte macAddress[6], const EM2ServerTarget& server,
        unsigned long dhcpTimeoutMs = 12000UL);
    bool beginStatic(const byte macAddress[6], const IPAddress& ip, const IPAddress& dns,
        const IPAddress& gateway, const IPAddress& subnet, const EM2ServerTarget& server);

    void setAgentId(const String& agentId);
    const String& getAgentId() const;

    void setServerTarget(const EM2ServerTarget& server);
    const EM2ServerTarget& getServerTarget() const;

    void setCheckInterval(unsigned long intervalMs);
    void setHeartbeatInterval(unsigned long intervalMs);
    void setControlPollInterval(unsigned long intervalMs);
    void enableAutoChecks(bool enabled);

    void addProbe(const String& name, const String& host, const String& path = "/",
        uint16_t port = 80, uint16_t minAcceptedStatus = 200,
        uint16_t maxAcceptedStatus = 399);
    void clearProbes();

    EM2SpeedResult runSpeedTest(const String& host = "speed.cloudflare.com",
        const String& path = "/__down?bytes=51200", uint16_t port = 80);
    void scanSubnet(uint8_t startOctet = 1, uint8_t endOctet = 30, uint16_t port = 80);
    void runPathTrace(const String& internetHost = "connectivitycheck.gstatic.com",
        const String& internetPath = "/generate_204", uint16_t port = 80);

    bool registerAgent();
    bool runChecks();
    bool sendHeartbeat();
    bool sendResults();
    bool sendNetworkSnapshot();
    bool fetchConfig();
    bool isServerReachable(unsigned long timeoutMs = 1500UL);
    bool pollServerControl();
    void loop();

    const ArrayList<EM2ProbeResult>& getResults() const;
    const ArrayList<EM2NetworkDevice>& getDiscoveredDevices() const;
    const ArrayList<EM2TraceHop>& getPathTrace() const;
    const ArrayList<String>& getEventLog() const;
    const EM2SpeedResult& getLastSpeedResult() const;

    IPAddress localIP() const;
    IPAddress gatewayIP() const;
    IPAddress dnsIP() const;
    IPAddress subnetMask() const;
    bool isEthernetReady() const;

    String diagnosticsJson() const;
    String networkSnapshotJson() const;
    String registrationJson() const;
    String heartbeatJson() const;
    String resultsJson() const;
    bool uploadSnapshotsToUnoQ(UnoQFileTransferClient& client, const String& basePath = "/em2");
    bool saveSnapshotsToFilesystem(fs::FS& filesystem, const String& basePath = "/em2");

#ifdef EM2_USE_BRIDGE
    // Bridge mode: skip Ethernet init; use Python container for networking and logging.
    bool beginBridge(const EM2ServerTarget& server);
    // Run probes via Bridge net_ping / net_check — no Ethernet hardware required.
    bool runChecksBridge();
    // Write all snapshot JSONs to the Python container filesystem.
    bool uploadSnapshotsToBridge(const String& basePath = "em2");
    // Push heartbeat JSON via Bridge tm_record for lightweight keep-alive logging.
    bool sendHeartbeatBridge();
#endif

    const String& getLastServerResponse() const;
    const String& getLastError() const;

private:
    uint8_t _ethernetCSPin;
    uint8_t _sdCSPin;
    bool _ethernetReady;
    bool _autoChecks;

#ifdef EM2_USE_BRIDGE
    bool _bridgeMode = false;
#endif

    unsigned long _checkIntervalMs;
    unsigned long _heartbeatIntervalMs;
    unsigned long _controlPollIntervalMs;
    unsigned long _lastCheckAt;
    unsigned long _lastHeartbeatAt;
    unsigned long _lastControlPollAt;

    EM2ServerTarget _serverTarget;
    String _agentId;
    String _lastServerResponse;
    String _lastError;

    ArrayList<EM2Probe> _probes;
    ArrayList<EM2ProbeResult> _results;
    ArrayList<EM2NetworkDevice> _devices;
    ArrayList<EM2TraceHop> _pathTrace;
    ArrayList<String> _eventLog;
    EM2SpeedResult _lastSpeedResult;

    bool runSingleProbe(const EM2Probe& probe, EM2ProbeResult& resultOut);
    bool postJson(const String& path, const String& body);
    bool getText(const String& path, String& bodyOut);
    bool completeControlCommand(long commandId, const String& status, const String& summary);
    String buildPath(const String& suffix) const;
    void recordEvent(const String& message);
    void prepareNetworkHardware();
    void updateOrAddDevice(const String& host, uint16_t port, bool isUp,
        unsigned long latencyMs, unsigned long lastSeenMs);
    void addTraceHop(const String& label, const String& host, const String& path,
        uint16_t port, uint16_t minAcceptedStatus, uint16_t maxAcceptedStatus);
    String linkStatusString() const;
    String hardwareStatusString() const;
    String jsonStringValue(const String& json, const char* key) const;
    long jsonLongValue(const String& json, const char* key, long defaultValue) const;
    int parseStatusCode(const String& statusLine) const;
    String readLine(EthernetClient& client, unsigned long timeoutMs = 1500UL) const;
};

#endif
