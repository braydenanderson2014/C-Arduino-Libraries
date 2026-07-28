// host_stress_tests.cpp
// Memory stress test runner for Arduino library containers.
//
// This binary measures capacity metrics (instance counts, element fill limits)
// against board-profile SRAM budgets.  It is a measurement tool — it always
// exits with status 0 regardless of whether a limit was reached.  Tests that
// check correctness live in host_arduino_sim_tests.cpp.
//
// Environment variables
// ---------------------
//   HOST_MEM_LIMIT_BYTES        Heap-delta budget in bytes (default: sramBytes*1024 set by workflow)
//   HOST_STRESS_BOARD           Board identifier string, e.g. "uno"  (default: "unknown")
//   HOST_STRESS_SRAM_BYTES      Original SRAM size in bytes for the board (default: 0)
//   HOST_STRESS_BACKEND         "SD" or "LittleFS"                      (default: "SD")
//   HOST_STRESS_OPTIONAL        "on" or "off" for JSON optional mode    (default: "off")
//   HOST_STRESS_REPORT          Output path for the stress JSON report
//   HOST_STRESS_MAX_INSTANCES   Max instances to create per probe        (default: 5000)
//   HOST_STRESS_MAX_ELEMENTS    Max elements to add per fill probe       (default: 100000)

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/resource.h>
#include <unistd.h>
#endif
#if defined(__linux__)
#include <malloc.h>
#endif

#include "Arduino.h"
#include "ArrayList.h"
#include "Hashtable.h"
#include "JSON.h"
#include "SimpleVector.h"

// Mask applied to std::size_t loop counters before casting to int/key, keeping
// the value non-negative regardless of the platform's int width.
static constexpr unsigned int MAX_POSITIVE_INT_MASK = 0x7fffffffu;

// ─── Memory measurement ──────────────────────────────────────────────────────

