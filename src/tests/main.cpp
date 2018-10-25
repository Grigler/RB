#include "gtest/gtest.h"

#include "rb/RB.h"

//World Behaviour
TEST(WorldBehaviour, AddSingleBodyGetValidPtr)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  ASSERT_FALSE(b.expired());
}
TEST(WorldBehaviour, WorldKillResultsInInvalidBodyPtr)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  EXPECT_FALSE(b.expired());
  w.Kill();
  ASSERT_TRUE(b.expired());
}

//World Limits
TEST(WorldLimits, LargeBodyCount)
{
  RB::World w;
  //Need to define a better number for this
  //1 million is too arbitrary
  for(size_t i = 0; i < 1000000; i++)
  {
    ASSERT_NO_THROW(w.AddBody());
  }
  w.Kill();
}

//Body Behaviour
TEST(BodyBehaviour, DefaultAtOrigin)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody();
  EXPECT_FALSE(b.expired());
  ASSERT_EQ(b.lock()->getPosition4(), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

//Body Limits

//AABB Behaviour
TEST(aabbBehaviour, PassCaseGeneric)
{
  //Defining two AABBs within reasonable numeric distance
  //that _should_ pass (i.e., within 1 unit and a size of 2)
}
TEST(aabbBehaviour, FailCaseGeneric)
{
  //Defining two AABBs at distance from eachother
}
TEST(aabbBehaviour, IdenticalAABB)
{
  //Declaring two identical AABBs and testing
}
TEST(aabbBehaviour, CloselyPassingAABB)
{
  //Defining two AABBs that are passing within
  //some arbitrarily close distance (0.0...0001f)
}
TEST(aabbBehaviour, IdenticalAfterFullRotations)
{
  //Define AABB

  //Rotate 4 times by 90 degrees
  for(size_t i = 0; i < 4; i++)
  {
    //Rotate
  }
  //Assert that size is approximately the same (within epsilon)

}

//AABB Limits
TEST(aabbLimits, PassCaseAtHigh)
{

}
TEST(aabbLimits, FailCaseAtHigh)
{

}
TEST(aabbLimits, PassCaseAtHighNeg)
{

}
TEST(aabbLimits, FailCaseAtHighNeg)
{

}
TEST(aabbLimits, PassCaseHighRelativeDistance)
{

}
TEST(aabbLimits, FailCaseHighRelativeDistance)
{

}

//BVH Behaviour
TEST(bvhBehaviour, ClearTreeResultsInNoBodyDataLoss)
{
  //Adding large amounts of bodies
  
  //Clearing the bvh

  //Checking for invalidated ptrs in bodies
}
TEST(bvhBehaviour, KillingWorldKillsBVH)
{
  //Adding large amounts of bodies

  //Kill world

  //Ensure invalidity of bvh
}
TEST(bvhBehaviour, aabbPassCaseTestInBVH)
{
  //Adding aabbs to bvh (sparse env)

  //Checking known collision between low-down in tree
}
TEST(bvhBehaviour, aabbFailCaseTestInBVH)
{
  //Adding aabbs to bvh(sparse env)

  //Checking known non-collision between low-down in tree
}
TEST(bvhBehaviour, BodySelfTest)
{
  //Create body

  //Force test between body AABB in the tree
}

//BVH Limits
TEST(bvhLimits, LargeNumberTest)
{
  //Adding large numbers to bvh and testing for non-crash
}
TEST(bvhLimits, NoBodyTraversal)
{
  //Adding no bodies to bvh and attempting to traverse
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
