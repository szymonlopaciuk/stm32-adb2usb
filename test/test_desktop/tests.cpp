#define private public
#pragma GCC diagnostic ignored "-Wc++11-extensions"

#include <unity.h>
#include "adb_devices.h"
#include "hid_keyboard.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_key_report_empty(void) {
    hid_key_report k = {0};
    
    for (uint8_t i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL(0, k.keys[i]);

    TEST_ASSERT_EQUAL(0, k.modifiers);
}

void test_hid_keyboard_add_key_to_report(void) {
    hid_key_report k = {0};
    
    TEST_ASSERT_TRUE(hid_keyboard_add_key_to_report(&k, 1));
    TEST_ASSERT_TRUE(hid_keyboard_add_key_to_report(&k, 2));
    TEST_ASSERT_TRUE(hid_keyboard_add_key_to_report(&k, 3));
    TEST_ASSERT_TRUE(hid_keyboard_add_key_to_report(&k, 2));

    TEST_ASSERT_EQUAL(1, k.keys[0]);
    TEST_ASSERT_EQUAL(2, k.keys[1]);
    TEST_ASSERT_EQUAL(3, k.keys[2]);
    TEST_ASSERT_EQUAL(0, k.keys[3]);
    TEST_ASSERT_EQUAL(0, k.keys[4]);
    TEST_ASSERT_EQUAL(0, k.keys[5]);
}

void test_hid_keyboard_remove_key_from_report(void) {
    hid_key_report k = {0};
    
    uint8_t initial_keys[] = {7, 8, 9, 0, 0, 0};
    for (uint8_t i = 0; i < 6; i++) {
        k.keys[i] = initial_keys[i];
    }
    
    hid_keyboard_remove_key_from_report(&k, 7);
    hid_keyboard_remove_key_from_report(&k, 9);
    hid_keyboard_remove_key_from_report(&k, 7);

    TEST_ASSERT_EQUAL(0, k.keys[0]);
    TEST_ASSERT_EQUAL(8, k.keys[1]);
    TEST_ASSERT_EQUAL(0, k.keys[2]);
    TEST_ASSERT_EQUAL(0, k.keys[3]);
    TEST_ASSERT_EQUAL(0, k.keys[4]);
    TEST_ASSERT_EQUAL(0, k.keys[5]);
}

void test_adb_kb_keypress(void) {
    uint8_t kp_bin[2] = {0b10000001, 0b00001011};

    adb_kb_keypress kp_stru;
    kp_stru.released0 = true;
    kp_stru.key0 = 1;
    kp_stru.released1 = false;
    kp_stru.key1 = 11;

    uint16_t kp_stru_bin = *((uint16_t*)&kp_stru);

    TEST_ASSERT_EQUAL(kp_bin, kp_stru_bin);
}

void test_adb_kb_modifiers() {
    uint8_t mod_bin[2] = {0b00110101, 0b11000010};
    adb_kb_modifiers mod_stru = *((adb_kb_modifiers*)&mod_bin);

    TEST_ASSERT_EQUAL(0, mod_stru.reserved0);
    TEST_ASSERT_EQUAL(0, mod_stru.backspace);
    TEST_ASSERT_EQUAL(1, mod_stru.caps_lock);
    TEST_ASSERT_EQUAL(1, mod_stru.reset);
    TEST_ASSERT_EQUAL(0, mod_stru.control);
    TEST_ASSERT_EQUAL(1, mod_stru.shift);
    TEST_ASSERT_EQUAL(0, mod_stru.option);
    TEST_ASSERT_EQUAL(1, mod_stru.command);
    TEST_ASSERT_EQUAL(1, mod_stru.num_lock);
    TEST_ASSERT_EQUAL(1, mod_stru.scroll_lock);
    TEST_ASSERT_EQUAL(0, mod_stru.reserved1);
    TEST_ASSERT_EQUAL(0, mod_stru.led_scroll);
    TEST_ASSERT_EQUAL(1, mod_stru.led_caps);
    TEST_ASSERT_EQUAL(0, mod_stru.led_num);
}

void test_adb_command() {
    uint8_t cmd_bin = 0b01101001;
    adb_command cmd_stru = *((adb_command*)&cmd_bin);

    TEST_ASSERT_EQUAL(6, cmd_stru.addr);
    TEST_ASSERT_EQUAL(2, cmd_stru.cmd);
    TEST_ASSERT_EQUAL(1, cmd_stru.reg);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_key_report_empty);
    RUN_TEST(test_hid_keyboard_add_key_to_report);
    RUN_TEST(test_hid_keyboard_remove_key_from_report);

    RUN_TEST(test_adb_kb_keypress);
    RUN_TEST(test_adb_kb_modifiers);
    RUN_TEST(test_adb_command);
    UNITY_END();

    return 0;
}