// Returns current allocator-managed heap bytes (Linux glibc only).
// Returns 0 on unsupported platforms; probes still run to maxCount in that case.
static std::size_t getHeapBytes() {
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

// ─── Environment helpers ─────────────────────────────────────────────────────

static std::size_t envSize(const char* key, std::size_t fallback) {
    const char* raw = std::getenv(key);
    if (!raw || !*raw) return fallback;
    char* end = nullptr;
    const unsigned long long v = std::strtoull(raw, &end, 10);
    return (end == raw) ? fallback : static_cast<std::size_t>(v);
}

static std::string envStr(const char* key, const std::string& fallback) {
    const char* raw = std::getenv(key);
    return (raw && *raw) ? std::string(raw) : fallback;
}

// ─── JSON escaping ────────────────────────────────────────────────────────────

static std::string escJson(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

// ─── Probe result structs ─────────────────────────────────────────────────────

struct InstanceProbeResult {
    std::string typeName;
    std::size_t maxInstances = 0;
    bool        limitReached = false;
    std::size_t bytesAtStop  = 0;
    std::size_t limitBytes   = 0;
};

struct ElementFillProbeResult {
    std::string containerType;
    std::string elementType;
    std::size_t maxElements  = 0;
    bool        limitReached = false;
    std::size_t bytesAtStop  = 0;
    std::size_t limitBytes   = 0;
};

// ─── Instance count probe ─────────────────────────────────────────────────────
// Creates heap-allocated instances via factory(), counting how many can exist
// simultaneously before the heap delta from baseline reaches limitBytes.

template <typename T, typename Factory>
static InstanceProbeResult probeInstanceCount(
    const std::string& typeName,
    std::size_t limitBytes,
    std::size_t maxInstances,
    Factory factory
) {
    InstanceProbeResult result;
    result.typeName   = typeName;
    result.limitBytes = limitBytes;

    const std::size_t baseline = getHeapBytes();
    std::vector<std::unique_ptr<T>> instances;
    instances.reserve(std::min(maxInstances, std::size_t{512}));

    for (std::size_t i = 0; i < maxInstances; ++i) {
        try {
            instances.push_back(std::unique_ptr<T>(factory()));
        } catch (const std::bad_alloc&) {
            result.limitReached = true;
            result.maxInstances = i;
            result.bytesAtStop  = getHeapBytes();
            return result;
        }

        // Check every 16 instances; skip check when heap measurement unavailable.
        if ((i % 16) == 0 && baseline > 0) {
            const std::size_t current = getHeapBytes();
            const std::size_t delta   = current > baseline ? current - baseline : 0;
            if (delta >= limitBytes) {
                result.limitReached = true;
                result.maxInstances = i + 1;
                result.bytesAtStop  = current;
                return result;
            }
        }
    }

    result.maxInstances = instances.size();
    result.bytesAtStop  = getHeapBytes();
    if (baseline > 0 && result.bytesAtStop > baseline) {
        result.limitReached = (result.bytesAtStop - baseline) >= limitBytes;
    }
    return result;
}

// ─── Element fill probe ───────────────────────────────────────────────────────
// Creates a single container with makeContainer(), then fills it with
// addElement(container, index) until the heap delta from baseline reaches
// limitBytes or maxElements is exhausted.

template <typename Container, typename MakeContainer, typename AddElement>
static ElementFillProbeResult probeElementFill(
    const std::string& containerType,
    const std::string& elementType,
    std::size_t limitBytes,
    std::size_t maxElements,
    MakeContainer makeContainer,
    AddElement    addElement
) {
    ElementFillProbeResult result;
    result.containerType = containerType;
    result.elementType   = elementType;
    result.limitBytes    = limitBytes;

    const std::size_t baseline = getHeapBytes();

    try {
        auto container = makeContainer();

        for (std::size_t i = 0; i < maxElements; ++i) {
            try {
                addElement(container, i);
            } catch (const std::bad_alloc&) {
                result.limitReached = true;
                result.maxElements  = i;
                result.bytesAtStop  = getHeapBytes();
                return result;
            }

            // Check every 256 elements; skip when heap measurement unavailable.
            if ((i % 256) == 0 && baseline > 0) {
                const std::size_t current = getHeapBytes();
                const std::size_t delta   = current > baseline ? current - baseline : 0;
                if (delta >= limitBytes) {
                    result.limitReached = true;
                    result.maxElements  = i + 1;
                    result.bytesAtStop  = current;
                    return result;
                }
            }
        }

        result.maxElements = maxElements;
        result.bytesAtStop = getHeapBytes();
        if (baseline > 0 && result.bytesAtStop > baseline) {
            result.limitReached = (result.bytesAtStop - baseline) >= limitBytes;
        }
    } catch (const std::bad_alloc&) {
        result.limitReached = true;
        result.maxElements  = 0;
        result.bytesAtStop  = getHeapBytes();
    }

    return result;
}

// ─── Report writing ───────────────────────────────────────────────────────────

static void writeStressReport(
    const std::filesystem::path& reportPath,
    const std::string& board,
    std::size_t sramBytes,
    std::size_t limitBytes,
    const std::string& backend,
    const std::string& optional,
    const std::vector<InstanceProbeResult>& instanceProbes,
    const std::vector<ElementFillProbeResult>& fillProbes
) {
    std::error_code ec;
    std::filesystem::create_directories(reportPath.parent_path(), ec);

    std::ofstream out(reportPath, std::ios::binary);
    if (!out.good()) {
        std::cerr << "Failed to open stress report for writing: " << reportPath << std::endl;
        return;
    }

    out << "{\n";
    out << "  \"board\": \"" << escJson(board) << "\",\n";
    out << "  \"sramBytes\": " << sramBytes << ",\n";
    out << "  \"limitBytes\": " << limitBytes << ",\n";
    out << "  \"backend\": \"" << escJson(backend) << "\",\n";
    out << "  \"optional\": \"" << escJson(optional) << "\",\n";

    out << "  \"instanceCountProbes\": [\n";
    for (std::size_t i = 0; i < instanceProbes.size(); ++i) {
        const auto& p = instanceProbes[i];
        out << "    {\n";
        out << "      \"type\": \""         << escJson(p.typeName)                     << "\",\n";
        out << "      \"maxInstances\": "   << p.maxInstances                          << ",\n";
        out << "      \"limitReached\": "   << (p.limitReached ? "true" : "false")     << ",\n";
        out << "      \"bytesAtStop\": "    << p.bytesAtStop                           << ",\n";
        out << "      \"limitBytes\": "     << p.limitBytes                            << "\n";
        out << "    }";
        if (i + 1 < instanceProbes.size()) out << ",";
        out << "\n";
    }
    out << "  ],\n";

    out << "  \"elementFillProbes\": [\n";
    for (std::size_t i = 0; i < fillProbes.size(); ++i) {
        const auto& p = fillProbes[i];
        out << "    {\n";
        out << "      \"container\": \""    << escJson(p.containerType)                << "\",\n";
        out << "      \"elementType\": \""  << escJson(p.elementType)                  << "\",\n";
        out << "      \"maxElements\": "    << p.maxElements                           << ",\n";
        out << "      \"limitReached\": "   << (p.limitReached ? "true" : "false")     << ",\n";
        out << "      \"bytesAtStop\": "    << p.bytesAtStop                           << ",\n";
        out << "      \"limitBytes\": "     << p.limitBytes                            << "\n";
        out << "    }";
        if (i + 1 < fillProbes.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";
}

// ─── main ─────────────────────────────────────────────────────────────────────

int main() {
    const std::size_t limitBytes   = envSize("HOST_MEM_LIMIT_BYTES",       8u * 1024u * 1024u);
    const std::size_t sramBytes    = envSize("HOST_STRESS_SRAM_BYTES",      0u);
    const std::size_t maxInstances = envSize("HOST_STRESS_MAX_INSTANCES",   5000u);
    const std::size_t maxElements  = envSize("HOST_STRESS_MAX_ELEMENTS",    100000u);
    const std::string board        = envStr ("HOST_STRESS_BOARD",           "unknown");
    const std::string backend      = envStr ("HOST_STRESS_BACKEND",         "SD");
    const std::string optional     = envStr ("HOST_STRESS_OPTIONAL",        "off");
    const std::filesystem::path reportPath = envStr(
        "HOST_STRESS_REPORT",
        "test/host_arduino_sim/out/host-stress-report.json"
    );

    std::cout << "Host stress tester starting." << std::endl;
    std::cout << "Board: "        << board     << std::endl;
    std::cout << "SRAM bytes: "   << sramBytes << std::endl;
    std::cout << "Limit bytes: "  << limitBytes << std::endl;
    std::cout << "Max instances per probe: " << maxInstances << std::endl;
    std::cout << "Max elements per probe:  " << maxElements  << std::endl;

    std::vector<InstanceProbeResult>    instanceProbes;
    std::vector<ElementFillProbeResult> fillProbes;

    // ── Instance count probes ────────────────────────────────────────────────
    // Measures how many empty heap-allocated instances fit within the budget.

    std::cout << "Running instance count probes..." << std::endl;

    instanceProbes.push_back(probeInstanceCount<ArrayList<int>>(
        "ArrayList_int", limitBytes, maxInstances,
        []() { return new ArrayList<int>(ArrayList<int>::DYNAMIC2, 8); }
    ));

    instanceProbes.push_back(probeInstanceCount<SimpleVector<int>>(
        "SimpleVector_int", limitBytes, maxInstances,
        []() { return new SimpleVector<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<Hashtable<int, int>>(
        "Hashtable_int_int", limitBytes, maxInstances,
        []() { return new Hashtable<int, int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<JSON>(
        "JSON", limitBytes, maxInstances,
        []() { return new JSON(); }
    ));

    // ── Element fill probes ──────────────────────────────────────────────────
    // Measures how many elements fit in a single container instance.

    std::cout << "Running element fill probes..." << std::endl;

    // ArrayList<int>
    fillProbes.push_back(probeElementFill<ArrayList<int>>(
        "ArrayList", "int", limitBytes, maxElements,
        []() { return ArrayList<int>(ArrayList<int>::DYNAMIC2, 8); },
        [](ArrayList<int>& c, std::size_t i) { c.add(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); }
    ));

    // ArrayList<float>
    fillProbes.push_back(probeElementFill<ArrayList<float>>(
        "ArrayList", "float", limitBytes, maxElements,
        []() { return ArrayList<float>(ArrayList<float>::DYNAMIC2, 8); },
        [](ArrayList<float>& c, std::size_t i) { c.add(static_cast<float>(i)); }
    ));

    // ArrayList<double>
    fillProbes.push_back(probeElementFill<ArrayList<double>>(
        "ArrayList", "double", limitBytes, maxElements,
        []() { return ArrayList<double>(ArrayList<double>::DYNAMIC2, 8); },
        [](ArrayList<double>& c, std::size_t i) { c.add(static_cast<double>(i)); }
    ));

    // ArrayList<String>
    fillProbes.push_back(probeElementFill<ArrayList<String>>(
        "ArrayList", "String", limitBytes, maxElements,
        []() { return ArrayList<String>(ArrayList<String>::DYNAMIC2, 8); },
        [](ArrayList<String>& c, std::size_t i) { c.add(String(static_cast<int>(i))); }
    ));

    // SimpleVector<int>
    fillProbes.push_back(probeElementFill<SimpleVector<int>>(
        "SimpleVector", "int", limitBytes, maxElements,
        []() { return SimpleVector<int>(); },
        [](SimpleVector<int>& c, std::size_t i) { c.put(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); }
    ));

    // SimpleVector<float>
    fillProbes.push_back(probeElementFill<SimpleVector<float>>(
        "SimpleVector", "float", limitBytes, maxElements,
        []() { return SimpleVector<float>(); },
        [](SimpleVector<float>& c, std::size_t i) { c.put(static_cast<float>(i)); }
    ));

    // SimpleVector<double>
    fillProbes.push_back(probeElementFill<SimpleVector<double>>(
        "SimpleVector", "double", limitBytes, maxElements,
        []() { return SimpleVector<double>(); },
        [](SimpleVector<double>& c, std::size_t i) { c.put(static_cast<double>(i)); }
    ));

    // Hashtable<int,int>
    fillProbes.push_back(probeElementFill<Hashtable<int, int>>(
        "Hashtable", "int_int", limitBytes, maxElements,
        []() { return Hashtable<int, int>(); },
        [](Hashtable<int, int>& c, std::size_t i) {
            const int k = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
            c.put(k, k);
        }
    ));

    // Hashtable<String,String>
    fillProbes.push_back(probeElementFill<Hashtable<String, String>>(
        "Hashtable", "String_String", limitBytes, maxElements,
        []() { return Hashtable<String, String>(); },
        [](Hashtable<String, String>& c, std::size_t i) {
            String key = String(static_cast<int>(i));
            c.put(key, key);
        }
    ));

    // ── Print summary ────────────────────────────────────────────────────────

    std::cout << "Stress probes complete. Results:" << std::endl;
    for (const auto& p : instanceProbes) {
        std::cout << "  Instance[" << p.typeName << "]: " << p.maxInstances
                  << " (limitReached=" << (p.limitReached ? "yes" : "no") << ")" << std::endl;
    }
    for (const auto& p : fillProbes) {
        std::cout << "  Fill[" << p.containerType << "<" << p.elementType << ">]: "
                  << p.maxElements << " elements"
                  << " (limitReached=" << (p.limitReached ? "yes" : "no") << ")" << std::endl;
    }

    // ── Write report ─────────────────────────────────────────────────────────

    std::cout << "Writing stress report to: " << reportPath << std::endl;
    writeStressReport(reportPath, board, sramBytes, limitBytes, backend, optional,
                      instanceProbes, fillProbes);
    std::cout << "Stress report written successfully." << std::endl;

    return 0;
}
