#ifndef UNOQ_FILE_TRANSFER_CLIENT_H
#define UNOQ_FILE_TRANSFER_CLIENT_H

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

class UnoQFileTransferClient {
public:
    class Transport {
    public:
        virtual ~Transport() = default;
        virtual bool request(const String& payload, String& responseOut) = 0;
    };

    // Generic stream transport for Serial/UART links (JSON lines).
    class StreamJsonTransport : public Transport {
    public:
        StreamJsonTransport(Stream& io, unsigned long timeoutMs = 2000UL)
            : _io(io), _timeoutMs(timeoutMs) {}

        bool request(const String& payload, String& responseOut) override {
            while (_io.available()) {
                (void)_io.read();
            }

            _io.print(payload);
            _io.print('\n');

            responseOut = "";
            unsigned long start = millis();
            while ((millis() - start) < _timeoutMs) {
                while (_io.available()) {
                    char c = static_cast<char>(_io.read());
                    if (c == '\r') {
                        continue;
                    }
                    if (c == '\n') {
                        responseOut.trim();
                        return responseOut.length() > 0;
                    }
                    responseOut += c;
                }
            }

            responseOut.trim();
            return responseOut.length() > 0;
        }

        void setTimeout(unsigned long timeoutMs) {
            _timeoutMs = timeoutMs;
        }

    private:
        Stream& _io;
        unsigned long _timeoutMs;
    };

    // Convenience alias for mbed-backed Arduino cores using Serial/Stream.
#if defined(ARDUINO_ARCH_MBED)
    class MbedSerialTransport : public StreamJsonTransport {
    public:
        MbedSerialTransport(Stream& io, unsigned long timeoutMs = 2000UL)
            : StreamJsonTransport(io, timeoutMs) {}
    };
#endif

    // Older style Linux bridge transport for Arduino Yun.
    // This mode invokes the Python backend once per request via Process.
#if UNOQ_HAS_YUN_PROCESS
    class YunProcessTransport : public Transport {
    public:
        YunProcessTransport(
            const String& backendScriptPath,
            const String& rootPath = "/mnt/sd/unoq_files",
            const String& pythonCmd = "python3")
            : _backendScriptPath(backendScriptPath),
              _rootPath(rootPath),
              _pythonCmd(pythonCmd) {}

