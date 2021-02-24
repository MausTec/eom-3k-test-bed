#include "../include/ANSI.h"
#include "../include/Console.h"

#include <Arduino.h>

extern key1Click();
extern key2Click();
extern key3Click();
extern key4Click();

namespace Console {
  void FKey(int f) {
    switch(f) {
      case 1:
        key1Click();
        break;
      case 2:
        key2Click();
        break;
      case 3:
        key3Click();
        break;
      case 4:
        key4Click();
        break;
    }
  }

  void flushBuffer() {
    if (buffer_len > 0) {
      buffer[buffer_len] = '\0';
      ANSI::log("> ", false);
      ANSI::logln(buffer);
      buffer_len = 0;
    }

    ANSI::ret();
    ANSI::clearLine();
    Serial.print("> ");
    ANSI::save();
  }

  void keypress(unsigned char keycode) {
    if (fKey) {
      if (keycode == '\r') {
        fKey = false;
      } else {
        FKey(keycode - 0x3F);
      }
    } else if (escKey) {
      ANSI::log("^E[0x", false);
      Serial.print(keycode, HEX);
      ANSI::logln("");
      escKey = false;
    } else if (isprint(keycode)) {
      if (buffer_len < CONSOLE_BUFFER_LEN) {
        buffer[buffer_len++] = keycode;
        Serial.print((char)keycode);
        ANSI::save();
      }
    } else if(keycode == '\r') {
      flushBuffer();
    } else if(keycode == '\b') {
      if (buffer_len > 0) {
        buffer_len--;
        Serial.print("\b \b");
        ANSI::save();
      }
    } else if (keycode == 0x01) {
      fKey = true;
    } else if (keycode == 0x9B) {
      escKey = true;
    } else {
      ANSI::log("0x", false);
      Serial.print(keycode, HEX);
      ANSI::logln("");
    }
  }

  void tick() {
    if (Serial.available() > 0) {
      while (Serial.available() > 0) {
        keypress(Serial.read());
      }
    }
  }
}
