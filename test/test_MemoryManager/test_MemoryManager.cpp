#include <Arduino.h>
#include <unity.h>
#include "MemoryManager.h"

MemoryManager memoryManager;

void test_memory_allocation() {
    void* ptr = memoryManager.malloc(64, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr);
    //memoryManager.free(ptr);
}
/*



void test_memory_free() {
    void* ptr = memoryManager.malloc(32, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL_INT(MM_SUCCESS, memoryManager.free(ptr));
}

void test_memory_leak_detection() {
    memoryManager.malloc(32, __FILE__, __LINE__); // Allocate without freeing
    TEST_ASSERT_EQUAL_INT(MM_MEMORY_LEAK_ERROR, memoryManager.detectMemoryLeaks());
}

void test_memory_realloc() {
    void* ptr = memoryManager.malloc(32, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr);
    void* new_ptr = memoryManager.realloc(ptr, 64, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(new_ptr);
    memoryManager.free(new_ptr);
}

void test_memory_calloc() {
    void* ptr = memoryManager.calloc(5, sizeof(int), __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr);
    int* intArray = (int*)ptr;
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(0, intArray[i]);  // Ensure memory is zero-initialized
    }
    memoryManager.free(ptr);
}

 */

void setup() {
    Serial.begin(115200);  // Ensure serial is initialized
    delay(1000);  // Allow time for Serial Monitor to initialize

    Serial.println("Starting MemoryManager Tests...");

    UNITY_BEGIN();
    RUN_TEST(test_memory_allocation);
    //RUN_TEST(test_memory_free);
    //RUN_TEST(test_memory_leak_detection);
    //RUN_TEST(test_memory_realloc);
    //RUN_TEST(test_memory_calloc);
    UNITY_END();

    Serial.println("MemoryManager Tests Completed!");
}




void loop() {}
