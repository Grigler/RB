#include "gtest/gtest.h"

#include "rb/RB.h"

//Particularly unsafe - but only a compile-time issue so easy to catch
#define ASSERT_FLOAT_VEC3_EQ(T,A){\
ASSERT_FLOAT_EQ(T.x,A.x);\
ASSERT_FLOAT_EQ(T.y,A.y);\
ASSERT_FLOAT_EQ(T.z,A.z);}
#define EXPECT_FLOAT_VEC3_EQ(T,A){\
EXPECT_FLOAT_EQ(T.x,A.x);\
EXPECT_FLOAT_EQ(T.y,A.y);\
EXPECT_FLOAT_EQ(T.z,A.z);}

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
TEST(WorldBehaviour, WorldTick)
{
  RB::World w;
  w.AddBody();
  ASSERT_NO_THROW(w.Tick());
}

//World Limits
TEST(WorldLimits, LargeBodyCount)
{
  RB::World w;
  //Need to define a better number for this
  //1 million is too arbitrary
  const size_t max = 1000000;
  for(size_t i = 0; i < max; i++)
  {
    ASSERT_NO_THROW(w.AddBody());
    if(i % 100000 == 0) printf("\rBody: %i", i);
  }
  printf("\rBody: %i\r", max);
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
TEST(BodyBehaviour, ParamCtor)
{
  RB::World w;
  std::weak_ptr<RB::Body> b = w.AddBody(glm::vec3(1.0f), glm::vec3(1.0f));
  EXPECT_FALSE(b.expired());
  ASSERT_EQ(b.lock()->getPosition4(), glm::vec4(1.0f));
}

TEST(BodyBehaviour, applyForceImpulse)
{
  RB::Body b;
  glm::vec3 f = glm::vec3(1.0f);
  b.applyForceImpulse(f);
  ASSERT_FLOAT_VEC3_EQ(b.accumulatedForce, f);
}
TEST(BodyBehaviour, applyForceImpulseLocation)
{
  RB::Body b;
  glm::vec3 f = glm::vec3(1.0f, 0.0f, 1.0f);
  glm::vec3 ll = glm::vec3(0.0f, 1.0f, 1.0f);
  b.applyForceImpulseAtLocation(f, ll);
  ASSERT_FLOAT_VEC3_EQ(b.accumulatedForce, f);
  //Should result in a torque of 
  ASSERT_FLOAT_VEC3_EQ(b.accumulatedTorque, glm::cross(ll, f));
}
TEST(BodyBehaviour, applyTorqueImpulse)
{
  RB::Body b;
  glm::vec3 t = glm::vec3(1.0f);
  b.applyTorqueImpulse(t);
  ASSERT_FLOAT_VEC3_EQ(b.accumulatedTorque, t);
}
//Body Limits
TEST(BodyLimits, applyLargeAndSmallTorque)
{
  RB::Body b;
  //TODO - replace with non-arbitrary numbers
  glm::vec3 bigF = glm::vec3(9999999.0f);
  glm::vec3 smallF = glm::vec3(0.0000002f);
  b.applyForceImpulse(bigF);
  EXPECT_FLOAT_VEC3_EQ(b.accumulatedForce, bigF);
  b.applyForceImpulse(smallF);
  ASSERT_FLOAT_VEC3_EQ(b.accumulatedForce, glm::vec3(9999999.0000002f));
}


//AABB Behaviour
TEST(aabbBehaviour, PassCaseGeneric)
{
  RB::AABB l(glm::vec3(-1.0f), glm::vec3(1.0f));
  RB::AABB r(glm::vec3(-0.5f), glm::vec3(0.5f));
  EXPECT_TRUE(RB::AABB::Check(l, r));
  ASSERT_TRUE(l.CheckAgainst(r));

}
TEST(aabbBehaviour, FailCaseGeneric)
{
  RB::AABB l(glm::vec3(-100.0f), glm::vec3(-99.0f));
  RB::AABB r(glm::vec3(99.0f), glm::vec3(100.0f));
  ASSERT_FALSE(RB::AABB::Check(l, r));
}
TEST(aabbBehaviour, IdenticalAABB)
{
  //Declaring two identical AABBs and testing
  RB::AABB i(glm::vec3(-1.0f), glm::vec3(1.0f));
  ASSERT_TRUE(RB::AABB::Check(i, i));
}
TEST(aabbBehaviour, CloselyPassingAABB)
{
  //Defining two AABBs that are passing within
  //some arbitrarily close distance (0.0...0001f)
  RB::AABB l(glm::vec3(0.0f), glm::vec3(1.0f));
  RB::AABB r(glm::vec3(1.0f + glm::epsilon<float>()), glm::vec3(2.0f));
  ASSERT_FALSE(RB::AABB::Check(l, r));
}
TEST(aabbBehaviour, OrientationUpdate)
{
  //1x2x1 box oriented 90 degrees to make a 1x2x3
  RB::AABB a(glm::vec3(0.0f), glm::vec3(1.0f, 2.0f, 3.0f));
  
  glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f) , glm::vec3(0, 0, 1));
  //Update
  ASSERT_NO_THROW(a.Update(r));
  //Ensuring new box bounds are 2x-1x3
  EXPECT_FLOAT_VEC3_EQ(a.getWorldMin(), glm::vec3(0.0f, -1.0f, 0.0f));
  EXPECT_FLOAT_VEC3_EQ(a.getWorldMax(), glm::vec3(2.0f, 0.0f, 3.0f));
  //Ensuring locals are identical
  EXPECT_FLOAT_VEC3_EQ(a.getLocalMin(), glm::vec3(0.0f));
  EXPECT_FLOAT_VEC3_EQ(a.getLocalMax(), glm::vec3(1.0f, 2.0f, 3.0f));
}

