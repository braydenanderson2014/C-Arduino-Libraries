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
//   HOST_STRESS_PROGRESS_INTERVAL_MS
//                               Progress heartbeat interval in milliseconds (default: 60000)

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <unordered_set>
#include <vector>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef NOUSER
#define NOUSER
#endif
#include <windows.h>
#include <psapi.h>
#else
#include <thread>
#endif

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
#include "BasicLinkedList.h"
#include "DynamicStorageLibrary.h"
#include "Hashtable.h"
#include "JSON.h"
#include "Operators.h"
#include "Optional.h"
#include "OrderedMap.h"
#include "Predicates.h"
#include "Queue.h"
#include "SimpleVector.h"
#include "ExtremeVariant.h"
#include "Stack.h"
#include "Variant.h"

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
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters {};
    const BOOL ok = GetProcessMemoryInfo(
        GetCurrentProcess(),
        reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters),
        sizeof(counters)
    );
    if (ok == 0) {
        return 0;
    }
    // PrivateUsage is the process-private commit, suitable for monotonic stress deltas.
    return static_cast<std::size_t>(counters.PrivateUsage);
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

struct HeartbeatState {
    std::string phase;
    std::string probeName;
    std::size_t maxCount = 0;
    std::size_t baselineBytes = 0;
    std::size_t limitBytes = 0;
    std::size_t currentCount = 0;
    std::uint64_t startedMs = 0;
    std::uint64_t lastAdvanceMs = 0;
    std::uint64_t lastPulseMs = 0;
    bool active = false;
};

static std::size_t gStressProgressIntervalMs = 60000u;
static std::string gStressBoard = "unknown";
static std::string gStressBackend = "unknown";
static std::string gStressOptional = "off";
static HeartbeatState gHeartbeat;
static std::mutex gHeartbeatMutex;
static bool gHeartbeatShutdown = false;

