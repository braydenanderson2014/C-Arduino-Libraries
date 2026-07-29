#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/resource.h>
#include <unistd.h>
#endif

#if defined(__linux__)
#include <malloc.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

#include "Arduino.h"
#include "SD.h"
#if defined(USE_LITTLEFS)
#include "LittleFS.h"
#endif

#include "ArrayList.h"
#include "AVLTree.h"
#include "Hashtable.h"
#include "JSON.h"
#include "SDList.h"
#include "SimpleVector.h"
#include "CustomString.h"

struct TestMemoryStat {
    std::string name;
    std::size_t beforeCurrentBytes;
    std::size_t afterCurrentBytes;
    std::size_t deltaCurrentBytes;
    std::size_t beforeHeapBytes;
    std::size_t afterHeapBytes;
    std::size_t deltaHeapBytes;
    std::size_t peakBytesAfterTest;
    bool passed;
    std::string error;
};


std::size_t getPeakResidentBytes() {
#if defined(__linux__) || defined(__APPLE__)
    struct rusage usage {};
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        return 0;
    }
#if defined(__APPLE__)
    return static_cast<std::size_t>(usage.ru_maxrss);
#else
    return static_cast<std::size_t>(usage.ru_maxrss) * 1024u;
#endif
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters {};
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters)) == 0) {
        return 0;
    }
    return static_cast<std::size_t>(counters.PeakWorkingSetSize);
#else
    return 0;
#endif
}

std::size_t getCurrentResidentBytes() {
#if defined(__linux__)
    std::ifstream statm("/proc/self/statm");
    std::size_t totalPages = 0;
    std::size_t residentPages = 0;
    if (!(statm >> totalPages >> residentPages)) {
        return 0;
    }

    const long pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize <= 0) {
        return 0;
    }

    return residentPages * static_cast<std::size_t>(pageSize);
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters {};
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters)) == 0) {
        return 0;
    }
    return static_cast<std::size_t>(counters.WorkingSetSize);
#elif defined(__APPLE__)
    struct rusage usage {};
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        return 0;
    }
    return static_cast<std::size_t>(usage.ru_maxrss);
#else
    return 0;
#endif
}

std::size_t getCurrentHeapBytes() {
#if defined(__linux__)
#if defined(__GLIBC__) && (__GLIBC__ >= 2)
    struct mallinfo2 info = mallinfo2();
    return static_cast<std::size_t>(info.uordblks);
#else
    struct mallinfo info = mallinfo();
    return static_cast<std::size_t>(info.uordblks);
#endif
#else
    return 0;
#endif
}

std::size_t envToSizeOrDefault(const char* key, std::size_t fallback) {
    const char* raw = std::getenv(key);
    if (!raw || !*raw) {
        return fallback;
    }

    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(raw, &end, 10);
    if (end == raw) {
        return fallback;
    }
    return static_cast<std::size_t>(parsed);
}

bool envToBoolOrDefault(const char* key, bool fallback) {
    const char* raw = std::getenv(key);
    if (!raw || !*raw) {
        return fallback;
    }

    if (std::strcmp(raw, "1") == 0 ||
        std::strcmp(raw, "true") == 0 ||
        std::strcmp(raw, "TRUE") == 0 ||
        std::strcmp(raw, "yes") == 0 ||
        std::strcmp(raw, "YES") == 0 ||
        std::strcmp(raw, "on") == 0 ||
        std::strcmp(raw, "ON") == 0) {
        return true;
    }

    if (std::strcmp(raw, "0") == 0 ||
        std::strcmp(raw, "false") == 0 ||
        std::strcmp(raw, "FALSE") == 0 ||
        std::strcmp(raw, "no") == 0 ||
        std::strcmp(raw, "NO") == 0 ||
        std::strcmp(raw, "off") == 0 ||
        std::strcmp(raw, "OFF") == 0) {
        return false;
    }

    return fallback;
}

std::string envToStringOrDefault(const char* key, const std::string& fallback) {
    const char* raw = std::getenv(key);
    if (!raw || !*raw) {
        return fallback;
    }
    return std::string(raw);
}

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::uint64_t mixChecksum(std::uint64_t seed, std::uint64_t value) {
    seed ^= value;
    seed *= 1099511628211ULL;
    return seed;
}

std::uint64_t checksumArrayList(const ArrayList<int>& list) {
    std::uint64_t hash = 1469598103934665603ULL;
    hash = mixChecksum(hash, static_cast<std::uint64_t>(list.size()));
    for (std::size_t i = 0; i < list.size(); ++i) {
        hash = mixChecksum(hash, static_cast<std::uint64_t>(list.get(i)));
    }
    return hash;
}

