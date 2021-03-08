#include "hid_mouse.h"
#include "usbd_hid_composite_if.h"
#include <Arduino.h>

void hid_mouse_init()
{
#ifdef PIO_FRAMEWORK_ARDUINO_ENABLE_HID
    HID_Composite_Init(HID_MOUSE);
#endif
}

void hid_mouse_close()
{
#ifdef PIO_FRAMEWORK_ARDUINO_ENABLE_HID
    HID_Composite_DeInit(HID_MOUSE);
#endif
}

void hid_mouse_send_report(bool button, int8_t offset_x, int8_t offset_y) {
    uint8_t m[4];
    m[0] = button;
    m[1] = offset_x;
    m[2] = offset_y;
    m[3] = 0;

#ifdef PIO_FRAMEWORK_ARDUINO_ENABLE_HID
    HID_Composite_mouse_sendReport(m, 4);
#endif
}