#ifndef TEST_ARRAYLIST_H
#define TEST_ARRAYLIST_H

#include <Arduino.h>
#include <unity.h>

#include "ArrayList.h"

ArrayList<int> intList;
ArrayList<String> stringList;
ArrayList<float> floatList;


void setUp(void) {
  // Set up code here, to run before each test
  intList = ArrayList<int>();
  stringList = ArrayList<String>();
  floatList = ArrayList<float>();
}

void tearDown(void) {
  // Clean up code here, to run after each test
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE ADD FUNCTION                                             |
 * +------------------------------------------------------------------------+
 */
void test_addInt(void) {
  intList.add(1);
  TEST_ASSERT_EQUAL_INT(1, intList.get(0));
}

void test_addString(void) {
  stringList.add("test");
  TEST_ASSERT_EQUAL_STRING("test", stringList.get(0).c_str());
}

void test_addFloat(void) {
  floatList.add(1.23f);
  TEST_ASSERT_EQUAL_FLOAT(1.23f, floatList.get(0));
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE ADDALL FUNCTION                                          |
 * +------------------------------------------------------------------------+
 */
void test_addAllInt(void) {
  intList.add(1);
  intList.add(2);
  intList.add(3);
  ArrayList<int> otherList = ArrayList<int>();
  otherList.add(4);
  otherList.add(5);
  otherList.add(6);
  intList.addAll(otherList);
  TEST_ASSERT_EQUAL_INT(1, intList.get(0));
  TEST_ASSERT_EQUAL_INT(2, intList.get(1));
  TEST_ASSERT_EQUAL_INT(3, intList.get(2));
  TEST_ASSERT_EQUAL_INT(4, intList.get(3));
  TEST_ASSERT_EQUAL_INT(5, intList.get(4));
  TEST_ASSERT_EQUAL_INT(6, intList.get(5));
}

void test_addAllString(void) {
  stringList.add("test1");
  stringList.add("test2");
  stringList.add("test3");
  ArrayList<String> otherList = ArrayList<String>();
  otherList.add("test4");
  otherList.add("test5");
  otherList.add("test6");
  stringList.addAll(otherList);
  TEST_ASSERT_EQUAL_STRING("test1", stringList.get(0).c_str());
  TEST_ASSERT_EQUAL_STRING("test2", stringList.get(1).c_str());
  TEST_ASSERT_EQUAL_STRING("test3", stringList.get(2).c_str());
  TEST_ASSERT_EQUAL_STRING("test4", stringList.get(3).c_str());
  TEST_ASSERT_EQUAL_STRING("test5", stringList.get(4).c_str());
  TEST_ASSERT_EQUAL_STRING("test6", stringList.get(5).c_str());
}

void test_addAllFloat(void) {
  floatList.add(1.23f);
  floatList.add(2.34f);
  floatList.add(3.45f);
  ArrayList<float> otherList = ArrayList<float>();
  otherList.add(4.56f);
  otherList.add(5.67f);
  otherList.add(6.78f);
  floatList.addAll(otherList);
  TEST_ASSERT_EQUAL_FLOAT(1.23f, floatList.get(0));
  TEST_ASSERT_EQUAL_FLOAT(2.34f, floatList.get(1));
  TEST_ASSERT_EQUAL_FLOAT(3.45f, floatList.get(2));
  TEST_ASSERT_EQUAL_FLOAT(4.56f, floatList.get(3));
  TEST_ASSERT_EQUAL_FLOAT(5.67f, floatList.get(4));
  TEST_ASSERT_EQUAL_FLOAT(6.78f, floatList.get(5));
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE REMOVE FUNCTION                                          |
 * +------------------------------------------------------------------------+
 */
void test_removeInt(void) {
  intList.add(1);
  intList.add(2);
  intList.add(3);
  intList.remove(1);
  TEST_ASSERT_EQUAL_INT(1, intList.get(0));
  TEST_ASSERT_EQUAL_INT(3, intList.get(1));
}

void test_removeString(void) {
  stringList.add("test1");
  stringList.add("test2");
  stringList.add("test3");
  stringList.remove(1);
  TEST_ASSERT_EQUAL_STRING("test1", stringList.get(0).c_str());
  TEST_ASSERT_EQUAL_STRING("test3", stringList.get(1).c_str());
}

void test_removeFloat(void) {
  floatList.add(1.23f);
  floatList.add(2.34f);
  floatList.add(3.45f);
  floatList.remove(1);
  TEST_ASSERT_EQUAL_FLOAT(1.23f, floatList.get(0));
  TEST_ASSERT_EQUAL_FLOAT(3.45f, floatList.get(1));
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE CLEAR FUNCTION                                           |
 * +------------------------------------------------------------------------+
 */
void test_clearInt(void) {
  intList.add(1);
  intList.add(2);
  intList.add(3);
  intList.clear();
  TEST_ASSERT_EQUAL_INT(0, intList.size());
}

void test_clearString(void) {
  stringList.add("test1");
  stringList.add("test2");
  stringList.add("test3");
  stringList.clear();
  TEST_ASSERT_EQUAL_INT(0, stringList.size());
}

void test_clearFloat(void) {
  floatList.add(1.23f);
  floatList.add(2.34f);
  floatList.add(3.45f);
  floatList.clear();
  TEST_ASSERT_EQUAL_INT(0, floatList.size());
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE SIZE FUNCTION                                            |
 * +------------------------------------------------------------------------+
 */
void test_sizeInt(void) {
  intList.add(1);
  intList.add(2);
  intList.add(3);
  TEST_ASSERT_EQUAL_INT(3, intList.size());
}

void test_sizeString(void) {
  stringList.add("test1");
  stringList.add("test2");
  stringList.add("test3");
  TEST_ASSERT_EQUAL_INT(3, stringList.size());
}

void test_sizeFloat(void) {
  floatList.add(1.23f);
  floatList.add(2.34f);
  floatList.add(3.45f);
  TEST_ASSERT_EQUAL_INT(3, floatList.size());
}

/**
 * +------------------------------------------------------------------------+
 * | TESTS FOR THE CONTAINS FUNCTION                                        |
 * +------------------------------------------------------------------------+
 */
void test_containsInt(void) {
  intList.add(1);
  intList.add(2);
  intList.add(3);
  TEST_ASSERT_TRUE(intList.contains(2));
}

void test_containsString(void) {
  stringList.add("test1");
  stringList.add("test2");
  stringList.add("test3");
  TEST_ASSERT_TRUE(stringList.contains("test2"));
}

void test_containsFloat(void) {
  floatList.add(1.23f);
  floatList.add(2.34f);
  floatList.add(3.45f);
  TEST_ASSERT_TRUE(floatList.contains(2.34f));
}

#endif // TEST_ARRAYLIST_H