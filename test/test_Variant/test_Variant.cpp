#include <Arduino.h>
#include <unity.h>
#include "../../lib/Variant/src/Variant.h"

void test_default_constructor_starts_as_single_default_value() {
    Variant<int> variant;

    TEST_ASSERT_TRUE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(1, variant.size());
    TEST_ASSERT_EQUAL_INT(0, variant.getSingle());
}

void test_single_value_constructor_sets_single_mode_and_value() {
    Variant<int> variant(55);

    TEST_ASSERT_TRUE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(1, variant.size());
    TEST_ASSERT_EQUAL_INT(55, variant.getSingle());
}

void test_list_constructor_sets_list_mode_and_preserves_elements() {
    ArrayList<int> values;
    values.add(3);
    values.add(6);
    values.add(9);

    Variant<int> variant(values);

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_TRUE(variant.isArrayList());
    TEST_ASSERT_EQUAL_INT(3, variant.size());
    TEST_ASSERT_EQUAL_INT(3, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(6, variant.getList().get(1));
    TEST_ASSERT_EQUAL_INT(9, variant.getList().get(2));
}

void test_get_single_in_list_mode_returns_first_element() {
    ArrayList<int> values;
    values.add(101);
    values.add(202);

    Variant<int> variant(values);

    TEST_ASSERT_EQUAL_INT(101, variant.getSingle());
}

void test_get_single_in_empty_list_mode_returns_default_value() {
    ArrayList<int> values;
    Variant<int> variant(values);

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(0, variant.size());
    TEST_ASSERT_EQUAL_INT(0, variant.getSingle());
}

void test_get_list_converts_single_to_list_preserving_value() {
    Variant<int> variant(77);

    ArrayList<int>& asList = variant.getList();

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_TRUE(variant.isArrayList());
    TEST_ASSERT_EQUAL_INT(1, variant.size());
    TEST_ASSERT_EQUAL_INT(77, asList.get(0));
}

void test_set_list_overrides_single_mode_and_values() {
    Variant<int> variant(5);

    ArrayList<int> values;
    values.add(11);
    values.add(22);
    values.add(33);

    variant.setList(values);

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(3, variant.size());
    TEST_ASSERT_EQUAL_INT(11, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(22, variant.getList().get(1));
    TEST_ASSERT_EQUAL_INT(33, variant.getList().get(2));
}

void test_add_value_in_single_mode_converts_and_appends() {
    Variant<int> variant(8);

    variant.addValue(9);

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(2, variant.size());
    TEST_ASSERT_EQUAL_INT(8, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(9, variant.getList().get(1));
}

void test_add_value_in_list_mode_appends_without_losing_existing_data() {
    ArrayList<int> values;
    values.add(1);
    values.add(2);

    Variant<int> variant(values);
    variant.addValue(3);

    TEST_ASSERT_EQUAL_INT(3, variant.size());
    TEST_ASSERT_EQUAL_INT(1, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(2, variant.getList().get(1));
    TEST_ASSERT_EQUAL_INT(3, variant.getList().get(2));
}

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

void test_clear_resets_to_default_single_mode() {
    ArrayList<int> values;
    values.add(13);
    values.add(26);

    Variant<int> variant(values);
    variant.clear();

    TEST_ASSERT_TRUE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(1, variant.size());
    TEST_ASSERT_EQUAL_INT(0, variant.getSingle());
}

void test_set_single_after_get_list_reverts_mode_and_value() {
    Variant<int> variant(40);

    variant.getList().add(41);
    variant.setSingle(99);

    TEST_ASSERT_TRUE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(1, variant.size());
    TEST_ASSERT_EQUAL_INT(99, variant.getSingle());

    const Variant<int>& readonly = variant;
    TEST_ASSERT_EQUAL_INT(0, readonly.getList().size());
}

void test_const_get_list_reflects_constructor_list_contents() {
    ArrayList<int> values;
    values.add(4);
    values.add(5);

    const Variant<int> variant(values);
    const ArrayList<int>& listRef = variant.getList();

    TEST_ASSERT_EQUAL_INT(2, listRef.size());
    TEST_ASSERT_EQUAL_INT(4, listRef.get(0));
    TEST_ASSERT_EQUAL_INT(5, listRef.get(1));
}

void test_multiple_mode_transitions_preserve_expected_state() {
    Variant<int> variant(1);

    variant.addValue(2);  // list: [1, 2]
    variant.addValue(3);  // list: [1, 2, 3]
    TEST_ASSERT_EQUAL_INT(3, variant.size());

    variant.setSingle(10);
    TEST_ASSERT_TRUE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(10, variant.getSingle());

    ArrayList<int> next;
    next.add(20);
    next.add(30);
    variant.setList(next);

    TEST_ASSERT_FALSE(variant.isSingle());
    TEST_ASSERT_EQUAL_INT(2, variant.size());
    TEST_ASSERT_EQUAL_INT(20, variant.getList().get(0));
    TEST_ASSERT_EQUAL_INT(30, variant.getList().get(1));
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    UNITY_BEGIN();
    RUN_TEST(test_default_constructor_starts_as_single_default_value);
    RUN_TEST(test_single_value_constructor_sets_single_mode_and_value);
    RUN_TEST(test_list_constructor_sets_list_mode_and_preserves_elements);
    RUN_TEST(test_get_single_in_list_mode_returns_first_element);
    RUN_TEST(test_get_single_in_empty_list_mode_returns_default_value);
    RUN_TEST(test_get_list_converts_single_to_list_preserving_value);
    RUN_TEST(test_set_list_overrides_single_mode_and_values);
    RUN_TEST(test_add_value_in_single_mode_converts_and_appends);
    RUN_TEST(test_add_value_in_list_mode_appends_without_losing_existing_data);
    RUN_TEST(test_add_value_after_set_single_discards_previous_list_contents);
    RUN_TEST(test_set_single_clears_backing_list_state);
    RUN_TEST(test_clear_resets_to_default_single_mode);
    RUN_TEST(test_set_single_after_get_list_reverts_mode_and_value);
    RUN_TEST(test_const_get_list_reflects_constructor_list_contents);
    RUN_TEST(test_multiple_mode_transitions_preserve_expected_state);
    UNITY_END();
}

void loop() {}
