#include "VoltageSense.h"
#include "pinout.h"

#include <Arduino.h>
#include <Adafruit_MCP23017.h>

extern Adafruit_MCP23017 IO1;

namespace VoltageSense {
  float resistanceVccTop = 5.0;

  void disableTestRails() {
    IO1.digitalWrite(IO1_VSE_12V, LOW);
    IO1.digitalWrite(IO1_VSE_5V, LOW);
    IO1.digitalWrite(IO1_VSE_3V3, LOW);
  }

  void enable12V() {
    disableTestRails();
    delay(20);
    IO1.digitalWrite(IO1_EN_12V, HIGH);
    delay(20);
  }

  void disable12V() {
    if (IO1.digitalRead(IO1_EN_12V) == HIGH) {
      IO1.digitalWrite(IO1_EN_12V, LOW);
      delay(20);
    }
  }

  void enableResistance12V() {
    disableTestRails();
    disable12V();
    IO1.digitalWrite(IO1_VSE_12V, HIGH);
    delay(30);
  }

  void enableResistance5V() {
    disableTestRails();
    disable12V();
    IO1.digitalWrite(IO1_VSE_5V, HIGH);
    delay(30);
  }

  void enableResistance3V3() {
    disableTestRails();
    disable12V();
    IO1.digitalWrite(IO1_VSE_3V3, HIGH);
    delay(100);
  }

  float measureResistance() {
    float adc = analogRead(VS_RHIGH_PIN);
    float vcc = 0.2 * adc * (resistanceVccTop / 1023.0);
    float rLow = -1 * ((2000 * vcc) / (vcc - 1));
    return rLow;
  }

  float readVcc(int pin, float maxV) {
    float adc = analogRead(pin);
    float vcc = adc * (maxV / 1023.0);
    return vcc;
  }

  float read12V() {
    return readVcc(VS_12V_PIN);
  }

  float read5V() {
    return readVcc(VS_5V_PIN);
  }

  float read3V3() {
    return readVcc(VS_3V3_PIN);
  }
}
