#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/resource.h>
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
#include "Hashtable.h"
#include "SDList.h"

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

int main() {
    const std::size_t memoryLimitBytes = envToSizeOrDefault("HOST_MEM_LIMIT_BYTES", 8u * 1024u * 1024u);
    const std::filesystem::path fsRoot = envToStringOrDefault("HOST_SIM_FS_ROOT", "test/host_arduino_sim/out/fs");
    const std::filesystem::path reportPath = envToStringOrDefault(
        "HOST_SIM_REPORT",
        "test/host_arduino_sim/out/host-arduino-sim-report.json"
    );

    std::filesystem::create_directories(fsRoot);
    std::filesystem::create_directories(reportPath.parent_path());

    SD.setRoot(fsRoot.string());
#if defined(USE_LITTLEFS)
    LittleFS.setRoot(fsRoot.string());
#endif

    bool success = false;
    std::string error;
    std::size_t peak = 0;

    try {
        testArrayListBasicBehavior();
        testHashtableBasicBehavior();
        testSDListMemoryMode();
        testSDListFileIOMode(fsRoot);

        peak = getPeakResidentBytes();
        expect(peak <= memoryLimitBytes, "Host memory peak exceeded expected budget");

        success = true;
        std::cout << "Host simulation tests passed." << std::endl;
        std::cout << "Memory peak bytes: " << peak << " (limit " << memoryLimitBytes << ")" << std::endl;
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

    return success ? 0 : 1;
}
