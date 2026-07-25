#include <Arduino.h>
#include <unity.h>
#include <ArrayList.h>

struct CopyTracked {
    int value;
    static volatile int assignmentCount;

    CopyTracked(int v = 0) : value(v) {}

    CopyTracked(const CopyTracked& other) : value(other.value) {
        ++assignmentCount;
    }

    CopyTracked& operator=(const CopyTracked& other) {
        if (this != &other) {
            value = other.value;
        }
        ++assignmentCount;
        return *this;
    }

    bool operator==(const CopyTracked& other) const {
        return value == other.value;
    }
};

volatile int CopyTracked::assignmentCount = 0;

static void reset_copy_counter() {
    CopyTracked::assignmentCount = 0;
}

static bool ascending_int_comparator(int a, int b) {
    return a > b;
}

static void assert_list_equals(ArrayList<int>& list, const int* expected, size_t expectedSize) {
    TEST_ASSERT_EQUAL_UINT((unsigned int)expectedSize, (unsigned int)list.size());
    for (size_t i = 0; i < expectedSize; ++i) {
        TEST_ASSERT_EQUAL_INT(expected[i], list.get(i));
    }
}

void test_add_does_not_resize_before_capacity_is_exceeded() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);

    for (int i = 0; i < 8; ++i) {
        list.add(i);
    }

    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.capacity());

    list.add(8);

    TEST_ASSERT_EQUAL_UINT(9, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() > 8);
}

void test_add_triggers_resize_only_when_necessary() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);

    for (int i = 0; i < 7; ++i) {
        list.add(i);
    }
    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.capacity());

    list.add(7);
    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.capacity());

    list.add(8);
    TEST_ASSERT_TRUE(list.capacity() > 8);
}

void test_add_all_fills_remaining_capacity_without_resizing() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    const int extraValues[] = {4, 5, 6, 7};

    for (int i = 0; i < 4; ++i) {
        list.add(i);
    }

    bool ok = list.addAll(extraValues, 4);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.capacity());
}

void test_add_all_array_resizes_when_insufficient_capacity() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    const int extraValues[] = {
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119
    };

    for (int i = 0; i < 7; ++i) {
        list.add(i);
    }

    bool ok = list.addAll(extraValues, sizeof(extraValues) / sizeof(extraValues[0]));

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(27, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() >= 27);

    const int expected[] = {
        0, 1, 2, 3, 4, 5, 6,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119
    };
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_add_all_list_resizes_and_preserves_order() {
    ArrayList<int> base(ArrayList<int>::DYNAMIC2, 8);
    ArrayList<int> other(ArrayList<int>::DYNAMIC2, 8);

    for (int i = 0; i < 5; ++i) {
        base.add(i);
    }
    for (int i = 50; i < 55; ++i) {
        other.add(i);
    }

    bool ok = base.addAll(other);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(10, (unsigned int)base.size());
    TEST_ASSERT_TRUE(base.capacity() > 8);

    const int expected[] = {0, 1, 2, 3, 4, 50, 51, 52, 53, 54};
    assert_list_equals(base, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_set_existing_item_does_not_resize() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);

    for (int i = 0; i < 8; ++i) {
        list.add(i);
    }

    size_t originalCapacity = list.capacity();
    bool ok = list.set(7, 42);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_INT(42, list.get(7));
    TEST_ASSERT_EQUAL_UINT((unsigned int)originalCapacity, (unsigned int)list.capacity());
}

