#include "gtest/gtest.h"


int foo(int _a, int _b)
{
  return _a*_b;
}

TEST(TestingTheTests, DoesNothing)
{
  EXPECT_EQ(foo(10,10),100);
  ASSERT_EQ(foo(999,0),0);
}
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
