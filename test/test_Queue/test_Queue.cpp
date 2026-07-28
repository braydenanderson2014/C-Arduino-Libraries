#include <Arduino.h>
#include <unity.h>
#include "../../lib/Queue/src/Queue.h"

// ---------------------------------------------------------------------------
// Basic construction and empty state
// ---------------------------------------------------------------------------

void test_new_queue_is_empty() {
    Queue<int> q;
    TEST_ASSERT_TRUE(q.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, q.count());
    TEST_ASSERT_TRUE(q.isValid());
}

void test_new_queue_with_custom_capacity_is_empty() {
    Queue<int> q(32);
    TEST_ASSERT_TRUE(q.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, q.count());
}

void test_zero_capacity_clamps_to_safe_default() {
    Queue<int> q(0);
    q.enqueue(42);
    TEST_ASSERT_EQUAL_INT(1, q.count());
    TEST_ASSERT_EQUAL_INT(42, q.peek());
}

void test_negative_capacity_clamps_to_safe_default() {
    Queue<int> q(-5);
    q.enqueue(7);
    TEST_ASSERT_EQUAL_INT(1, q.count());
    TEST_ASSERT_EQUAL_INT(7, q.peek());
}

// ---------------------------------------------------------------------------
// enqueue / count
// ---------------------------------------------------------------------------

void test_enqueue_increments_count() {
    Queue<int> q;
    q.enqueue(1);
    TEST_ASSERT_EQUAL_INT(1, q.count());
    q.enqueue(2);
    TEST_ASSERT_EQUAL_INT(2, q.count());
    q.enqueue(3);
    TEST_ASSERT_EQUAL_INT(3, q.count());
}

void test_enqueue_clears_empty_flag() {
    Queue<int> q;
    TEST_ASSERT_TRUE(q.isEmpty());
    q.enqueue(99);
    TEST_ASSERT_FALSE(q.isEmpty());
}

// ---------------------------------------------------------------------------
// peek
// ---------------------------------------------------------------------------

void test_peek_returns_front_without_removing() {
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    TEST_ASSERT_EQUAL_INT(10, q.peek()); // front
    TEST_ASSERT_EQUAL_INT(2, q.count()); // not removed
}

void test_peek_on_empty_returns_default() {
    Queue<int> q;
    int result = q.peek();
    TEST_ASSERT_EQUAL_INT(0, result); // T() == 0 for int
}

void test_peek_does_not_change_count() {
    Queue<int> q;
    q.enqueue(5);
    q.peek();
    q.peek();
    TEST_ASSERT_EQUAL_INT(1, q.count());
}

// ---------------------------------------------------------------------------
// dequeue
// ---------------------------------------------------------------------------

void test_dequeue_returns_front_element() {
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    TEST_ASSERT_EQUAL_INT(10, q.dequeue());
}

void test_dequeue_decrements_count() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.dequeue();
    TEST_ASSERT_EQUAL_INT(2, q.count());
}

void test_dequeue_on_empty_returns_default() {
    Queue<int> q;
    int result = q.dequeue();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, q.count());
}

void test_dequeue_to_empty_sets_isEmpty() {
    Queue<int> q;
    q.enqueue(1);
    q.dequeue();
    TEST_ASSERT_TRUE(q.isEmpty());
}

// ---------------------------------------------------------------------------
// FIFO ordering
// ---------------------------------------------------------------------------

void test_fifo_order_enqueue_then_dequeue() {
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    TEST_ASSERT_EQUAL_INT(10, q.dequeue());
    TEST_ASSERT_EQUAL_INT(20, q.dequeue());
    TEST_ASSERT_EQUAL_INT(30, q.dequeue());
    TEST_ASSERT_TRUE(q.isEmpty());
}

void test_interleaved_enqueue_dequeue_maintains_order() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    TEST_ASSERT_EQUAL_INT(1, q.dequeue());
    q.enqueue(3);
    TEST_ASSERT_EQUAL_INT(2, q.dequeue());
    TEST_ASSERT_EQUAL_INT(3, q.dequeue());
    TEST_ASSERT_TRUE(q.isEmpty());
}

// ---------------------------------------------------------------------------
// isFull / auto-resize
// ---------------------------------------------------------------------------

void test_is_full_when_capacity_reached() {
    Queue<int> q(4);
    q.enqueue(1); q.enqueue(2); q.enqueue(3); q.enqueue(4);
    TEST_ASSERT_TRUE(q.isFull());
}

void test_auto_resize_allows_enqueue_beyond_initial_capacity() {
    Queue<int> q(4);
    for (int i = 0; i < 20; i++) {
        q.enqueue(i);
    }
    TEST_ASSERT_EQUAL_INT(20, q.count());
}

