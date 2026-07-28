#include <Arduino.h>
#include <unity.h>

#include <SimpleVector.h>
#include <SegmentTree.h>

void test_segment_tree_sum_query() {
    SimpleVector<int> values;
    values.push_back(1);
    values.push_back(3);
    values.push_back(5);
    values.push_back(7);
    values.push_back(9);
    values.push_back(11);

    SegmentTree<int> tree(values);

    TEST_ASSERT_EQUAL_INT(15, tree.getSum(1, 3));
}

void test_segment_tree_range_and_point_updates() {
    SimpleVector<int> values;
    values.push_back(1);
    values.push_back(3);
    values.push_back(5);
    values.push_back(7);

    SegmentTree<int> tree(values);

    tree.updateRange(1, 3, 2);
    TEST_ASSERT_EQUAL_INT(21, tree.getSum(0, 3));

    tree.updateValue(values, 2, 10);
    TEST_ASSERT_EQUAL_INT(24, tree.getSum(0, 3));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_segment_tree_sum_query);
    RUN_TEST(test_segment_tree_range_and_point_updates);
    UNITY_END();
}

void loop() {
}
