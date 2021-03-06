#ifndef UNIT_TEST

#include <Arduino.h>
#include <stm32f1xx_hal_gpio.h>
#include "hid_tables.h"
#include "adb_structures.h"
#include "hid_keyboard.h"
#include "hid_mouse.h"
#include "adb_devices.h"
#include "Mouse.h"

#define POLL_DELAY      5

bool apple_extended_detected = false;
bool keyboard_present = false, mouse_present = false;

#ifdef PIO_FRAMEWORK_ARDUINO_ENABLE_CDC
void print16b(uint16_t buff) {
    uint16_t mask = (1 << 15);
    for (uint8_t i = 0; i < 16; i++)
    {
      Serial.write(buff & mask ? '1' : '0');
      mask >>= 1;
    }
    Serial.write('\n');
    Serial.flush();
}

void printkbresp(adb_kb_keypress buff) {
    //Keyboard.print(" key1 ");
    Serial.print(buff.released0 ? "u" : "d");
    Serial.print(buff.key0, 16);
    Serial.print(", ");
    Serial.print(buff.released1 ? "u" : "d");
    Serial.print(buff.key1, 16);
    Serial.print("\n");
    Serial.flush();
}
#endif

void setup() {
    // Turn the led on at the beginning of setup
    pinMode(PC13, OUTPUT);
    digitalWrite(PC13, LOW);

#ifdef PIO_FRAMEWORK_ARDUINO_ENABLE_CDC
    Serial.begin(115200);
#endif

    // Set up HID
    hid_keyboard_init();

    // Set up the ADB bus
    adb_init();

    delay(1000); // A wait for good measure, apparently AEKII can take a moment to reset

    // Initialise the ADB devices
    // Switch the keyboard to Apple Extended if available
    bool error = false;
    adb_data<adb_register3> reg3 = {0}, mask = {0};
    reg3.data.device_handler_id = 0x03;
    mask.data.device_handler_id = 0xFF;
    apple_extended_detected = adb_device_update_register3(ADB_ADDR_KEYBOARD, reg3, mask.raw, &error);
    if (!error) keyboard_present = true,

    // Switch the mouse to higher resolution, if available
    // TODO: Apple Extended Mouse Protocol (Handler = 4)
    error = false;
    reg3.raw = 0;
    mask.raw = 0;
    reg3.data.device_handler_id = 0x02;
    mask.data.device_handler_id = 0xFF;
    adb_device_update_register3(ADB_ADDR_MOUSE, reg3, mask.raw, &error);
    if (!error) mouse_present = true;

    // Set-up successful, turn of the LED
    digitalWrite(PC13, HIGH);
}

void keyboard_handler() {
    static bool led_caps = false;
    static hid_key_report key_report = {0};
    bool error = false;

    auto key_press = adb_keyboard_read_key_press(&error);

    if (error) return;  // don't continue changing the hid report if there was
                        // an error reading from ADB – most often it's a timeout

    bool report_changed = hid_keyboard_set_keys_from_adb_register(&key_report, key_press);

    // Handle the `toggle' caps lock key.
    // Every action reported by an ADB keyboard on the caps lock key
    // should be interpreted as key down, followed by key released,
    // unless the keypress was so fast, that it was pressed and depressed
    // within the same `register'. Then ignore.
    bool caps_lock_action = key_press.data.key0 == ADB_KEY_CAPS_LOCK ||
            key_press.data.key1 == ADB_KEY_CAPS_LOCK;

    bool caps_lock_twice_in_register = key_press.data.key0 == ADB_KEY_CAPS_LOCK &&
            key_press.data.key1 == ADB_KEY_CAPS_LOCK &&
            key_press.data.released0 != key_press.data.released1;

    if (caps_lock_action && !caps_lock_twice_in_register)
    {
        hid_keyboard_add_key_to_report(&key_report, KEY_CAPSLOCK);

        // Send the preliminary report, with caps lock down
        hid_keyboard_send_report(&key_report);

        led_caps = !led_caps;
        adb_keyboard_write_leds(0, led_caps, 0);

        // Wait a little bit
        delay(80);
        
        // Now just release the caps lock key, and continue as before
        hid_keyboard_remove_key_from_report(&key_report, KEY_CAPSLOCK);

        report_changed = true;
    }
    
    // Send the finished report
    //if (report_changed)
        hid_keyboard_send_report(&key_report);
}

void mouse_handler() {
    bool error = false;
    auto mouse_data = adb_mouse_read_data(&error);

    if (error || mouse_data.raw == 0) return;

    int8_t mouse_x = ADB_MOUSE_CONV_AXIS(mouse_data.data.x_offset);
    int8_t mouse_y = ADB_MOUSE_CONV_AXIS(mouse_data.data.y_offset);
    
    hid_mouse_send_report(mouse_data.data.button ? 0 : 1, mouse_x, mouse_y);
}

void loop() {
    if (keyboard_present) {
        keyboard_handler();
        // Wait a tiny bit before polling again,
        // while ADB seems fairly tolerent of quick requests
        // we don't want to overwhelm USB either
        delay(POLL_DELAY);
    }

    if (mouse_present) {
        mouse_handler();
        delay(POLL_DELAY);
    }
}

#endif