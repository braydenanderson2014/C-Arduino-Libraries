#include <Arduino.h>
#include <unity.h>
#include "../../lib/OrderedMap/src/OrderedMap.h"

// ---------------------------------------------------------------------------
// Basic construction
// ---------------------------------------------------------------------------

void test_new_map_is_empty() {
    OrderedMap<int, int> m;
    TEST_ASSERT_EQUAL_INT(0, (int)m.size());
}

// ---------------------------------------------------------------------------
// put / get
// ---------------------------------------------------------------------------

void test_put_and_get_basic() {
    OrderedMap<int, int> m;
    m.put(1, 100);
    TEST_ASSERT_EQUAL_INT(100, m.get(1));
}

void test_put_multiple_entries() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.put(2, 20);
    m.put(3, 30);
    TEST_ASSERT_EQUAL_INT(10, m.get(1));
    TEST_ASSERT_EQUAL_INT(20, m.get(2));
    TEST_ASSERT_EQUAL_INT(30, m.get(3));
}

void test_put_overwrites_existing_key() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.put(1, 99);
    TEST_ASSERT_EQUAL_INT(99, m.get(1));
}

void test_put_overwrite_does_not_increase_size() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.put(1, 20);
    TEST_ASSERT_EQUAL_INT(1, (int)m.size());
}

void test_get_missing_key_returns_default() {
    OrderedMap<int, int> m;
    TEST_ASSERT_EQUAL_INT(0, m.get(999));
}

// ---------------------------------------------------------------------------
// size
// ---------------------------------------------------------------------------

void test_size_tracks_unique_insertions() {
    OrderedMap<int, int> m;
    TEST_ASSERT_EQUAL_INT(0, (int)m.size());
    m.put(1, 10);
    TEST_ASSERT_EQUAL_INT(1, (int)m.size());
    m.put(2, 20);
    TEST_ASSERT_EQUAL_INT(2, (int)m.size());
    m.put(3, 30);
    TEST_ASSERT_EQUAL_INT(3, (int)m.size());
}

// ---------------------------------------------------------------------------
// Insertion order preservation
// ---------------------------------------------------------------------------

void test_first_entry_inserted_is_first_accessible() {
    OrderedMap<int, int> m;
    m.put(10, 100);
    m.put(20, 200);
    m.put(30, 300);
    // OrderedMap stores in insertion order — get() should find them correctly
    TEST_ASSERT_EQUAL_INT(100, m.get(10));
    TEST_ASSERT_EQUAL_INT(200, m.get(20));
    TEST_ASSERT_EQUAL_INT(300, m.get(30));
}

void test_update_does_not_change_insertion_order_size() {
    OrderedMap<int, int> m;
    m.put(1, 1);
    m.put(2, 2);
    m.put(3, 3);
    m.put(2, 99); // update middle entry
    TEST_ASSERT_EQUAL_INT(3, (int)m.size());
    TEST_ASSERT_EQUAL_INT(99, m.get(2));
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

void test_clear_empties_map() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.put(2, 20);
    m.clear();
    TEST_ASSERT_EQUAL_INT(0, (int)m.size());
}

void test_clear_then_put_works() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.clear();
    m.put(2, 20);
    TEST_ASSERT_EQUAL_INT(1, (int)m.size());
    TEST_ASSERT_EQUAL_INT(20, m.get(2));
    TEST_ASSERT_EQUAL_INT(0, m.get(1)); // cleared key returns default
}

void test_clear_empty_map_is_harmless() {
    OrderedMap<int, int> m;
    m.clear();
    TEST_ASSERT_EQUAL_INT(0, (int)m.size());
    m.put(5, 50);
    TEST_ASSERT_EQUAL_INT(50, m.get(5));
}

// ---------------------------------------------------------------------------
// String keys
// ---------------------------------------------------------------------------

void test_put_and_get_string_keys() {
    OrderedMap<String, int> m;
    m.put(String("foo"), 1);
    m.put(String("bar"), 2);
    TEST_ASSERT_EQUAL_INT(1, m.get(String("foo")));
    TEST_ASSERT_EQUAL_INT(2, m.get(String("bar")));
}

void test_string_key_overwrite() {
    OrderedMap<String, int> m;
    m.put(String("key"), 10);
    m.put(String("key"), 99);
    TEST_ASSERT_EQUAL_INT(99, m.get(String("key")));
    TEST_ASSERT_EQUAL_INT(1, (int)m.size());
}

// ---------------------------------------------------------------------------
// String values
// ---------------------------------------------------------------------------

void test_put_and_get_string_values() {
    OrderedMap<int, String> m;
    m.put(1, String("hello"));
    m.put(2, String("world"));
    TEST_ASSERT_EQUAL_STRING("hello", m.get(1).c_str());
    TEST_ASSERT_EQUAL_STRING("world", m.get(2).c_str());
}

// ---------------------------------------------------------------------------
// Mixed types
// ---------------------------------------------------------------------------

void test_int_to_float_map() {
    OrderedMap<int, float> m;
    m.put(1, 1.5f);
    m.put(2, 2.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, m.get(1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.5f, m.get(2));
}

// ---------------------------------------------------------------------------
// Large insertion
// ---------------------------------------------------------------------------

void test_large_insertions_all_accessible() {
    OrderedMap<int, int> m;
    const int N = 30;
    for (int i = 0; i < N; i++) {
        m.put(i, i * 3);
    }
    TEST_ASSERT_EQUAL_INT(N, (int)m.size());
    for (int i = 0; i < N; i++) {
        TEST_ASSERT_EQUAL_INT(i * 3, m.get(i));
    }
}

// ---------------------------------------------------------------------------
// Repeated overwrites do not corrupt other entries
// ---------------------------------------------------------------------------

void test_multiple_overwrites_do_not_corrupt_siblings() {
    OrderedMap<int, int> m;
    m.put(1, 10);
    m.put(2, 20);
    m.put(3, 30);

    // Overwrite key 2 several times
    for (int i = 0; i < 5; i++) {
        m.put(2, i * 7);
    }

    TEST_ASSERT_EQUAL_INT(10, m.get(1)); // unchanged
    TEST_ASSERT_EQUAL_INT(28, m.get(2)); // 4*7
    TEST_ASSERT_EQUAL_INT(30, m.get(3)); // unchanged
    TEST_ASSERT_EQUAL_INT(3, (int)m.size());
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting OrderedMap Tests...");

    UNITY_BEGIN();

    RUN_TEST(test_new_map_is_empty);

    RUN_TEST(test_put_and_get_basic);
    RUN_TEST(test_put_multiple_entries);
    RUN_TEST(test_put_overwrites_existing_key);
    RUN_TEST(test_put_overwrite_does_not_increase_size);
    RUN_TEST(test_get_missing_key_returns_default);

    RUN_TEST(test_size_tracks_unique_insertions);

    RUN_TEST(test_first_entry_inserted_is_first_accessible);
    RUN_TEST(test_update_does_not_change_insertion_order_size);

    RUN_TEST(test_clear_empties_map);
    RUN_TEST(test_clear_then_put_works);
    RUN_TEST(test_clear_empty_map_is_harmless);

    RUN_TEST(test_put_and_get_string_keys);
    RUN_TEST(test_string_key_overwrite);

    RUN_TEST(test_put_and_get_string_values);

    RUN_TEST(test_int_to_float_map);

    RUN_TEST(test_large_insertions_all_accessible);
    RUN_TEST(test_multiple_overwrites_do_not_corrupt_siblings);

    UNITY_END();

    Serial.println("OrderedMap Tests Completed!");
}

void loop() {}