void test_insert_on_full_list_resizes_and_shifts_items() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.add(1);
    list.add(2);
    list.add(4);
    list.add(5);

    bool ok = list.insert(2, 3);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(5, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() > 4);

    const int expected[] = {1, 2, 3, 4, 5};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_insert_all_list_exact_fit_does_not_resize() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    ArrayList<int> block(ArrayList<int>::DYNAMIC2, 4);

    list.add(1);
    list.add(2);
    list.add(7);
    list.add(8);

    block.add(3);
    block.add(4);
    block.add(5);
    block.add(6);

    size_t capBefore = list.capacity();
    bool ok = list.insertAll(2, block);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT((unsigned int)capBefore, (unsigned int)list.capacity());

    const int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_insert_all_list_resizes_multiple_times_when_needed() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    ArrayList<int> block(ArrayList<int>::DYNAMIC2, 8);

    for (int i = 0; i < 7; ++i) {
        list.add(i);
    }
    for (int i = 100; i < 120; ++i) {
        block.add(i);
    }

    bool ok = list.insertAll(3, block);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(27, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() >= 27);

    const int expected[] = {
        0, 1, 2,
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
        3, 4, 5, 6
    };
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_insert_on_full_fixed_list_returns_false_and_leaves_contents_intact() {
    ArrayList<int> list(ArrayList<int>::FIXED, 4);

    list.add(1);
    list.add(2);
    list.add(4);
    list.add(5);

    size_t capacityBefore = list.capacity();
    bool ok = list.insert(2, 3);

    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_UINT((unsigned int)capacityBefore, (unsigned int)list.capacity());

    const int expected[] = {1, 2, 4, 5};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_insert_all_raw_on_full_fixed_list_returns_false() {
    ArrayList<int> list(ArrayList<int>::FIXED, 4);
    const int extraValues[] = {3, 4};

    list.add(1);
    list.add(2);
    list.add(5);
    list.add(6);

    size_t capacityBefore = list.capacity();
    bool ok = list.insertAll(2, extraValues, 2);

    TEST_ASSERT_FALSE(ok);
    TEST_ASSERT_EQUAL_UINT((unsigned int)capacityBefore, (unsigned int)list.capacity());

    const int expected[] = {1, 2, 5, 6};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_insert_all_raw_resizes_multiple_times_when_needed() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    const int extraValues[] = {
        200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
        210, 211, 212, 213, 214, 215, 216, 217, 218, 219
    };

    for (int i = 0; i < 7; ++i) {
        list.add(i);
    }

    bool ok = list.insertAll(4, extraValues, sizeof(extraValues) / sizeof(extraValues[0]));

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT(27, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() >= 27);

    const int expected[] = {
        0, 1, 2, 3,
        200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
        210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
        4, 5, 6
    };
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_sort_with_merge_sort_respects_comparator() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    list.add(8);
    list.add(3);
    list.add(7);
    list.add(1);

    list.sort(ascending_int_comparator, ArrayList<int>::MERGE_SORT);

    const int expected[] = {1, 3, 7, 8};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_begin_and_end_are_safe_for_zero_capacity_lists() {
    ArrayList<int> list(ArrayList<int>::FIXED, 0);

    TEST_ASSERT_EQUAL_PTR(list.begin(), list.end());
}

void test_insert_all_raw_with_zero_length_is_a_no_op() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);
    const int extraValues[] = {3, 4};

    list.add(1);
    list.add(2);

    size_t sizeBefore = list.size();
    size_t capacityBefore = list.capacity();
    bool ok = list.insertAll(0, extraValues, 0);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_UINT((unsigned int)sizeBefore, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT((unsigned int)capacityBefore, (unsigned int)list.capacity());

    const int expected[] = {1, 2};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_get_reference_updates_item_in_place() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.add(10);
    int& value = list.getReference(0);
    value = 99;

    TEST_ASSERT_EQUAL_INT(99, list.get(0));
}

void test_operator_index_oob_returns_reset_default_reference_each_time() {
    ArrayList<int> listA(ArrayList<int>::DYNAMIC2, 4);
    ArrayList<int> listB(ArrayList<int>::DYNAMIC2, 4);

    int& oobA = listA[999];
    oobA = 1234;

    int& oobB = listB[999];
    TEST_ASSERT_EQUAL_INT(0, oobB);

    oobB = -44;
    int& oobASecond = listA[555];
    TEST_ASSERT_EQUAL_INT(0, oobASecond);
}

void test_get_reference_oob_resets_and_does_not_leak_between_instances() {
    ArrayList<int> listA(ArrayList<int>::DYNAMIC2, 4);
    ArrayList<int> listB(ArrayList<int>::DYNAMIC2, 4);

    int& aRef = listA.getReference(100);
    aRef = 777;

    int& bRef = listB.getReference(100);
    TEST_ASSERT_EQUAL_INT(0, bRef);

    bRef = 888;
    int& aRefSecond = listA.getReference(101);
    TEST_ASSERT_EQUAL_INT(0, aRefSecond);
}

void test_sort_with_quick_sort_enum_compiles_and_sorts() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    list.add(9);
    list.add(1);
    list.add(6);
    list.add(3);

    list.sort(ascending_int_comparator, ArrayList<int>::QUICK_SORT);

    const int expected[] = {1, 3, 6, 9};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_sort_with_quick_sort_selected_via_setter_compiles_and_sorts() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    list.add(5);
    list.add(2);
    list.add(4);
    list.add(1);

    list.setSortAlgorithm(ArrayList<int>::QUICK_SORT);
    list.sort(ascending_int_comparator);

    const int expected[] = {1, 2, 4, 5};
    assert_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

void test_sort_on_empty_list_is_a_no_op() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.sort(ascending_int_comparator);
    list.sort(ascending_int_comparator, ArrayList<int>::QUICK_SORT);

    TEST_ASSERT_EQUAL_UINT(0, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT(4, (unsigned int)list.capacity());
}

void test_copy_assignment_preserves_sort_algorithm_setting() {
    ArrayList<int> source(ArrayList<int>::DYNAMIC2, 4);
    ArrayList<int> target(ArrayList<int>::DYNAMIC2, 4);

    source.setSortAlgorithm(ArrayList<int>::QUICK_SORT);
    target.setSortAlgorithm(ArrayList<int>::BUBBLE_SORT);

    target = source;

    TEST_ASSERT_EQUAL(source.getSortAlgorithm(), target.getSortAlgorithm());
}

void test_sort_algorithm_defaults_to_merge_sort() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    TEST_ASSERT_EQUAL(ArrayList<int>::MERGE_SORT, list.getSortAlgorithm());
}