//AABB Limits
TEST(aabbLimits, PassCaseFarFromOrigin)
{
  RB::AABB l(glm::vec3(500000.0f), glm::vec3(500001.0f));
  RB::AABB r(glm::vec3(500000.5f), glm::vec3(500001.5f));
  ASSERT_TRUE(RB::AABB::Check(l, r));
}
TEST(aabbLimits, FailCaseFarFromOrigin)
{
  RB::AABB l(glm::vec3(500000.0f), glm::vec3(500001.0f));
  RB::AABB r(glm::vec3(500002.0f), glm::vec3(500003.0f));
  ASSERT_FALSE(RB::AABB::Check(l, r));
}
TEST(aabbLimits, PassCaseHighRelativeDistance)
{
  RB::AABB l(glm::vec3(-5000000.0f), glm::vec3(6000000.0f));
  RB::AABB r(glm::vec3(5000000.0f), glm::vec3(5000001.0f));
  ASSERT_TRUE(RB::AABB::Check(l, r));
}
TEST(aabbLimits, FailCaseHighRelativeDistance)
{
  RB::AABB l(glm::vec3(-5000000.0f), glm::vec3(-4999999.0f));
  RB::AABB r(glm::vec3(5000000.0f), glm::vec3(5000001.0f));
  ASSERT_FALSE(RB::AABB::Check(l, r));
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


TEST(IntegratorFactory, NoIntegratorOnInit)
{
  ASSERT_EQ(RB::IntegratorFactory::getGlobalFunction(), nullptr);
}
TEST(IntegratorFactory, ClearIntegratorOnBadAssign)
{
  EXPECT_NO_THROW(RB::IntegratorFactory::setGlobal("Not A Function At All"));
  ASSERT_EQ(RB::IntegratorFactory::getGlobalFunction(), nullptr);
}
TEST(IntegratorFactory, AssignAndCallGlobalFunc)
{
  ASSERT_NO_THROW
  (
    RB::ForwardEuler::registerFunc();
    RB::IntegratorFactory::setGlobal(RB::Integrators::ForwardEuler);
  );
  std::shared_ptr<RB::Body> b = std::make_shared<RB::Body>();
  ASSERT_NO_THROW(RB::IntegratorFactory::getGlobalFunction()(b,0.008f));
}

TEST(ForwardEuler, RegisterCallUnregister)
{
  ASSERT_NO_THROW(RB::ForwardEuler::registerFunc());
  std::shared_ptr<RB::Body> b = std::make_shared<RB::Body>();
  ASSERT_NO_THROW(RB::IntegratorFactory::getFunction(RB::Integrators::ForwardEuler)(b,0.008f));
  ASSERT_NO_THROW(RB::ForwardEuler::unregisterFunc());
  ASSERT_EQ(RB::IntegratorFactory::getFunction(RB::Integrators::ForwardEuler), nullptr);
}
TEST(SemiImplicitEuler, RegisterCallUnregister)
{
  ASSERT_NO_THROW(RB::SemiImplicitEuler::registerFunc());
  std::shared_ptr<RB::Body> b = std::make_shared<RB::Body>();
  ASSERT_NO_THROW(RB::IntegratorFactory::getFunction(RB::Integrators::SemiImplicitEuler)(b, 0.008f));
  ASSERT_NO_THROW(RB::SemiImplicitEuler::unregisterFunc());
  ASSERT_EQ(RB::IntegratorFactory::getFunction(RB::Integrators::SemiImplicitEuler), nullptr);
}
TEST(RK4, RegisterCallUnregister)
{
  ASSERT_NO_THROW(RB::RK4::registerFunc());
  std::shared_ptr<RB::Body> b = std::make_shared<RB::Body>();
  ASSERT_NO_THROW(RB::IntegratorFactory::getFunction(RB::Integrators::RK4)(b,0.008f));
  ASSERT_NO_THROW(RB::RK4::unregisterFunc());
  ASSERT_EQ(RB::IntegratorFactory::getFunction(RB::Integrators::RK4), nullptr);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}