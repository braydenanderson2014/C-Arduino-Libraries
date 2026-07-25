#include <Arduino.h>
#include <unity.h>
#include "../../lib/Variant/src/Variant.h"

void test_add_value_after_set_single_discards_previous_list_contents() {
    ArrayList<int> original;
    original.add(1);
    original.add(2);

    Variant<int> variant(original);
    variant.setSingle(42);
    variant.addValue(100);

    TEST_ASSERT_EQUAL_INT(2, variant.size());
    TEST_ASSERT_EQUAL_INT(42, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(100, variant.getList().get(1));
}

void test_set_single_clears_backing_list_state() {
    ArrayList<int> original;
    original.add(10);
    original.add(20);

    Variant<int> variant(original);
    variant.setSingle(7);

    const Variant<int>& readonly = variant;
    TEST_ASSERT_TRUE(readonly.isSingle());
    TEST_ASSERT_EQUAL_INT(0, readonly.getList().size());
    TEST_ASSERT_EQUAL_INT(7, readonly.getSingle());
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    UNITY_BEGIN();
    RUN_TEST(test_add_value_after_set_single_discards_previous_list_contents);
    RUN_TEST(test_set_single_clears_backing_list_state);
    UNITY_END();
}

void loop() {}
