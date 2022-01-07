#include "TermUI.h"
#include "TestRun.h"
#include "Console.h"

namespace TermUI {
  void drawStatus() {
    ANSI::drawBox(1, 2, COL_BREAK - 2, 3, "Status");
    ANSI::ret();
  }

  void drawReadings() {
    ANSI::drawBox(1, 6, COL_BREAK - 2, 8, "Readings");
    ANSI::setCursor(3, 7);
    Serial.println("CURR:");
    ANSI::setCursor(3, 8);
    Serial.println("RES:");
    ANSI::setCursor(3, 9);
    Serial.println("12V:");
    ANSI::setCursor(3, 10);
    Serial.println("5V:");
    ANSI::setCursor(3, 11);
    Serial.println("3V3:");
    ANSI::setCursor(3, 12);
    Serial.println("VPRES:");
    ANSI::setCursor(3, 13);
    Serial.println("PVSS:");
    ANSI::ret();
  }

  void drawConsole() {
    ANSI::drawHLine(SCREEN_ROWS - TERM_ROWS - 1, 1, 2);
    Serial.print("Console");
    ANSI::drawHLine(SCREEN_ROWS - TERM_ROWS - 1, 10);
    ANSI::setScrollRegion(SCREEN_ROWS - TERM_ROWS, SCREEN_ROWS - 2);
    ANSI::setCursor(0, SCREEN_ROWS - 1);
    Serial.print("> ");
    ANSI::save();
  }

  void drawFnKeys() {
    ANSI::setCursor(0, SCREEN_ROWS);
    ANSI::invert();
    Serial.print(" F1 - STOP     ");
    ANSI::boxChar(ANSI::Vertical);
    Serial.print(" F2 - PRINT    ");
    ANSI::boxChar(ANSI::Vertical);
    Serial.print(" F3 - MENU     ");
    ANSI::boxChar(ANSI::Vertical);
    Serial.print(" F4 - RUN      ");
    ANSI::boxChar(ANSI::Vertical);
    Serial.print(" F5 - READVALS  ");
    ANSI::reset();
    ANSI::ret();
  }

  void drawAnalogReadings() {
    char buf[10];

    ANSI::setCursor(COL_BREAK - 10 - 2, 7);
    dtostrf(analogRead(A0), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 8);
    dtostrf(VoltageSense::measureResistance(), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 9);
    dtostrf(VoltageSense::read12V(), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 10);
    dtostrf(VoltageSense::read5V(), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 11);
    dtostrf(VoltageSense::read3V3(), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 12);
    dtostrf(VoltageSense::readVcc(A5), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::setCursor(COL_BREAK - 10 - 2, 13);
    dtostrf(VoltageSense::readVcc(A6), 1, 1, buf);
    ANSI::printRight(buf, 10);
    ANSI::ret();
  }

  void drawHeader() {
    ANSI::setCursor(1,1);
    ANSI::invert();
    ANSI::printCenter("**** MAUS-TEC ELECTRONICS TEST STATION ****");
    ANSI::reset();
    ANSI::ret();
  }

  void drawTestContainer() {
    ANSI::drawBox(COL_BREAK + 1, 2, SCREEN_COLS - COL_BREAK - 1, SCREEN_ROWS - TERM_ROWS - 4, "Tests");
    ANSI::ret();
  }

  void drawTests(TestResultNode *node) {
    int n = 0;
    TestRun *run = TestRun::recent();

    if (run != nullptr) {
      TestResultNode *p = run->first();
      while (p != nullptr) {
        if (node == nullptr || node->test == p->test) {
          if (node == nullptr) {
            ANSI::setCursor(COL_BREAK + 3, 3 + n);
            ANSI::printLeft(p->test->getName(), 16);
            Serial.print(" | ");
          } else {
            ANSI::setCursor(COL_BREAK + 22, 3 + n);
          }

          if (p->result == nullptr) {
            ANSI::printRight("--", 36);
          } else {
            char value[10];
            dtostrf(p->result->value, 1, 1, value);

            ANSI::printLeft(p->result->message, 20);
            Serial.print(" | ");
            ANSI::printLeft(value, 8);
            Serial.print(" | ");

            if (p->result->pass)
              ANSI::printRight("OK", 2);
            else
              ANSI::printRight("F!", 2);
          }
        }

        p = p->next;
        n++;
      }
    }

    ANSI::ret();
  }

  void init() {
    ANSI::home();
    drawHeader();
    drawStatus();
    drawReadings();
    drawTestContainer();
    drawConsole();
    drawFnKeys();
    drawTests();
  }
}