std::string escapeJsonString(const std::string& input) {
    std::string escaped;
    escaped.reserve(input.size());

    for (char ch : input) {
        switch (ch) {
            case '\\':
                escaped += "\\\\";
                break;
            case '"':
                escaped += "\\\"";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += ch;
                break;
        }
    }

    return escaped;
}

template <typename Func>
void runTestWithMemoryStats(const char* testName, Func&& fn, std::vector<TestMemoryStat>& memoryStats) {
    TestMemoryStat stat {};
    stat.name = testName;
    stat.beforeCurrentBytes = getCurrentResidentBytes();
    stat.beforeHeapBytes = getCurrentHeapBytes();

    try {
        fn();
        stat.passed = true;
    } catch (const std::exception& ex) {
        stat.passed = false;
        stat.error = ex.what();
        stat.afterCurrentBytes = getCurrentResidentBytes();
        stat.deltaCurrentBytes =
            (stat.afterCurrentBytes >= stat.beforeCurrentBytes)
                ? (stat.afterCurrentBytes - stat.beforeCurrentBytes)
                : 0;
        stat.afterHeapBytes = getCurrentHeapBytes();
        stat.deltaHeapBytes =
            (stat.afterHeapBytes >= stat.beforeHeapBytes)
                ? (stat.afterHeapBytes - stat.beforeHeapBytes)
                : 0;
        stat.peakBytesAfterTest = getPeakResidentBytes();
        memoryStats.push_back(stat);
        throw;
    } catch (...) {
        stat.passed = false;
        stat.error = "Unknown failure";
        stat.afterCurrentBytes = getCurrentResidentBytes();
        stat.deltaCurrentBytes =
            (stat.afterCurrentBytes >= stat.beforeCurrentBytes)
                ? (stat.afterCurrentBytes - stat.beforeCurrentBytes)
                : 0;
        stat.afterHeapBytes = getCurrentHeapBytes();
        stat.deltaHeapBytes =
            (stat.afterHeapBytes >= stat.beforeHeapBytes)
                ? (stat.afterHeapBytes - stat.beforeHeapBytes)
                : 0;
        stat.peakBytesAfterTest = getPeakResidentBytes();
        memoryStats.push_back(stat);
        throw;
    }

    stat.afterCurrentBytes = getCurrentResidentBytes();
    stat.deltaCurrentBytes =
        (stat.afterCurrentBytes >= stat.beforeCurrentBytes)
            ? (stat.afterCurrentBytes - stat.beforeCurrentBytes)
            : 0;
    stat.afterHeapBytes = getCurrentHeapBytes();
    stat.deltaHeapBytes =
        (stat.afterHeapBytes >= stat.beforeHeapBytes)
            ? (stat.afterHeapBytes - stat.beforeHeapBytes)
            : 0;
    stat.peakBytesAfterTest = getPeakResidentBytes();
    memoryStats.push_back(stat);
}

std::vector<std::uint8_t> readAllBytes(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    expect(file.good(), "Unable to open output file for validation");

    file.seekg(0, std::ios::end);
    const std::streamoff endPos = file.tellg();
    expect(endPos >= 0, "Unable to determine output file size");

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(endPos));
    file.seekg(0, std::ios::beg);
    if (!bytes.empty()) {
        file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }
    return bytes;
}

