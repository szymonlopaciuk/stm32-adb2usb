#ifndef UNIT_TEST

#include <Arduino.h>
#include <stm32f1xx_hal_gpio.h>
#include <Keyboard.h>
#include "adb.h"

void setup() {
  Keyboard.begin();
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);

  delay(5000);
  digitalWrite(PC13, HIGH);

  Keyboard.print("Setting up IO...\n");
  pinMode(ADB_DATA_PIN, OUTPUT_OPEN_DRAIN);
  ADB_WRITE(HIGH);
  Keyboard.print("Waiting for ADB high... ");

  while(LOW == ADB_READ());
  Keyboard.print("Ready.\n");

  Keyboard.print("Resetting... ");
  adb_reset();
  delay(500); // A wait for good measure, apparently AEKII can take a moment to reset
  Keyboard.print("Ready.\n");
}

void print16b(uint16_t buff) {
  uint16_t mask = (1 << 15);
  for (uint8_t i = 0; i < 16; i++)
  {
    Keyboard.write(buff & mask ? '1' : '0');
    mask >>= 1;
  }
}

void printkbresp(uint16_t buff) {
  //Keyboard.print(" key1 ");
  Keyboard.print(buff & (1 << 15) ? "u" : "d");
  Keyboard.print((buff & (0x7F << 8)) >> 8, 16);
  //Keyboard.print(" key2 ");
  //Keyboard.print(buff & (1 << 7) ? "up  \t" : "down\t");
  //Keyboard.print(buff & 0x7F, 16);
  Keyboard.print("\n");
}

void loop() {
  //Keyboard.print("Talk addr 3 reg 0... ");
  //noInterrupts();
  uint16_t buffer = 0;
  adb_write_command(ADB_CMD_TALK | ADB_ADDRESS(2) | ADB_REGISTER(0));
  adb_wait_tlt();
  bool success = adb_read_data_packet(&buffer, 17);
  
  //interrupts();
  if (success) {
    //print16b(buffer);
    printkbresp(buffer);
  }
  //else
  //  Keyboard.print("timeout.\n");
  delay(5);
}

#endif