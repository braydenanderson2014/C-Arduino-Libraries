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
//   HOST_STRESS_APPEND          Append report objects to report file       (default: 0)
//   HOST_STRESS_MAX_INSTANCES   Max instances to create per probe        (default: 5000)
//   HOST_STRESS_MAX_ELEMENTS    Max elements to add per fill probe       (default: 1000000)

#include <algorithm>
#include <array>
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
#include "AVLTree.h"
#include "DynamicStorageLibrary.h"
#include "Hashtable.h"
#include "JSON.h"
#include "Operators.h"
#include "Predicates.h"
#include "SimpleVector.h"

// Mask applied to std::size_t loop counters before casting to int/key, keeping
// the value non-negative regardless of the platform's int width.
static constexpr unsigned int MAX_POSITIVE_INT_MASK = 0x7fffffffu;
// A large positive key kept below INT_MAX to avoid collision with sequential inserts.
static constexpr int AVLTREE_PROBE_SENTINEL = 2147483000;

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

static bool envBool(const char* key, bool fallback) {
    const char* raw = std::getenv(key);
    if (!raw || !*raw) return fallback;
    if (std::strcmp(raw, "1") == 0 || std::strcmp(raw, "true") == 0 || std::strcmp(raw, "TRUE") == 0) {
        return true;
    }
    if (std::strcmp(raw, "0") == 0 || std::strcmp(raw, "false") == 0 || std::strcmp(raw, "FALSE") == 0) {
        return false;
    }
    return fallback;
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
    std::size_t baselineBytes = 0;
    std::size_t postCleanupBytes = 0;
    std::size_t postCleanupDelta = 0;
};

struct ElementFillProbeResult {
    std::string containerType;
    std::string elementType;
    std::size_t maxElements  = 0;
    bool        limitReached = false;
    std::size_t bytesAtStop  = 0;
    std::size_t limitBytes   = 0;
    std::size_t baselineBytes = 0;
    std::size_t postCleanupBytes = 0;
    std::size_t postCleanupDelta = 0;
    std::size_t healthChecks = 0;
    std::size_t healthFailures = 0;
    bool        recoveryCheckPassed = true;
    std::uint64_t checksumAtStop = 0;
};

template <typename T>
static std::uint64_t toU64Bits(const T& value) {
    std::uint64_t out = 0;
    constexpr std::size_t copyBytes = sizeof(T) < sizeof(std::uint64_t) ? sizeof(T) : sizeof(std::uint64_t);
    std::memcpy(&out, &value, copyBytes);
    return out;
}

static std::uint64_t toU64Bits(const String& value) {
    std::uint64_t hash = 1469598103934665603ULL;
    const char* ptr = value.c_str();
    while (ptr && *ptr) {
        hash ^= static_cast<std::uint64_t>(static_cast<unsigned char>(*ptr));
        hash *= 1099511628211ULL;
        ++ptr;
    }
    return hash;
}

static std::uint64_t mixU64(std::uint64_t seed, std::uint64_t value) {
    constexpr std::uint64_t kMul = 1099511628211ULL;
    seed ^= value;
    seed *= kMul;
    return seed;
}

