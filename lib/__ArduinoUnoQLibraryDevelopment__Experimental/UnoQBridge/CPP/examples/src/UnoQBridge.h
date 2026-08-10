#ifndef UNOQ_BRIDGE_H
#define UNOQ_BRIDGE_H

#include <Arduino.h>

class UnoQBridge {
public:
    static constexpr uint16_t kMagic = 0x5155;
    static constexpr uint8_t kProtocolVersion = 1;
    static constexpr size_t kMaxPayloadBytes = 256;

    enum class OpCode : uint8_t {
        Ping = 1,
        Status = 2,
        Read = 3,
        Write = 4,
        Custom = 255
    };

    struct FrameHeader {
        uint16_t magic;
        uint8_t version;
        uint8_t opcode;
        uint16_t sequenceId;
        uint16_t resourceId;
        uint16_t payloadLength;
    };

    struct Frame {
        FrameHeader header;
        const uint8_t* payload;
        size_t payloadLength;
    };

    UnoQBridge()
        : _ready(false),
          _lastSequenceId(0),
          _lastError() {
    }

    bool begin() {
        _ready = true;
        _lastError = "";
        return true;
    }

    bool isReady() const {
        return _ready;
    }

    uint16_t nextSequenceId() {
        ++_lastSequenceId;
        if (_lastSequenceId == 0) {
            _lastSequenceId = 1;
        }
        return _lastSequenceId;
    }

    const String& lastError() const {
        return _lastError;
    }

    void setLastError(const String& error) {
        _lastError = error;
    }

    void reset() {
        _ready = false;
        _lastSequenceId = 0;
        _lastError = "";
    }

private:
    bool _ready;
    uint16_t _lastSequenceId;
    String _lastError;
};

#endif
