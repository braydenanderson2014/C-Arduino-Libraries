#include <UnoQBridge.h>
#include <Arduino.h>

UnoQBridge bridge;

static uint8_t passed = 0;
static uint8_t failed = 0;

static void check(const char* name, bool condition) {
    if (condition) {
        Serial.print(F("[PASS] "));
        ++passed;
    } else {
        Serial.print(F("[FAIL] "));
        ++failed;
    }
    Serial.println(name);
}

static void section(const char* title) {
    Serial.println(F("----------------------------------------"));
    Serial.print(F("[Section] "));
    Serial.println(title);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    Serial.println(F("=== UnoQBridge Self-Test ==="));

    // --- Initialization ---
    section("Initialization");
    check("begin() returns true",           bridge.begin());
    check("isReady() true after begin()",   bridge.isReady());
    check("lastError() empty after begin()", bridge.lastError().length() == 0);

    // --- Sequence ID ---
    section("Sequence ID");
    uint16_t id1 = bridge.nextSequenceId();
    uint16_t id2 = bridge.nextSequenceId();
    uint16_t id3 = bridge.nextSequenceId();
    check("1st nextSequenceId() == 1", id1 == 1);
    check("2nd nextSequenceId() == 2", id2 == 2);
    check("3rd nextSequenceId() == 3", id3 == 3);
    check("IDs are monotonically increasing", id1 < id2 && id2 < id3);

    // --- Error handling ---
    section("Error Handling");
    bridge.setLastError("test error");
    check("lastError() returns set value",      bridge.lastError() == "test error");
    bridge.setLastError("");
    check("lastError() clears to empty string", bridge.lastError().length() == 0);
    bridge.setLastError("second error");
    check("lastError() holds second value",     bridge.lastError() == "second error");

    // --- Reset ---
    section("Reset");
    bridge.reset();
    check("isReady() false after reset()",        !bridge.isReady());
    check("lastError() empty after reset()",       bridge.lastError().length() == 0);
    bridge.begin();
    check("isReady() true after begin() post-reset", bridge.isReady());
    uint16_t idAfterReset = bridge.nextSequenceId();
    check("nextSequenceId() restarts at 1 after reset+begin", idAfterReset == 1);

    // --- FrameHeader struct ---
    section("FrameHeader");
    UnoQBridge::FrameHeader hdr;
    hdr.magic         = UnoQBridge::kMagic;
    hdr.version       = UnoQBridge::kProtocolVersion;
    hdr.opcode        = static_cast<uint8_t>(UnoQBridge::OpCode::Ping);
    hdr.sequenceId    = bridge.nextSequenceId();
    hdr.resourceId    = 42;
    hdr.payloadLength = 0;

    check("FrameHeader magic matches kMagic",              hdr.magic         == UnoQBridge::kMagic);
    check("FrameHeader version matches kProtocolVersion",  hdr.version       == UnoQBridge::kProtocolVersion);
    check("FrameHeader opcode set to Ping (1)",            hdr.opcode        == 1);
    check("FrameHeader sequenceId == 2 (second after reset)", hdr.sequenceId == 2);
    check("FrameHeader resourceId round-trips",            hdr.resourceId    == 42);
    check("FrameHeader payloadLength == 0",                hdr.payloadLength == 0);

    // --- Protocol constants ---
    section("Protocol Constants");
    check("kMagic == 0x5155",        UnoQBridge::kMagic            == 0x5155);
    check("kProtocolVersion == 1",   UnoQBridge::kProtocolVersion  == 1);
    check("kMaxPayloadBytes == 256", UnoQBridge::kMaxPayloadBytes  == 256);
    check("OpCode::Ping   == 1",     static_cast<uint8_t>(UnoQBridge::OpCode::Ping)   == 1);
    check("OpCode::Status == 2",     static_cast<uint8_t>(UnoQBridge::OpCode::Status) == 2);
    check("OpCode::Read   == 3",     static_cast<uint8_t>(UnoQBridge::OpCode::Read)   == 3);
    check("OpCode::Write  == 4",     static_cast<uint8_t>(UnoQBridge::OpCode::Write)  == 4);
    check("OpCode::Custom == 255",   static_cast<uint8_t>(UnoQBridge::OpCode::Custom) == 255);

    // --- Summary ---
    Serial.println(F("========================================"));
    Serial.print(F("Results: "));
    Serial.print(passed);
    Serial.print(F(" passed, "));
    Serial.print(failed);
    Serial.println(F(" failed"));
    Serial.println(failed == 0 ? F("ALL TESTS PASSED") : F("SOME TESTS FAILED"));
    Serial.println(F("========================================"));
}

void loop() {}
