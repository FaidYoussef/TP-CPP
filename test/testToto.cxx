#include <gtest/gtest.h>

int Factorial(int n) {
return n <= 1 ? 1 : n * Factorial(n - 1);
}

// Test factoriel de 0.
TEST(FactorialTest, HandlesZeroInput) {
EXPECT_EQ(Factorial(0), 1);
}
// Tests factoriels de nombre positifs.
TEST(FactorialTest, HandlesPositiveInput) {
EXPECT_EQ(Factorial(1), 1);
EXPECT_EQ(Factorial(2), 2);
EXPECT_EQ(Factorial(3), 6);
EXPECT_EQ(Factorial(8), 40320);
}