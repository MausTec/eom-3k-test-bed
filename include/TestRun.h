#ifndef __test_run_h
#define __test_run_h

#include "Test.h"
#include "Hardware.h"

typedef struct TestResultNode {
  TestResult *result;
  TestCase *test;
  TestResultNode *next;
  TestResultNode *prev;
} TestResultNode;

class TestRun {
public:
  static TestRun *init();

  static TestRun *recent();
  static void tick();

  void execute();

  void addTestResult(TestResult *result, TestCase *test);

  void updateTestResult(TestResult *result, TestCase *test);
  void startAsync();
  void executeNext();
  void finalize();

  TestResultNode *first();
private:
  inline static TestRun *p_recent = nullptr;
  TestResultNode *p_first = nullptr;
  TestResultNode *p_last = nullptr;
  TestResultNode *p_current = nullptr;

  int pass_count = 0;
  int fail_count = 0;
};

#endif