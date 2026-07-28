#ifndef HOST_ARDUINO_SIM_ETHERNET_H
#define HOST_ARDUINO_SIM_ETHERNET_H

#include "Arduino.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

class IPAddress {
public:
    IPAddress() : _address{0, 0, 0, 0} {}
    IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) : _address{a, b, c, d} {}

    uint8_t operator[](size_t index) const {
        return index < 4 ? _address[index] : 0;
    }

    uint8_t& operator[](size_t index) {
        if (index >= 4) {
            static uint8_t dummy = 0;
            return dummy;
        }
        return _address[index];
    }

private:
    uint8_t _address[4];
};

enum EthernetLinkStatus {
    Unknown,
    LinkON,
    LinkOFF,
};

enum EthernetHardwareStatus {
    EthernetNoHardware,
    EthernetW5100,
    EthernetW5200,
    EthernetW5500,
};

class EthernetClient {
public:
    EthernetClient() : _connected(false) {}

    bool connect(const char*, uint16_t) {
        _connected = true;
        return true;
    }

    bool connect(const IPAddress&, uint16_t) {
        _connected = true;
        return true;
    }

    int available() const {
        return 0;
    }

    int read() {
        return -1;
    }

    int read(uint8_t*, size_t) {
        return 0;
    }

    size_t write(uint8_t) {
        return 1;
    }

    size_t write(const uint8_t*, size_t size) {
        return size;
    }

    size_t print(const String& value) {
        return value.length();
    }

    size_t print(const char* value) {
        return value ? std::strlen(value) : 0;
    }

    size_t print(char) {
        return 1;
    }

    size_t print(int) {
        return 1;
    }

    size_t print(unsigned long) {
        return 1;
    }

    size_t println() {
        return 2;
    }

    size_t println(const String& value) {
        return value.length() + 2;
    }

    size_t println(const char* value) {
        return (value ? std::strlen(value) : 0) + 2;
    }

    bool connected() const {
        return _connected;
    }

    void stop() {
        _connected = false;
    }

    explicit operator bool() const {
        return true;
    }

private:
    bool _connected;
};

class EthernetServer {
public:
    explicit EthernetServer(uint16_t) {}

    void begin() {}

    EthernetClient available() {
        return EthernetClient();
    }
};

class EthernetClass {
public:
    EthernetClass()
        : _localIp(192, 168, 1, 200),
          _dnsIp(8, 8, 8, 8),
          _gatewayIp(192, 168, 1, 1),
          _subnetIp(255, 255, 255, 0),
          _linkStatus(LinkON),
          _hardwareStatus(EthernetW5500) {}

    void init(uint8_t) {}

    int begin(uint8_t*, unsigned long = 0) {
        return 1;
    }

    void begin(uint8_t*, const IPAddress& ip, const IPAddress& dns, const IPAddress& gateway, const IPAddress& subnet) {
        _localIp = ip;
        _dnsIp = dns;
        _gatewayIp = gateway;
        _subnetIp = subnet;
    }

    IPAddress localIP() const {
        return _localIp;
    }

    IPAddress dnsServerIP() const {
        return _dnsIp;
    }

    IPAddress gatewayIP() const {
        return _gatewayIp;
    }

    IPAddress subnetMask() const {
        return _subnetIp;
    }

    void setRetransmissionTimeout(uint16_t) {}

    void setRetransmissionCount(uint8_t) {}

    EthernetLinkStatus linkStatus() const {
        return _linkStatus;
    }

    EthernetHardwareStatus hardwareStatus() const {
        return _hardwareStatus;
    }

private:
    IPAddress _localIp;
    IPAddress _dnsIp;
    IPAddress _gatewayIp;
    IPAddress _subnetIp;
    EthernetLinkStatus _linkStatus;
    EthernetHardwareStatus _hardwareStatus;
};

inline EthernetClass Ethernet;

#endif
