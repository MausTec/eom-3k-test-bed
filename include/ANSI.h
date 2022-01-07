#ifndef __ansi_h
#define __ansi_h

#include <Arduino.h>

#define SCREEN_COLS 80
#define SCREEN_ROWS 24

#define TERM_ROWS 8

namespace ANSI {
  namespace {
    size_t logcol = 0;
    bool cursorDirty = false;
  }

  enum BoxChar {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    TopTee,
    BottomTee,
    LeftTee,
    RightTee,
    Cross,
    Vertical,
    Horizontal
  };

  void escape(String code);
  void escape(const char *code);
  void invert();
  void reset();
  void home();
  void init();
  void setCursor(int col, int row);
  void setCursor(int col);
  void setScrollRegion(int start = 0, int end = SCREEN_ROWS);
  void altChar(char code);
  void save();
  void ret();

  void hideCursor();
  void showCursor();

  void log(const char *str, bool ret = true);
  void log(int i);
  void logln(const char *str);

  // Print Commands
  void printCenter(const char *str, int startCol = 1, int endCol = SCREEN_COLS);

  void printLeft(const char *str, int minWidth = 0);

  void printRight(const char *str, int minWidth = 0);

  void drawHLine(int row, int startCol = 1, int endCol = SCREEN_COLS);

  void drawVLine(int col, int startRow = 1, int endRow = SCREEN_ROWS);

  void drawBox(int x, int y, int width, int height, const char *title = "");
  void boxChar(BoxChar c);
  void clearLine();
}

#endif