void test_copy_constructor_copies_initial_size_and_sort_algorithm() {
    ArrayList<int> original(ArrayList<int>::DYNAMIC2, 12);
    original.setInitialSize(37);
    original.setSortAlgorithm(ArrayList<int>::QUICK_SORT);
    original.add(10);
    original.add(20);

    ArrayList<int> copy(original);

    TEST_ASSERT_EQUAL_UINT(37, (unsigned int)copy.getInitialSize());
    TEST_ASSERT_EQUAL(ArrayList<int>::QUICK_SORT, copy.getSortAlgorithm());
    TEST_ASSERT_EQUAL_UINT((unsigned int)original.size(), (unsigned int)copy.size());
    TEST_ASSERT_EQUAL_INT(10, copy.get(0));
    TEST_ASSERT_EQUAL_INT(20, copy.get(1));
}

void test_add_all_non_trivial_type_uses_assignment_path() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC2, 8);
    list.add(CopyTracked(1));
    list.add(CopyTracked(2));

    CopyTracked extra[] = {CopyTracked(3), CopyTracked(4)};
    reset_copy_counter();

    bool ok = list.addAll(extra, 2);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 2);
    TEST_ASSERT_EQUAL_INT(3, list.get(2).value);
    TEST_ASSERT_EQUAL_INT(4, list.get(3).value);
}

