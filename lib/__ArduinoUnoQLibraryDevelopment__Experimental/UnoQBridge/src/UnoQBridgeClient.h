#ifndef UNOQ_BRIDGE_CLIENT_H
#define UNOQ_BRIDGE_CLIENT_H

#include <Arduino.h>
#include <string.h>

#if defined(__has_include) && __has_include(<Bridge.h>)
    #include <Bridge.h>
#endif

#if defined(__has_include) && __has_include(<Process.h>)
    #include <Process.h>
    #define UNOQ_HAS_YUN_PROCESS 1
#else
    #define UNOQ_HAS_YUN_PROCESS 0
#endif

class UnoQBridgeClient {
public:
    static constexpr uint16_t kMagic = 0x5155;
    static constexpr uint8_t kProtocolVersion = 1;
    static constexpr uint8_t kPayloadJson = 1;
    static constexpr uint8_t kPayloadMsgPack = 2;
    static constexpr size_t kDefaultMaxPayloadBytes = 4096;

    class Transport {
    public:
        virtual ~Transport() = default;
        virtual bool request(
            const uint8_t* payload,
            size_t payloadLength,
            uint8_t payloadFormat,
            String& responseOut) = 0;
    };

    class BinaryStreamTransport : public Transport {
    public:
        BinaryStreamTransport(Stream& io, unsigned long timeoutMs = 2000UL, size_t maxPayloadBytes = kDefaultMaxPayloadBytes)
            : _io(io), _timeoutMs(timeoutMs), _maxPayloadBytes(maxPayloadBytes), _sequenceId(0) {}

        bool request(
            const uint8_t* payload,
            size_t payloadLength,
            uint8_t payloadFormat,
            String& responseOut) override {
            if (payload == nullptr || payloadLength == 0 || payloadLength > _maxPayloadBytes) {
                return false;
            }

            while (_io.available()) {
                (void)_io.read();
            }

            const uint16_t sequenceId = nextSequenceId();
            uint8_t header[10];
            writeUint16(header + 0, kMagic);
            header[2] = kProtocolVersion;
            header[3] = payloadFormat;
            writeUint16(header + 4, sequenceId);
            writeUint32(header + 6, static_cast<uint32_t>(payloadLength));

            if (_io.write(header, sizeof(header)) != sizeof(header)) {
                return false;
            }
            if (_io.write(payload, payloadLength) != payloadLength) {
                return false;
            }
            _io.flush();

            uint8_t responseHeader[10];
            if (!readExact(responseHeader, sizeof(responseHeader), _timeoutMs)) {
                return false;
            }

            if (readUint16(responseHeader + 0) != kMagic || responseHeader[2] != kProtocolVersion) {
                return false;
            }

            const uint32_t responseLength = readUint32(responseHeader + 6);
            if (responseLength > _maxPayloadBytes) {
                return false;
            }

            responseOut = "";
            responseOut.reserve(responseLength + 1);
            if (responseLength == 0) {
                return true;
            }

            for (uint32_t i = 0; i < responseLength; ++i) {
                int value = timedRead(_timeoutMs);
                if (value < 0) {
                    return false;
                }
                responseOut += static_cast<char>(value);
            }

            return true;
        }

        void setTimeout(unsigned long timeoutMs) {
            _timeoutMs = timeoutMs;
        }

        void setMaxPayloadBytes(size_t maxPayloadBytes) {
            _maxPayloadBytes = maxPayloadBytes;
        }

    private:
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

        bool readExact(uint8_t* buffer, size_t length, unsigned long timeoutMs) {
            size_t offset = 0;
            const unsigned long start = millis();
            while (offset < length) {
                int value = timedRead(timeoutMs, start);
                if (value < 0) {
                    return false;
                }
                buffer[offset++] = static_cast<uint8_t>(value);
            }
            return true;
        }

        int timedRead(unsigned long timeoutMs, unsigned long start = 0) {
            if (_io.available()) {
                return _io.read();
            }
            if (timeoutMs == 0) {
                while (!_io.available()) {
                    ::yield();
                    delay(1);
                }
                return _io.read();
            }

            const unsigned long began = (start == 0) ? millis() : start;
            while ((millis() - began) < timeoutMs) {
                if (_io.available()) {
                    return _io.read();
                }
                ::yield();
                delay(1);
            }
            return -1;
        }

