#ifndef UNOQ_BRIDGE_H
#define UNOQ_BRIDGE_H

#include <Arduino.h>

#include <SimpleVector.h>

class UnoQBridge {
public:
    static constexpr uint16_t kMagic = 0x5155;
    static constexpr uint8_t kProtocolVersion = 1;
    static constexpr uint8_t kPayloadJson = 1;
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

    typedef bool (*Handler)(const String&, String&);

    struct HandlerEntry {
        String name;
        Handler handler;
    };

    UnoQBridge()
        : _ready(false),
          _stream(nullptr),
          _timeoutMs(2000UL),
          _lastSequenceId(0),
          _lastError() {
    }

    bool begin() {
        _ready = true;
        _stream = nullptr;
        _lastError = "";
        return true;
    }

    bool begin(Stream& stream, unsigned long timeoutMs = 2000UL) {
        _stream = &stream;
        _timeoutMs = timeoutMs;
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
        _stream = nullptr;
        _lastSequenceId = 0;
        _lastError = "";
    }

    void registerHandler(const String& name, Handler handler) {
        HandlerEntry entry;
        entry.name = name;
        entry.handler = handler;
        _handlers.push_back(entry);
    }

    bool poll() {
        if (_stream == nullptr) {
            _lastError = "bridge stream not initialized";
            return false;
        }

        uint8_t header[10];
        if (!readExact(header, sizeof(header), _timeoutMs)) {
            _lastError = "read frame header timeout";
            return false;
        }

        if (readUint16(header + 0) != kMagic || header[2] != kProtocolVersion) {
            _lastError = "invalid frame magic or version";
            return false;
        }

        const uint16_t sequenceId = readUint16(header + 4);
        const uint32_t payloadLength = readUint32(header + 6);
        if (payloadLength == 0 || payloadLength > kMaxPayloadBytes) {
            _lastError = "invalid payload length";
            return false;
        }

        String request;
        request.reserve(static_cast<unsigned int>(payloadLength) + 1);
        for (uint32_t i = 0; i < payloadLength; ++i) {
            const int value = timedRead(_timeoutMs);
            if (value < 0) {
                _lastError = "read payload timeout";
                return false;
            }
            request += static_cast<char>(value);
        }

        String response;
        if (!dispatchRequest(request, response)) {
            response = makeErrorResponse(sequenceId, "UNHANDLED_REQUEST", _lastError.length() ? _lastError : "request not handled");
        }

        if (!writeFrame(sequenceId, response.c_str(), response.length(), kPayloadJson)) {
            _lastError = "failed to write response frame";
            return false;
        }

        _lastError = "";
        return true;
    }

private:
    int timedRead(unsigned long timeoutMs) {
        if (_stream == nullptr) {
            return -1;
        }
        if (_stream->available()) {
            return _stream->read();
        }

        const unsigned long started = millis();
        while ((millis() - started) < timeoutMs) {
            if (_stream->available()) {
                return _stream->read();
            }
            yield();
            delay(1);
        }
        return -1;
    }

    bool readExact(uint8_t* buffer, size_t length, unsigned long timeoutMs) {
        size_t offset = 0;
        const unsigned long started = millis();
        while (offset < length) {
            if ((millis() - started) > timeoutMs && offset > 0) {
                return false;
            }
            if (_stream == nullptr || !_stream->available()) {
                if (timeoutMs == 0) {
                    continue;
                }
                if ((millis() - started) >= timeoutMs) {
                    return false;
                }
                yield();
                delay(1);
                continue;
            }
            buffer[offset++] = static_cast<uint8_t>(_stream->read());
        }
        return true;
    }

    bool writeFrame(uint16_t sequenceId, const char* payload, size_t payloadLength, uint8_t payloadFormat) {
        if (_stream == nullptr) {
            return false;
        }

        uint8_t header[10];
        writeUint16(header + 0, kMagic);
        header[2] = kProtocolVersion;
        header[3] = payloadFormat;
        writeUint16(header + 4, sequenceId);
        writeUint32(header + 6, static_cast<uint32_t>(payloadLength));

        if (_stream->write(header, sizeof(header)) != sizeof(header)) {
            return false;
        }
        if (payloadLength != 0 && _stream->write(reinterpret_cast<const uint8_t*>(payload), payloadLength) != payloadLength) {
            return false;
        }
        _stream->flush();
        return true;
    }

