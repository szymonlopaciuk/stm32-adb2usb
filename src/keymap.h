#ifndef KEYMAP_h
#define KEYMAP_h
#include <cstdint>

#define KEY_POWER 0x66

#define ADB_KEY_POWER_DOWN  0x7F7F
#define ADB_KEY_POWER_UP    0xFFFF

uint8_t* adb_keycode_to_hid = {0x0}; // TODO

#endif