#include "formula.hpp"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  math::Formula formula;
  return formula.identity(0) == 0;
}
