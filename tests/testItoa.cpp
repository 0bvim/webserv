#include "../include/common.hpp"
#include <gtest/gtest.h>

TEST(ItoaTest, ZeroValue) {
  EXPECT_EQ(itoa(0), "0");
}

TEST(ItoaTest, PositiveValues) {
  EXPECT_EQ(itoa(123), "123");
  EXPECT_EQ(itoa(4567), "4567");
  EXPECT_EQ(itoa(98765), "98765");
}

TEST(ItoaTest, NegativeValues) {
  EXPECT_EQ(itoa(-123), "-123");
  EXPECT_EQ(itoa(-4567), "-4567");
  EXPECT_EQ(itoa(-98765), "-98765");
}

TEST(ItoaTest, LargeValues) {
  EXPECT_EQ(itoa(2147483647), "2147483647"); // Maximum positive int
  EXPECT_EQ(itoa(-2147483648), "-2147483648"); // Minimum negative int (implementation-dependent)
}