void test_insert_all_non_trivial_type_uses_assignment_for_inserted_block() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC2, 8);
    ArrayList<CopyTracked> block(ArrayList<CopyTracked>::DYNAMIC2, 4);

    list.add(CopyTracked(1));
    list.add(CopyTracked(2));
    list.add(CopyTracked(5));
    list.add(CopyTracked(6));

    block.add(CopyTracked(3));
    block.add(CopyTracked(4));

    reset_copy_counter();
    bool ok = list.insertAll(2, block);

    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 4);
    TEST_ASSERT_EQUAL_INT(1, list.get(0).value);
    TEST_ASSERT_EQUAL_INT(2, list.get(1).value);
    TEST_ASSERT_EQUAL_INT(3, list.get(2).value);
    TEST_ASSERT_EQUAL_INT(4, list.get(3).value);
    TEST_ASSERT_EQUAL_INT(5, list.get(4).value);
    TEST_ASSERT_EQUAL_INT(6, list.get(5).value);
}

void test_to_array_non_trivial_type_uses_assignment_path() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC2, 8);
    list.add(CopyTracked(11));
    list.add(CopyTracked(22));
    list.add(CopyTracked(33));

    CopyTracked out[3];
    reset_copy_counter();
    list.toArray(out);

    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 3);
    TEST_ASSERT_EQUAL_INT(11, out[0].value);
    TEST_ASSERT_EQUAL_INT(22, out[1].value);
    TEST_ASSERT_EQUAL_INT(33, out[2].value);
}

void test_ensure_capacity_non_trivial_type_uses_assignment_path() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC2, 4);
    list.add(CopyTracked(9));
    list.add(CopyTracked(8));
    list.add(CopyTracked(7));

    reset_copy_counter();
    list.ensureCapacity(16);

    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 3);
    TEST_ASSERT_EQUAL_UINT(16, (unsigned int)list.capacity());
    TEST_ASSERT_EQUAL_INT(9, list.get(0).value);
    TEST_ASSERT_EQUAL_INT(8, list.get(1).value);
    TEST_ASSERT_EQUAL_INT(7, list.get(2).value);
}

void test_trim_to_size_non_trivial_type_uses_assignment_path() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC2, 8);
    list.add(CopyTracked(1));
    list.add(CopyTracked(2));
    list.add(CopyTracked(3));

    reset_copy_counter();
    list.trimToSize();

    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 3);
    TEST_ASSERT_EQUAL_UINT((unsigned int)list.size(), (unsigned int)list.capacity());
}

void test_add_grows_after_trimming_empty_list_to_zero_capacity() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.trimToSize();

    TEST_ASSERT_EQUAL_UINT(0, (unsigned int)list.capacity());

    list.add(1);

    TEST_ASSERT_EQUAL_UINT(1, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() >= 1);
    TEST_ASSERT_EQUAL_INT(1, list.get(0));
}

void test_add_grows_from_one_slot_capacity() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 1);

    list.add(7);
    size_t capacityBefore = list.capacity();

    list.add(8);

    TEST_ASSERT_EQUAL_UINT(2, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() > capacityBefore);
    TEST_ASSERT_EQUAL_INT(7, list.get(0));
    TEST_ASSERT_EQUAL_INT(8, list.get(1));
}

void test_resize1_non_trivial_type_uses_assignment_path() {
    ArrayList<CopyTracked> list(ArrayList<CopyTracked>::DYNAMIC, 4);
    list.add(CopyTracked(1));
    list.add(CopyTracked(2));
    list.add(CopyTracked(3));
    list.add(CopyTracked(4));

    reset_copy_counter();
    list.add(CopyTracked(5));

    TEST_ASSERT_TRUE(CopyTracked::assignmentCount >= 5);
    TEST_ASSERT_EQUAL_UINT(5, (unsigned int)list.size());
    TEST_ASSERT_TRUE(list.capacity() > 4);
}

