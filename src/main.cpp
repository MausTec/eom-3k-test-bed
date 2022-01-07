#include <Arduino.h>

#include "pinout.h"
#include "keypad.h"

#include "Test.h"
#include "ANSI.h"
#include "Hardware.h"
#include "TermUI.h"
#include "VoltageSense.h"
#include "TestRun.h"
#include "Console.h"

#include <SPI.h>
#include <SD.h>
#include <OneButton.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_LiquidCrystal.h>

OneButton Key1(KEY_1_PIN, true, false);
OneButton Key2(KEY_2_PIN, true, false);
OneButton Key3(KEY_3_PIN, true, false);
OneButton Key4(KEY_4_PIN, true, false);

Adafruit_MCP23017 IO1;
Adafruit_MCP23017 IO2;

Adafruit_LiquidCrystal LCD(2);

void setStatusLed(byte colors);
TestResultNode *result_cursor = nullptr;

void key1Click() {
  TestRun::reset();
  result_cursor = nullptr;
  Hardware::setStatus1("READY");
  Hardware::setStatus2("");
  setStatusLed(0xFF);
}

void key2Click() {
  TestRun* r = TestRun::recent();
  if (r->finalized()) {
    r->printResults();
  }
}

void key3Click() {
  TestRun* r = TestRun::recent();
  if (r->finalized()) {
    r->printResults(true);
  }
}

void key4Click() {
  TestRun* r = TestRun::recent();
  if (!r->finalized()) {
    r->startAsync();
    result_cursor = nullptr;
  } else {
    if (result_cursor == nullptr) {
      result_cursor = r->first();
    } else {
      result_cursor = result_cursor->next;
    }

    if (result_cursor != nullptr) {
      char buf[20];
      dtostrf(result_cursor->result->value, 1, 1, buf);
      for (int i = strlen(buf); i < 16 - 4; i++) {
        buf[i] = ' ';
        buf[i+1] = '\0';
      }
      strlcat(buf, result_cursor->result->pass ? "PASS" : "FAIL", 20);
      Hardware::setStatus1(result_cursor->result->message);
      Hardware::setStatus2(buf);
    }
  }
}

/**
 * Set Keys, left to right:
 * 0bRGRGRGRG
 */
void setKeypadLed(byte colors) {
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(LSBFIRST);
  digitalWrite(KEY_LED_EN_PIN, LOW);
  SPI.transfer(colors);
  digitalWrite(KEY_LED_EN_PIN, HIGH);
}

void animateKeypad() {
  for (int i = 0; i < 255; i += 16) {
    setKeypadLed(i % 255);
    delay(100);
  }
}

/**
 * Set Status LED:
 * 0b000000RG
 */
void setStatusLed(byte colors) {
  digitalWrite(STATUS_R_PIN, colors & 2);
  digitalWrite(STATUS_G_PIN, colors & 1);
}

void initIo1() {
  IO1.pinMode(IO1_VSE_12V, OUTPUT);
  IO1.digitalWrite(IO1_VSE_12V, LOW);
  IO1.pinMode(IO1_EN_12V, OUTPUT);
  IO1.digitalWrite(IO1_EN_12V, LOW);
  IO1.pinMode(IO1_VSE_5V, OUTPUT);
  IO1.digitalWrite(IO1_VSE_5V, LOW);
  IO1.pinMode(IO1_VSE_3V3, OUTPUT);
  IO1.digitalWrite(IO1_VSE_3V3, LOW);
}

byte count = 0;
byte lastKey = 0;


void initIo2() {

}

float read12V() {
  float av = analogRead(VS_12V_PIN);
  float vcc = av * (15.0 / 1023.0);
  return vcc;
}

void setup() {
  Serial.begin(9600);
  ANSI::init();

  Serial.println("Configuring I/O...");
  pinMode(KEY_LED_EN_PIN, OUTPUT);
  digitalWrite(KEY_LED_EN_PIN, HIGH);

  pinMode(STATUS_R_PIN, OUTPUT);
  pinMode(STATUS_G_PIN, OUTPUT);
  setStatusLed(0xFF);

  pinMode(CURR_SENSE_PIN, INPUT);
  pinMode(VS_12V_PIN, INPUT);
  pinMode(VS_RHIGH_PIN, INPUT);
  pinMode(VS_12V_PIN, INPUT);
  pinMode(VS_5V_PIN, INPUT);
  pinMode(VS_3V3_PIN, INPUT);
  pinMode(TEST_V_PRES_PIN, INPUT);
  pinMode(VS_PRES_PIN, INPUT);

  Serial.println("Attaching hardware handlers...");
  Key1.attachClick(key1Click);
  Key2.attachClick(key2Click);
  Key3.attachClick(key3Click);
  Key4.attachClick(key4Click);

  Serial.println("Initializing I/O expanders...");
  // Compiler doesn't know the difference between int literal and pointer,
  // and will complain about ambiguity here. Cast to uint8_t to hint at the
  // correct override.
  IO1.begin((uint8_t) 0x01);
  initIo1();

  IO2.begin((uint8_t) 0x00);
  initIo2();

  LCD.begin(16, 2);

  Serial.println("Initializing SPI peripherals...");
  // This is for the LED keypad
  SPI.begin();

  Serial.println("Initializing LCD...");
  // Boot Sequence
  LCD.setBacklight(HIGH);
  LCD.print("Starting...");

  Serial.print("Initializing SD...");
  if (SD.begin(SD_CS_PIN)) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }

  Serial.println("Initializing test run.");
  TestRun::init();

  // Initialize TermUI
  TermUI::init();
  ANSI::logln("READY.");
  Hardware::setStatus1("READY");

  setKeypadLed(0);
}

long lastTick = 0;
float reading = 0.0;

void loop() {
  Key1.tick();
  Key2.tick();
  Key3.tick();
  Key4.tick();

  if (millis() - lastTick > 300) {
    TermUI::drawAnalogReadings();
    lastTick = millis();
  }

  Console::tick();
  TestRun::tick();
}