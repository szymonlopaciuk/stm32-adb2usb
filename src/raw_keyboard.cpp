#include <Arduino.h>
#include "raw_keyboard.h"
#include "keymap.h"

#if defined(USBCON)
#include "usbd_hid_composite_if.h"

RawKeyboard::RawKeyboard() {
    keyReport = (KeyReport) {0};
}

void RawKeyboard::begin(void)
{
  HID_Composite_Init(HID_KEYBOARD);
}

void RawKeyboard::end(void)
{
  HID_Composite_DeInit(HID_KEYBOARD);
}

void RawKeyboard::sendReport()
{
    uint8_t buf[8] = {keyReport.modifiers, keyReport.reserved, keyReport.keys[0],
        keyReport.keys[1], keyReport.keys[2], keyReport.keys[3], keyReport.keys[4],
        keyReport.keys[5]
    };

  HID_Composite_keyboard_sendReport(buf, 8);

  //delay required to prevent persistent key when call print
  delay(20);
}

void RawKeyboard::setKeysFromADBRegister(uint16_t reg) {
    if (reg == ADB_KEY_POWER_DOWN) // power button is a special case
    {                              // residing in both octets
        updateKeyInReport(KEY_POWER, false);
    }
    else if (reg == ADB_KEY_POWER_UP) {
        updateKeyInReport(KEY_POWER, true);
    }
    else {
         // Higher octet key:
        uint8_t key0 = (reg >> 8) & 0x7F;
        bool released0 = (reg >> 8) & 0x80;
        updateKeyInReport(adb_keycode_to_hid[key0], released0);

        // Lower octet key:
        uint8_t key1 = reg & 0x7F;
        bool released1 = reg & 0x80;
        updateKeyInReport(adb_keycode_to_hid[key1], released1);
    }
}

void RawKeyboard::setModifiersFromADBRegister(uint16_t reg) {
    
}

void RawKeyboard::updateKeyInReport(uint8_t hid_keycode, bool released) {
    if (released)
        removeKeyFromReport(hid_keycode);
    else
        addKeyToReport(hid_keycode);
}

// Returns true if after execution the key is in the report
bool RawKeyboard::addKeyToReport(uint8_t hid_keycode) {
    int8_t free_slot = -1;

    for (uint8_t i = 0; i < KEY_REPORT_KEYS_COUNT; i++) {
        if (keyReport.keys[i] == hid_keycode)
            return true; // key is already in the report

        if (keyReport.keys[i] == 0 && free_slot == -1)
            free_slot = i; // memorise empty entry idx
    }

    if (free_slot == -1)
        return false; // key report is full
    
    keyReport.keys[free_slot] = hid_keycode;
    return true;
}

void RawKeyboard::removeKeyFromReport(uint8_t hid_keycode) {
    for (uint8_t i = 0; i < KEY_REPORT_KEYS_COUNT; i++) {
        if (keyReport.keys[i] == hid_keycode) {
            keyReport.keys[i] = 0;
        }
    }
}

#endif