#include <Arduino.h>
#include <unity.h>
#include "Hashtable.h"

void test_is_empty_tracks_element_count() {
    Hashtable<int, int> table;

    TEST_ASSERT_TRUE(table.isEmpty());

    table.put(11, 1);
    TEST_ASSERT_FALSE(table.isEmpty());

    table.remove(11);
    TEST_ASSERT_TRUE(table.isEmpty());
}

void test_operator_brackets_returns_inserted_value_after_resize() {
    Hashtable<int, int> table(1, 0.5f);

    table[1] = 100;
    table[2] = 200;

    TEST_ASSERT_EQUAL_INT(100, table.getElement(1));
    TEST_ASSERT_EQUAL_INT(200, table.getElement(2));
    TEST_ASSERT_EQUAL_INT(2, table.elements());
}

void test_zero_capacity_constructor_uses_safe_default_size() {
    Hashtable<int, int> table(0, 0.75f);

    table.put(5, 55);

    TEST_ASSERT_TRUE(table.exists(5));
    TEST_ASSERT_EQUAL_INT(55, table.getElement(5));
}

void test_get_element_with_null_output_pointer_returns_false() {
    Hashtable<int, int> table;
    table.put(1, 10);

    TEST_ASSERT_FALSE(table.getElement(1, nullptr));
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting Hashtable Tests...");

    UNITY_BEGIN();
    RUN_TEST(test_is_empty_tracks_element_count);
    RUN_TEST(test_operator_brackets_returns_inserted_value_after_resize);
    RUN_TEST(test_zero_capacity_constructor_uses_safe_default_size);
    RUN_TEST(test_get_element_with_null_output_pointer_returns_false);
    UNITY_END();

    Serial.println("Hashtable Tests Completed!");
}

void loop() {}
