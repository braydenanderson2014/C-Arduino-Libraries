#include <Arduino.h>
#include <unity.h>
#include <EthernetConnectionChecker.h>

EthernetConnectionChecker checker(LED_BUILTIN, 10, 4, 80);

void test_diagnostics_json_has_required_keys() {
    String json = checker.diagnosticsJson();
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"ip\"") >= 0, "Missing ip key");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"gateway\"") >= 0, "Missing gateway key");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"dns\"") >= 0, "Missing dns key");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"sdReady\"") >= 0, "Missing sdReady key");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"link\"") >= 0, "Missing link key");
}

void test_results_json_schema_exists() {
    String json = checker.resultsJson();
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("\"results\"") >= 0, "Missing top-level results key");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("[") >= 0, "Missing array start");
    TEST_ASSERT_TRUE_MESSAGE(json.indexOf("]") >= 0, "Missing array end");
}

void test_run_checks_without_begin_sets_error() {
    bool ok = checker.runChecks();
    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_TRUE_MESSAGE(checker.getLastError().hasValue(), "Expected error after runChecks before begin");
}

void test_api_text_is_not_empty() {
    String diag = checker.diagnosticsJson();
    String results = checker.resultsJson();

    TEST_ASSERT_TRUE_MESSAGE(diag.length() > 16, "Diagnostics JSON unexpectedly empty");
    TEST_ASSERT_TRUE_MESSAGE(results.length() > 8, "Results JSON unexpectedly empty");
}

void setup() {
    Serial.begin(115200);
    unsigned long serialWaitStart = millis();
    while (!Serial && (millis() - serialWaitStart) < 4000UL) {
        delay(10);
    }
    for (int i = 0; i < 6; ++i) {
        Serial.print("[ECC TEST] Beacon ");
        Serial.println(i + 1);
        delay(200);
    }

    Serial.println("[ECC TEST] Unity start");
    Serial.flush();

    UNITY_BEGIN();

    Serial.println("[ECC TEST] Running test_diagnostics_json_has_required_keys");
    Serial.flush();
    RUN_TEST(test_diagnostics_json_has_required_keys);

    Serial.println("[ECC TEST] Running test_results_json_schema_exists");
    Serial.flush();
    RUN_TEST(test_results_json_schema_exists);

    Serial.println("[ECC TEST] Running test_run_checks_without_begin_sets_error");
    Serial.flush();
    RUN_TEST(test_run_checks_without_begin_sets_error);

    Serial.println("[ECC TEST] Running test_api_text_is_not_empty");
    Serial.flush();
    RUN_TEST(test_api_text_is_not_empty);

    UNITY_END();

    Serial.println("[ECC TEST] Completed.");
    Serial.flush();
}

void loop() {}