void test_auto_resize_preserves_all_elements_in_order() {
    Queue<int> q(4);
    for (int i = 0; i < 16; i++) {
        q.enqueue(i);
    }
    for (int i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL_INT(i, q.dequeue());
    }
    TEST_ASSERT_TRUE(q.isEmpty());
}

void test_not_full_after_resize() {
    Queue<int> q(2);
    q.enqueue(1); q.enqueue(2); // full now
    q.enqueue(3);               // triggers resize, no longer full
    TEST_ASSERT_FALSE(q.isFull());
    TEST_ASSERT_EQUAL_INT(3, q.count());
}

// ---------------------------------------------------------------------------
// Compaction: rear reaches end but elements < size
// ---------------------------------------------------------------------------

void test_compaction_allows_enqueue_without_allocation() {
    Queue<int> q(10);
    // Fill fully then dequeue half to free the front
    for (int i = 0; i < 10; i++) q.enqueue(i);
    for (int i = 0; i < 5; i++) q.dequeue();
    // rear is at index 9 (size-1); elements == 5 < size==10 → compaction
    q.enqueue(99);
    TEST_ASSERT_EQUAL_INT(6, q.count());
    TEST_ASSERT_EQUAL_INT(5, q.peek()); // front is the 6th original element
}

void test_compaction_preserves_fifo_order() {
    Queue<int> q(6);
    for (int i = 0; i < 6; i++) q.enqueue(i);   // 0..5, rear=5=size-1, full
    q.dequeue(); q.dequeue(); q.dequeue();        // remove 0,1,2; front=3, elements=3
    q.enqueue(10); q.enqueue(11); q.enqueue(12); // compacts then enqueues
    // Expected order: 3,4,5,10,11,12
    TEST_ASSERT_EQUAL_INT(3,  q.dequeue());
    TEST_ASSERT_EQUAL_INT(4,  q.dequeue());
    TEST_ASSERT_EQUAL_INT(5,  q.dequeue());
    TEST_ASSERT_EQUAL_INT(10, q.dequeue());
    TEST_ASSERT_EQUAL_INT(11, q.dequeue());
    TEST_ASSERT_EQUAL_INT(12, q.dequeue());
    TEST_ASSERT_TRUE(q.isEmpty());
}

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

void test_clear_makes_queue_empty() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.clear();
    TEST_ASSERT_TRUE(q.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, q.count());
}

void test_clear_then_enqueue_works_correctly() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.clear();
    q.enqueue(99);
    TEST_ASSERT_EQUAL_INT(1, q.count());
    TEST_ASSERT_EQUAL_INT(99, q.peek());
}

void test_clear_empty_queue_is_harmless() {
    Queue<int> q;
    q.clear();
    TEST_ASSERT_TRUE(q.isEmpty());
    q.enqueue(7);
    TEST_ASSERT_EQUAL_INT(7, q.dequeue());
}

// ---------------------------------------------------------------------------
// Copy constructor
// ---------------------------------------------------------------------------

void test_copy_constructor_makes_independent_copy() {
    Queue<int> q;
    q.enqueue(10);
    q.enqueue(20);

    Queue<int> copy(q);

    TEST_ASSERT_EQUAL_INT(2, copy.count());
    TEST_ASSERT_EQUAL_INT(10, copy.dequeue());
    TEST_ASSERT_EQUAL_INT(20, copy.dequeue());
    TEST_ASSERT_TRUE(copy.isEmpty());

    // Original should be unaffected
    TEST_ASSERT_EQUAL_INT(2, q.count());
}

void test_copy_constructor_is_independent() {
    Queue<int> q;
    q.enqueue(1);

    Queue<int> copy(q);
    copy.enqueue(2); // modify copy

    TEST_ASSERT_EQUAL_INT(1, q.count()); // original unchanged
    TEST_ASSERT_EQUAL_INT(2, copy.count());
}

void test_copy_constructor_on_non_zero_front() {
    Queue<int> q(6);
    for (int i = 0; i < 6; i++) q.enqueue(i);
    q.dequeue(); q.dequeue(); // front advances to 2

    Queue<int> copy(q);

    // copy should compact to front=0, elements 2..5
    TEST_ASSERT_EQUAL_INT(4, copy.count());
    TEST_ASSERT_EQUAL_INT(2, copy.dequeue());
    TEST_ASSERT_EQUAL_INT(3, copy.dequeue());
}

// ---------------------------------------------------------------------------
// Assignment operator
// ---------------------------------------------------------------------------

