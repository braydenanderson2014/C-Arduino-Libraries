#include <Arduino.h>
#include <unity.h>
#include "../../lib/Hashtable/src/Hashtable.h"

// ---------------------------------------------------------------------------
// Helper type for iterator / value-type tests
// ---------------------------------------------------------------------------

struct IteratorValueType {
    int id;

    IteratorValueType(int value = 0) : id(value) {}

    bool operator==(const IteratorValueType& other) const {
        return id == other.id;
    }
};

// ---------------------------------------------------------------------------
// isEmpty / elements
// ---------------------------------------------------------------------------

void test_new_table_is_empty() {
    Hashtable<int, int> table;
    TEST_ASSERT_TRUE(table.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, table.elements());
}

void test_is_empty_tracks_element_count() {
    Hashtable<int, int> table;

    TEST_ASSERT_TRUE(table.isEmpty());

    table.put(11, 1);
    TEST_ASSERT_FALSE(table.isEmpty());

    table.remove(11);
    TEST_ASSERT_TRUE(table.isEmpty());
}

void test_elements_count_tracks_insertions() {
    Hashtable<int, int> table;
    table.put(1, 10);
    TEST_ASSERT_EQUAL_INT(1, table.elements());
    table.put(2, 20);
    TEST_ASSERT_EQUAL_INT(2, table.elements());
    table.put(3, 30);
    TEST_ASSERT_EQUAL_INT(3, table.elements());
}

// ---------------------------------------------------------------------------
// put / getElement
// ---------------------------------------------------------------------------

void test_put_and_get_element_basic() {
    Hashtable<int, int> table;
    table.put(42, 100);
    TEST_ASSERT_EQUAL_INT(100, table.getElement(42));
}

void test_put_overwrites_existing_key() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(1, 99);
    TEST_ASSERT_EQUAL_INT(99, table.getElement(1));
    TEST_ASSERT_EQUAL_INT(1, table.elements()); // still just one entry
}

void test_get_element_missing_key_returns_default() {
    Hashtable<int, int> table;
    TEST_ASSERT_EQUAL_INT(0, table.getElement(999));
}

void test_put_string_keys() {
    Hashtable<String, int> table;
    table.put(String("alpha"), 1);
    table.put(String("beta"), 2);
    TEST_ASSERT_EQUAL_INT(1, table.getElement(String("alpha")));
    TEST_ASSERT_EQUAL_INT(2, table.getElement(String("beta")));
}

void test_put_string_values() {
    Hashtable<int, String> table;
    table.put(1, String("hello"));
    table.put(2, String("world"));
    TEST_ASSERT_EQUAL_STRING("hello", table.getElement(1).c_str());
    TEST_ASSERT_EQUAL_STRING("world", table.getElement(2).c_str());
}

// ---------------------------------------------------------------------------
// getElement with out-parameter
// ---------------------------------------------------------------------------

void test_get_element_out_param_returns_true_and_value() {
    Hashtable<int, int> table;
    table.put(7, 77);
    int out = 0;
    TEST_ASSERT_TRUE(table.getElement(7, &out));
    TEST_ASSERT_EQUAL_INT(77, out);
}

void test_get_element_out_param_missing_key_returns_false() {
    Hashtable<int, int> table;
    int out = 0;
    TEST_ASSERT_FALSE(table.getElement(999, &out));
}

void test_get_element_with_null_output_pointer_returns_false() {
    Hashtable<int, int> table;
    table.put(1, 10);
    TEST_ASSERT_FALSE(table.getElement(1, nullptr));
}

// ---------------------------------------------------------------------------
// get (pointer overload)
// ---------------------------------------------------------------------------

void test_get_pointer_returns_non_null_for_existing_key() {
    Hashtable<int, int> table;
    table.put(5, 55);
    int* ptr = table.get(5);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL_INT(55, *ptr);
}

void test_get_pointer_returns_null_for_missing_key() {
    Hashtable<int, int> table;
    TEST_ASSERT_NULL(table.get(999));
}

// ---------------------------------------------------------------------------
// exists
// ---------------------------------------------------------------------------

void test_exists_returns_true_for_inserted_key() {
    Hashtable<int, int> table;
    table.put(3, 30);
    TEST_ASSERT_TRUE(table.exists(3));
}

void test_exists_returns_false_for_missing_key() {
    Hashtable<int, int> table;
    TEST_ASSERT_FALSE(table.exists(999));
}

void test_exists_with_value_out_param() {
    Hashtable<int, int> table;
    table.put(4, 44);
    int val = 0;
    TEST_ASSERT_TRUE(table.exists(4, val));
    TEST_ASSERT_EQUAL_INT(44, val);
}

// ---------------------------------------------------------------------------
// containsKey / containsValue
// ---------------------------------------------------------------------------

void test_contains_key_true() {
    Hashtable<int, int> table;
    table.put(10, 100);
    TEST_ASSERT_TRUE(table.containsKey(10));
}