template <typename Getter>
static std::uint64_t sampleChecksum(std::size_t count, Getter getValue) {
    std::uint64_t hash = 1469598103934665603ULL;
    hash = mixU64(hash, static_cast<std::uint64_t>(count));
    if (count == 0) return hash;
    const std::size_t stride = std::max<std::size_t>(1u, count / 64u);
    for (std::size_t i = 0; i < count; i += stride) {
        hash = mixU64(hash, toU64Bits(getValue(i)));
    }
    if ((count - 1) % stride != 0) {
        hash = mixU64(hash, toU64Bits(getValue(count - 1)));
    }
    return hash;
}

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
    result.baselineBytes = getHeapBytes();
    const std::size_t baseline = result.baselineBytes;

    {
        std::vector<std::unique_ptr<T>> instances;
        instances.reserve(std::min(maxInstances, std::size_t{512}));

        for (std::size_t i = 0; i < maxInstances; ++i) {
            try {
                instances.push_back(std::unique_ptr<T>(factory()));
            } catch (const std::bad_alloc&) {
                result.limitReached = true;
                result.maxInstances = i;
                result.bytesAtStop  = getHeapBytes();
                break;
            }

            result.maxInstances = i + 1;

            // Check every 16 instances; skip check when heap measurement unavailable.
            if ((i % 16) == 0 && baseline > 0) {
                const std::size_t current = getHeapBytes();
                const std::size_t delta   = current > baseline ? current - baseline : 0;
                if (delta >= limitBytes) {
                    result.limitReached = true;
                    result.bytesAtStop  = current;
                    break;
                }
            }
        }

        if (result.bytesAtStop == 0) {
            result.bytesAtStop = getHeapBytes();
        }
    }

    result.postCleanupBytes = getHeapBytes();
    if (baseline > 0 && result.postCleanupBytes > baseline) {
        result.postCleanupDelta = result.postCleanupBytes - baseline;
    }
    if (baseline > 0 && result.bytesAtStop > baseline) {
        result.limitReached = (result.bytesAtStop - baseline) >= limitBytes;
    }
    return result;
}

// ─── Element fill probe ───────────────────────────────────────────────────────
// Creates a single container with makeContainer(), then fills it with
// addElement(container, index) until the heap delta from baseline reaches
// limitBytes or maxElements is exhausted.

