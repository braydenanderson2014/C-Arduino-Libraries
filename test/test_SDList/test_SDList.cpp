#include <Arduino.h>
#include <unity.h>
#include <SDList.h>

// Simple POD payload used to verify non-primitive type behavior.
struct Point {
    int16_t x;
    int16_t y;
};

// Shared assertion helper for validating full list contents and order.
static void assert_int_list_equals(SDList<int, 4>& list, const int* expected, size_t expectedSize) {
    TEST_ASSERT_EQUAL_UINT32((uint32_t)expectedSize, (uint32_t)list.size());
    for (size_t i = 0; i < expectedSize; ++i) {
        TEST_ASSERT_EQUAL_INT(expected[i], list.get((uint32_t)i));
    }
}

// Verifies MEMORY mode can be initialized via begin() and reports ready state.
void test_memory_begin_sets_ready_and_mode() {
    SDList<int, 4> list(MEMORY, 2);

    TEST_ASSERT_TRUE(list.begin(4, "sdlist_memory_begin.bin"));
    TEST_ASSERT_TRUE(list.isReady());
    TEST_ASSERT_EQUAL(MEMORY, list.getMode());
    TEST_ASSERT_TRUE(list.isEmpty());
}

// Ensures reserveCapacity below BATCH_SIZE is clamped up to BATCH_SIZE.
void test_constructor_clamps_minimum_capacity_to_batch_size() {
    SDList<int, 8> list(MEMORY, 1);
    list.begin(4, "sdlist_clamp.bin");

    list.append(10);

    TEST_ASSERT_TRUE(list.capacity() >= 8);
    TEST_ASSERT_EQUAL_UINT32(1, (uint32_t)list.size());
    TEST_ASSERT_EQUAL_INT(10, list.get(0));
}

// Confirms append() and add() place elements at the tail and preserve order.
void test_append_and_add_preserve_order() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_append.bin");

    for (int i = 0; i < 6; ++i) {
        TEST_ASSERT_TRUE(list.append(i * 10));
    }
    TEST_ASSERT_TRUE(list.add(60));

    const int expected[] = {0, 10, 20, 30, 40, 50, 60};
    assert_int_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

// SDList contract: out-of-range get() returns default-constructed T().
void test_get_out_of_range_returns_default_value() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_get_default.bin");
    list.append(123);

    TEST_ASSERT_EQUAL_INT(0, list.get(99));
}

// set() should modify valid indexes and reject writes past the end.
void test_set_updates_existing_element_and_rejects_out_of_range() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_set.bin");

    list.append(5);
    list.append(6);

    TEST_ASSERT_TRUE(list.set(1, 42));
    TEST_ASSERT_EQUAL_INT(42, list.get(1));

    TEST_ASSERT_FALSE(list.set(2, 100));
    TEST_ASSERT_EQUAL_UINT32(2, (uint32_t)list.size());
}

// Covers insert semantics at front/middle/end and append-on-out-of-range insert.
void test_insert_at_beginning_middle_end_and_past_end() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_insert.bin");

    list.append(1);
    list.append(3);
    list.append(5);

    TEST_ASSERT_TRUE(list.insert(0, 0));
    TEST_ASSERT_TRUE(list.insert(2, 2));
    TEST_ASSERT_TRUE(list.insert(5, 6));
    TEST_ASSERT_TRUE(list.insert(200, 7));

    const int expected[] = {0, 1, 2, 3, 5, 6, 7};
    assert_int_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

// Validates remove shifts correctly and invalid indexes fail without mutation.
void test_remove_at_various_positions_and_invalid_index() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_remove.bin");

    for (int i = 0; i < 6; ++i) {
        list.append(i);
    }

    TEST_ASSERT_TRUE(list.remove(0));
    TEST_ASSERT_TRUE(list.remove(2));
    TEST_ASSERT_TRUE(list.remove((uint32_t)list.size() - 1));
    TEST_ASSERT_FALSE(list.remove(100));

    const int expected[] = {1, 2, 4};
    assert_int_list_equals(list, expected, sizeof(expected) / sizeof(expected[0]));
}

// clear() should drop logical size while keeping MEMORY mode allocation.
void test_clear_resets_size_and_empty_state() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_clear.bin");

    for (int i = 0; i < 10; ++i) {
        list.append(i);
    }

    size_t capBefore = list.capacity();
    list.clear();

    TEST_ASSERT_TRUE(list.isEmpty());
    TEST_ASSERT_EQUAL_UINT32(0, (uint32_t)list.size());
    TEST_ASSERT_EQUAL_UINT32((uint32_t)capBefore, (uint32_t)list.capacity());
    TEST_ASSERT_EQUAL_INT(0, list.get(0));
}

// MEMORY growth should start at BATCH_SIZE then expand geometrically.
void test_memory_capacity_grows_in_powers_of_two_from_batch_size() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_capacity_growth.bin");

    TEST_ASSERT_EQUAL_UINT32(0, (uint32_t)list.capacity());

    list.append(1);
    TEST_ASSERT_EQUAL_UINT32(4, (uint32_t)list.capacity());

    for (int i = 2; i <= 4; ++i) {
        list.append(i);
    }
    TEST_ASSERT_EQUAL_UINT32(4, (uint32_t)list.capacity());

    list.append(5);
    TEST_ASSERT_EQUAL_UINT32(8, (uint32_t)list.capacity());
}

