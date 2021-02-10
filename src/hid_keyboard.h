#ifndef HID_KEYBOARD_h
#define HID_KEYBOARD_h

#include <cstdint>
#include "adb_structures.h"

#define KEY_REPORT_KEYS_COUNT 6

struct hid_key_report {
  uint8_t modifiers;
  uint8_t keys[KEY_REPORT_KEYS_COUNT];
};

void hid_keyboard_init();
void hid_keyboard_close();
void hid_keyboard_send_report(hid_key_report* report);
bool hid_keyboard_set_keys_from_adb_register(hid_key_report* report, adb_kb_data<adb_kb_keypress> reg);
bool hid_keyboard_set_modifiers_from_adb_register(hid_key_report* report, adb_kb_data<adb_kb_keypress> reg);
bool hid_keyboard_update_key_in_report(hid_key_report* report, uint8_t hid_keycode, bool released);
bool hid_keyboard_update_modifier_in_report(hid_key_report* report, uint8_t adb_keycode, bool released);
bool hid_keyboard_add_key_to_report(hid_key_report* report, uint8_t hid_keycode);
bool hid_keyboard_remove_key_from_report(hid_key_report* report, uint8_t hid_keycode);

#endif