void test_contains_key_false() {
    Hashtable<int, int> table;
    TEST_ASSERT_FALSE(table.containsKey(42));
}

void test_contains_value_true() {
    Hashtable<int, int> table;
    table.put(1, 777);
    TEST_ASSERT_TRUE(table.containsValue(777));
}

void test_contains_value_false() {
    Hashtable<int, int> table;
    table.put(1, 5);
    TEST_ASSERT_FALSE(table.containsValue(999));
}

// ---------------------------------------------------------------------------
// remove
// ---------------------------------------------------------------------------

void test_remove_existing_key_returns_true() {
    Hashtable<int, int> table;
    table.put(1, 10);
    TEST_ASSERT_TRUE(table.remove(1));
}

void test_remove_decrements_count() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.remove(1);
    TEST_ASSERT_EQUAL_INT(1, table.elements());
}

void test_remove_makes_key_inaccessible() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.remove(1);
    TEST_ASSERT_FALSE(table.exists(1));
    TEST_ASSERT_EQUAL_INT(0, table.getElement(1));
}

void test_remove_nonexistent_key_returns_false() {
    Hashtable<int, int> table;
    TEST_ASSERT_FALSE(table.remove(999));
}

void test_remove_all_entries_leaves_empty_table() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.put(3, 30);
    table.remove(1);
    table.remove(2);
    table.remove(3);
    TEST_ASSERT_TRUE(table.isEmpty());
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

void test_clear_empties_table() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.clear();
    TEST_ASSERT_TRUE(table.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, table.elements());
}

void test_clear_then_put_works() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.clear();
    table.put(2, 20);
    TEST_ASSERT_EQUAL_INT(1, table.elements());
    TEST_ASSERT_EQUAL_INT(20, table.getElement(2));
}

// ---------------------------------------------------------------------------
// operator[]
// ---------------------------------------------------------------------------

void test_operator_brackets_returns_inserted_value_after_resize() {
    Hashtable<int, int> table(1, 0.5f);

    table[1] = 100;
    table[2] = 200;

    TEST_ASSERT_EQUAL_INT(100, table.getElement(1));
    TEST_ASSERT_EQUAL_INT(200, table.getElement(2));
    TEST_ASSERT_EQUAL_INT(2, table.elements());
}

void test_operator_brackets_creates_entry_for_missing_key() {
    Hashtable<int, int> table;
    table[55] = 555;
    TEST_ASSERT_TRUE(table.exists(55));
    TEST_ASSERT_EQUAL_INT(555, table.getElement(55));
}

void test_operator_brackets_can_update_existing_value() {
    Hashtable<int, int> table;
    table.put(3, 30);
    table[3] = 99;
    TEST_ASSERT_EQUAL_INT(99, table.getElement(3));
    TEST_ASSERT_EQUAL_INT(1, table.elements());
}

// ---------------------------------------------------------------------------
// Resize / load factor
// ---------------------------------------------------------------------------

void test_zero_capacity_constructor_uses_safe_default_size() {
    Hashtable<int, int> table(0, 0.75f);

    table.put(5, 55);

    TEST_ASSERT_TRUE(table.exists(5));
    TEST_ASSERT_EQUAL_INT(55, table.getElement(5));
}

void test_all_entries_accessible_after_resize() {
    Hashtable<int, int> table(1, 0.5f); // tiny initial size — will resize quickly

    const int N = 30;
    for (int i = 0; i < N; i++) {
        table.put(i, i * 10);
    }
    TEST_ASSERT_EQUAL_INT(N, table.elements());
    for (int i = 0; i < N; i++) {
        TEST_ASSERT_EQUAL_INT(i * 10, table.getElement(i));
    }
}

// ---------------------------------------------------------------------------
// keys() / values()
// ---------------------------------------------------------------------------

void test_keys_returns_all_keys() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.put(3, 30);

    SimpleVector<int> k = table.keys();
    TEST_ASSERT_EQUAL_INT(3, k.elements());

    bool found1 = false, found2 = false, found3 = false;
    for (unsigned int i = 0; i < k.elements(); i++) {
        if (k[i] == 1) found1 = true;
        if (k[i] == 2) found2 = true;
        if (k[i] == 3) found3 = true;
    }
    TEST_ASSERT_TRUE(found1 && found2 && found3);
}

void test_values_returns_all_values() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.put(3, 30);

    SimpleVector<int> v = table.values();
    TEST_ASSERT_EQUAL_INT(3, v.elements());

    bool found10 = false, found20 = false, found30 = false;
    for (unsigned int i = 0; i < v.elements(); i++) {
        if (v[i] == 10) found10 = true;
        if (v[i] == 20) found20 = true;
        if (v[i] == 30) found30 = true;
    }
    TEST_ASSERT_TRUE(found10 && found20 && found30);
}

// ---------------------------------------------------------------------------
// Iterator
// ---------------------------------------------------------------------------

