#include <Arduino.h>
#include <unity.h>
#include "../../lib/Stack/src/Stack.h"

// ---------------------------------------------------------------------------
// Basic construction and empty state
// ---------------------------------------------------------------------------

void test_new_stack_is_empty() {
    Stack<int> s;
    TEST_ASSERT_TRUE(s.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, s.count());
    TEST_ASSERT_TRUE(s.isValid());
}

void test_new_stack_with_custom_capacity_is_empty() {
    Stack<int> s(32);
    TEST_ASSERT_TRUE(s.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, s.count());
}

void test_zero_capacity_clamps_to_safe_default() {
    Stack<int> s(0);
    s.push(42);
    TEST_ASSERT_EQUAL_INT(1, s.count());
    TEST_ASSERT_EQUAL_INT(42, s.peek());
}

void test_negative_capacity_clamps_to_safe_default() {
    Stack<int> s(-5);
    s.push(7);
    TEST_ASSERT_EQUAL_INT(1, s.count());
    TEST_ASSERT_EQUAL_INT(7, s.peek());
}

// ---------------------------------------------------------------------------
// push / count
// ---------------------------------------------------------------------------

void test_push_increments_count() {
    Stack<int> s;
    s.push(1);
    TEST_ASSERT_EQUAL_INT(1, s.count());
    s.push(2);
    TEST_ASSERT_EQUAL_INT(2, s.count());
    s.push(3);
    TEST_ASSERT_EQUAL_INT(3, s.count());
}

void test_push_clears_empty_flag() {
    Stack<int> s;
    TEST_ASSERT_TRUE(s.isEmpty());
    s.push(99);
    TEST_ASSERT_FALSE(s.isEmpty());
}

// ---------------------------------------------------------------------------
// peek
// ---------------------------------------------------------------------------

void test_peek_returns_top_without_removing() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    TEST_ASSERT_EQUAL_INT(20, s.peek());
    TEST_ASSERT_EQUAL_INT(2, s.count()); // not removed
}

void test_peek_on_empty_returns_default() {
    Stack<int> s;
    int result = s.peek();
    TEST_ASSERT_EQUAL_INT(0, result); // T() == 0 for int
}

void test_peek_does_not_change_count() {
    Stack<int> s;
    s.push(5);
    s.peek();
    s.peek();
    TEST_ASSERT_EQUAL_INT(1, s.count());
}

// ---------------------------------------------------------------------------
// pop
// ---------------------------------------------------------------------------

void test_pop_returns_top_element() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    TEST_ASSERT_EQUAL_INT(30, s.pop());
}

void test_pop_decrements_count() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    TEST_ASSERT_EQUAL_INT(2, s.count());
}

void test_pop_on_empty_returns_default() {
    Stack<int> s;
    int result = s.pop();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, s.count());
}

void test_pop_to_empty_sets_isEmpty() {
    Stack<int> s;
    s.push(1);
    s.pop();
    TEST_ASSERT_TRUE(s.isEmpty());
}

// ---------------------------------------------------------------------------
// LIFO ordering
// ---------------------------------------------------------------------------

void test_lifo_order_push_then_pop() {
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    TEST_ASSERT_EQUAL_INT(30, s.pop());
    TEST_ASSERT_EQUAL_INT(20, s.pop());
    TEST_ASSERT_EQUAL_INT(10, s.pop());
    TEST_ASSERT_TRUE(s.isEmpty());
}

void test_interleaved_push_pop_maintains_order() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    TEST_ASSERT_EQUAL_INT(2, s.pop());
    s.push(3);
    TEST_ASSERT_EQUAL_INT(3, s.pop());
    TEST_ASSERT_EQUAL_INT(1, s.pop());
    TEST_ASSERT_TRUE(s.isEmpty());
}

// ---------------------------------------------------------------------------
// isFull / auto-resize
// ---------------------------------------------------------------------------

void test_is_full_when_capacity_reached() {
    Stack<int> s(4);
    s.push(1); s.push(2); s.push(3); s.push(4);
    TEST_ASSERT_TRUE(s.isFull());
}

void test_auto_resize_allows_push_beyond_initial_capacity() {
    Stack<int> s(4);
    for (int i = 0; i < 20; i++) {
        s.push(i);
    }
    TEST_ASSERT_EQUAL_INT(20, s.count());
}

void test_auto_resize_preserves_all_elements() {
    Stack<int> s(4);
    for (int i = 0; i < 16; i++) {
        s.push(i);
    }
    for (int i = 15; i >= 0; i--) {
        TEST_ASSERT_EQUAL_INT(i, s.pop());
    }
    TEST_ASSERT_TRUE(s.isEmpty());
}

void test_not_full_after_resize() {
    Stack<int> s(2);
    s.push(1); s.push(2); // full now
    s.push(3);             // triggers resize, no longer full
    TEST_ASSERT_FALSE(s.isFull());
    TEST_ASSERT_EQUAL_INT(3, s.count());
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

void test_clear_makes_stack_empty() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.clear();
    TEST_ASSERT_TRUE(s.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, s.count());
}

void test_clear_then_push_works_correctly() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.clear();
    s.push(99);
    TEST_ASSERT_EQUAL_INT(1, s.count());
    TEST_ASSERT_EQUAL_INT(99, s.peek());
}

