#include "pinout.h"
#include "keypad.h"

#include "include/Test.h"
#include "include/ANSI.h"
#include "include/Hardware.h"
#include "include/TermUI.h"
#include "include/VoltageSense.h"
#include "include/TestRun.h"
#include "include/Console.h"

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

#include "tests/Electrical.cpp"
#include "tests/BridgeTests.cpp"
#include "tests/FunctionalTests.cpp"

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
  delay(1000);
  TermUI::init();
  Serial.println("READY.");
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

byte count = 0;
byte lastKey = 0;

void key1Click() {
  setKeypadLed(lastKey == 1 ? K1R : K1G);
  setStatusLed(1);
  lastKey = 1;

  Serial.println("Enabling 12V vsense relays...");
  VoltageSense::enableResistance12V();
  reading = 12;
}

void key2Click() {
  setKeypadLed(lastKey == 2 ? K2R : K2G);
  setStatusLed(2);
  lastKey = 2;

  Serial.println("Enabling 5V vsense relays...");
  VoltageSense::enableResistance5V();
  reading = 5;
}

void key3Click() {
  setKeypadLed(lastKey == 3 ? K3R : K3G);
  setStatusLed(0xFF);
  lastKey = 3;

  Serial.println("Enabling 5V5 vsense relays...");
  VoltageSense::enableResistance3V3();
  reading = 3.3;
}

void key4Click() {
  TestRun *r = TestRun::recent();
  r->startAsync();
}

void animateKeypad() {
  for (int i = 0 ; i < 255 ; i += 16) {
    setKeypadLed(i % 255);
    delay(100);
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

void initIo2() {
  
}

float read12V() {
  float av = analogRead(VS_12V_PIN);
  float vcc = av * (15.0 / 1023.0);
  return vcc;
}
