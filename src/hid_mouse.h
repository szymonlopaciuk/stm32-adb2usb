#ifndef HID_MOUSE_h
#define HID_MOUSE_h

#include <cstdint>

void hid_mouse_init();
void hid_mouse_close();
void hid_mouse_send_report(bool button, int8_t offset_x, int8_t offset_y);

#endif