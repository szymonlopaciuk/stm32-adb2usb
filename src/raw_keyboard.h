#ifndef RAWKEYBOARD_h
#define RAWKEYBOARD_h

#if !defined(USBCON) || !defined(USBD_USE_HID_COMPOSITE)

#error "USB HID not enabled! Select 'HID' in the 'Tools->USB interface' menu."

#else

#include <cstdint>

#define KEY_REPORT_KEYS_COUNT  6

typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;


class RawKeyboard {
    private:
        RawKeyboard();
        KeyReport keyReport;
        void updateKeyInReport(uint8_t hid_keycode, bool released);
        bool addKeyToReport(uint8_t hid_keycode);
        void removeKeyFromReport(uint8_t hid_keycode);
    public:
        void begin();
        void end();
        void setKeysFromADBRegister(uint16_t);
        void setModifiersFromADBRegister(uint16_t);
        void sendReport();
};

#endif

#endif