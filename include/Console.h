#ifndef __console_h
#define __console_h

#define CONSOLE_BUFFER_LEN 80

namespace Console {
  namespace {
    char buffer[CONSOLE_BUFFER_LEN + 1] = { 0 };
    size_t buffer_len = 0;
  }

  void keypress(char keycode) {
    if (isprint(keycode)) {
      if (buffer_len < CONSOLE_BUFFER_LEN) {
        buffer[buffer_len++] = keycode;
        Serial.print(keycode);
        ANSI::save();
      }
    } else if(keycode == '\r') {
      ANSI::clearLine();
      ANSI::save();
    } else {
      Serial.print("0x");
      Serial.print(keycode, HEX);
      Serial.print(' ');
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

#endif