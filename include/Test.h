#ifndef __test_h
#define __test_h

#include <Arduino.h>

typedef struct TestResult {
  TestResult(bool p, const char *m) : pass(p) { 
    message = (char*) malloc((sizeof(char) * strlen(m)) + 1);
    memcpy(message, m, strlen(m));
  }
  TestResult(bool p, const char *m, float v) : pass(p), value(v) { 
    message = (char*) malloc((sizeof(char) * strlen(m)) + 1);
    memcpy(message, m, strlen(m));
  }
  ~TestResult() { free(message); }
  bool pass;
  char *message;
  float value;
} TestResult;

class TestCase {
public:
  static bool reg(TestCase *tc);

  static TestCase *first() { return p_first; };
  TestCase *next() { return p_next; };
  virtual const char *getName() { return name; };
  virtual TestResult *execute();

protected:
  inline static TestCase *p_first = nullptr;
  inline static TestCase *p_last = nullptr;
  TestCase *p_next = nullptr;
  TestCase *p_prev = nullptr;

private:
  char *name;
};

#define TEST(name) \
  class TestCase_##name : public TestCase { \
    TestResult *execute(); \
    const char *getName() override { return name; }; \
    const char *name = #name; \
  }; \
  bool tc_registered_##name = TestCase::reg(new TestCase_##name()); \
  TestResult *TestCase_##name::execute()

#define PASS(message) \
  return new TestResult(true, #message)

#define FAIL(message) \
  return new TestResult(false, #message)

#define PASSF(value) \
  char message[16] = ""; \
  dtostrf(value, 1, 1, message); \
  return new TestResult(true, message);

#define FAILF(value) \
  char message[16] = ""; \
  dtostrf(value, 1, 1, message); \
  return new TestResult(false, message);

#define DEPEND(test) \
  void

#define EXPECT(test, value) \
  if (test) { \
    return new TestResult(true, #test, value); \
  } else { \
    return new TestResult(false, #test, value); \
  }


/**
 * TEST: 3v3 Voltage
 * STATUS: PASS
 * VALUE: 3.34 Volts
 *
 * ----------------------------------------
 * Resistance - 12V    >10K Ohm          OK
 * Resistance - 5V     204 Ohm           OK
 * Resistance - 3V3    1503 Ohm          OK
 * Voltage - 3V3       3.24V             OK
 * Voltage - 5V        4.96V             OK
 * Voltage - 12V       11.93V            OK
 * 
 *
 */

#endif