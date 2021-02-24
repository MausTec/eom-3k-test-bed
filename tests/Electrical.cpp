#include "../include/Test.h"
#include "../include/VoltageSense.h"
#include "../pinout.h"

TEST(SHORT_12V) {
  VoltageSense::enableResistance12V();
  float res = VoltageSense::measureResistance();
  VoltageSense::disableTestRails();

  EXPECT(res > 1000, res);
}

TEST(SHORT_5V) {
  VoltageSense::enableResistance5V();
  float res = VoltageSense::measureResistance();
  VoltageSense::disableTestRails();

  EXPECT(res > 250, res);
}

TEST(SHORT_3V3) {
  VoltageSense::enableResistance3V3();
  float res = VoltageSense::measureResistance();
  VoltageSense::disableTestRails();

  EXPECT(res > 1000, res);
}

TEST(VOLTAGE_12V) {
  VoltageSense::enable12V();
  float vcc = VoltageSense::read12V();
  delay(10);
  VoltageSense::disable12V();

  EXPECT(vcc > 11 && vcc < 13, vcc);
}

TEST(VOLTAGE_5V) {
  VoltageSense::enable12V();
  float v = VoltageSense::read5V();
  delay(10);
  VoltageSense::disable12V();

  EXPECT(v > 4.8 && v < 5.2, v);
}

TEST(VOLTAGE_3V3) {
  VoltageSense::enable12V();
  float v = VoltageSense::read3V3();
  delay(10);
  VoltageSense::disable12V();

  EXPECT(v > 3.2 && v < 3.6, v);
}

TEST(PRES_VSS_3V3) {
  VoltageSense::enable12V();
  float v = VoltageSense::readVcc(VS_PRES_PIN);
  delay(10);
  VoltageSense::disable12V();

  EXPECT(v > 3.2 && v < 3.6, v);
}