#ifndef __term_ui_h
#define __term_ui_h

#include "ANSI.h"
#include "VoltageSense.h"
#include "TestRun.h"

#define COL_BREAK 21

namespace TermUI {
  void drawStatus();

  void drawReadings();

  void drawConsole();

  void drawFnKeys();

  void drawAnalogReadings();

  void drawHeader();

  void drawTestContainer();

  void drawTests(TestResultNode *n = nullptr);

  void init();
}

#endif