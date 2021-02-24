#ifndef __hardware_h
#define __hardware_h

#include <Adafruit_LiquidCrystal.h>
#include "ANSI.h"
#include "../pinout.h"

extern Adafruit_LiquidCrystal LCD;

namespace Hardware {
  void setStatus1(char *status);
  void setStatus2(char *status);
}

#endif