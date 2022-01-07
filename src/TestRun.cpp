#include "TestRun.h"
#include "Test.h"
#include "TermUI.h"
#include "Hardware.h"

#include "citizen-printer/CitizenPrinter.hpp"

CitizenPrinter printer = CitizenPrinter(VSER_RXD_PIN, VSER_TXD_PIN, VSER_CTS_PIN, 0);

TestRun *TestRun::init() {
  printer.begin();

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

void TestRun::reset() {
  if (p_recent != nullptr) {
    TestRun *run = p_recent;
    TestResultNode *n = p_recent->first();
    while (n != nullptr) {
      TestResultNode *next = n->next;
      if (n->result != nullptr) {
        delete(n->result);
      }
      delete (n);
      n = next;
    }

    delete(run);
    run = new TestRun();
    TestCase *p = TestCase::first();

    while (p != nullptr) {
      run->addTestResult(nullptr, p);
      p = p->next();
    }

    TestRun::p_recent = run;
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

bool TestRun::finalized() {
  return is_finalized;
}

void TestRun::finalize() {
  char msg[18];
  char val[9];
  is_finalized = true;
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

void TestRun::printResults(bool shortForm) {
  char lineBuffer[41];

  ANSI::logln("Printing test results...");
  printer.printCenter("============= TEST RESULTS =============");

  if (fail_count > 0) {
    printer.red();
  }
  printer.printTable("Result:", fail_count > 0 ? "FAIL" : "PASS");
  printer.printTable("Serial No:", "N/A");
  ANSI::logln("    - printing table");
  printer.hr();

  TestResultNode *p = first();
  bool printedOne = false;
  while (p != nullptr) {
    if (shortForm && (p->result == nullptr || p->result->pass)) {
      p = p->next;
      continue;
    }
    printedOne = true;
    if (p->result == nullptr) {
      printer.printTable(p->test->getName(), "SKIP");
    } else {
      if (!p->result->pass) { printer.red(); printer.reverse(); }
      printer.printTable(p->test->getName(), p->result->pass ? "Pass" : "FAIL");
      if (!p->result->pass) printer.red();
      dtostrf(p->result->value, 1, 1, lineBuffer);
      printer.printTable(p->result->message, lineBuffer);
    }

    p = p->next;
  }

  ANSI::logln("    - post-table summary");
  if (printedOne) printer.hr();
  itoa(pass_count, lineBuffer, 10);
  printer.printTable("Pass:", lineBuffer);
  itoa(fail_count, lineBuffer, 10);
  printer.red();
  printer.printTable("Fail:", lineBuffer);

  ANSI::logln("    - ejecting sheet yeet");
  printer.println();
  printer.println();
  printer.println();
  printer.println();
  printer.println();
  printer.println();
  printer.println();
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