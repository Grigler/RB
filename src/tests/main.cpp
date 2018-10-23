#include "gtest/gtest.h"

#include <RB.h>

TEST(World, AddBody)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  ASSERT_FALSE(b.expired());
}
TEST(World, KillAllBodies)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  EXPECT_FALSE(b.expired());
  w.Kill();
  ASSERT_TRUE(b.expired());
}

TEST(Body, DefaultAtOrigin)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  EXPECT_FALSE(b.expired());
  ASSERT_EQ(b.lock()->getPosition4(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
