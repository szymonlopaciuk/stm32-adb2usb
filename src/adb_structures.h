#ifndef ADB_STRUCTURES_h
#define ADB_STRUCTURES_h

#include <cstdint>

#define KEY_POWER 0x66

#define ADB_KEY_POWER_DOWN  0x7F7F
#define ADB_KEY_POWER_UP    0xFFFF

#define ADB_KEY_LEFT_SHIFT      0x38
#define ADB_KEY_RIGHT_SHIFT     0x7B
#define ADB_KEY_LEFT_CONTROL    0x36
#define ADB_KEY_RIGHT_CONTROL   0x7D
#define ADB_KEY_LEFT_OPTION     0x3A
#define ADB_KEY_RIGHT_OPTION    0x7C
#define ADB_KEY_LEFT_COMMAND    0x37
#define ADB_KEY_RIGHT_COMMAND   0x37
#define ADB_KEY_CAPS_LOCK       0x39

struct adb_kb_keypress {
    uint8_t key1 : 7;
    bool released1 : 1;
    uint8_t key0 : 7;
    bool released0 : 1;
};

struct adb_kb_modifiers {
    // Only on Apple Extended Keyboard:
    bool led_num : 1;       // 0, ditto
    bool led_caps : 1;      // 1, ditto
    bool led_scroll : 1;    // 2, changeable with `listen' command
    uint8_t reserved1 : 3;  // 5–3
    bool scroll_lock : 1;   // 6
    bool num_lock : 1;      // 7
    // Apple Standard Keyboard:
    bool command : 1;       // 8
    bool option : 1;        // 9
    bool shift : 1;         // 10
    bool control : 1;       // 11
    bool reset : 1;         // 12
    bool caps_lock : 1;     // 13
    bool backspace : 1;     // 14
    uint8_t reserved0 : 1;  // 15
};

// Contains device info and params
struct adb_register3 {
    uint8_t device_handler_id : 8;
    uint8_t device_address : 4;
    uint8_t reserved1 : 1;         // must be 0
    uint8_t srq_enable : 1;
    uint8_t exceptional_event : 1; // device specific, always 1 if not used
    uint8_t reserved0 : 1;         // must be 0
};

template <typename T>
union adb_kb_data {
    uint16_t raw;
    T data;
};

#endif