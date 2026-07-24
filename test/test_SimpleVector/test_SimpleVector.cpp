#include <Arduino.h>
#include <unity.h>
#include "SimpleVector.h"

void test_erase_single_element() {
    SimpleVector<int> vec;
    vec.put(10);
    vec.put(20);
    vec.put(30);
    
    // Erase the element at index 1 (value 20)
    vec.erase(1);
    
    TEST_ASSERT_EQUAL_INT(2, vec.elements());
    TEST_ASSERT_EQUAL_INT(10, vec[0]);
    TEST_ASSERT_EQUAL_INT(30, vec[1]);
}

void test_erase_with_duplicate_values() {
    SimpleVector<int> vec;
    vec.put(5);
    vec.put(10);
    vec.put(5);  // Duplicate value
    vec.put(15);
    vec.put(5);  // Another duplicate
    
    // Erase only the element at index 2 (one of the 5s)
    vec.erase(2);
    
    // Should have 4 elements remaining (not all 5s removed)
    TEST_ASSERT_EQUAL_INT(4, vec.elements());
    TEST_ASSERT_EQUAL_INT(5, vec[0]);
    TEST_ASSERT_EQUAL_INT(10, vec[1]);
    TEST_ASSERT_EQUAL_INT(15, vec[2]);  // The 15 should move to index 2
    TEST_ASSERT_EQUAL_INT(5, vec[3]);   // The last 5 should still be there
}

void test_erase_first_element() {
    SimpleVector<int> vec;
    vec.put(100);
    vec.put(200);
    vec.put(300);
    
    vec.erase(0);
    
    TEST_ASSERT_EQUAL_INT(2, vec.elements());
    TEST_ASSERT_EQUAL_INT(200, vec[0]);
    TEST_ASSERT_EQUAL_INT(300, vec[1]);
}

void test_erase_last_element() {
    SimpleVector<int> vec;
    vec.put(100);
    vec.put(200);
    vec.put(300);
    
    vec.erase(2);
    
    TEST_ASSERT_EQUAL_INT(2, vec.elements());
    TEST_ASSERT_EQUAL_INT(100, vec[0]);
    TEST_ASSERT_EQUAL_INT(200, vec[1]);
}

void test_release_memory_then_put_reinitializes_storage() {
    SimpleVector<int> vec;
    vec.put(1);
    vec.releaseMemory();

    vec.put(42);

    TEST_ASSERT_EQUAL_INT(1, vec.elements());
    TEST_ASSERT_EQUAL_INT(42, vec[0]);
}

void test_zero_capacity_constructor_can_grow() {
    SimpleVector<int> vec(0);

    vec.put(7);

    TEST_ASSERT_EQUAL_INT(1, vec.elements());
    TEST_ASSERT_EQUAL_INT(7, vec[0]);
}

void test_index_of_returns_minus_one_when_missing() {
    SimpleVector<int> vec;
    vec.put(10);
    vec.put(20);

    TEST_ASSERT_EQUAL_INT(-1, vec.indexOf(999));
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Starting SimpleVector erase() Tests...");
    
    UNITY_BEGIN();
    RUN_TEST(test_erase_single_element);
    RUN_TEST(test_erase_with_duplicate_values);
    RUN_TEST(test_erase_first_element);
    RUN_TEST(test_erase_last_element);
    RUN_TEST(test_release_memory_then_put_reinitializes_storage);
    RUN_TEST(test_zero_capacity_constructor_can_grow);
    RUN_TEST(test_index_of_returns_minus_one_when_missing);
    UNITY_END();
    
    Serial.println("SimpleVector Tests Completed!");
}

void loop() {}