void test_iterator_operator_star_supports_non_string_value_types() {
    Hashtable<String, IteratorValueType> table;

    table.put(String("alpha"), IteratorValueType(7));

    auto it = table.begin();
    TEST_ASSERT_TRUE(it != table.end());

    auto kv = *it;

    TEST_ASSERT_EQUAL_STRING("alpha", kv.key.c_str());
    TEST_ASSERT_EQUAL_INT(7, kv.value.id);
}

void test_iterator_traverses_all_entries() {
    Hashtable<int, int> table;
    table.put(1, 10);
    table.put(2, 20);
    table.put(3, 30);

    int count = 0;
    int sum = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        auto kv = *it;
        sum += kv.value;
        count++;
    }
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(60, sum);
}

void test_iterator_on_empty_table_is_end() {
    Hashtable<int, int> table;
    TEST_ASSERT_FALSE(table.begin() != table.end());
}

void test_iterator_after_clear_is_end() {
    Hashtable<int, int> table;
    table.put(1, 1);
    table.clear();
    TEST_ASSERT_FALSE(table.begin() != table.end());
}

// ---------------------------------------------------------------------------
// Bucket helpers
// ---------------------------------------------------------------------------

void test_bucket_count_is_positive() {
    Hashtable<int, int> table;
    TEST_ASSERT_TRUE(table.bucketCount() > 0);
}

void test_load_factor_is_zero_on_empty_table() {
    Hashtable<int, int> table;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, table.loadFactor());
}

void test_load_factor_increases_with_inserts() {
    Hashtable<int, int> table;
    float lf0 = table.loadFactor();
    table.put(1, 10);
    float lf1 = table.loadFactor();
    TEST_ASSERT_TRUE(lf1 > lf0);
}

// ---------------------------------------------------------------------------
// Large-scale
// ---------------------------------------------------------------------------

void test_large_insertion_and_lookup() {
    Hashtable<int, int> table;
    const int N = 50;
    for (int i = 0; i < N; i++) {
        table.put(i, i * 2);
    }
    TEST_ASSERT_EQUAL_INT(N, table.elements());
    for (int i = 0; i < N; i++) {
        TEST_ASSERT_EQUAL_INT(i * 2, table.getElement(i));
    }
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting Hashtable Tests...");

    UNITY_BEGIN();

    RUN_TEST(test_new_table_is_empty);
    RUN_TEST(test_is_empty_tracks_element_count);
    RUN_TEST(test_elements_count_tracks_insertions);

    RUN_TEST(test_put_and_get_element_basic);
    RUN_TEST(test_put_overwrites_existing_key);
    RUN_TEST(test_get_element_missing_key_returns_default);
    RUN_TEST(test_put_string_keys);
    RUN_TEST(test_put_string_values);

    RUN_TEST(test_get_element_out_param_returns_true_and_value);
    RUN_TEST(test_get_element_out_param_missing_key_returns_false);
    RUN_TEST(test_get_element_with_null_output_pointer_returns_false);

    RUN_TEST(test_get_pointer_returns_non_null_for_existing_key);
    RUN_TEST(test_get_pointer_returns_null_for_missing_key);

    RUN_TEST(test_exists_returns_true_for_inserted_key);
    RUN_TEST(test_exists_returns_false_for_missing_key);
    RUN_TEST(test_exists_with_value_out_param);

    RUN_TEST(test_contains_key_true);
    RUN_TEST(test_contains_key_false);
    RUN_TEST(test_contains_value_true);
    RUN_TEST(test_contains_value_false);

    RUN_TEST(test_remove_existing_key_returns_true);
    RUN_TEST(test_remove_decrements_count);
    RUN_TEST(test_remove_makes_key_inaccessible);
    RUN_TEST(test_remove_nonexistent_key_returns_false);
    RUN_TEST(test_remove_all_entries_leaves_empty_table);

    RUN_TEST(test_clear_empties_table);
    RUN_TEST(test_clear_then_put_works);

    RUN_TEST(test_operator_brackets_returns_inserted_value_after_resize);
    RUN_TEST(test_operator_brackets_creates_entry_for_missing_key);
    RUN_TEST(test_operator_brackets_can_update_existing_value);

    RUN_TEST(test_zero_capacity_constructor_uses_safe_default_size);
    RUN_TEST(test_all_entries_accessible_after_resize);

    RUN_TEST(test_keys_returns_all_keys);
    RUN_TEST(test_values_returns_all_values);

    RUN_TEST(test_iterator_operator_star_supports_non_string_value_types);
    RUN_TEST(test_iterator_traverses_all_entries);
    RUN_TEST(test_iterator_on_empty_table_is_end);
    RUN_TEST(test_iterator_after_clear_is_end);

    RUN_TEST(test_bucket_count_is_positive);
    RUN_TEST(test_load_factor_is_zero_on_empty_table);
    RUN_TEST(test_load_factor_increases_with_inserts);

    RUN_TEST(test_large_insertion_and_lookup);

    UNITY_END();

    Serial.println("Hashtable Tests Completed!");
}

void loop() {}