template <typename Container, typename MakeContainer, typename AddElement, typename HealthCheck, typename Checksum, typename RecoveryCheck>
static ElementFillProbeResult probeElementFill(
    const std::string& containerType,
    const std::string& elementType,
    std::size_t limitBytes,
    std::size_t maxElements,
    MakeContainer makeContainer,
    AddElement    addElement,
    HealthCheck   healthCheck,
    Checksum      checksum,
    RecoveryCheck recoveryCheck
) {
    ElementFillProbeResult result;
    result.containerType = containerType;
    result.elementType   = elementType;
    result.limitBytes    = limitBytes;

    result.baselineBytes = getHeapBytes();
    const std::size_t baseline = result.baselineBytes;

    try {
        auto container = makeContainer();
        bool stop = false;

        for (std::size_t i = 0; i < maxElements; ++i) {
            try {
                addElement(container, i);
            } catch (const std::bad_alloc&) {
                result.limitReached = true;
                result.maxElements  = i;
                result.bytesAtStop  = getHeapBytes();
                break;
            }

            result.maxElements = i + 1;

            if ((i % 128) == 0) {
                ++result.healthChecks;
                if (!healthCheck(container, i + 1)) {
                    ++result.healthFailures;
                    result.limitReached = true;
                    result.bytesAtStop = getHeapBytes();
                    stop = true;
                    break;
                }
                result.checksumAtStop = checksum(container, i + 1);
            }

            // Check every 256 elements; skip when heap measurement unavailable.
            if ((i % 256) == 0 && baseline > 0) {
                const std::size_t current = getHeapBytes();
                const std::size_t delta   = current > baseline ? current - baseline : 0;
                if (delta >= limitBytes) {
                    result.limitReached = true;
                    result.maxElements  = i + 1;
                    result.bytesAtStop  = current;
                    stop = true;
                    break;
                }
            }
        }

        if (!stop && result.maxElements == 0) {
            result.maxElements = maxElements;
        }
        if (result.bytesAtStop == 0) {
            result.bytesAtStop = getHeapBytes();
        }
        result.recoveryCheckPassed = recoveryCheck(container, result.maxElements);
        ++result.healthChecks;
        if (!healthCheck(container, result.maxElements)) {
            ++result.healthFailures;
            result.recoveryCheckPassed = false;
        }
        result.checksumAtStop = checksum(container, result.maxElements);
        if (baseline > 0 && result.bytesAtStop > baseline) {
            result.limitReached = (result.bytesAtStop - baseline) >= limitBytes;
        }
    } catch (const std::bad_alloc&) {
        result.limitReached = true;
        result.maxElements  = 0;
        result.bytesAtStop  = getHeapBytes();
        result.recoveryCheckPassed = false;
    }

    result.postCleanupBytes = getHeapBytes();
    if (baseline > 0 && result.postCleanupBytes > baseline) {
        result.postCleanupDelta = result.postCleanupBytes - baseline;
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
    bool append,
    const std::vector<InstanceProbeResult>& instanceProbes,
    const std::vector<ElementFillProbeResult>& fillProbes
) {
    std::error_code ec;
    std::filesystem::create_directories(reportPath.parent_path(), ec);

    std::ios::openmode mode = std::ios::binary;
    if (append) {
        mode |= std::ios::app;
    }
    std::ofstream out(reportPath, mode);
    if (!out.good()) {
        std::cerr << "Failed to open stress report for writing: " << reportPath << std::endl;
        return;
    }

    if (append) {
        out << "\n";
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
        out << "      \"limitBytes\": "     << p.limitBytes                            << ",\n";
        out << "      \"baselineBytes\": "  << p.baselineBytes                         << ",\n";
        out << "      \"postCleanupBytes\": " << p.postCleanupBytes                    << ",\n";
        out << "      \"postCleanupDelta\": " << p.postCleanupDelta                    << "\n";
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
        out << "      \"limitBytes\": "     << p.limitBytes                            << ",\n";
        out << "      \"baselineBytes\": "  << p.baselineBytes                         << ",\n";
        out << "      \"postCleanupBytes\": " << p.postCleanupBytes                    << ",\n";
        out << "      \"postCleanupDelta\": " << p.postCleanupDelta                    << ",\n";
        out << "      \"healthChecks\": "   << p.healthChecks                          << ",\n";
        out << "      \"healthFailures\": " << p.healthFailures                        << ",\n";
        out << "      \"recoveryCheckPassed\": " << (p.recoveryCheckPassed ? "true" : "false") << ",\n";
        out << "      \"checksumAtStop\": " << p.checksumAtStop                        << "\n";
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
    const std::size_t maxElements  = envSize("HOST_STRESS_MAX_ELEMENTS",    1000000u);
    const std::string board        = envStr ("HOST_STRESS_BOARD",           "unknown");
    const std::string backend      = envStr ("HOST_STRESS_BACKEND",         "SD");
    const std::string optional     = envStr ("HOST_STRESS_OPTIONAL",        "off");
    const bool appendReport        = envBool("HOST_STRESS_APPEND",          false);
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
    std::cout << "Append report mode: " << (appendReport ? "on" : "off") << std::endl;

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

    instanceProbes.push_back(probeInstanceCount<AVLTree<int>>(
        "AVLTree_int", limitBytes, maxInstances,
        []() { return new AVLTree<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<DynamicStorage<String, int>>(
        "DynamicStorage_String_int", limitBytes, maxInstances,
        []() { return new DynamicStorage<String, int>(DynamicStorage<String, int>::RAM); }
    ));

    instanceProbes.push_back(probeInstanceCount<Predicates<int>>(
        "Predicates_int", limitBytes, maxInstances,
        []() { return new Predicates<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<Operators<int>>(
        "Operators_int", limitBytes, maxInstances,
        []() { return new Operators<int>(); }
    ));

    // ── Element fill probes ──────────────────────────────────────────────────
    // Measures how many elements fit in a single container instance.

    std::cout << "Running element fill probes..." << std::endl;

    // ArrayList<int>
    fillProbes.push_back(probeElementFill<ArrayList<int>>(
        "ArrayList", "int", limitBytes, maxElements,
        []() { return ArrayList<int>(ArrayList<int>::DYNAMIC2, 8); },
        [](ArrayList<int>& c, std::size_t i) { c.add(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); },
        [](ArrayList<int>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            const std::array<std::size_t, 3> idx = {0, expected / 2, expected - 1};
            for (std::size_t i : idx) {
                if (c.get(i) != static_cast<int>(i & MAX_POSITIVE_INT_MASK)) return false;
            }
            return true;
        },
        [](ArrayList<int>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(i); });
        },
        [](ArrayList<int>& c, std::size_t expected) {
            const std::size_t before = c.size();
            int last = 0;
            if (before > 0) last = c.get(before - 1);
            try {
                if (!c.insert(before, 0x5a5a5a5a)) {
                    return c.size() == before && (before == 0 || c.get(before - 1) == last);
                }
            } catch (const std::bad_alloc&) {
                return c.size() == before && (before == 0 || c.get(before - 1) == last);
            }
            c.remove(c.size() - 1);
            return c.size() == before && (before == 0 || c.get(before - 1) == last) && before == expected;
        }
    ));

    // ArrayList<float>
    fillProbes.push_back(probeElementFill<ArrayList<float>>(
        "ArrayList", "float", limitBytes, maxElements,
        []() { return ArrayList<float>(ArrayList<float>::DYNAMIC2, 8); },
        [](ArrayList<float>& c, std::size_t i) { c.add(static_cast<float>(i)); },
        [](ArrayList<float>& c, std::size_t expected) { return c.size() == expected; },
        [](ArrayList<float>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(i); });
        },
        [](ArrayList<float>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try {
                if (!c.insert(before, 1234.5f)) return c.size() == before;
            } catch (const std::bad_alloc&) {
                return c.size() == before;
            }
            c.remove(c.size() - 1);
            return c.size() == before && before == expected;
        }
    ));

    // ArrayList<double>
    fillProbes.push_back(probeElementFill<ArrayList<double>>(
        "ArrayList", "double", limitBytes, maxElements,
        []() { return ArrayList<double>(ArrayList<double>::DYNAMIC2, 8); },
        [](ArrayList<double>& c, std::size_t i) { c.add(static_cast<double>(i)); },
        [](ArrayList<double>& c, std::size_t expected) { return c.size() == expected; },
        [](ArrayList<double>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(i); });
        },
        [](ArrayList<double>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try {
                if (!c.insert(before, 98765.4321)) return c.size() == before;
            } catch (const std::bad_alloc&) {
                return c.size() == before;
            }
            c.remove(c.size() - 1);
            return c.size() == before && before == expected;
        }
    ));

    // ArrayList<String>
    fillProbes.push_back(probeElementFill<ArrayList<String>>(
        "ArrayList", "String", limitBytes, maxElements,
        []() { return ArrayList<String>(ArrayList<String>::DYNAMIC2, 8); },
        [](ArrayList<String>& c, std::size_t i) { c.add(String(static_cast<int>(i))); },
        [](ArrayList<String>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            return c.get(expected - 1) == String(static_cast<int>(expected - 1));
        },
        [](ArrayList<String>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(i); });
        },
        [](ArrayList<String>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try {
                if (!c.insert(before, String("recovery"))) return c.size() == before;
            } catch (const std::bad_alloc&) {
                return c.size() == before;
            }
            c.remove(c.size() - 1);
            return c.size() == before && before == expected;
        }
    ));

    // SimpleVector<int>
    fillProbes.push_back(probeElementFill<SimpleVector<int>>(
        "SimpleVector", "int", limitBytes, maxElements,
        []() { return SimpleVector<int>(); },
        [](SimpleVector<int>& c, std::size_t i) { c.put(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); },
        [](SimpleVector<int>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.elements()) != expected) return false;
            if (expected == 0) return true;
            return c.get(static_cast<unsigned int>(expected - 1)) == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](SimpleVector<int>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(static_cast<unsigned int>(i)); });
        },
        [](SimpleVector<int>& c, std::size_t expected) {
            const unsigned int before = c.elements();
            int last = 0;
            if (before > 0) last = c.get(before - 1);
            try {
                c.put(0x7f7f7f7f);
            } catch (const std::bad_alloc&) {
                return c.elements() == before && (before == 0 || c.get(before - 1) == last);
            }
            c.erase(static_cast<int>(c.elements() - 1));
            return c.elements() == before && (before == 0 || c.get(before - 1) == last) && before == expected;
        }
    ));

    // SimpleVector<float>
    fillProbes.push_back(probeElementFill<SimpleVector<float>>(
        "SimpleVector", "float", limitBytes, maxElements,
        []() { return SimpleVector<float>(); },
        [](SimpleVector<float>& c, std::size_t i) { c.put(static_cast<float>(i)); },
        [](SimpleVector<float>& c, std::size_t expected) { return static_cast<std::size_t>(c.elements()) == expected; },
        [](SimpleVector<float>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(static_cast<unsigned int>(i)); });
        },
        [](SimpleVector<float>& c, std::size_t expected) {
            const unsigned int before = c.elements();
            try {
                c.put(1.0f);
            } catch (const std::bad_alloc&) {
                return c.elements() == before;
            }
            c.erase(static_cast<int>(c.elements() - 1));
            return c.elements() == before && before == expected;
        }
    ));

    // SimpleVector<double>
    fillProbes.push_back(probeElementFill<SimpleVector<double>>(
        "SimpleVector", "double", limitBytes, maxElements,
        []() { return SimpleVector<double>(); },
        [](SimpleVector<double>& c, std::size_t i) { c.put(static_cast<double>(i)); },
        [](SimpleVector<double>& c, std::size_t expected) { return static_cast<std::size_t>(c.elements()) == expected; },
        [](SimpleVector<double>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) { return c.get(static_cast<unsigned int>(i)); });
        },
        [](SimpleVector<double>& c, std::size_t expected) {
            const unsigned int before = c.elements();
            try {
                c.put(2.0);
            } catch (const std::bad_alloc&) {
                return c.elements() == before;
            }
            c.erase(static_cast<int>(c.elements() - 1));
            return c.elements() == before && before == expected;
        }
    ));

    // Hashtable<int,int>
    fillProbes.push_back(probeElementFill<Hashtable<int, int>>(
        "Hashtable", "int_int", limitBytes, maxElements,
        []() { return Hashtable<int, int>(); },
        [](Hashtable<int, int>& c, std::size_t i) {
            const int k = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
            c.put(k, k);
        },
        [](Hashtable<int, int>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.elements()) != expected) return false;
            if (expected == 0) return true;
            const std::array<std::size_t, 3> idx = {0, expected / 2, expected - 1};
            for (std::size_t i : idx) {
                const int key = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
                if (!c.exists(key)) return false;
                if (c.getElement(key) != key) return false;
            }
            return true;
        },
        [](Hashtable<int, int>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                const int key = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
                return c.getElement(key);
            });
        },
        [](Hashtable<int, int>& c, std::size_t expected) {
            const int probeKey = expected > 0 ? static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK) : 0;
            const int probeValue = expected > 0 ? c.getElement(probeKey) : 0;
            const std::size_t before = static_cast<std::size_t>(c.elements());
            const int recoveryKey = -2147480000 + static_cast<int>(before % 1000);
            try {
                c.put(recoveryKey, 42);
            } catch (const std::bad_alloc&) {
                return static_cast<std::size_t>(c.elements()) == before &&
                       (expected == 0 || c.getElement(probeKey) == probeValue);
            }
            if (!c.remove(recoveryKey)) return false;
            return static_cast<std::size_t>(c.elements()) == before &&
                   (expected == 0 || c.getElement(probeKey) == probeValue);
        }
    ));

    // Hashtable<String,String>
    fillProbes.push_back(probeElementFill<Hashtable<String, String>>(
        "Hashtable", "String_String", limitBytes, maxElements,
        []() { return Hashtable<String, String>(); },
        [](Hashtable<String, String>& c, std::size_t i) {
            String key = String(static_cast<int>(i));
            c.put(key, key);
        },
        [](Hashtable<String, String>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.elements()) != expected) return false;
            if (expected == 0) return true;
            String last = String(static_cast<int>(expected - 1));
            return c.exists(last) && c.getElement(last) == last;
        },
        [](Hashtable<String, String>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                return c.getElement(String(static_cast<int>(i)));
            });
        },
        [](Hashtable<String, String>& c, std::size_t expected) {
            const std::size_t before = static_cast<std::size_t>(c.elements());
            String probe = expected > 0 ? String(static_cast<int>(expected - 1)) : String("0");
            String probeValue = c.getElement(probe);
            String recoveryKey("__recovery__");
            try {
                c.put(recoveryKey, "ok");
            } catch (const std::bad_alloc&) {
                return static_cast<std::size_t>(c.elements()) == before &&
                       (expected == 0 || c.getElement(probe) == probeValue);
            }
            if (!c.remove(recoveryKey)) return false;
            return static_cast<std::size_t>(c.elements()) == before &&
                   (expected == 0 || c.getElement(probe) == probeValue);
        }
    ));

    // AVLTree<int>
    fillProbes.push_back(probeElementFill<AVLTree<int>>(
        "AVLTree", "int", limitBytes, maxElements,
        []() { return AVLTree<int>(); },
        [](AVLTree<int>& c, std::size_t i) {
            c.insert(static_cast<int>(i & MAX_POSITIVE_INT_MASK));
        },
        [](AVLTree<int>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            const std::array<std::size_t, 3> idx = {0, expected / 2, expected - 1};
            for (std::size_t i : idx) {
                const int key = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
                if (!c.contains(key)) return false;
            }
            return c.findMin() == 0 && c.findMax() == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](AVLTree<int>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                const int key = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
                return c.find(key);
            });
        },
        [](AVLTree<int>& c, std::size_t expected) {
            const std::size_t before = c.size();
            const int probe = expected > 0 ? static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK) : 0;
            const bool hadProbe = expected > 0 && c.contains(probe);
            try { c.insert(AVLTREE_PROBE_SENTINEL); } catch (const std::bad_alloc&) {}
            c.remove(AVLTREE_PROBE_SENTINEL);
            return c.size() == before && (!hadProbe || c.contains(probe));
        }
    ));

    // DynamicStorage<String,int> in RAM mode
    fillProbes.push_back(probeElementFill<DynamicStorage<String, int>>(
        "DynamicStorage", "String_int", limitBytes, maxElements,
        []() { return DynamicStorage<String, int>(DynamicStorage<String, int>::RAM); },
        [](DynamicStorage<String, int>& c, std::size_t i) {
            const int key = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
            c.put(String(key), static_cast<int>(i & MAX_POSITIVE_INT_MASK));
        },
        [](DynamicStorage<String, int>& c, std::size_t expected) {
            if (expected == 0) return true;
            const int probeKey = static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
            const String probe = String(probeKey);
            return c.hasKey(probe) && c.get(probe) == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](DynamicStorage<String, int>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                return c.get(String(static_cast<int>(i)));
            });
        },
        [](DynamicStorage<String, int>& c, std::size_t expected) {
            if (expected == 0) {
                c.put("__recovery__", 1);
                return c.get("__recovery__") == 1;
            }
            const String probe = String(static_cast<int>(expected - 1));
            const int beforeValue = c.get(probe);
            c.put("__recovery__", 7);
            return c.get("__recovery__") == 7 && c.get(probe) == beforeValue;
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
                      appendReport,
                      instanceProbes, fillProbes);
    std::cout << "Stress report written successfully." << std::endl;

    return 0;
}