static std::uint64_t monotonicMs() {
    using namespace std::chrono;
    return static_cast<std::uint64_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

static void logStressProgress(
    const char* phase,
    const std::string& probeName,
    std::size_t currentCount,
    std::size_t maxCount,
    std::size_t baselineBytes,
    std::size_t limitBytes,
    std::uint64_t elapsedMs,
    std::uint64_t stalledMs,
    bool force = false
) {
    const std::size_t currentHeap = getHeapBytes();
    const std::size_t heapDelta = (baselineBytes > 0 && currentHeap > baselineBytes) ? (currentHeap - baselineBytes) : 0;

    std::cout << "[stress-progress]"
              << " phase=" << phase
              << " probe=" << probeName
              << " board=" << gStressBoard
              << " backend=" << gStressBackend
              << " optional=" << gStressOptional
              << " count=" << currentCount << "/" << maxCount
              << " elapsedMs=" << elapsedMs
              << " stalledMs=" << stalledMs;

    if (baselineBytes > 0 && limitBytes > 0) {
        const std::size_t pct = static_cast<std::size_t>((heapDelta * 100u) / limitBytes);
        std::cout << " heapDelta=" << heapDelta
                  << " limit=" << limitBytes
                  << " fillPct=" << pct;
    } else {
        std::cout << " heapDelta=unknown"
                  << " limit=" << limitBytes
                  << " fillPct=unknown";
    }

    if (force) {
        std::cout << " marker=forced";
    }

    std::cout << std::endl;
}

static void heartbeatStartProbe(
    const std::string& phase,
    const std::string& probeName,
    std::size_t maxCount,
    std::size_t baselineBytes,
    std::size_t limitBytes
) {
    const std::uint64_t nowMs = monotonicMs();
    {
        std::lock_guard<std::mutex> lock(gHeartbeatMutex);
        gHeartbeat.phase = phase;
        gHeartbeat.probeName = probeName;
        gHeartbeat.maxCount = maxCount;
        gHeartbeat.baselineBytes = baselineBytes;
        gHeartbeat.limitBytes = limitBytes;
        gHeartbeat.currentCount = 0;
        gHeartbeat.startedMs = nowMs;
        gHeartbeat.lastAdvanceMs = nowMs;
        gHeartbeat.lastPulseMs = nowMs;
        gHeartbeat.active = true;
    }
    logStressProgress((phase + ":start").c_str(), probeName, 0, maxCount, baselineBytes, limitBytes, 0, 0, true);
}

static void heartbeatAdvance(std::size_t currentCount) {
    const std::uint64_t nowMs = monotonicMs();
    std::lock_guard<std::mutex> lock(gHeartbeatMutex);
    if (!gHeartbeat.active) {
        return;
    }
    gHeartbeat.currentCount = currentCount;
    gHeartbeat.lastAdvanceMs = nowMs;
}

static void heartbeatEndProbe(std::size_t currentCount) {
    HeartbeatState snapshot;
    const std::uint64_t nowMs = monotonicMs();
    {
        std::lock_guard<std::mutex> lock(gHeartbeatMutex);
        if (!gHeartbeat.active) {
            return;
        }
        gHeartbeat.currentCount = currentCount;
        snapshot = gHeartbeat;
        gHeartbeat.active = false;
    }
    const std::uint64_t elapsedMs = (nowMs > snapshot.startedMs) ? (nowMs - snapshot.startedMs) : 0;
    const std::uint64_t stalledMs = (nowMs > snapshot.lastAdvanceMs) ? (nowMs - snapshot.lastAdvanceMs) : 0;
    logStressProgress((snapshot.phase + ":end").c_str(), snapshot.probeName, snapshot.currentCount, snapshot.maxCount,
                      snapshot.baselineBytes, snapshot.limitBytes, elapsedMs, stalledMs, true);
}

static void heartbeatWorkerLoop() {
    while (true) {
        HeartbeatState snapshot;
        {
            std::lock_guard<std::mutex> lock(gHeartbeatMutex);
            if (gHeartbeatShutdown) {
                break;
            }
            snapshot = gHeartbeat;
        }

        if (snapshot.active && gStressProgressIntervalMs > 0) {
            const std::uint64_t nowMs = monotonicMs();
            if ((nowMs - snapshot.lastPulseMs) >= gStressProgressIntervalMs) {
                const std::uint64_t elapsedMs = (nowMs > snapshot.startedMs) ? (nowMs - snapshot.startedMs) : 0;
                const std::uint64_t stalledMs = (nowMs > snapshot.lastAdvanceMs) ? (nowMs - snapshot.lastAdvanceMs) : 0;
                logStressProgress((snapshot.phase + ":running").c_str(), snapshot.probeName, snapshot.currentCount,
                                  snapshot.maxCount, snapshot.baselineBytes, snapshot.limitBytes, elapsedMs, stalledMs, false);
                std::lock_guard<std::mutex> lock(gHeartbeatMutex);
                if (gHeartbeat.active) {
                    gHeartbeat.lastPulseMs = nowMs;
                }
            }
        }

        #if defined(_WIN32)
        Sleep(250);
        #else
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        #endif
    }
}

#if defined(_WIN32)
static DWORD WINAPI heartbeatThreadProc(LPVOID) {
    heartbeatWorkerLoop();
    return 0;
}
#endif

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
    std::uint64_t durationMs = 0;
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
    std::uint64_t durationMs = 0;
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

static std::string toLowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

static bool isOptionalOnMode(const std::string& optionalMode) {
    return toLowerCopy(optionalMode) == "on";
}

static bool isSdBackendMode(const std::string& backendMode) {
    const std::string normalized = toLowerCopy(backendMode);
    return normalized == "sd";
}

static bool supportsOptionalOnForInstance(const std::string& typeName) {
    static const std::unordered_set<std::string> kSupported = {
        "JSON",
        "AVLTree_int",
    };
    return kSupported.find(typeName) != kSupported.end();
}

static bool supportsOptionalOnForFill(const std::string& containerType, const std::string& elementType) {
    const std::string key = containerType + "[" + elementType + "]";
    static const std::unordered_set<std::string> kSupported = {
        "AVLTree[int]",
    };
    return kSupported.find(key) != kSupported.end();
}

static bool requiresSdBackendForInstance(const std::string& typeName) {
    (void)typeName;
    return false;
}

static bool requiresSdBackendForFill(const std::string& containerType, const std::string& elementType) {
    (void)containerType;
    (void)elementType;
    return false;
}

static void filterStressProbesForCapabilities(
    const std::string& backend,
    const std::string& optional,
    std::vector<InstanceProbeResult>& instanceProbes,
    std::vector<ElementFillProbeResult>& fillProbes
) {
    const bool optionalOn = isOptionalOnMode(optional);
    const bool sdBackend = isSdBackendMode(backend);

    std::vector<InstanceProbeResult> filteredInstances;
    filteredInstances.reserve(instanceProbes.size());
    for (const auto& probe : instanceProbes) {
        if (optionalOn && !supportsOptionalOnForInstance(probe.typeName)) {
            continue;
        }
        if (requiresSdBackendForInstance(probe.typeName) && !sdBackend) {
            continue;
        }
        filteredInstances.push_back(probe);
    }

    std::vector<ElementFillProbeResult> filteredFills;
    filteredFills.reserve(fillProbes.size());
    for (const auto& probe : fillProbes) {
        if (optionalOn && !supportsOptionalOnForFill(probe.containerType, probe.elementType)) {
            continue;
        }
        if (requiresSdBackendForFill(probe.containerType, probe.elementType) && !sdBackend) {
            continue;
        }
        filteredFills.push_back(probe);
    }

    instanceProbes.swap(filteredInstances);
    fillProbes.swap(filteredFills);
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
    const std::uint64_t startedMs = monotonicMs();
    InstanceProbeResult result;
    result.typeName   = typeName;
    result.limitBytes = limitBytes;
    result.baselineBytes = getHeapBytes();
    const std::size_t baseline = result.baselineBytes;

    heartbeatStartProbe("instance", typeName, maxInstances, baseline, limitBytes);

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
            heartbeatAdvance(result.maxInstances);

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
    const std::uint64_t endedMs = monotonicMs();
    result.durationMs = (endedMs > startedMs) ? (endedMs - startedMs) : 0;
    heartbeatEndProbe(result.maxInstances);
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
    const std::uint64_t startedMs = monotonicMs();
    ElementFillProbeResult result;
    result.containerType = containerType;
    result.elementType   = elementType;
    result.limitBytes    = limitBytes;

    result.baselineBytes = getHeapBytes();
    const std::size_t baseline = result.baselineBytes;
    const std::string probeName = containerType + "<" + elementType + ">";
    heartbeatStartProbe("fill", probeName, maxElements, baseline, limitBytes);

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
            heartbeatAdvance(result.maxElements);

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
    const std::uint64_t endedMs = monotonicMs();
    result.durationMs = (endedMs > startedMs) ? (endedMs - startedMs) : 0;
    heartbeatEndProbe(result.maxElements);
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
    std::uint64_t totalDurationMs,
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
    out << "  \"totalDurationMs\": " << totalDurationMs << ",\n";

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
        out << "      \"postCleanupDelta\": " << p.postCleanupDelta                    << ",\n";
        out << "      \"durationMs\": " << p.durationMs                              << "\n";
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
        out << "      \"checksumAtStop\": " << p.checksumAtStop                        << ",\n";
        out << "      \"durationMs\": " << p.durationMs                              << "\n";
        out << "    }";
        if (i + 1 < fillProbes.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";
}

// ─── main ─────────────────────────────────────────────────────────────────────

int main() {
    const std::uint64_t runStartedMs = monotonicMs();
    const std::size_t limitBytes   = envSize("HOST_MEM_LIMIT_BYTES",       8u * 1024u * 1024u);
    const std::size_t sramBytes    = envSize("HOST_STRESS_SRAM_BYTES",      0u);
    const std::size_t maxInstances = envSize("HOST_STRESS_MAX_INSTANCES",   5000u);
    const std::size_t maxElements  = envSize("HOST_STRESS_MAX_ELEMENTS",    1000000u);
    const std::size_t progressIntervalMs = envSize("HOST_STRESS_PROGRESS_INTERVAL_MS", 60000u);
    const std::string board        = envStr ("HOST_STRESS_BOARD",           "unknown");
    const std::string backend      = envStr ("HOST_STRESS_BACKEND",         "SD");
    const std::string optional     = envStr ("HOST_STRESS_OPTIONAL",        "off");
    const bool appendReport        = envBool("HOST_STRESS_APPEND",          false);
    const std::filesystem::path reportPath = envStr(
        "HOST_STRESS_REPORT",
        "test/host_arduino_sim/out/host-stress-report.json"
    );

    gStressProgressIntervalMs = progressIntervalMs;
    gStressBoard = board;
    gStressBackend = backend;
    gStressOptional = optional;
    gHeartbeatShutdown = false;
    #if defined(_WIN32)
    HANDLE heartbeatThread = CreateThread(nullptr, 0, heartbeatThreadProc, nullptr, 0, nullptr);
    #else
    std::thread heartbeatThread(heartbeatWorkerLoop);
    #endif

    std::cout << "Host stress tester starting." << std::endl;
    std::cout << "Board: "        << board     << std::endl;
    std::cout << "SRAM bytes: "   << sramBytes << std::endl;
    std::cout << "Limit bytes: "  << limitBytes << std::endl;
    std::cout << "Max instances per probe: " << maxInstances << std::endl;
    std::cout << "Max elements per probe:  " << maxElements  << std::endl;
    std::cout << "Progress interval (ms):  " << progressIntervalMs << std::endl;
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

    instanceProbes.push_back(probeInstanceCount<Stack<int>>(
        "Stack_int", limitBytes, maxInstances,
        []() { return new Stack<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<Queue<int>>(
        "Queue_int", limitBytes, maxInstances,
        []() { return new Queue<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<LinkedList<int>>(
        "LinkedList_int", limitBytes, maxInstances,
        []() { return new LinkedList<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<OrderedMap<int, int>>(
        "OrderedMap_int_int", limitBytes, maxInstances,
        []() { return new OrderedMap<int, int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<Variant<int>>(
        "Variant_int", limitBytes, maxInstances,
        []() { return new Variant<int>(); }
    ));

    instanceProbes.push_back(probeInstanceCount<ExtremeVariant<int, int>>(
        "ExtremeVariant_int_int", limitBytes, maxInstances,
        []() { return new ExtremeVariant<int, int>(); }
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

    // ArrayList<Optional<int>>
    // Measures how many Optional<int>-wrapped elements fit vs bare int (see ArrayList<int> above).
    fillProbes.push_back(probeElementFill<ArrayList<Optional<int>>>(
        "ArrayList", "Optional_int", limitBytes, maxElements,
        []() { return ArrayList<Optional<int>>(ArrayList<Optional<int>>::DYNAMIC2, 8); },
        [](ArrayList<Optional<int>>& c, std::size_t i) {
            c.add(Optional<int>(static_cast<int>(i & MAX_POSITIVE_INT_MASK)));
        },
        [](ArrayList<Optional<int>>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            const Optional<int> last = c.get(expected - 1);
            return last.hasValue() && last.getValue() == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](ArrayList<Optional<int>>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                const Optional<int> v = c.get(i);
                return v.hasValue() ? static_cast<double>(v.getValue()) : 0.0;
            });
        },
        [](ArrayList<Optional<int>>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try {
                if (!c.insert(before, Optional<int>(0x5a5a5a5a))) return c.size() == before;
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

    // SimpleVector<Optional<int>>
    // Measures how many Optional<int>-wrapped elements fit vs bare int (see SimpleVector<int> above).
    fillProbes.push_back(probeElementFill<SimpleVector<Optional<int>>>(
        "SimpleVector", "Optional_int", limitBytes, maxElements,
        []() { return SimpleVector<Optional<int>>(); },
        [](SimpleVector<Optional<int>>& c, std::size_t i) {
            c.put(Optional<int>(static_cast<int>(i & MAX_POSITIVE_INT_MASK)));
        },
        [](SimpleVector<Optional<int>>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.elements()) != expected) return false;
            if (expected == 0) return true;
            const Optional<int> last = c.get(static_cast<unsigned int>(expected - 1));
            return last.hasValue() && last.getValue() == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](SimpleVector<Optional<int>>& c, std::size_t expected) {
            return sampleChecksum(expected, [&](std::size_t i) {
                const Optional<int> v = c.get(static_cast<unsigned int>(i));
                return v.hasValue() ? static_cast<double>(v.getValue()) : 0.0;
            });
        },
        [](SimpleVector<Optional<int>>& c, std::size_t expected) {
            const unsigned int before = c.elements();
            try {
                c.put(Optional<int>(0x7f7f7f7f));
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

    // Stack<int>
    fillProbes.push_back(probeElementFill<Stack<int>>(
        "Stack", "int", limitBytes, maxElements,
        []() { return Stack<int>(); },
        [](Stack<int>& c, std::size_t i) { c.push(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); },
        [](Stack<int>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.count()) != expected) return false;
            if (expected == 0) return true;
            return c.peek() == static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
        },
        [](Stack<int>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) {
                return expected > 0 ? c.peek() : 0;
            });
        },
        [](Stack<int>& c, std::size_t expected) {
            const int before = c.count();
            try {
                c.push(0x5a5a5a5a);
            } catch (const std::bad_alloc&) {
                return c.count() == before;
            }
            c.pop();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Stack<float>
    fillProbes.push_back(probeElementFill<Stack<float>>(
        "Stack", "float", limitBytes, maxElements,
        []() { return Stack<float>(); },
        [](Stack<float>& c, std::size_t i) { c.push(static_cast<float>(i)); },
        [](Stack<float>& c, std::size_t expected) { return static_cast<std::size_t>(c.count()) == expected; },
        [](Stack<float>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : 0.0f; });
        },
        [](Stack<float>& c, std::size_t expected) {
            const int before = c.count();
            try { c.push(1.5f); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.pop();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Stack<double>
    fillProbes.push_back(probeElementFill<Stack<double>>(
        "Stack", "double", limitBytes, maxElements,
        []() { return Stack<double>(); },
        [](Stack<double>& c, std::size_t i) { c.push(static_cast<double>(i)); },
        [](Stack<double>& c, std::size_t expected) { return static_cast<std::size_t>(c.count()) == expected; },
        [](Stack<double>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : 0.0; });
        },
        [](Stack<double>& c, std::size_t expected) {
            const int before = c.count();
            try { c.push(2.5); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.pop();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Stack<String>
    fillProbes.push_back(probeElementFill<Stack<String>>(
        "Stack", "String", limitBytes, maxElements,
        []() { return Stack<String>(); },
        [](Stack<String>& c, std::size_t i) { c.push(String(static_cast<int>(i))); },
        [](Stack<String>& c, std::size_t expected) {
            if (static_cast<std::size_t>(c.count()) != expected) return false;
            if (expected == 0) return true;
            return c.peek() == String(static_cast<int>(expected - 1));
        },
        [](Stack<String>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : String(""); });
        },
        [](Stack<String>& c, std::size_t expected) {
            const int before = c.count();
            try { c.push(String("recovery")); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.pop();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Queue<int>
    fillProbes.push_back(probeElementFill<Queue<int>>(
        "Queue", "int", limitBytes, maxElements,
        []() { return Queue<int>(); },
        [](Queue<int>& c, std::size_t i) { c.enqueue(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); },
        [](Queue<int>& c, std::size_t expected) {
            return static_cast<std::size_t>(c.count()) == expected;
        },
        [](Queue<int>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : 0; });
        },
        [](Queue<int>& c, std::size_t expected) {
            const int before = c.count();
            try { c.enqueue(0x5a5a5a5a); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.dequeue();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Queue<float>
    fillProbes.push_back(probeElementFill<Queue<float>>(
        "Queue", "float", limitBytes, maxElements,
        []() { return Queue<float>(); },
        [](Queue<float>& c, std::size_t i) { c.enqueue(static_cast<float>(i)); },
        [](Queue<float>& c, std::size_t expected) { return static_cast<std::size_t>(c.count()) == expected; },
        [](Queue<float>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : 0.0f; });
        },
        [](Queue<float>& c, std::size_t expected) {
            const int before = c.count();
            try { c.enqueue(1.5f); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.dequeue();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Queue<double>
    fillProbes.push_back(probeElementFill<Queue<double>>(
        "Queue", "double", limitBytes, maxElements,
        []() { return Queue<double>(); },
        [](Queue<double>& c, std::size_t i) { c.enqueue(static_cast<double>(i)); },
        [](Queue<double>& c, std::size_t expected) { return static_cast<std::size_t>(c.count()) == expected; },
        [](Queue<double>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : 0.0; });
        },
        [](Queue<double>& c, std::size_t expected) {
            const int before = c.count();
            try { c.enqueue(2.5); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.dequeue();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // Queue<String>
    fillProbes.push_back(probeElementFill<Queue<String>>(
        "Queue", "String", limitBytes, maxElements,
        []() { return Queue<String>(); },
        [](Queue<String>& c, std::size_t i) { c.enqueue(String(static_cast<int>(i))); },
        [](Queue<String>& c, std::size_t expected) { return static_cast<std::size_t>(c.count()) == expected; },
        [](Queue<String>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) { return expected > 0 ? c.peek() : String(""); });
        },
        [](Queue<String>& c, std::size_t expected) {
            const int before = c.count();
            try { c.enqueue(String("recovery")); } catch (const std::bad_alloc&) { return c.count() == before; }
            c.dequeue();
            return c.count() == before && before == static_cast<int>(expected);
        }
    ));

    // LinkedList<int>
    fillProbes.push_back(probeElementFill<LinkedList<int>>(
        "LinkedList", "int", limitBytes, maxElements,
        []() { return LinkedList<int>(); },
        [](LinkedList<int>& c, std::size_t i) { c.append(static_cast<int>(i & MAX_POSITIVE_INT_MASK)); },
        [](LinkedList<int>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            // Use head (index 0) for O(1) access — get(N-1) on a singly-linked
            // list without a tail pointer is O(N) and causes O(N²) probe time.
            int* head = c.get(0);
            return head != nullptr;
        },
        [](LinkedList<int>& c, std::size_t expected) {
            // Sample only the head element (O(1)) — random index access on a
            // singly-linked list is O(N) per call, making full sampleChecksum O(N²).
            return sampleChecksum(1, [&](std::size_t) {
                int* v = c.get(0);
                return v ? *v : 0;
            });
        },
        [](LinkedList<int>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try { c.append(0x5a5a5a5a); } catch (const std::bad_alloc&) { return c.size() == before; }
            c.remove(static_cast<int>(c.size() - 1));
            return c.size() == before && before == expected;
        }
    ));

    // LinkedList<float>
    fillProbes.push_back(probeElementFill<LinkedList<float>>(
        "LinkedList", "float", limitBytes, maxElements,
        []() { return LinkedList<float>(); },
        [](LinkedList<float>& c, std::size_t i) { c.append(static_cast<float>(i)); },
        [](LinkedList<float>& c, std::size_t expected) { return c.size() == expected; },
        [](LinkedList<float>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) {
                float* v = c.get(0);
                return v ? *v : 0.0f;
            });
        },
        [](LinkedList<float>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try { c.append(1.5f); } catch (const std::bad_alloc&) { return c.size() == before; }
            c.remove(static_cast<int>(c.size() - 1));
            return c.size() == before && before == expected;
        }
    ));

    // LinkedList<double>
    fillProbes.push_back(probeElementFill<LinkedList<double>>(
        "LinkedList", "double", limitBytes, maxElements,
        []() { return LinkedList<double>(); },
        [](LinkedList<double>& c, std::size_t i) { c.append(static_cast<double>(i)); },
        [](LinkedList<double>& c, std::size_t expected) { return c.size() == expected; },
        [](LinkedList<double>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) {
                double* v = c.get(0);
                return v ? *v : 0.0;
            });
        },
        [](LinkedList<double>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try { c.append(2.5); } catch (const std::bad_alloc&) { return c.size() == before; }
            c.remove(static_cast<int>(c.size() - 1));
            return c.size() == before && before == expected;
        }
    ));

    // LinkedList<String>
    fillProbes.push_back(probeElementFill<LinkedList<String>>(
        "LinkedList", "String", limitBytes, maxElements,
        []() { return LinkedList<String>(); },
        [](LinkedList<String>& c, std::size_t i) { c.append(String(static_cast<int>(i))); },
        [](LinkedList<String>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            String* last = c.get(expected - 1);
            return last && *last == String(static_cast<int>(expected - 1));
        },
        [](LinkedList<String>& c, std::size_t expected) {
            return sampleChecksum(1, [&](std::size_t) {
                String* v = c.get(0);
                return v ? *v : String("");
            });
        },
        [](LinkedList<String>& c, std::size_t expected) {
            const std::size_t before = c.size();
            try { c.append(String("recovery")); } catch (const std::bad_alloc&) { return c.size() == before; }
            c.remove(static_cast<int>(c.size() - 1));
            return c.size() == before && before == expected;
        }
    ));

    // OrderedMap<int,int>
    fillProbes.push_back(probeElementFill<OrderedMap<int, int>>(
        "OrderedMap", "int_int", limitBytes, maxElements,
        []() { return OrderedMap<int, int>(); },
        [](OrderedMap<int, int>& c, std::size_t i) {
            const int k = static_cast<int>(i & MAX_POSITIVE_INT_MASK);
            c.put(k, k);
        },
        [](OrderedMap<int, int>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            const int last = static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
            return c.get(last) == last;
        },
        [](OrderedMap<int, int>& c, std::size_t expected) {
            // OrderedMap::get() is O(N) linear scan; calling it 64 times per
            // checksum makes the probe O(N²).  Sample only one entry instead.
            return sampleChecksum(1, [&](std::size_t) {
                if (expected == 0) return 0;
                const int k = static_cast<int>((expected - 1) & MAX_POSITIVE_INT_MASK);
                return c.get(k);
            });
        },
        [](OrderedMap<int, int>& c, std::size_t expected) {
            const std::size_t before = c.size();
            const int recoveryKey = -1000000 - static_cast<int>(before % 1000);
            try { c.put(recoveryKey, 42); } catch (const std::bad_alloc&) { return c.size() == before; }
            return c.get(recoveryKey) == 42;
        }
    ));

    // OrderedMap<String,String>
    fillProbes.push_back(probeElementFill<OrderedMap<String, String>>(
        "OrderedMap", "String_String", limitBytes, maxElements,
        []() { return OrderedMap<String, String>(); },
        [](OrderedMap<String, String>& c, std::size_t i) {
            String k = String(static_cast<int>(i));
            c.put(k, k);
        },
        [](OrderedMap<String, String>& c, std::size_t expected) {
            if (c.size() != expected) return false;
            if (expected == 0) return true;
            String last = String(static_cast<int>(expected - 1));
            return c.get(last) == last;
        },
        [](OrderedMap<String, String>& c, std::size_t expected) {
            // OrderedMap::get() is O(N) linear scan; calling it 64 times per
            // checksum makes the probe O(N²).  Sample only one entry instead.
            return sampleChecksum(1, [&](std::size_t) {
                if (expected == 0) return String("");
                return c.get(String(static_cast<int>(expected - 1)));
            });
        },
        [](OrderedMap<String, String>& c, std::size_t expected) {
            const std::size_t before = c.size();
            String recoveryKey("__recovery__");
            try { c.put(recoveryKey, String("ok")); } catch (const std::bad_alloc&) { return c.size() == before; }
            return c.get(recoveryKey) == String("ok");
        }
    ));

    filterStressProbesForCapabilities(backend, optional, instanceProbes, fillProbes);

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
    const std::uint64_t runEndedMs = monotonicMs();
    const std::uint64_t totalDurationMs = (runEndedMs > runStartedMs) ? (runEndedMs - runStartedMs) : 0;
    writeStressReport(reportPath, board, sramBytes, limitBytes, backend, optional,
                      totalDurationMs,
                      appendReport,
                      instanceProbes, fillProbes);
    {
        std::lock_guard<std::mutex> lock(gHeartbeatMutex);
        gHeartbeatShutdown = true;
        gHeartbeat.active = false;
    }
    #if defined(_WIN32)
    if (heartbeatThread != nullptr) {
        WaitForSingleObject(heartbeatThread, INFINITE);
        CloseHandle(heartbeatThread);
    }
    #else
    heartbeatThread.join();
    #endif
    std::cout << "Stress report written successfully." << std::endl;

    return 0;
}
