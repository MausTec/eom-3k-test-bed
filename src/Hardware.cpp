#include <Adafruit_LiquidCrystal.h>

#include "../include/Hardware.h"
#include "../include/ANSI.h"
#include "../pinout.h"

extern Adafruit_LiquidCrystal LCD;

namespace Hardware {
  void setStatus1(char *status) {
    LCD.setCursor(0,0);
    LCD.print(status);
    for (int i = strlen(status); i < 16; i++) {
      LCD.print(' ');
    }

    ANSI::setCursor(3,3);
    ANSI::printLeft(status, 16);
    ANSI::ret();
  }

  void setStatus2(char *status) {
    LCD.setCursor(0,1);
    LCD.print(status);
    for (int i = strlen(status); i < 16; i++) {
      LCD.print(' ');
    }

    ANSI::setCursor(3,4);
    ANSI::printLeft(status, 16);
    ANSI::ret();
  }
}