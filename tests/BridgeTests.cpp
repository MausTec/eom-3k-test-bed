#include "../include/Test.h"
#include <Arduino.h>
#include <Adafruit_MCP23017.h>

extern Adafruit_MCP23017 IO2;

const int wires[] = {
    IO2_TEST_A,
    IO2_TEST_OLED_CS,
    IO2_TEST_OLED_DC,
    IO2_TEST_OLED_RST,
    IO2_TEST_MISO,
    IO2_TEST_SCK,
    IO2_TEST_MOSI,
    IO2_TEST_SD_CS,
    IO2_TEST_B,
    IO2_TEST_BL,
    IO2_TEST_SW,
    IO2_TEST_GR,
    IO2_TEST_RD,
    IO2_TEST_K3,
    IO2_TEST_K2,
    IO2_TEST_K1,
};

const int wire_count = sizeof(wires) / sizeof(int);

TEST(BRIDGE_CHECK) {
  int a, b, shorts = 0;
  for(int i = 0; i < wire_count; i++)
    IO2.pinMode(wires[i], INPUT);

  for(int i = 0; i < wire_count; i++) {
    a = wires[i];
    IO2.pinMode(a, OUTPUT);
    IO2.digitalWrite(a, HIGH);
    delay(10);

    for(int j = i+1; j < wire_count; j++) {
      b = wires[j];
      if (IO2.digitalRead(b)) {
        ANSI::log("\tShort Check: ");
        ANSI::log(a);
        ANSI::log(" <> ");
        ANSI::log(b);
        ANSI::log(" = ");
        ANSI::logln("SHORT");
        shorts++;
      }
    }

    IO2.digitalWrite(a, LOW);
    IO2.pinMode(a, INPUT);
  }

  for(int i = 0; i < wire_count; i++)
    IO2.pinMode(wires[i], INPUT);

  EXPECT(shorts == 0, shorts);
}