void test_clear_keeps_capacity_and_allows_reuse() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);
    for (int i = 0; i < 6; ++i) {
        list.add(i);
    }

    size_t capBefore = list.capacity();
    list.clear();

    TEST_ASSERT_EQUAL_UINT(0, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT((unsigned int)capBefore, (unsigned int)list.capacity());

    list.add(42);
    TEST_ASSERT_EQUAL_INT(42, list.get(0));
}

void test_set_size_type_switch_to_fixed_blocks_growth() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.add(1);
    list.add(2);
    list.add(3);
    list.add(4);
    TEST_ASSERT_EQUAL(ArrayList<int>::DYNAMIC2, list.getSizeType());

    list.setSizeType(ArrayList<int>::FIXED);
    list.add(5);

    TEST_ASSERT_EQUAL(ArrayList<int>::FIXED, list.getSizeType());
    TEST_ASSERT_EQUAL_UINT(4, (unsigned int)list.size());
    TEST_ASSERT_EQUAL_UINT(4, (unsigned int)list.capacity());
}

void test_initial_size_getter_and_setter_round_trip() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 8);

    TEST_ASSERT_EQUAL_UINT(8, (unsigned int)list.getInitialSize());
    list.setInitialSize(32);
    TEST_ASSERT_EQUAL_UINT(32, (unsigned int)list.getInitialSize());
}

void setup() {
    Serial.begin(115200);
    delay(250);

    UNITY_BEGIN();
    RUN_TEST(test_add_does_not_resize_before_capacity_is_exceeded);
    RUN_TEST(test_add_triggers_resize_only_when_necessary);
    RUN_TEST(test_add_all_fills_remaining_capacity_without_resizing);
    RUN_TEST(test_add_all_array_resizes_when_insufficient_capacity);
    RUN_TEST(test_add_all_list_resizes_and_preserves_order);
    RUN_TEST(test_set_existing_item_does_not_resize);
    RUN_TEST(test_insert_on_full_list_resizes_and_shifts_items);
    RUN_TEST(test_insert_all_list_exact_fit_does_not_resize);
    RUN_TEST(test_insert_all_list_resizes_multiple_times_when_needed);
    RUN_TEST(test_get_reference_updates_item_in_place);
    RUN_TEST(test_operator_index_oob_returns_reset_default_reference_each_time);
    RUN_TEST(test_get_reference_oob_resets_and_does_not_leak_between_instances);
    RUN_TEST(test_sort_with_quick_sort_enum_compiles_and_sorts);
    RUN_TEST(test_sort_with_quick_sort_selected_via_setter_compiles_and_sorts);
    RUN_TEST(test_sort_with_merge_sort_respects_comparator);
    RUN_TEST(test_begin_and_end_are_safe_for_zero_capacity_lists);
    RUN_TEST(test_sort_on_empty_list_is_a_no_op);
    RUN_TEST(test_sort_algorithm_defaults_to_merge_sort);
    RUN_TEST(test_copy_constructor_copies_initial_size_and_sort_algorithm);
    RUN_TEST(test_add_all_non_trivial_type_uses_assignment_path);
    RUN_TEST(test_insert_all_non_trivial_type_uses_assignment_for_inserted_block);
    RUN_TEST(test_to_array_non_trivial_type_uses_assignment_path);
    RUN_TEST(test_ensure_capacity_non_trivial_type_uses_assignment_path);
    RUN_TEST(test_trim_to_size_non_trivial_type_uses_assignment_path);
    RUN_TEST(test_add_grows_after_trimming_empty_list_to_zero_capacity);
    RUN_TEST(test_add_grows_from_one_slot_capacity);
    RUN_TEST(test_resize1_non_trivial_type_uses_assignment_path);
    RUN_TEST(test_insert_all_raw_resizes_multiple_times_when_needed);
    RUN_TEST(test_clear_keeps_capacity_and_allows_reuse);
    RUN_TEST(test_set_size_type_switch_to_fixed_blocks_growth);
    RUN_TEST(test_initial_size_getter_and_setter_round_trip);
    UNITY_END();
}

void loop() {}