        uint16_t nextSequenceId() {
            ++_sequenceId;
            if (_sequenceId == 0) {
                _sequenceId = 1;
            }
            return _sequenceId;
        }

        Stream& _io;
        unsigned long _timeoutMs;
        size_t _maxPayloadBytes;
        uint16_t _sequenceId;
    };

#if UNOQ_HAS_YUN_PROCESS
    class JsonProcessTransport : public Transport {
    public:
        JsonProcessTransport(
            const String& backendScriptPath,
            const String& rootPath = "/mnt/sd/unoq_files",
            const String& pythonCmd = "python3")
            : _backendScriptPath(backendScriptPath),
              _rootPath(rootPath),
              _pythonCmd(pythonCmd) {}

        bool request(
            const uint8_t* payload,
            size_t payloadLength,
            uint8_t payloadFormat,
            String& responseOut) override {
            if (payload == nullptr || payloadLength == 0 || payloadFormat != kPayloadJson) {
                return false;
            }

            String payloadText;
            payloadText.reserve(payloadLength + 1);
            for (size_t i = 0; i < payloadLength; ++i) {
                payloadText += static_cast<char>(payload[i]);
            }

            Process process;
            process.begin(_pythonCmd.c_str());
            process.addParameter(_backendScriptPath.c_str());
            process.addParameter("--mode");
            process.addParameter("json");
            process.addParameter("--root");
            process.addParameter(_rootPath.c_str());
            process.addParameter("--once");
            process.addParameter(payloadText.c_str());
            process.run();

            responseOut = "";
            while (process.available()) {
                responseOut += static_cast<char>(process.read());
            }
            responseOut.trim();
            return process.exitValue() == 0 && responseOut.length() > 0;
        }

        void setPythonCommand(const String& pythonCmd) {
            _pythonCmd = pythonCmd;
        }

        void setRootPath(const String& rootPath) {
            _rootPath = rootPath;
        }

    private:
        String _backendScriptPath;
        String _rootPath;
        String _pythonCmd;
    };
#endif

    explicit UnoQBridgeClient(Transport& transport)
        : _transport(transport), _lastError(""), _nextRequestId(1) {}