    static void writeUint16(uint8_t* out, uint16_t value) {
        out[0] = static_cast<uint8_t>(value & 0xFF);
        out[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }

    static void writeUint32(uint8_t* out, uint32_t value) {
        out[0] = static_cast<uint8_t>(value & 0xFF);
        out[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        out[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
        out[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
    }

    static uint16_t readUint16(const uint8_t* in) {
        return static_cast<uint16_t>(in[0]) |
               (static_cast<uint16_t>(in[1]) << 8);
    }

    static uint32_t readUint32(const uint8_t* in) {
        return static_cast<uint32_t>(in[0]) |
               (static_cast<uint32_t>(in[1]) << 8) |
               (static_cast<uint32_t>(in[2]) << 16) |
               (static_cast<uint32_t>(in[3]) << 24);
    }

    bool dispatchRequest(const String& request, String& responseOut) {
        String op = extractStringField(request, "op");
        if (op.length() == 0) {
            if (request.indexOf("\"op\"") == -1) {
                _lastError = "missing op";
                return false;
            }
            _lastError = "invalid op field";
            return false;
        }

        const String trimmed = op;
        for (size_t i = 0; i < _handlers.size(); ++i) {
            if (_handlers[i].name == trimmed) {
                return _handlers[i].handler(request, responseOut);
            }
        }

        if (trimmed == "ping") {
            responseOut = makeOkResponse(extractNumericId(request), "{\"service\":\"UnoQBridge\",\"status\":\"ok\"}");
            return true;
        }

        _lastError = String("unknown op: ") + trimmed;
        return false;
    }

    static uint16_t extractNumericId(const String& request) {
        const String needle = String("\"id\":");
        const int index = request.indexOf(needle.c_str());
        if (index < 0) {
            return 0;
        }

        size_t valueStart = static_cast<size_t>(index + needle.length());
        while (valueStart < request.length() && (request[valueStart] == ' ' || request[valueStart] == '\t' || request[valueStart] == '\n' || request[valueStart] == '\r')) {
            ++valueStart;
        }

        String value;
        while (valueStart < request.length()) {
            const char ch = request[valueStart];
            if (ch >= '0' && ch <= '9') {
                value += ch;
                ++valueStart;
                continue;
            }
            break;
        }

        if (value.length() == 0) {
            return 0;
        }

        return static_cast<uint16_t>(strtoul(value.c_str(), nullptr, 10));
    }

    static String extractStringField(const String& source, const char* fieldName) {
        const String needle = String("\"") + fieldName + String("\":");
        const int start = source.indexOf(needle.c_str());
        if (start < 0) {
            return "";
        }

        size_t index = static_cast<size_t>(start + needle.length());
        while (index < source.length() && (source[index] == ' ' || source[index] == '\t' || source[index] == '\n' || source[index] == '\r')) {
            ++index;
        }

        if (index >= source.length() || source[index] != '"') {
            return "";
        }

        String value;
        bool escaped = false;
        for (++index; index < source.length(); ++index) {
            const char ch = source[index];
            if (escaped) {
                switch (ch) {
                    case 'n': value += '\n'; break;
                    case 'r': value += '\r'; break;
                    case 't': value += '\t'; break;
                    case 'b': value += '\b'; break;
                    case 'f': value += '\f'; break;
                    default: value += ch; break;
                }
                escaped = false;
                continue;
            }
            if (ch == '\\') {
                escaped = true;
                continue;
            }
            if (ch == '"') {
                return value;
            }
            value += ch;
        }
        return "";
    }

    static String makeOkResponse(uint16_t requestId, const String& resultJson) {
        String response = String("{\"id\":") + String(requestId) + String(",\"ok\":true,\"result\":") + resultJson + String("}");
        return response;
    }

    static String makeErrorResponse(uint16_t requestId, const char* code, const String& message) {
        String response = String("{\"id\":") + String(requestId) +
                          String(",\"ok\":false,\"error\":{\"code\":\"") + code +
                          String("\",\"message\":\"") + message +
                          String("\"}}");
        return response;
    }

    bool _ready;
    Stream* _stream;
    unsigned long _timeoutMs;
    uint16_t _lastSequenceId;
    String _lastError;
    SimpleVector<HandlerEntry> _handlers;
};

#endif
