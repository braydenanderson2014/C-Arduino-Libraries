#include <cstdint>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Arduino.h"
#include "UnoQBridgeClient.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

uint16_t readLe16(const uint8_t* data) {
    return static_cast<uint16_t>(data[0]) |
           (static_cast<uint16_t>(data[1]) << 8);
}

uint32_t readLe32(const uint8_t* data) {
    return static_cast<uint32_t>(data[0]) |
           (static_cast<uint32_t>(data[1]) << 8) |
           (static_cast<uint32_t>(data[2]) << 16) |
           (static_cast<uint32_t>(data[3]) << 24);
}

class FakeStream : public Stream {
public:
    std::vector<uint8_t> written;
    std::deque<uint8_t> readable;
    std::string pendingResponsePayload;
    uint16_t pendingResponseSequenceId = 0;
    bool queueOnFlush = false;

    int available() override {
        return static_cast<int>(readable.size());
    }

    int read() override {
        if (readable.empty()) {
            return -1;
        }
        const uint8_t value = readable.front();
        readable.pop_front();
        return static_cast<int>(value);
    }

    size_t write(uint8_t value) override {
        written.push_back(value);
        return 1;
    }

    size_t write(const uint8_t* data, size_t size) override {
        written.insert(written.end(), data, data + size);
        return size;
    }

    void queueResponse(const std::string& payload, uint16_t sequenceId) {
        pendingResponsePayload = payload;
        pendingResponseSequenceId = sequenceId;
        queueOnFlush = true;
    }

    void flush() override {
        if (!queueOnFlush) {
            return;
        }

        const std::string payload = pendingResponsePayload;
        const uint16_t sequenceId = pendingResponseSequenceId;
        queueOnFlush = false;

        const uint32_t payloadLength = static_cast<uint32_t>(payload.size());
        const uint8_t header[10] = {
            static_cast<uint8_t>(UnoQBridgeClient::kMagic & 0xFF),
            static_cast<uint8_t>((UnoQBridgeClient::kMagic >> 8) & 0xFF),
            UnoQBridgeClient::kProtocolVersion,
            UnoQBridgeClient::kPayloadJson,
            static_cast<uint8_t>(sequenceId & 0xFF),
            static_cast<uint8_t>((sequenceId >> 8) & 0xFF),
            static_cast<uint8_t>(payloadLength & 0xFF),
            static_cast<uint8_t>((payloadLength >> 8) & 0xFF),
            static_cast<uint8_t>((payloadLength >> 16) & 0xFF),
            static_cast<uint8_t>((payloadLength >> 24) & 0xFF)
        };

        for (size_t i = 0; i < sizeof(header); ++i) {
            readable.push_back(header[i]);
        }
        for (char ch : payload) {
            readable.push_back(static_cast<uint8_t>(ch));
        }
    }
};

class ScriptedTransport : public UnoQBridgeClient::Transport {
public:
    struct Step {
        bool requestSucceeded;
        String response;
    };

    struct SeenRequest {
        String payload;
        uint8_t format;
    };

    std::deque<Step> steps;
    std::vector<SeenRequest> seen;

    bool request(const uint8_t* payload, size_t payloadLength, uint8_t payloadFormat, String& responseOut) override {
        SeenRequest req;
        req.format = payloadFormat;
        for (size_t i = 0; i < payloadLength; ++i) {
            req.payload += static_cast<char>(payload[i]);
        }
        seen.push_back(req);

        if (steps.empty()) {
            responseOut = "";
            return false;
        }

        const Step step = steps.front();
        steps.pop_front();

        responseOut = step.response;
        return step.requestSucceeded;
    }
};

