#include <cstdio>
#include <cstdint>

typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class Keyboard_ {
  private:
    KeyReport _keyReport;
    void sendReport(KeyReport *keys) {};
  public:
    Keyboard_(void) {};
    void begin(void) {};
    void end(void) {};
    size_t write(uint8_t k) { return 0; };
    size_t press(uint8_t k) { return 0; };
    size_t release(uint8_t k) { return 0; };
    void releaseAll(void) {};
};