std::uint32_t readU32LE(const std::vector<std::uint8_t>& data, std::size_t offset) {
    expect(offset + 4 <= data.size(), "Attempted to read outside file bounds");
    return static_cast<std::uint32_t>(data[offset]) |
           (static_cast<std::uint32_t>(data[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(data[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(data[offset + 3]) << 24);
}

void testArrayListBasicBehavior() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);
    for (int i = 0; i < 20; ++i) {
        list.add(i * 10);
    }

    expect(list.size() == 20, "ArrayList size should be 20 after inserts");
    expect(list.capacity() >= 20, "ArrayList capacity should grow to fit inserted values");
    expect(list.get(0) == 0, "ArrayList first element mismatch");
    expect(list.get(19) == 190, "ArrayList final element mismatch");

    expect(list.insert(5, 1234), "ArrayList insert at index should succeed");
    expect(list.get(5) == 1234, "ArrayList insert value mismatch");

    list.remove(5);
    expect(list.get(5) == 50, "ArrayList values should shift after remove");
}

void testHashtableBasicBehavior() {
    Hashtable<int, int> table(2, 0.5f);
    for (int i = 0; i < 50; ++i) {
        table.put(i, i * 3);
    }

    expect(table.elements() == 50, "Hashtable should hold 50 key/value pairs");

    int extracted = 0;
    const bool found = table.getElement(37, &extracted);
    expect(found, "Hashtable should find key 37");
    expect(extracted == 111, "Hashtable returned incorrect value for key 37");

    table.remove(37);
    expect(!table.exists(37), "Hashtable key should be removed");
}

void testSDListMemoryMode() {
    SDList<int, 4> list(MEMORY, 4);
    expect(list.begin(4, "memory-mode.bin"), "SDList memory mode begin should succeed");

    for (int i = 0; i < 12; ++i) {
        expect(list.append(i + 1), "SDList memory append should succeed");
    }

    expect(list.size() == 12, "SDList memory mode size mismatch");
    expect(list.get(0) == 1, "SDList memory mode first value mismatch");
    expect(list.get(11) == 12, "SDList memory mode last value mismatch");

    expect(list.insert(3, 999), "SDList memory insert should succeed");
    expect(list.get(3) == 999, "SDList memory inserted value mismatch");

    expect(list.remove(3), "SDList memory remove should succeed");
    expect(list.get(3) == 4, "SDList memory remove shift mismatch");
}

void testSDListFileIOMode(const std::filesystem::path& rootPath) {
    const std::filesystem::path dataFile = rootPath / "sdlist_host_sim.bin";
    std::error_code ec;
    std::filesystem::remove(dataFile, ec);

    const String filename = "sdlist_host_sim.bin";

    SDList<std::int32_t, 4> writer(SDCARD, 4);
    expect(writer.begin(4, filename), "SDList file mode begin should succeed");

    expect(writer.append(10), "SDList file mode append 10 failed");
    expect(writer.append(20), "SDList file mode append 20 failed");
    expect(writer.append(30), "SDList file mode append 30 failed");
    expect(writer.flush(), "SDList file mode flush failed");

    expect(std::filesystem::exists(dataFile), "SDList file mode should create the backing file");

    const std::vector<std::uint8_t> bytes = readAllBytes(dataFile);
    expect(bytes.size() >= 12 + (4 * sizeof(std::int32_t)), "SDList output file size is smaller than expected");

    expect(bytes[0] == 'S' && bytes[1] == 'D' && bytes[2] == 'L' && bytes[3] == '2',
           "SDList output file magic header mismatch");

    const std::uint32_t count = readU32LE(bytes, 4);
    const std::uint32_t reserved = readU32LE(bytes, 8);
    expect(count == 3, "SDList output file count field mismatch");
    expect(reserved >= 4, "SDList output file reserved field mismatch");

    const std::int32_t payload0 = static_cast<std::int32_t>(readU32LE(bytes, 12));
    const std::int32_t payload1 = static_cast<std::int32_t>(readU32LE(bytes, 16));
    const std::int32_t payload2 = static_cast<std::int32_t>(readU32LE(bytes, 20));
    expect(payload0 == 10 && payload1 == 20 && payload2 == 30, "SDList payload values mismatch");

    SDList<std::int32_t, 4> reader(SDCARD, 4);
    expect(reader.begin(4, filename), "SDList reader begin should succeed");
    expect(reader.size() == 3, "SDList reader size mismatch");
    expect(reader.get(0) == 10 && reader.get(1) == 20 && reader.get(2) == 30,
           "SDList reader values mismatch");
}

void testCustomStringBehavior() {
    Custom_String::String greeting("  hello world  ");
    const Custom_String::String trimmed = greeting.Trim();

    expect(trimmed == "hello world", "CustomString Trim should remove surrounding whitespace");
    expect(trimmed.startsWith("hello"), "CustomString startsWith should match valid prefixes");
    expect(!trimmed.startsWith("hello world!"), "CustomString startsWith should reject longer prefixes");
    expect(trimmed.endsWith("world"), "CustomString endsWith should match valid suffixes");
    expect(!trimmed.endsWith("hello world!!!"), "CustomString endsWith should reject longer suffixes");
    expect(trimmed.Replace("missing", "value") == "hello world",
           "CustomString Replace should leave the string unchanged when substring is absent");
    expect(trimmed.remove("missing") == "hello world",
           "CustomString remove should leave the string unchanged when substring is absent");

    Custom_String::String builder;
    builder += "ab";
    builder += 'c';
    builder.append("def");
    expect(builder == "abcdef", "CustomString append and operator+= should preserve content");

    Custom_String::String copied("copy me");
    copied.clear();
    copied.append("done");
    expect(copied == "done", "CustomString clear should reset content and allow reuse");
}

void testJSONRoundTrip() {
    JSON json;
    json.setString("profile.name", "Brayden");
    json.setNumber("profile.version", 2.5);
    json.setBool("profile.active", true);
    json.setNull("profile.empty");
    json.pushBack("items", "alpha");
    json.pushBack("items", "beta");
    json.setString("createdItems.0", "first");
    json.setString("createdItems.1", "second");
    json.setString("profile.note", "quote \"test\"\nline");

    expect(json.hasKey("profile.name"), "JSON should create nested keys with dot notation");
    expect(json.getString("profile.name") == "Brayden", "JSON should return stored string values");
    expect(json.getNumber("profile.version") == 2.5, "JSON should return stored numeric values");
    expect(json.getBool("profile.active"), "JSON should return stored bool values");
    expect(json.isNull("profile.empty"), "JSON should track null values");
    expect(json.getString("items.0") == "alpha" && json.getString("items.1") == "beta",
           "JSON array pushBack should preserve insertion order");
    bool createdItemsIsArray = false;
    const JSON::Node& root = json.getRoot();
    if (root.children) {
        for (size_t i = 0; i < root.children->elements(); ++i) {
            const JSON::Node& child = root.children->get(i);
            if (child.key && std::strcmp(child.key, "createdItems") == 0) {
                createdItemsIsArray = child.type == JSON::ValueType::Array &&
                                      child.children &&
                                      child.children->elements() == 2;
                break;
            }
        }
    }
    expect(createdItemsIsArray, "JSON dot-path creation should treat numeric segments as array indices");

    char* serialized = json.writeToString(false);
    expect(serialized != nullptr, "JSON serialization should allocate an output buffer");

    JSON parsed;
    expect(parsed.readFromString(serialized), "JSON should parse its own serialized output");
    expect(parsed.getString("profile.note") == "quote \"test\"\nline",
           "JSON should preserve escaped string content through a round trip");
    expect(parsed.getString("items.1") == "beta", "JSON should parse serialized arrays correctly");
    expect(parsed.remove("profile.version"), "JSON should remove nested object members");
    expect(!parsed.hasKey("profile.version"), "JSON remove should make nested keys unavailable");

#if JSON_ENABLE_OPTIONAL_RETURNS
    const Optional<String> presentName = parsed.tryGetString("profile.name");
    const Optional<double> presentBoolAsNumber = parsed.tryGetNumber("profile.active");
    const Optional<bool> presentStringAsBool = parsed.tryGetBool("items.0");
    const Optional<double> missingNumber = parsed.tryGetNumber("profile.version");

    expect(presentName.hasValue() && presentName.getValue() == "Brayden",
           "JSON optional getter should return present values when keys exist");
    expect(presentBoolAsNumber.hasValue() && presentBoolAsNumber.getValue() == 1.0,
           "JSON optional number getter should convert bool nodes");
    expect(!presentStringAsBool.hasValue(),
           "JSON optional bool getter should return empty for unsupported string values");
    expect(!missingNumber.hasValue(),
           "JSON optional getter should return empty when keys are missing");
#endif

    std::free(serialized);
}

void testJSONOptionalFeatureGateBehavior() {
    JSON json;
    json.setString("coercion.numericText", "42.5");
    json.setString("coercion.boolTextTrue", "true");
    json.setString("coercion.boolTextFalse", "0");
    json.setString("coercion.invalidBoolText", "not-a-bool");
    json.setNumber("coercion.number", 0.0);
    json.setBool("coercion.bool", true);

#if JSON_ENABLE_OPTIONAL_RETURNS
    const Optional<String> asStringFromNumber = json.tryGetString("coercion.number");
    const Optional<double> asNumberFromString = json.tryGetNumber("coercion.numericText");
    const Optional<bool> asBoolTrue = json.tryGetBool("coercion.boolTextTrue");
    const Optional<bool> asBoolFalse = json.tryGetBool("coercion.boolTextFalse");
    const Optional<bool> invalidBool = json.tryGetBool("coercion.invalidBoolText");
    const Optional<double> missingNumber = json.tryGetNumber("coercion.missing");

    expect(asStringFromNumber.hasValue() && asStringFromNumber.getValue() == "0",
           "JSON optional string getter should convert numbers");
    expect(asNumberFromString.hasValue() && asNumberFromString.getValue() == 42.5,
           "JSON optional number getter should parse numeric strings");
    expect(asBoolTrue.hasValue() && asBoolTrue.getValue(),
           "JSON optional bool getter should parse true-like strings");
    expect(asBoolFalse.hasValue() && !asBoolFalse.getValue(),
           "JSON optional bool getter should parse false-like strings");
    expect(!invalidBool.hasValue(),
           "JSON optional bool getter should reject unsupported strings");
    expect(!missingNumber.hasValue(),
           "JSON optional getters should return empty for missing keys");
#else
    expect(json.getString("coercion.missing", "fallback") == "fallback",
           "JSON non-optional string getter should return provided default for missing keys");
    expect(json.getNumber("coercion.missing", 123.0) == 123.0,
           "JSON non-optional number getter should return provided default for missing keys");
    expect(json.getBool("coercion.missing", true),
           "JSON non-optional bool getter should return provided default for missing keys");
    expect(json.getBool("coercion.boolTextTrue", false),
           "JSON non-optional bool getter should still parse true-like strings");
    expect(!json.getBool("coercion.boolTextFalse", true),
           "JSON non-optional bool getter should still parse false-like strings");
#endif
}

void testJSONFileRoundTrip(const std::filesystem::path& rootPath) {
    const std::filesystem::path filePath = rootPath / "json_host_sim.bin";
    std::error_code ec;
    std::filesystem::remove(filePath, ec);

    JSON writer;
    writer.setString("metadata.name", "host-json");
    writer.setNumber("metadata.revision", 7);
    writer.pushBack("values", "one");
    writer.pushBack("values", "two");

    expect(writer.writeToFile(filePath.string().c_str()) == JSON::JSON_WRITE_SUCCESS,
           "JSON writeToFile should persist serialized content");
    expect(std::filesystem::exists(filePath), "JSON writeToFile should create an output file");

    JSON reader;
    expect(reader.readFromFile(filePath.string().c_str()) == JSON::JSON_READ_SUCCESS,
           "JSON readFromFile should restore serialized content");
    expect(reader.getString("metadata.name") == "host-json", "JSON file read should restore string fields");
    expect(reader.getNumber("metadata.revision") == 7.0, "JSON file read should restore numeric fields");
    expect(reader.getString("values.0") == "one" && reader.getString("values.1") == "two",
           "JSON file read should restore array values");
}

void testArrayListChurnAndBoundaryHealth() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    std::vector<int> model;
    std::uint32_t state = 0x1234abcdU;

    for (int op = 0; op < 3000; ++op) {
        state = state * 1664525U + 1013904223U;
        const int action = static_cast<int>(state % 4U);

        if (action == 0) {
            const int value = static_cast<int>(state & 0x7fffffffU);
            list.add(value);
            model.push_back(value);
        } else if (action == 1) {
            const int value = static_cast<int>((state >> 3) & 0x7fffffffU);
            const std::size_t index = model.empty() ? 0 : static_cast<std::size_t>((state >> 8) % (model.size() + 1));
            expect(list.insert(index, value), "ArrayList churn insert should succeed");
            model.insert(model.begin() + static_cast<std::ptrdiff_t>(index), value);
        } else if (action == 2) {
            if (!model.empty()) {
                const std::size_t index = static_cast<std::size_t>((state >> 12) % model.size());
                list.remove(index);
                model.erase(model.begin() + static_cast<std::ptrdiff_t>(index));
            }
        } else {
            if (!model.empty()) {
                const std::size_t index = static_cast<std::size_t>((state >> 16) % model.size());
                const int value = static_cast<int>((state >> 1) & 0x7fffffffU);
                list[index] = value;
                model[index] = value;
            }
        }

        if ((op % 64) == 0) {
            expect(list.size() == model.size(), "ArrayList churn size mismatch");
            if (!model.empty()) {
                const std::array<std::size_t, 3> indices = {0, model.size() / 2, model.size() - 1};
                for (std::size_t idx : indices) {
                    expect(list.get(idx) == model[idx], "ArrayList churn content mismatch");
                }
            }
            std::uint64_t modelHash = 1469598103934665603ULL;
            modelHash = mixChecksum(modelHash, static_cast<std::uint64_t>(model.size()));
            for (int value : model) {
                modelHash = mixChecksum(modelHash, static_cast<std::uint64_t>(value));
            }
            expect(checksumArrayList(list) == modelHash, "ArrayList churn checksum mismatch");
        }
    }

    const std::size_t beforeBoundary = list.size();
    expect(!list.insert(beforeBoundary + 1, 111), "ArrayList insert should reject index beyond size");
    list.remove(beforeBoundary + 10);
    expect(list.size() == beforeBoundary, "ArrayList out-of-range remove should not change size");
}

void testHashtableChurnAndBoundaryHealth() {
    Hashtable<int, int> table(8, 0.7f);
    std::unordered_map<int, int> model;
    std::uint32_t state = 0x98765432U;

    for (int op = 0; op < 3500; ++op) {
        state = state * 1103515245U + 12345U;
        const int key = static_cast<int>((state >> 1) & 0x7fff);
        const int action = static_cast<int>(state % 3U);

        if (action == 0) {
            const int value = static_cast<int>((state >> 8) & 0x7fffffffU);
            table.put(key, value);
            model[key] = value;
        } else if (action == 1) {
            const bool removed = table.remove(key);
            const bool existed = model.erase(key) > 0;
            expect(removed == existed, "Hashtable remove result mismatch");
        } else {
            int got = 0;
            const bool found = table.getElement(key, &got);
            const auto it = model.find(key);
            expect(found == (it != model.end()), "Hashtable getElement existence mismatch");
            if (it != model.end()) {
                expect(got == it->second, "Hashtable getElement value mismatch");
            }
        }

        if ((op % 96) == 0) {
            expect(table.elements() == static_cast<int>(model.size()), "Hashtable churn size mismatch");
            int checked = 0;
            for (const auto& [k, v] : model) {
                expect(table.exists(k), "Hashtable churn missing expected key");
                expect(table.getElement(k) == v, "Hashtable churn key/value mismatch");
                if (++checked >= 32) break;
            }
        }
    }

    expect(!table.remove(std::numeric_limits<int>::max()), "Hashtable remove should fail for missing key");
}

void testSimpleVectorLifecycleStress() {
    for (int cycle = 0; cycle < 96; ++cycle) {
        SimpleVector<int> vec;
        for (int i = 0; i < 300; ++i) {
            vec.put(cycle * 1000 + i);
        }
        expect(vec.elements() == 300, "SimpleVector lifecycle fill size mismatch");
        expect(vec.get(0) == cycle * 1000, "SimpleVector lifecycle first value mismatch");
        expect(vec.get(299) == cycle * 1000 + 299, "SimpleVector lifecycle last value mismatch");

        vec.erase(9999);
        expect(vec.elements() == 300, "SimpleVector out-of-range erase should not change size");
        vec.clear();
        expect(vec.elements() == 0, "SimpleVector clear should empty vector");

        vec.put(42);
        expect(vec.elements() == 1 && vec.get(0) == 42, "SimpleVector should remain reusable after clear");
    }
}

void testSDListPersistenceStress(const std::filesystem::path& rootPath) {
    const std::filesystem::path dataFile = rootPath / "sdlist_persistence_stress.bin";
    std::error_code ec;
    std::filesystem::remove(dataFile, ec);

    const String filename = "sdlist_persistence_stress.bin";
    for (int round = 0; round < 12; ++round) {
        SDList<std::int32_t, 8> writer(SDCARD, 8);
        expect(writer.begin(4, filename), "SDList persistence writer begin should succeed");
        writer.clear();
        for (int i = 0; i < 80; ++i) {
            expect(writer.append(round * 1000 + i), "SDList persistence append should succeed");
        }
        expect(writer.flush(), "SDList persistence flush should succeed");
    }

    SDList<std::int32_t, 8> reader(SDCARD, 8);
    expect(reader.begin(4, filename), "SDList persistence reader begin should succeed");
    expect(reader.size() == 80, "SDList persistence reader size mismatch");
    expect(reader.get(0) == 11000, "SDList persistence first value mismatch");
    expect(reader.get(79) == 11079, "SDList persistence last value mismatch");

    {
        std::ofstream trunc(dataFile, std::ios::binary | std::ios::trunc);
        trunc.write("BAD!", 4);
    }

    SDList<std::int32_t, 8> recovered(SDCARD, 8);
    expect(recovered.begin(4, filename), "SDList should recover from truncated/corrupt file");
    expect(recovered.size() == 0, "SDList recovery from corrupt file should reset to empty");
    expect(recovered.append(77), "SDList should remain writable after recovery");
    expect(recovered.get(0) == 77, "SDList recovery write/read mismatch");
}

void testJSONPersistenceStress(const std::filesystem::path& rootPath) {
    const std::filesystem::path filePath = rootPath / "json_persistence_stress.bin";
    std::error_code ec;
    std::filesystem::remove(filePath, ec);

    for (int round = 0; round < 20; ++round) {
        JSON writer;
        writer.setString("meta.name", "stress");
        writer.setNumber("meta.round", round);
        for (int i = 0; i < 40; ++i) {
            writer.setNumber((String("values.") + String(i)).c_str(), round * 100 + i);
        }

        expect(writer.writeToFile(filePath.string().c_str(), false) == JSON::JSON_WRITE_SUCCESS,
               "JSON persistence write should succeed");

        JSON reader;
        expect(reader.readFromFile(filePath.string().c_str()) == JSON::JSON_READ_SUCCESS,
               "JSON persistence read should succeed");
        expect(reader.getNumber("meta.round") == static_cast<double>(round),
               "JSON persistence round-trip round mismatch");
        expect(reader.getNumber("values.39") == static_cast<double>(round * 100 + 39),
               "JSON persistence round-trip payload mismatch");
    }

    {
        std::ofstream trunc(filePath, std::ios::binary | std::ios::trunc);
        trunc << "{\"meta\":{\"round\":";
    }

    JSON broken;
    const int readResult = broken.readFromFile(filePath.string().c_str());
    expect(readResult == JSON::JSON_FILE_PARSE_ERROR || readResult == JSON::JSON_READ_ERROR,
           "JSON truncated file should return parse/read error");
}

void testAVLTreeBasicBehavior() {
    AVLTree<int> tree;
    expect(tree.isEmpty(), "AVLTree should start empty");
    expect(tree.size() == 0, "AVLTree size should start at 0");
    expect(tree.findMin() == 0 && tree.findMax() == 0,
           "AVLTree empty min/max should return default int");

    tree.insert(30);
    tree.insert(20);
    tree.insert(40);
    tree.insert(10);
    tree.insert(25);
    tree.insert(35);
    tree.insert(50);
    tree.insert(25); // duplicate ignored

    expect(tree.size() == 7, "AVLTree should ignore duplicate inserts");
    expect(!tree.isEmpty(), "AVLTree should no longer be empty after inserts");
    expect(tree.contains(10) && tree.contains(35), "AVLTree should contain inserted keys");
    expect(!tree.contains(99), "AVLTree should report missing keys");
    expect(tree.find(35) == 35, "AVLTree find should return matching key");
    expect(tree.find(99) == 0, "AVLTree find should return default value when missing");
    expect(tree.findMin() == 10, "AVLTree findMin mismatch");
    expect(tree.findMax() == 50, "AVLTree findMax mismatch");

    tree.remove(40);
    tree.deleteNode(10);
    expect(!tree.contains(40) && !tree.contains(10), "AVLTree remove/deleteNode should erase keys");
    expect(tree.size() == 5, "AVLTree size should decrease after removals");
    expect(tree.getBalance() >= -1 && tree.getBalance() <= 1, "AVLTree root should remain balanced");

    tree.clear();
    expect(tree.isEmpty(), "AVLTree clear should empty tree");
    expect(tree.size() == 0, "AVLTree size should be zero after clear");
}

void testAVLTreeChurnAndHeightHealth() {
    AVLTree<int> tree;

    for (int i = 0; i < 512; ++i) {
        tree.insert(i);
    }
    expect(tree.size() == 512, "AVLTree size mismatch after sequential inserts");
    expect(tree.height() >= 0 && tree.height() <= 20, "AVLTree height should stay logarithmic for 512 inserts");
    expect(tree.findMin() == 0 && tree.findMax() == 511, "AVLTree min/max mismatch after fill");

    for (int i = 1; i < 512; i += 2) {
        tree.remove(i);
    }
    expect(tree.size() == 256, "AVLTree size mismatch after odd removals");
    for (int i = 0; i < 512; ++i) {
        const bool shouldExist = (i % 2) == 0;
        expect(tree.contains(i) == shouldExist, "AVLTree key presence mismatch after churn");
    }
    expect(tree.height() >= 0 && tree.height() <= 20, "AVLTree height should remain bounded after churn");

    for (int i = 0; i < 512; i += 2) {
        tree.remove(i);
    }
    expect(tree.isEmpty(), "AVLTree should be empty after removing all elements");
    expect(tree.findMin() == 0 && tree.findMax() == 0,
           "AVLTree empty min/max should return default after full clear-by-removal");
}

void writeReport(const std::filesystem::path& reportPath,
                 bool success,
                 std::size_t peakBytes,
                 std::size_t limitBytes,
                 const std::string& backend,
                 const std::string& errorMessage) {
    std::ofstream report(reportPath, std::ios::binary);
    if (!report.good()) {
        return;
    }

    report << "{\n";
    report << "  \"success\": " << (success ? "true" : "false") << ",\n";
    report << "  \"backend\": \"" << backend << "\",\n";
    report << "  \"memory\": {\n";
    report << "    \"peakBytes\": " << peakBytes << ",\n";
    report << "    \"limitBytes\": " << limitBytes << "\n";
    report << "  },\n";
    report << "  \"error\": \"" << errorMessage << "\"\n";
    report << "}\n";
}

void writeMemoryStatsReport(const std::filesystem::path& statsPath,
                            const std::vector<TestMemoryStat>& memoryStats,
                            bool success,
                            std::size_t peakBytes,
                            std::size_t limitBytes,
                            const std::string& backend,
                            const std::string& errorMessage) {
    std::ofstream report(statsPath, std::ios::binary);
    if (!report.good()) {
        return;
    }

    report << "{\n";
    report << "  \"success\": " << (success ? "true" : "false") << ",\n";
    report << "  \"backend\": \"" << escapeJsonString(backend) << "\",\n";
    report << "  \"memory\": {\n";
    report << "    \"peakBytes\": " << peakBytes << ",\n";
    report << "    \"limitBytes\": " << limitBytes << "\n";
    report << "  },\n";
    report << "  \"tests\": [\n";

    for (std::size_t i = 0; i < memoryStats.size(); ++i) {
        const TestMemoryStat& stat = memoryStats[i];
        report << "    {\n";
        report << "      \"name\": \"" << escapeJsonString(stat.name) << "\",\n";
        report << "      \"passed\": " << (stat.passed ? "true" : "false") << ",\n";
        report << "      \"beforeCurrentBytes\": " << stat.beforeCurrentBytes << ",\n";
        report << "      \"afterCurrentBytes\": " << stat.afterCurrentBytes << ",\n";
        report << "      \"deltaCurrentBytes\": " << stat.deltaCurrentBytes << ",\n";
        report << "      \"beforeHeapBytes\": " << stat.beforeHeapBytes << ",\n";
        report << "      \"afterHeapBytes\": " << stat.afterHeapBytes << ",\n";
        report << "      \"deltaHeapBytes\": " << stat.deltaHeapBytes << ",\n";
        report << "      \"peakBytesAfterTest\": " << stat.peakBytesAfterTest << ",\n";
        report << "      \"error\": \"" << escapeJsonString(stat.error) << "\"\n";
        report << "    }";
        if (i + 1 < memoryStats.size()) {
            report << ",";
        }
        report << "\n";
    }

    report << "  ],\n";
    report << "  \"error\": \"" << escapeJsonString(errorMessage) << "\"\n";
    report << "}\n";
}


int main() {
    const std::size_t memoryLimitBytes = envToSizeOrDefault("HOST_MEM_LIMIT_BYTES", 8u * 1024u * 1024u);
    const std::filesystem::path fsRoot = envToStringOrDefault("HOST_SIM_FS_ROOT", "test/host_arduino_sim/out/fs");
    const std::filesystem::path reportPath = envToStringOrDefault(
        "HOST_SIM_REPORT",
        "test/host_arduino_sim/out/host-arduino-sim-report.json"
    );
    const std::filesystem::path statsReportPath = envToStringOrDefault(
        "HOST_SIM_STATS_REPORT",
        "test/host_arduino_sim/out/host-arduino-sim-stats.json"
    );

    std::filesystem::create_directories(fsRoot);
    std::filesystem::create_directories(reportPath.parent_path());
    std::filesystem::create_directories(statsReportPath.parent_path());

    SD.setRoot(fsRoot.string());
#if defined(USE_LITTLEFS)
    LittleFS.setRoot(fsRoot.string());
#endif

    bool success = false;
    std::string error;
    std::size_t peak = 0;
    std::vector<TestMemoryStat> memoryStats;

    try {
        runTestWithMemoryStats("testArrayListBasicBehavior", testArrayListBasicBehavior, memoryStats);
        runTestWithMemoryStats("testHashtableBasicBehavior", testHashtableBasicBehavior, memoryStats);
        runTestWithMemoryStats("testSDListMemoryMode", testSDListMemoryMode, memoryStats);
        runTestWithMemoryStats(
            "testSDListFileIOMode",
            [&]() { testSDListFileIOMode(fsRoot); },
            memoryStats
        );
        runTestWithMemoryStats("testCustomStringBehavior", testCustomStringBehavior, memoryStats);
        runTestWithMemoryStats("testJSONRoundTrip", testJSONRoundTrip, memoryStats);
        runTestWithMemoryStats("testArrayListChurnAndBoundaryHealth", testArrayListChurnAndBoundaryHealth, memoryStats);
        runTestWithMemoryStats("testHashtableChurnAndBoundaryHealth", testHashtableChurnAndBoundaryHealth, memoryStats);
        runTestWithMemoryStats("testSimpleVectorLifecycleStress", testSimpleVectorLifecycleStress, memoryStats);
        runTestWithMemoryStats(
            "testJSONOptionalFeatureGateBehavior",
            testJSONOptionalFeatureGateBehavior,
            memoryStats
        );
        runTestWithMemoryStats(
            "testJSONFileRoundTrip",
            [&]() { testJSONFileRoundTrip(fsRoot); },
            memoryStats
        );
        runTestWithMemoryStats(
            "testSDListPersistenceStress",
            [&]() { testSDListPersistenceStress(fsRoot); },
            memoryStats
        );
        runTestWithMemoryStats(
            "testJSONPersistenceStress",
            [&]() { testJSONPersistenceStress(fsRoot); },
            memoryStats
        );
        runTestWithMemoryStats("testAVLTreeBasicBehavior", testAVLTreeBasicBehavior, memoryStats);
        runTestWithMemoryStats("testAVLTreeChurnAndHeightHealth", testAVLTreeChurnAndHeightHealth, memoryStats);

        peak = getPeakResidentBytes();
        success = true;
        std::cout << "Host simulation tests passed." << std::endl;
        std::cout << "Memory peak bytes: " << peak << std::endl;
    } catch (const std::exception& ex) {
        error = ex.what();
        std::cerr << "Host simulation tests failed: " << error << std::endl;
    } catch (...) {
        error = "Unknown failure";
        std::cerr << "Host simulation tests failed: unknown failure" << std::endl;
    }

    if (peak == 0) {
        peak = getPeakResidentBytes();
    }
    const std::string backend =
#if defined(USE_LITTLEFS)
        "LittleFS";
#else
        "SD";
#endif

    writeReport(reportPath, success, peak, memoryLimitBytes, backend, error);
    writeMemoryStatsReport(
        statsReportPath,
        memoryStats,
        success,
        peak,
        memoryLimitBytes,
        backend,
        error
    );

    return success ? 0 : 1;
}
