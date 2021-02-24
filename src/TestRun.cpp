#include "../include/TestRun.h"
#include "../include/Test.h"
#include "../include/TermUI.h"
#include "../include/Hardware.h"

TestRun *TestRun::init() {
  TestRun *run = new TestRun();
  TestCase *p = TestCase::first();

  while (p != nullptr) {
    run->addTestResult(nullptr, p);
    p = p->next();
  }

  TestRun::p_recent = run;
  return run;
}

TestRun *TestRun::recent() {
  return p_recent;
}

void TestRun::tick() {
  if (p_recent) {
    p_recent->executeNext();
  }
}

void TestRun::execute() {
  TestResultNode *n = p_first;
  Hardware::setStatus1("Testing...");
  while (n != nullptr) {
    Hardware::setStatus2(n->test->getName());
    TestResult *result = n->test->execute();

    if (n->result != nullptr)
      free(n->result);
    n->result = result;

    TermUI::drawTests(n);
    n = n->next;
  }
}

void TestRun::startAsync() {
  pass_count = 0;
  fail_count = 0;
  p_current = p_first;
  ANSI::logln("== Starting Test Run ==");
  Hardware::setStatus1("Testing...");
}

void TestRun::executeNext() {
  TestResultNode *n = p_current;
  if (n == nullptr) {
    return;
  }

  ANSI::log("Test: ");
  ANSI::logln(n->test->getName());
  Hardware::setStatus2(n->test->getName());
  TestResult *result = n->test->execute();

  ANSI::log("\t");
  ANSI::log(result->message);
  ANSI::log("\t");
  char buf[10];
  dtostrf(result->value, 1, 1, buf);
  ANSI::log(buf);
  ANSI::log("\t");
  ANSI::logln(result->pass ? "PASS" : "FAIL");

  if (n->result != nullptr)
    free(n->result);

  n->result = result;

  if (result->pass) {
    pass_count++;
  } else {
    fail_count++;
  }

  TermUI::drawTests(n);
  p_current = n->next;

  if (p_current == nullptr) {
    finalize();
  }
}

extern void setStatusLed(byte);

void TestRun::finalize() {
  char msg[18];
  char val[9];
  strcpy(msg, "Pass: ");
  itoa(pass_count, val, 10);
  strcat(msg, val);
  Hardware::setStatus1(msg);
  strcpy(msg, "FAIL: ");
  itoa(fail_count, val, 10);
  strcat(msg, val);
  Hardware::setStatus2(msg);

  if (fail_count > 0) {
    setStatusLed(0x1);
  } else {
    setStatusLed(0x2);
  }
}

void TestRun::addTestResult(TestResult *result, TestCase *test) {
  TestResultNode *n = new TestResultNode();
  n->result = result;
  n->test = test;
  n->prev = p_last;
  n->next = nullptr;

  if (p_first == nullptr) {
    p_first = n;
  }
  if (p_last != nullptr) {
    p_last->next = n;
  }
  p_last = n;
}

void TestRun::updateTestResult(TestResult *result, TestCase *test) {
  TestResultNode *n = p_first;
  while (n != nullptr) {
    if (n->test == test) {
      if (n->result != nullptr)
        free(n->result);
      n->result = result;
      return;
    }
    n = n->next;
  }

  addTestResult(result, test);
}

TestResultNode *TestRun::first() {
  return p_first;
}