void test_assignment_operator_copies_elements() {
    Queue<int> q;
    q.enqueue(5);
    q.enqueue(15);

    Queue<int> other;
    other = q;

    TEST_ASSERT_EQUAL_INT(2, other.count());
    TEST_ASSERT_EQUAL_INT(5, other.dequeue());
    TEST_ASSERT_EQUAL_INT(15, other.dequeue());
}

void test_assignment_operator_is_independent() {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);

    Queue<int> other;
    other = q;
    other.enqueue(3); // modify other

    TEST_ASSERT_EQUAL_INT(2, q.count()); // q unchanged
}

void test_self_assignment_is_safe() {
    Queue<int> q;
    q.enqueue(42);
    q = q;
    TEST_ASSERT_EQUAL_INT(1, q.count());
    TEST_ASSERT_EQUAL_INT(42, q.peek());
}

// ---------------------------------------------------------------------------
// Non-int types
// ---------------------------------------------------------------------------

void test_queue_of_float() {
    Queue<float> q;
    q.enqueue(1.5f);
    q.enqueue(2.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, q.dequeue());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.5f, q.dequeue());
}

void test_queue_of_string() {
    Queue<String> q;
    q.enqueue(String("hello"));
    q.enqueue(String("world"));
    TEST_ASSERT_EQUAL_STRING("hello", q.dequeue().c_str());
    TEST_ASSERT_EQUAL_STRING("world", q.dequeue().c_str());
}

// ---------------------------------------------------------------------------
// Stress: many enqueues and dequeues
// ---------------------------------------------------------------------------

void test_large_enqueue_dequeue_cycle() {
    Queue<int> q;
    const int N = 100;
    for (int i = 0; i < N; i++) {
        q.enqueue(i);
    }
    TEST_ASSERT_EQUAL_INT(N, q.count());
    for (int i = 0; i < N; i++) {
        TEST_ASSERT_EQUAL_INT(i, q.dequeue());
    }
    TEST_ASSERT_TRUE(q.isEmpty());
}

void test_repeated_fill_and_drain_cycles() {
    Queue<int> q(4);
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 8; i++) q.enqueue(i * 10);
        for (int i = 0; i < 8; i++) {
            TEST_ASSERT_EQUAL_INT(i * 10, q.dequeue());
        }
        TEST_ASSERT_TRUE(q.isEmpty());
    }
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting Queue tests...");

    UNITY_BEGIN();

    RUN_TEST(test_new_queue_is_empty);
    RUN_TEST(test_new_queue_with_custom_capacity_is_empty);
    RUN_TEST(test_zero_capacity_clamps_to_safe_default);
    RUN_TEST(test_negative_capacity_clamps_to_safe_default);

    RUN_TEST(test_enqueue_increments_count);
    RUN_TEST(test_enqueue_clears_empty_flag);

    RUN_TEST(test_peek_returns_front_without_removing);
    RUN_TEST(test_peek_on_empty_returns_default);
    RUN_TEST(test_peek_does_not_change_count);

    RUN_TEST(test_dequeue_returns_front_element);
    RUN_TEST(test_dequeue_decrements_count);
    RUN_TEST(test_dequeue_on_empty_returns_default);
    RUN_TEST(test_dequeue_to_empty_sets_isEmpty);

    RUN_TEST(test_fifo_order_enqueue_then_dequeue);
    RUN_TEST(test_interleaved_enqueue_dequeue_maintains_order);

    RUN_TEST(test_is_full_when_capacity_reached);
    RUN_TEST(test_auto_resize_allows_enqueue_beyond_initial_capacity);
    RUN_TEST(test_auto_resize_preserves_all_elements_in_order);
    RUN_TEST(test_not_full_after_resize);

    RUN_TEST(test_compaction_allows_enqueue_without_allocation);
    RUN_TEST(test_compaction_preserves_fifo_order);

    RUN_TEST(test_clear_makes_queue_empty);
    RUN_TEST(test_clear_then_enqueue_works_correctly);
    RUN_TEST(test_clear_empty_queue_is_harmless);

    RUN_TEST(test_copy_constructor_makes_independent_copy);
    RUN_TEST(test_copy_constructor_is_independent);
    RUN_TEST(test_copy_constructor_on_non_zero_front);

    RUN_TEST(test_assignment_operator_copies_elements);
    RUN_TEST(test_assignment_operator_is_independent);
    RUN_TEST(test_self_assignment_is_safe);

    RUN_TEST(test_queue_of_float);
    RUN_TEST(test_queue_of_string);

    RUN_TEST(test_large_enqueue_dequeue_cycle);
    RUN_TEST(test_repeated_fill_and_drain_cycles);

    UNITY_END();

    Serial.println("Queue Tests Completed!");
}

void loop() {}