    bool ping() {
        String response;
        if (!sendJsonRequest(String("{\"id\":") + String(nextRequestId()) + ",\"op\":\"ping\"}", response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = String("ping failed: ") + response;
            return false;
        }

        _lastError = "";
        return true;
    }

    bool exists(const String& path, bool& existsOut) {
        String request = String("{\"id\":") + String(nextRequestId()) +
                         String(",\"op\":\"file.exists\",\"path\":\"") + escapeJson(path) + String("\"}");
        String response;
        if (!sendJsonRequest(request, response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = extractErrorMessage(response);
            return false;
        }

        bool parsed = false;
        bool value = extractBoolField(response, "exists", parsed);
        if (!parsed) {
            _lastError = "exists response parse failure";
            return false;
        }

        existsOut = value;
        _lastError = "";
        return true;
    }

    bool readText(const String& path, String& contentOut) {
        String request = String("{\"id\":") + String(nextRequestId()) +
                         String(",\"op\":\"file.read_text\",\"path\":\"") + escapeJson(path) + String("\"}");
        String response;
        if (!sendJsonRequest(request, response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = extractErrorMessage(response);
            return false;
        }

        bool parsed = false;
        String content = extractStringField(response, "content", parsed);
        if (!parsed) {
            _lastError = "read_text response parse failure";
            return false;
        }

        contentOut = content;
        _lastError = "";
        return true;
    }

    bool writeText(const String& path, const String& content, bool append = false) {
        String request = String("{\"id\":") + String(nextRequestId()) +
                         String(",\"op\":\"file.write_text\",\"path\":\"") + escapeJson(path) +
                         String("\",\"content\":\"") + escapeJson(content) +
                         String("\",\"append\":") + (append ? "true" : "false") + String("}");
        String response;
        if (!sendJsonRequest(request, response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = extractErrorMessage(response);
            return false;
        }

        _lastError = "";
        return true;
    }

    bool readBytesBase64(const String& path, String& dataB64Out, int offset = 0, int size = -1) {
        String request = String("{\"id\":") + String(nextRequestId()) +
                         String(",\"op\":\"file.read_bytes\",\"path\":\"") + escapeJson(path) +
                         String("\",\"offset\":") + String(offset);
        if (size >= 0) {
            request += String(",\"size\":") + String(size);
        }
        request += "}";

        String response;
        if (!sendJsonRequest(request, response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = extractErrorMessage(response);
            return false;
        }

        bool parsed = false;
        String data = extractStringField(response, "data", parsed);
        if (!parsed) {
            _lastError = "read_bytes response parse failure";
            return false;
        }

        dataB64Out = data;
        _lastError = "";
        return true;
    }

    bool writeBytesBase64(const String& path, const String& dataB64, bool append = false) {
        String request = String("{\"id\":") + String(nextRequestId()) +
                         String(",\"op\":\"file.write_bytes\",\"path\":\"") + escapeJson(path) +
                         String("\",\"data\":\"") + escapeJson(dataB64) +
                         String("\",\"append\":") + (append ? "true" : "false") + String("}");

        String response;
        if (!sendJsonRequest(request, response)) {
            return false;
        }

        bool ok = false;
        if (!parseOk(response, ok) || !ok) {
            _lastError = extractErrorMessage(response);
            return false;
        }

        _lastError = "";
        return true;
    }

    const String& lastError() const {
        return _lastError;
    }

private:
    bool sendJsonRequest(const String& request, String& responseOut) {
        return _transport.request(
            reinterpret_cast<const uint8_t*>(request.c_str()),
            request.length(),
            kPayloadJson,
            responseOut);
    }

    uint16_t nextRequestId() {
        ++_nextRequestId;
        if (_nextRequestId == 0) {
            _nextRequestId = 1;
        }
        return _nextRequestId;
    }

    static String escapeJson(const String& input) {
        String output;
        output.reserve(input.length() + 8);
        for (size_t i = 0; i < input.length(); ++i) {
            const char c = input[i];
            switch (c) {
                case '\\': output += "\\\\"; break;
                case '"': output += "\\\""; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default: output += c; break;
            }
        }
        return output;
    }

    static bool parseOk(const String& response, bool& okOut) {
        const int index = response.indexOf("\"ok\":true");
        if (index >= 0) {
            okOut = true;
            return true;
        }
        const int falseIndex = response.indexOf("\"ok\":false");
        if (falseIndex >= 0) {
            okOut = false;
            return true;
        }
        return false;
    }

    static String extractErrorMessage(const String& response) {
        bool parsed = false;
        String error = extractStringField(response, "message", parsed);
        if (parsed) {
            return error;
        }
        return response;
    }

    static bool extractBoolField(const String& response, const char* fieldName, bool& parsed) {
        parsed = false;
        const String needle = String("\"") + fieldName + "\":";
        const int index = response.indexOf(needle);
        if (index < 0) {
            return false;
        }

        const int valueStart = index + needle.length();
        if (response.startsWith("true", valueStart)) {
            parsed = true;
            return true;
        }
        if (response.startsWith("false", valueStart)) {
            parsed = true;
            return false;
        }
        return false;
    }

    static String extractStringField(const String& response, const char* fieldName, bool& parsed) {
        parsed = false;
        const String needle = String("\"") + fieldName + "\":\"";
        const int start = response.indexOf(needle);
        if (start < 0) {
            return "";
        }

        String value;
        bool escaped = false;
        for (int i = start + needle.length(); i < response.length(); ++i) {
            const char c = response[i];
            if (escaped) {
                switch (c) {
                    case 'n': value += '\n'; break;
                    case 'r': value += '\r'; break;
                    case 't': value += '\t'; break;
                    case 'b': value += '\b'; break;
                    case 'f': value += '\f'; break;
                    default: value += c; break;
                }
                escaped = false;
                continue;
            }
            if (c == '\\') {
                escaped = true;
                continue;
            }
            if (c == '"') {
                parsed = true;
                return value;
            }
            value += c;
        }
        return "";
    }

    Transport& _transport;
    String _lastError;
    uint16_t _nextRequestId;
};

using UnoQFileTransferClient = UnoQBridgeClient;

#endif