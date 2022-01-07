#include "Test.h"
#include "VoltageSense.h"
#include "pinout.h"
#include <Adafruit_MCP23017.h>

extern Adafruit_MCP23017 IO1;

float runTest(uint8_t a, uint8_t b) {
  VoltageSense::enable12V();

  if (a) {
    IO1.pinMode(IO1_PRES_VIN1, OUTPUT);
    IO1.digitalWrite(IO1_PRES_VIN1, HIGH);
  } else {
    IO1.pinMode(IO1_PRES_VIN1, INPUT);
  }

  if (b) {
    IO1.pinMode(IO1_PRES_VIN2, OUTPUT);
    IO1.digitalWrite(IO1_PRES_VIN2, HIGH);
  } else {
    IO1.pinMode(IO1_PRES_VIN2, INPUT);
  }

  delay(100);
  float vcc = VoltageSense::readVcc(TEST_V_PRES_PIN, 5.0);

  IO1.pinMode(IO1_PRES_VIN1, INPUT);
  IO1.pinMode(IO1_PRES_VIN2, INPUT);
  VoltageSense::disable12V();

  return vcc;
}

TEST(NO_PRES_AMP) {
  float v = runTest(LOW, LOW);
  EXPECT(v == 0.0, v);
}

TEST(LOW_PRES_AMP) {
  float v = runTest(LOW, HIGH);
  EXPECT(v >= 1.6, v);
}

TEST(MID_PRES_AMP) {
  float v = runTest(HIGH, LOW);
  EXPECT(v >= 2.4, v);
}

TEST(HIGH_PRES_AMP) {
  float v = runTest(HIGH, HIGH);
  EXPECT(v >= 2.9, v);
}