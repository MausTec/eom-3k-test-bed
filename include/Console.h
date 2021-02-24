#ifndef __console_h
#define __console_h

#define CONSOLE_BUFFER_LEN 80

namespace Console {
  namespace {
    char buffer[CONSOLE_BUFFER_LEN + 1] = { 0 };
    size_t buffer_len = 0;
    bool fKey = false;
    bool escKey = false;
  }

  void flushBuffer();
  void keypress(unsigned char keycode);
  void tick();
}

#endif