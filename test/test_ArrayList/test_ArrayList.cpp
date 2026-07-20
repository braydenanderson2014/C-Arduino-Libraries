#include <Arduino.h>
#include <unity.h>
#include <ArrayList.h>

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

void test_get_reference_updates_item_in_place() {
    ArrayList<int> list(ArrayList<int>::DYNAMIC2, 4);

    list.add(10);
    int& value = list.getReference(0);
    value = 99;

    TEST_ASSERT_EQUAL_INT(99, list.get(0));
}

void setup() {
    Serial.begin(115200);
    delay(250);

    UNITY_BEGIN();
    RUN_TEST(test_add_does_not_resize_before_capacity_is_exceeded);
    RUN_TEST(test_add_all_fills_remaining_capacity_without_resizing);
    RUN_TEST(test_set_existing_item_does_not_resize);
    RUN_TEST(test_get_reference_updates_item_in_place);
    UNITY_END();
}

void loop() {}