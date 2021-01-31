#define private public
#pragma GCC diagnostic ignored "-Wc++11-extensions"

#include <unity.h>
#include "raw_keyboard.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_keyReport_empty(void) {
    RawKeyboard k;
    
    for (uint8_t i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL(0, k.keyReport.keys[i]);

    TEST_ASSERT_EQUAL(0, k.keyReport.reserved);
    TEST_ASSERT_EQUAL(0, k.keyReport.modifiers);
}

void test_addKeyToReport(void) {
    RawKeyboard k;
    
    TEST_ASSERT_TRUE(k.addKeyToReport(1));
    TEST_ASSERT_TRUE(k.addKeyToReport(2));
    TEST_ASSERT_TRUE(k.addKeyToReport(3));
    TEST_ASSERT_TRUE(k.addKeyToReport(2));

    TEST_ASSERT_EQUAL(1, k.keyReport.keys[0]);
    TEST_ASSERT_EQUAL(2, k.keyReport.keys[1]);
    TEST_ASSERT_EQUAL(3, k.keyReport.keys[2]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[3]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[4]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[5]);
}

void test_removeKeyFromReport(void) {
    RawKeyboard k;
    
    uint8_t initial_keys[] = {7, 8, 9, 0, 0, 0};
    for (uint8_t i = 0; i < 6; i++) {
        k.keyReport.keys[i] = initial_keys[i];
    }
    
    k.removeKeyFromReport(7);
    k.removeKeyFromReport(9);
    k.removeKeyFromReport(7);

    TEST_ASSERT_EQUAL(0, k.keyReport.keys[0]);
    TEST_ASSERT_EQUAL(8, k.keyReport.keys[1]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[2]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[3]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[4]);
    TEST_ASSERT_EQUAL(0, k.keyReport.keys[5]);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_keyReport_empty);
    RUN_TEST(test_addKeyToReport);
    RUN_TEST(test_removeKeyFromReport);
    UNITY_END();

    return 0;
}
