#include "../include/ANSI.h"

namespace ANSI {
  void escape(String code) {Serial.print("\033["); Serial.print(code); }
  void escape(const char *code) { Serial.print("\033["); Serial.print(code); }
  void invert() { escape("7m"); }
  void reset() { escape("m"); }
  void home() { escape("2J"); escape("H"); }
  void setCursor(int col, int row) { escape(String(row) + ";" + String(col) + "H"); }
  void setCursor(int col) { escape(String(col) + "F"); }
  void setScrollRegion(int start = 0, int end = SCREEN_ROWS) { escape(String(start) + ";" + String(end) + "r"); }
  void altChar(char code) { Serial.print("\033(0"); Serial.print(code); Serial.print("\033(B"); }
  void save() { Serial.print("\0337"); }
  void ret() { Serial.print("\0338"); }
  void putChar(char code) { Serial.print(code); }

  void boxCharVT100(BoxChar c) {
    switch(c) {
      case TopLeft:
        altChar('\x6C');
        break;
      case TopRight:
        altChar('\x6B');
        break;
      case BottomLeft:
        altChar('\x6D');
        break;
      case BottomRight:
        altChar('\x6A');
        break;
      case Cross:
        altChar('\x6E');
        break;
      case Horizontal:
        altChar('\x71');
        break;
      case Vertical:
        altChar('\x78');
        break;
      case LeftTee:
        altChar('\x74');
        break;
      case RightTee:
        altChar('\x75');
        break;
      case TopTee:
        altChar('\x77');
        break;
      case BottomTee:
        altChar('\x76');
        break;
      default:
        altChar('\x60');
    }
  }

  void boxCharAltosIII(BoxChar c) {
    switch(c) {
      case TopLeft:
        altChar('\x4D');
        break;
      case TopRight:
        altChar('\x4C');
        break;
      case BottomLeft:
        altChar('\x4E');
        break;
      case BottomRight:
        altChar('\x4B');
        break;
      case Cross:
        altChar('\x4F');
        break;
      case Horizontal:
        altChar('\x52');
        break;
      case Vertical:
        altChar('\x59');
        break;
      case LeftTee:
        altChar('\x55');
        break;
      case RightTee:
        altChar('\x56');
        break;
      case TopTee:
        altChar('\x58');
        break;
      case BottomTee:
        altChar('\x57');
        break;
      default:
        altChar('\x42');
    }
  }

  void boxChar(BoxChar c) {
    return boxCharAltosIII(c);
    return boxCharVT100(c);
  }

  void init() {
    escape('z');
    home();
  }

  void clearLine() {
    Serial.print("\r\033T");
  }

  // Print Commands
  void printCenter(const char *str, int startCol = 1, int endCol = SCREEN_COLS) {
    int len = strlen(str);
    int width = endCol - startCol + 1;
    int space_pre = (width - len) / 2;
    int space_post = width - space_pre - len;

    setCursor(startCol);
    for (int i = 0; i < space_pre; i++) {
      Serial.print(" ");
    }

    Serial.print(str);

    for (int i = 0; i < space_post; i++) {
      Serial.print(" ");
    }
  }

  void printLeft(const char *str, int minWidth = 0) {
    int len = strlen(str);
    int space_post = minWidth - len;

    Serial.print(str);
    for (int i = 0; i < space_post; i++) {
      Serial.print(" ");
    }
  }

  void printRight(const char *str, int minWidth = 0) {
    int len = strlen(str);
    int space_pre = minWidth - len;

    for (int i = 0; i < space_pre; i++) {
      Serial.print(" ");
    }

    Serial.print(str);
  }

  void drawHLine(int row, int startCol = 1, int endCol = SCREEN_COLS) {
    setCursor(startCol, row);
    for (int i = startCol; i <= endCol; i++) {
      boxChar(Horizontal);
    }
  }

  void drawVLine(int col, int startRow = 1, int endRow = SCREEN_ROWS) {
    for (int i = startRow; i <= endRow; i++) {
      setCursor(col, i);
      boxChar(Vertical);
    }
  }

  void drawBox(int x, int y, int width, int height, const char *title = "") {
    const int titleIndent = 3;

    int titleLen = strlen(title);
    // Top Row
    setCursor(x, y);
    boxChar(TopLeft);
    for (int i = 1; i < titleIndent; i++)
      boxChar(Horizontal);
    if (titleLen > 0)
      Serial.print(title);
    drawHLine(y, x+titleIndent+titleLen, x+width-1);
    boxChar(TopRight);

    // Bottom Row
    setCursor(x, y+height);
    boxChar(BottomLeft);
    drawHLine(y+height, x+1, x+width-1);
    boxChar(BottomRight);

    // VLines
    drawVLine(x, y+1, y+height-1);
    drawVLine(x+width, y+1, y+height-1);
  }
}