void test_clear_empty_stack_is_harmless() {
    Stack<int> s;
    s.clear();
    TEST_ASSERT_TRUE(s.isEmpty());
    s.push(7);
    TEST_ASSERT_EQUAL_INT(7, s.pop());
}

// ---------------------------------------------------------------------------
// Copy constructor
// ---------------------------------------------------------------------------

void test_copy_constructor_makes_independent_copy() {
    Stack<int> s;
    s.push(10);
    s.push(20);

    Stack<int> copy(s);

    TEST_ASSERT_EQUAL_INT(2, copy.count());
    TEST_ASSERT_EQUAL_INT(20, copy.pop());
    TEST_ASSERT_EQUAL_INT(10, copy.pop());
    TEST_ASSERT_TRUE(copy.isEmpty());

    // Original should be unaffected
    TEST_ASSERT_EQUAL_INT(2, s.count());
}

void test_copy_constructor_copy_is_independent() {
    Stack<int> s;
    s.push(1);

    Stack<int> copy(s);
    copy.push(2); // modify copy

    TEST_ASSERT_EQUAL_INT(1, s.count()); // original unchanged
    TEST_ASSERT_EQUAL_INT(2, copy.count());
}

// ---------------------------------------------------------------------------
// Assignment operator
// ---------------------------------------------------------------------------

void test_assignment_operator_copies_elements() {
    Stack<int> s;
    s.push(5);
    s.push(15);

    Stack<int> other;
    other = s;

    TEST_ASSERT_EQUAL_INT(2, other.count());
    TEST_ASSERT_EQUAL_INT(15, other.pop());
    TEST_ASSERT_EQUAL_INT(5, other.pop());
}

void test_assignment_operator_is_independent() {
    Stack<int> s;
    s.push(1);
    s.push(2);

    Stack<int> other;
    other = s;
    other.push(3); // modify other

    TEST_ASSERT_EQUAL_INT(2, s.count()); // s unchanged
}

void test_self_assignment_is_safe() {
    Stack<int> s;
    s.push(42);
    s = s;
    TEST_ASSERT_EQUAL_INT(1, s.count());
    TEST_ASSERT_EQUAL_INT(42, s.peek());
}

// ---------------------------------------------------------------------------
// Non-int types
// ---------------------------------------------------------------------------

void test_stack_of_float() {
    Stack<float> s;
    s.push(1.5f);
    s.push(2.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.5f, s.pop());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, s.pop());
}

void test_stack_of_string() {
    Stack<String> s;
    s.push(String("hello"));
    s.push(String("world"));
    TEST_ASSERT_EQUAL_STRING("world", s.pop().c_str());
    TEST_ASSERT_EQUAL_STRING("hello", s.pop().c_str());
}

// ---------------------------------------------------------------------------
// Stress: many pushes and pops
// ---------------------------------------------------------------------------

void test_large_push_pop_cycle() {
    Stack<int> s;
    const int N = 100;
    for (int i = 0; i < N; i++) {
        s.push(i);
    }
    TEST_ASSERT_EQUAL_INT(N, s.count());
    for (int i = N - 1; i >= 0; i--) {
        TEST_ASSERT_EQUAL_INT(i, s.pop());
    }
    TEST_ASSERT_TRUE(s.isEmpty());
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting Stack tests...");

    UNITY_BEGIN();

    RUN_TEST(test_new_stack_is_empty);
    RUN_TEST(test_new_stack_with_custom_capacity_is_empty);
    RUN_TEST(test_zero_capacity_clamps_to_safe_default);
    RUN_TEST(test_negative_capacity_clamps_to_safe_default);

    RUN_TEST(test_push_increments_count);
    RUN_TEST(test_push_clears_empty_flag);

    RUN_TEST(test_peek_returns_top_without_removing);
    RUN_TEST(test_peek_on_empty_returns_default);
    RUN_TEST(test_peek_does_not_change_count);

    RUN_TEST(test_pop_returns_top_element);
    RUN_TEST(test_pop_decrements_count);
    RUN_TEST(test_pop_on_empty_returns_default);
    RUN_TEST(test_pop_to_empty_sets_isEmpty);

    RUN_TEST(test_lifo_order_push_then_pop);
    RUN_TEST(test_interleaved_push_pop_maintains_order);

    RUN_TEST(test_is_full_when_capacity_reached);
    RUN_TEST(test_auto_resize_allows_push_beyond_initial_capacity);
    RUN_TEST(test_auto_resize_preserves_all_elements);
    RUN_TEST(test_not_full_after_resize);

    RUN_TEST(test_clear_makes_stack_empty);
    RUN_TEST(test_clear_then_push_works_correctly);
    RUN_TEST(test_clear_empty_stack_is_harmless);

    RUN_TEST(test_copy_constructor_makes_independent_copy);
    RUN_TEST(test_copy_constructor_copy_is_independent);

    RUN_TEST(test_assignment_operator_copies_elements);
    RUN_TEST(test_assignment_operator_is_independent);
    RUN_TEST(test_self_assignment_is_safe);

    RUN_TEST(test_stack_of_float);
    RUN_TEST(test_stack_of_string);

    RUN_TEST(test_large_push_pop_cycle);

    UNITY_END();

    Serial.println("Stack Tests Completed!");
}

void loop() {}