        bool request(const String& payload, String& responseOut) override {
            Process process;
            process.begin(_pythonCmd.c_str());
            process.addParameter(_backendScriptPath.c_str());
            process.addParameter("--mode");
            process.addParameter("json");
            process.addParameter("--root");
            process.addParameter(_rootPath.c_str());
            process.addParameter("--once");
            process.addParameter(payload.c_str());
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

    explicit UnoQFileTransferClient(Transport& transport)
        : _transport(transport), _lastError("") {}

    bool ping() {
        String response;
        if (!sendRequest("{\"id\":1,\"op\":\"ping\"}", response)) {
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
        String request = String("{\"id\":2,\"op\":\"exists\",\"path\":\"") + escapeJson(path) + "\"}";
        String response;
        if (!sendRequest(request, response)) {
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
        String request = String("{\"id\":3,\"op\":\"read_text\",\"path\":\"") + escapeJson(path) + "\"}";
        String response;
        if (!sendRequest(request, response)) {
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
        String request = String("{\"id\":4,\"op\":\"write_text\",\"path\":\"") +
                         escapeJson(path) +
                         "\",\"content\":\"" +
                         escapeJson(content) +
                         "\",\"append\":" +
                         (append ? "true" : "false") + "}";
        String response;
        if (!sendRequest(request, response)) {
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
        String request = String("{\"id\":5,\"op\":\"read_bytes\",\"path\":\"") +
                         escapeJson(path) +
                         "\",\"offset\":" + String(offset);
        if (size >= 0) {
            request += String(",\"size\":") + String(size);
        }
        request += "}";

        String response;
        if (!sendRequest(request, response)) {
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
        String request = String("{\"id\":6,\"op\":\"write_bytes\",\"path\":\"") +
                         escapeJson(path) +
                         "\",\"data\":\"" +
                         escapeJson(dataB64) +
                         "\",\"append\":" +
                         (append ? "true" : "false") + "}";

        String response;
        if (!sendRequest(request, response)) {
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

    const String& getLastError() const {
        return _lastError;
    }

    // MessagePack request builders for bridge implementations that exchange
    // binary payloads with arduino-router directly.
    static size_t buildMsgPackPing(uint8_t* out, size_t capacity, int id = 1) {
        size_t o = 0;
        if (!mpWriteFixMap(out, capacity, o, 2)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "id", id)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "op", "ping")) return 0;
        return o;
    }

    static size_t buildMsgPackExists(uint8_t* out, size_t capacity, const String& path, int id = 2) {
        size_t o = 0;
        if (!mpWriteFixMap(out, capacity, o, 3)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "id", id)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "op", "exists")) return 0;
        if (!mpWriteStringKV(out, capacity, o, "path", path.c_str())) return 0;
        return o;
    }

    static size_t buildMsgPackReadText(uint8_t* out, size_t capacity, const String& path, int id = 3) {
        size_t o = 0;
        if (!mpWriteFixMap(out, capacity, o, 3)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "id", id)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "op", "read_text")) return 0;
        if (!mpWriteStringKV(out, capacity, o, "path", path.c_str())) return 0;
        return o;
    }

    static size_t buildMsgPackWriteText(
        uint8_t* out,
        size_t capacity,
        const String& path,
        const String& content,
        bool append = false,
        int id = 4) {
        size_t o = 0;
        if (!mpWriteFixMap(out, capacity, o, 5)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "id", id)) return 0;
        if (!mpWriteStringKV(out, capacity, o, "op", "write_text")) return 0;
        if (!mpWriteStringKV(out, capacity, o, "path", path.c_str())) return 0;
        if (!mpWriteStringKV(out, capacity, o, "content", content.c_str())) return 0;
        if (!mpWriteBoolKV(out, capacity, o, "append", append)) return 0;
        return o;
    }

    // Keep this helper visible because the backend is often run in JSON mode during bring-up.
    static String escapeJson(const String& input) {
        String out;
        out.reserve(input.length() + 8);

        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            switch (c) {
                case '\\': out += "\\\\"; break;
                case '"': out += "\\\""; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        out += "?";
                    } else {
                        out += c;
                    }
                    break;
            }
        }

        return out;
    }

private:
    Transport& _transport;
    String _lastError;

    static bool mpWriteByte(uint8_t* out, size_t capacity, size_t& offset, uint8_t value) {
        if (!out || offset >= capacity) {
            return false;
        }
        out[offset++] = value;
        return true;
    }

    static bool mpWriteRaw(uint8_t* out, size_t capacity, size_t& offset, const uint8_t* data, size_t length) {
        if (!out || !data || (offset + length) > capacity) {
            return false;
        }
        for (size_t i = 0; i < length; ++i) {
            out[offset++] = data[i];
        }
        return true;
    }

    static bool mpWriteFixMap(uint8_t* out, size_t capacity, size_t& offset, uint8_t entries) {
        if (entries > 15) {
            return false;
        }
        return mpWriteByte(out, capacity, offset, static_cast<uint8_t>(0x80 | entries));
    }

