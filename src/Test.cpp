#include "Test.h"

bool TestCase::reg(TestCase *tc) {
  if (p_first == nullptr) {
    p_first = tc;
  }
  if (p_last != nullptr) {
    p_last->p_next = tc;
    tc->p_prev = p_last;
  }
  p_last = tc;

  return true;
}