void testBinaryStreamTransportRoundTrip() {
    FakeStream fake;
    const std::string expectedResponse = "{\"ok\":true}";
    fake.queueResponse(expectedResponse, 1);

    UnoQBridgeClient::BinaryStreamTransportImpl<FakeStream> transport(fake, 50UL);

    const uint8_t payload[] = "{\"id\":1,\"op\":\"ping\"}";
    String response;
    const bool ok = transport.request(
        payload,
        sizeof(payload) - 1,
        UnoQBridgeClient::kPayloadJson,
        response
    );

    expect(ok, "Binary stream transport should succeed when framed response is available");
    expect(response == expectedResponse.c_str(), "Binary stream transport should return the framed payload text");

    expect(fake.written.size() == (10 + sizeof(payload) - 1), "Transport should write frame header + payload");
    expect(readLe16(&fake.written[0]) == UnoQBridgeClient::kMagic, "Request header magic mismatch");
    expect(fake.written[2] == UnoQBridgeClient::kProtocolVersion, "Request protocol version mismatch");
    expect(fake.written[3] == UnoQBridgeClient::kPayloadJson, "Request payload format mismatch");
    expect(readLe32(&fake.written[6]) == (sizeof(payload) - 1), "Request payload size in header mismatch");

    std::string sentPayload(fake.written.begin() + 10, fake.written.end());
    expect(sentPayload == "{\"id\":1,\"op\":\"ping\"}", "Transport should emit original payload bytes");
}

void testBinaryStreamTransportTimeout() {
    FakeStream fake;
    UnoQBridgeClient::BinaryStreamTransportImpl<FakeStream> transport(fake, 1UL);

    const uint8_t payload[] = "{}";
    String response;
    const bool ok = transport.request(payload, sizeof(payload) - 1, UnoQBridgeClient::kPayloadJson, response);
    expect(!ok, "Binary stream transport should fail when no response frame arrives");
}

void testClientJsonOperations() {
    ScriptedTransport transport;
    transport.steps.push_back({true, "{\"id\":2,\"ok\":true,\"result\":{\"service\":\"UnoQBridgeService\"}}"});
    transport.steps.push_back({true, "{\"id\":3,\"ok\":true,\"result\":{\"exists\":true}}"});
    transport.steps.push_back({true, "{\"id\":4,\"ok\":true,\"result\":{\"content\":\"line1\\nline2\\tend\"}}"});
    transport.steps.push_back({true, "{\"id\":5,\"ok\":true,\"result\":{\"written\":true}}"});
    transport.steps.push_back({true, "{\"id\":6,\"ok\":true,\"result\":{\"data\":\"AQID\"}}"});

    UnoQBridgeClient client(transport);

    expect(client.ping(), "Client ping should pass on ok=true response");

    bool existsOut = false;
    expect(client.exists("/tmp/demo.txt", existsOut), "Client exists should parse success response");
    expect(existsOut, "Client exists should parse exists=true");

    String content;
    expect(client.readText("/tmp/demo.txt", content), "Client readText should parse content field");
    expect(content == "line1\nline2\tend", "Client readText should unescape newline/tab sequences");

    expect(client.writeText("/tmp/demo.txt", "hello", true), "Client writeText should pass on ok response");

    String data;
    expect(client.readBytesBase64("/tmp/blob.bin", data, 0, 3), "Client readBytesBase64 should parse base64 payload");
    expect(data == "AQID", "Client readBytesBase64 should return expected base64 string");

    expect(transport.seen.size() == 5, "Expected five outbound requests");
    expect(transport.seen[0].payload.find("\"op\":\"ping\"") != String::npos, "Ping payload should include ping opcode");
    expect(transport.seen[1].payload.find("\"op\":\"file.exists\"") != String::npos, "Exists payload should include file.exists opcode");
    expect(transport.seen[2].payload.find("\"op\":\"file.read_text\"") != String::npos, "Read payload should include file.read_text opcode");
    expect(transport.seen[3].payload.find("\"append\":true") != String::npos, "Write payload should preserve append=true");
    expect(transport.seen[4].payload.find("\"op\":\"file.read_bytes\"") != String::npos, "Read-bytes payload should include file.read_bytes opcode");
}

void testClientErrorSurface() {
    ScriptedTransport transport;
    transport.steps.push_back({true, "{\"id\":7,\"ok\":false,\"error\":{\"message\":\"not found\"}}"});

    UnoQBridgeClient client(transport);

    String content;
    const bool ok = client.readText("/missing.txt", content);
    expect(!ok, "Client readText should fail when backend returns ok=false");
    expect(client.lastError().find("not found") != String::npos, "Client should surface backend error message");
}

} // namespace

int main() {
    try {
        testBinaryStreamTransportRoundTrip();
        testBinaryStreamTransportTimeout();
        testClientJsonOperations();
        testClientErrorSurface();
        std::cout << "Experimental UnoQBridge host tests passed." << std::endl;
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Experimental UnoQBridge host tests failed: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Experimental UnoQBridge host tests failed: unknown error" << std::endl;
        return 1;
    }
}