    static bool mpWriteString(uint8_t* out, size_t capacity, size_t& offset, const char* value) {
        if (!value) {
            return false;
        }
        size_t len = strlen(value);
        if (len <= 31) {
            if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>(0xA0 | len))) return false;
        } else if (len <= 255) {
            if (!mpWriteByte(out, capacity, offset, 0xD9)) return false;
            if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>(len))) return false;
        } else {
            if (len > 65535) return false;
            if (!mpWriteByte(out, capacity, offset, 0xDA)) return false;
            if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>((len >> 8) & 0xFF))) return false;
            if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>(len & 0xFF))) return false;
        }
        return mpWriteRaw(out, capacity, offset, reinterpret_cast<const uint8_t*>(value), len);
    }

    static bool mpWriteInt(uint8_t* out, size_t capacity, size_t& offset, int value) {
        if (value >= 0 && value <= 127) {
            return mpWriteByte(out, capacity, offset, static_cast<uint8_t>(value));
        }
        if (value >= -32 && value < 0) {
            return mpWriteByte(out, capacity, offset, static_cast<uint8_t>(0xE0 | (value + 32)));
        }
        if (!mpWriteByte(out, capacity, offset, 0xD2)) return false;
        if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>((value >> 24) & 0xFF))) return false;
        if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>((value >> 16) & 0xFF))) return false;
        if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>((value >> 8) & 0xFF))) return false;
        if (!mpWriteByte(out, capacity, offset, static_cast<uint8_t>(value & 0xFF))) return false;
        return true;
    }

    static bool mpWriteBool(uint8_t* out, size_t capacity, size_t& offset, bool value) {
        return mpWriteByte(out, capacity, offset, value ? 0xC3 : 0xC2);
    }

    static bool mpWriteStringKV(uint8_t* out, size_t capacity, size_t& offset, const char* key, const char* value) {
        return mpWriteString(out, capacity, offset, key) && mpWriteString(out, capacity, offset, value);
    }

    static bool mpWriteStringKV(uint8_t* out, size_t capacity, size_t& offset, const char* key, int value) {
        return mpWriteString(out, capacity, offset, key) && mpWriteInt(out, capacity, offset, value);
    }

    static bool mpWriteBoolKV(uint8_t* out, size_t capacity, size_t& offset, const char* key, bool value) {
        return mpWriteString(out, capacity, offset, key) && mpWriteBool(out, capacity, offset, value);
    }

    bool sendRequest(const String& request, String& responseOut) {
        if (!_transport.request(request, responseOut)) {
            _lastError = "transport request failed";
            return false;
        }
        return true;
    }

    static bool parseOk(const String& json, bool& okOut) {
        int idx = json.indexOf("\"ok\"");
        if (idx < 0) {
            return false;
        }

        idx = json.indexOf(':', idx);
        if (idx < 0) {
            return false;
        }

        int t = findNonWhitespace(json, idx + 1);
        if (t < 0) {
            return false;
        }

        if (json.startsWith("true", static_cast<unsigned int>(t))) {
            okOut = true;
            return true;
        }
        if (json.startsWith("false", static_cast<unsigned int>(t))) {
            okOut = false;
            return true;
        }

        return false;
    }

    static int findNonWhitespace(const String& s, int start) {
        for (int i = start; i < static_cast<int>(s.length()); ++i) {
            char c = s[static_cast<unsigned int>(i)];
            if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r')) {
                return i;
            }
        }
        return -1;
    }

    static String extractErrorMessage(const String& json) {
        bool parsed = false;
        String msg = extractStringField(json, "message", parsed);
        if (parsed) {
            return msg;
        }
        return String("backend error: ") + json;
    }

    static bool extractBoolField(const String& json, const char* key, bool& parsed) {
        parsed = false;
        int idx = json.indexOf(String("\"") + key + "\"");
        if (idx < 0) {
            return false;
        }

        idx = json.indexOf(':', idx);
        if (idx < 0) {
            return false;
        }

        int t = findNonWhitespace(json, idx + 1);
        if (t < 0) {
            return false;
        }

        if (json.startsWith("true", static_cast<unsigned int>(t))) {
            parsed = true;
            return true;
        }
        if (json.startsWith("false", static_cast<unsigned int>(t))) {
            parsed = true;
            return false;
        }

        return false;
    }

    static String extractStringField(const String& json, const char* key, bool& parsed) {
        parsed = false;

        int keyIdx = json.indexOf(String("\"") + key + "\"");
        if (keyIdx < 0) {
            return "";
        }

        int colonIdx = json.indexOf(':', keyIdx);
        if (colonIdx < 0) {
            return "";
        }

        int q0 = json.indexOf('"', colonIdx + 1);
        if (q0 < 0) {
            return "";
        }

        String out;
        bool escape = false;
        for (int i = q0 + 1; i < static_cast<int>(json.length()); ++i) {
            char c = json[static_cast<unsigned int>(i)];
            if (escape) {
                switch (c) {
                    case 'n': out += '\n'; break;
                    case 'r': out += '\r'; break;
                    case 't': out += '\t'; break;
                    case '\\': out += '\\'; break;
                    case '"': out += '"'; break;
                    default: out += c; break;
                }
                escape = false;
                continue;
            }

            if (c == '\\') {
                escape = true;
                continue;
            }

            if (c == '"') {
                parsed = true;
                return out;
            }

            out += c;
        }

        return "";
    }
};

#endif
