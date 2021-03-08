#ifndef ADB_DEVICES_h
#define ADB_DEVICES_h

#include <cstdint>

#include "adb.h"
#include "hid_keyboard.h"
#include "adb_structures.h"

#define ADB_POLL_DELAY 5

adb_data<adb_kb_modifiers> adb_keyboard_read_modifiers(bool* error) {
    adb_data<adb_kb_modifiers> modifiers = {0};
    adb_write_command(ADB_CMD_TALK | ADB_ADDRESS(ADB_ADDR_KEYBOARD) | ADB_REGISTER(2));
    adb_wait_tlt(true);
    *error = !adb_read_data_packet(&modifiers.raw, 16);
    return modifiers;
}

adb_data<adb_kb_keypress> adb_keyboard_read_key_press(bool* error) {
    adb_data<adb_kb_keypress> key_press = {0};
    adb_write_command(ADB_CMD_TALK | ADB_ADDRESS(ADB_ADDR_KEYBOARD) | ADB_REGISTER(0));
    adb_wait_tlt(true);
    *error = !adb_read_data_packet(&key_press.raw, 16);
    return key_press;
}

adb_data<adb_register3> adb_device_read_register3(uint8_t addr, bool* error) {
    adb_data<adb_register3> reg3 = {0};
    adb_write_command(ADB_CMD_TALK | ADB_ADDRESS(addr) | ADB_REGISTER(3));
    adb_wait_tlt(true);
    *error = !adb_read_data_packet(&reg3.raw, 16);
    return reg3;
}


bool adb_device_update_register3(uint8_t addr, adb_data<adb_register3> new_reg3, uint16_t mask, bool* error) {
    // Read the initial value
    adb_data<adb_register3> reg3 = adb_device_read_register3(addr, error);
    if (*error) return false;
    delay(ADB_POLL_DELAY);

    // Apply the change
    reg3.raw = (reg3.raw & ~mask) | (new_reg3.raw & mask);

    // Write the new value
    adb_write_command(ADB_CMD_LISTEN | ADB_ADDRESS(addr) | ADB_REGISTER(3));
    adb_wait_tlt(false);
    adb_write_data_packet(reg3.raw, 16);
    delay(ADB_POLL_DELAY);

    // Read the current value
    reg3 = adb_device_read_register3(addr, error);
    if (*error) return false;

    // Check if update worked, return true
    if ((reg3.raw & mask) == (new_reg3.raw & mask))
        return true;
    
    return false;
}

void adb_keyboard_write_leds(bool scroll, bool caps, bool num) {
    adb_data<adb_kb_modifiers> modifiers = {0};
    modifiers.data.led_scroll = !scroll;
    modifiers.data.led_caps = !caps;
    modifiers.data.led_num = !num;

    adb_write_command(ADB_CMD_LISTEN | ADB_ADDRESS(ADB_ADDR_KEYBOARD) | ADB_REGISTER(2));
    adb_wait_tlt(false);
    adb_write_data_packet(modifiers.raw, 16);
}

adb_data<adb_mouse_data> adb_mouse_read_data(bool* error) {
    adb_data<adb_mouse_data> mouse_data = {0};
    adb_write_command(ADB_CMD_TALK | ADB_ADDRESS(ADB_ADDR_MOUSE) | ADB_REGISTER(0));
    adb_wait_tlt(true);
    *error = !adb_read_data_packet(&mouse_data.raw, 16);
    return mouse_data;
}

#endif