#include "formula.hpp"
#include "tests_include.hpp"

TEST(blaTest, test1) {
  math::Formula formula;
  EXPECT_EQ(formula.identity(0), 0);
  EXPECT_EQ(formula.identity(10), 10);
  EXPECT_EQ(formula.identity(50), 50);
}
