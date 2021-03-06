#include <World.h>

#include <BVH.h>
#include <Body.h>

#include <IntegratorFactory.h>
#include <ForwardEuler.h>
#include <SemiImplicitEuler.h>

#include <Constraint.h>

#include <LCPFactory.h>
#include <SISolver.h>

#include <GreedyCollider.h>

#include <chrono>

using namespace RB;

std::unique_ptr<BVH> World::bvh;

float World::timeAccumulator = 0.0f;
float World::fixedTimestep = 1.0f/120.0f;
float World::maxTimeStep = 0.25f;

World::World()
{
  bvh = std::make_unique<BVH>();

  //World registers all default integrators on creation
  IntegratorFactory::registerFunc(Integrators::ForwardEuler, ForwardEuler::integrate);
  IntegratorFactory::registerFunc(Integrators::SemiImplicitEuler, SemiImplicitEuler::integrate);

  //Sets ForwardEuler integrator as default
  IntegratorFactory::setGlobal(Integrators::ForwardEuler);

  //World registers all default LCPSolvers on creation
  LCPFactory::registerFunc(LCPSolvers::SISolver, SISolver::Solve);

  //Sets SISolver LCPSolver as default
  LCPFactory::setGlobal(LCPSolvers::SISolver);
}
World::~World()
{
  bvh.reset();
}

//TODO

void World::Tick(float _dt)
{
  //Adding delta time to static accumulator
  _dt = glm::min(_dt, maxTimeStep);
  timeAccumulator += _dt;

  //Consuming frame time with multiple fixed sub-steps
  while (timeAccumulator >= fixedTimestep)
  {
    //printf("! tick\n");
    //Itegration
    //could be parallel with openMP pretty easily - CPU intensive though
    for (auto i = bodies.begin(); i != bodies.end(); i++)
    {
      //Reset collision flag - set in broadphase
      (*i)->boundingBox->collisionFlag = false;
      //Always uses global function from factory - user can set their own
      if((*i)->invMass > 0.0f)
        IntegratorFactory::getGlobalFunction()(*i, fixedTimestep);
      //Updating bv for broadphase
      (*i)->boundingBox->Update((*i)->getModelMat());
    }

    //Broadphase
    
    //DEBUG
    //TODO - cache this vec at max size and populate each update - save alloc time
    std::vector<broadColPair> b = BroadphaseBruteForce();    

    //Narrowphase
    for (auto cp = b.begin(); cp != b.end(); cp++)
    {
      auto lCol = cp->l.lock()->collider;
      auto rCol = cp->r.lock()->collider;

      std::vector<std::shared_ptr<Constraint>> possibleCs;

      switch (lCol->type & rCol->type)
      {
      case CollisionType::SphereSphere:
        //Sphere-Sphere collision only ever returns 1 contact
        possibleCs.push_back(GreedyCollider::SphereSphere(*lCol, *rCol));
        break;
      case CollisionType::SphereOBB:
        //Sphere-OBB collision only ever returns 1 contact
        possibleCs.push_back(GreedyCollider::SphereOBB(*lCol, *rCol));
        break;
      case CollisionType::OBBOBB:
        //Add some number of constraints to possibleCs
        possibleCs = GreedyCollider::OBBOBB(*lCol, *rCol);
        break;
      }
      //Add constraints to World constraints vector
      for (auto c = possibleCs.begin(); c != possibleCs.end(); c++)
      {
        //An empty shared_ptr (i.e., use_count == 0)
        //means that there was no collision
        if ((*c).use_count()) constraints.push_back((*c));
      }
    }


    //Collision solving
    //Hand constraints ref to LCPFactory::Global
    LCPFactory::getGlobalFunction()(constraints);
    //TODO - CHECK IF THEY ARE SOLVED BEFORE CLEARING
    constraints.clear();

    //Reducing accumulator by fixed timestep
    timeAccumulator -= fixedTimestep;
  }
}
void World::KillWorld()
{
  bodies.clear();
}

std::weak_ptr<Body> World::AddBody()
{
  bodies.push_back(std::make_shared<Body>());
  bodies.back()->selfRef = bodies.back();
  return std::weak_ptr<Body>(bodies.back());
}
std::weak_ptr<Body> World::AddBody(glm::vec3 _p, glm::vec3 _o)
{
  bodies.push_back(std::make_shared<Body>(_p, _o));
  bodies.back()->selfRef = bodies.back();
  return std::weak_ptr<Body>(bodies.back());
}

void World::KillBody(std::weak_ptr<Body> _body)
{
  for (auto b = bodies.begin(); b != bodies.end(); b++)
  {
    //operator== compares the raw pointers, so this should be
    //pretty quick - although with some dereferences and locks
    if ((*b) == _body.lock())
    {
      bodies.erase(b);
      return;
    }
  }
  //TODO - Throw error for trying to remove non-existant body
}

std::vector<broadColPair> World::BroadphaseBruteForce()
{
  std::vector<broadColPair> ret;
  for (auto l = bodies.begin(); l != bodies.end(); l++)
  {
    std::shared_ptr<AABB> lbv = (*l)->boundingBox;
    for (auto r = std::next(l); r != bodies.end(); r++)
    {
      std::shared_ptr<AABB> rbv = (*r)->boundingBox;
      
      if (AABB::Check(*lbv, *rbv))
      {
        //Creating tuple of relevant bodies
        ret.push_back({(*l),(*r)});
        //Setting flags for debug rendering
        lbv->collisionFlag = true;
        rbv->collisionFlag = true;
      }
    }
  }
  return ret;
}