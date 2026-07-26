#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Arduino.h"
#include "SD.h"
#if defined(USE_LITTLEFS)
#include "LittleFS.h"
#endif

#include "JSON.h"

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

void writeReport(const std::filesystem::path& reportPath,
                 bool success,
                 const std::string& mode,
                 const std::string& errorMessage) {
    std::ofstream report(reportPath, std::ios::binary);
    if (!report.good()) {
        return;
    }

    report << "{\n";
    report << "  \"success\": " << (success ? "true" : "false") << ",\n";
    report << "  \"mode\": \"" << mode << "\",\n";
    report << "  \"error\": \"" << errorMessage << "\"\n";
    report << "}\n";
}

void testRoundTripBasics() {
    JSON json;
    json.setString("profile.name", "Brayden");
    json.setNumber("profile.version", 2.5);
    json.setBool("profile.active", true);

    char* serialized = json.writeToString(false);
    expect(serialized != nullptr, "JSON serialization should allocate output");

    JSON parsed;
    expect(parsed.readFromString(serialized), "JSON should parse serialized payload");
    expect(parsed.getString("profile.name") == "Brayden", "JSON should preserve string fields");
    expect(parsed.getNumber("profile.version") == 2.5, "JSON should preserve numeric fields");
    expect(parsed.getBool("profile.active"), "JSON should preserve bool fields");

    std::free(serialized);
}

void testModeSpecificBehavior() {
    JSON json;
    json.setString("values.numericText", "42.5");
    json.setString("values.boolText", "true");
    json.setString("values.invalidBoolText", "not-a-bool");
    json.setBool("values.flag", true);

#if JSON_ENABLE_OPTIONAL_RETURNS
    const Optional<double> number = json.tryGetNumber("values.numericText");
    const Optional<bool> parsedTrue = json.tryGetBool("values.boolText");
    const Optional<bool> invalid = json.tryGetBool("values.invalidBoolText");
    const Optional<double> missing = json.tryGetNumber("values.missing");

    expect(number.hasValue() && number.getValue() == 42.5,
           "Optional mode should parse numeric strings");
    expect(parsedTrue.hasValue() && parsedTrue.getValue(),
           "Optional mode should parse true-like strings");
    expect(!invalid.hasValue(),
           "Optional mode should return empty for invalid bool strings");
    expect(!missing.hasValue(),
           "Optional mode should return empty for missing keys");
#else
    expect(json.getString("values.missing", "fallback") == "fallback",
           "Non-optional mode should return provided default for missing strings");
    expect(json.getNumber("values.missing", 99.0) == 99.0,
           "Non-optional mode should return provided default for missing numbers");
    expect(json.getBool("values.missing", true),
           "Non-optional mode should return provided default for missing bools");
#endif
}

int main() {
    const std::filesystem::path fsRoot = envToStringOrDefault("HOST_SIM_FS_ROOT", "test/host_arduino_sim/out/json-mode-fs");
    const std::filesystem::path reportPath = envToStringOrDefault(
        "HOST_JSON_MODE_REPORT",
        "test/host_arduino_sim/out/json-mode-report.json"
    );

    std::filesystem::create_directories(fsRoot);
    std::filesystem::create_directories(reportPath.parent_path());

    SD.setRoot(fsRoot.string());
#if defined(USE_LITTLEFS)
    LittleFS.setRoot(fsRoot.string());
#endif

    bool success = false;
    std::string error;

    try {
        testRoundTripBasics();
        testModeSpecificBehavior();
        success = true;
        std::cout << "JSON mode tests passed." << std::endl;
    } catch (const std::exception& ex) {
        error = ex.what();
        std::cerr << "JSON mode tests failed: " << error << std::endl;
    } catch (...) {
        error = "Unknown failure";
        std::cerr << "JSON mode tests failed: unknown failure" << std::endl;
    }

#if JSON_ENABLE_OPTIONAL_RETURNS
    const std::string mode = "optional-on";
#else
    const std::string mode = "optional-off";
#endif

    writeReport(reportPath, success, mode, error);
    return success ? 0 : 1;
}