// Confirms POD structs survive insert/get/remove without field corruption.
void test_struct_round_trip_and_insert_remove_with_structs() {
    SDList<Point, 4> points(MEMORY, 4);
    points.begin(4, "sdlist_struct.bin");

    Point p1 = {10, 20};
    Point p2 = {-5, 30};
    Point p3 = {99, -99};

    TEST_ASSERT_TRUE(points.append(p1));
    TEST_ASSERT_TRUE(points.append(p2));
    TEST_ASSERT_TRUE(points.insert(1, p3));

    Point r0 = points.get(0);
    Point r1 = points.get(1);
    Point r2 = points.get(2);

    TEST_ASSERT_EQUAL_INT16(10, r0.x);
    TEST_ASSERT_EQUAL_INT16(20, r0.y);
    TEST_ASSERT_EQUAL_INT16(99, r1.x);
    TEST_ASSERT_EQUAL_INT16(-99, r1.y);
    TEST_ASSERT_EQUAL_INT16(-5, r2.x);
    TEST_ASSERT_EQUAL_INT16(30, r2.y);

    TEST_ASSERT_TRUE(points.remove(1));
    Point afterRemove = points.get(1);
    TEST_ASSERT_EQUAL_INT16(-5, afterRemove.x);
    TEST_ASSERT_EQUAL_INT16(30, afterRemove.y);
}

// In SDCARD mode, operations before begin() should be safely rejected.
void test_not_ready_sdcard_mode_rejects_writes_before_begin() {
    SDList<int, 4> list(SDCARD, 4);

    TEST_ASSERT_FALSE(list.isReady());
    TEST_ASSERT_FALSE(list.append(7));
    TEST_ASSERT_FALSE(list.set(0, 9));
    TEST_ASSERT_FALSE(list.remove(0));
    TEST_ASSERT_EQUAL_INT(0, list.get(0));
}

// Switching MEMORY -> SDCARD without readiness should be a no-op.
void test_set_mode_to_sdcard_without_readiness_is_noop() {
    SDList<int, 4> list(MEMORY, 4);

    list.append(10);
    list.append(20);
    TEST_ASSERT_EQUAL(MEMORY, list.getMode());

    list.setMode(SDCARD);

    TEST_ASSERT_EQUAL(MEMORY, list.getMode());
    TEST_ASSERT_EQUAL_UINT32(2, (uint32_t)list.size());
    TEST_ASSERT_EQUAL_INT(10, list.get(0));
    TEST_ASSERT_EQUAL_INT(20, list.get(1));
}

// Stresses mixed operations and continuously compares against a simple model.
void test_fuzz_like_mixed_operations_match_reference_model() {
    SDList<int, 4> list(MEMORY, 4);
    list.begin(4, "sdlist_fuzz.bin");

    int model[128];
    size_t modelSize = 0;

    for (int i = 0; i < 96; ++i) {
        int action = i % 4;

        if (action == 0 || modelSize == 0) {
            int value = i * 3;
            TEST_ASSERT_TRUE(list.append(value));
            model[modelSize++] = value;
        } else if (action == 1) {
            uint32_t idx = (uint32_t)(i % (int)modelSize);
            int value = i * 7;
            TEST_ASSERT_TRUE(list.set(idx, value));
            model[idx] = value;
        } else if (action == 2) {
            uint32_t idx = (uint32_t)(i % ((int)modelSize + 1));
            int value = -i;
            TEST_ASSERT_TRUE(list.insert(idx, value));
            for (size_t j = modelSize; j > idx; --j) {
                model[j] = model[j - 1];
            }
            model[idx] = value;
            ++modelSize;
        } else {
            uint32_t idx = (uint32_t)(i % (int)modelSize);
            TEST_ASSERT_TRUE(list.remove(idx));
            for (size_t j = idx; j + 1 < modelSize; ++j) {
                model[j] = model[j + 1];
            }
            --modelSize;
        }

        TEST_ASSERT_EQUAL_UINT32((uint32_t)modelSize, (uint32_t)list.size());
        for (size_t k = 0; k < modelSize; ++k) {
            TEST_ASSERT_EQUAL_INT(model[k], list.get((uint32_t)k));
        }
    }
}

// Arduino/Unity entrypoint: run all tests once in setup().
void setup() {
    Serial.begin(115200);
    delay(500);

    UNITY_BEGIN();
    RUN_TEST(test_memory_begin_sets_ready_and_mode);
    RUN_TEST(test_constructor_clamps_minimum_capacity_to_batch_size);
    RUN_TEST(test_append_and_add_preserve_order);
    RUN_TEST(test_get_out_of_range_returns_default_value);
    RUN_TEST(test_set_updates_existing_element_and_rejects_out_of_range);
    RUN_TEST(test_insert_at_beginning_middle_end_and_past_end);
    RUN_TEST(test_remove_at_various_positions_and_invalid_index);
    RUN_TEST(test_clear_resets_size_and_empty_state);
    RUN_TEST(test_memory_capacity_grows_in_powers_of_two_from_batch_size);
    RUN_TEST(test_struct_round_trip_and_insert_remove_with_structs);
    RUN_TEST(test_not_ready_sdcard_mode_rejects_writes_before_begin);
    RUN_TEST(test_set_mode_to_sdcard_without_readiness_is_noop);
    RUN_TEST(test_fuzz_like_mixed_operations_match_reference_model);
    UNITY_END();
}

void loop() {}
