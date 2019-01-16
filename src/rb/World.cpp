#include <World.h>

#include <BVH.h>
#include <Body.h>
#include <IntegratorFactory.h>

#include <ForwardEuler.h>

#include <chrono>

using namespace RB;

std::unique_ptr<BVH> World::bvh;

float World::timeAccumulator = 0.0f;
float World::fixedTimestep = 1.0f/120.0f;
float World::maxTimeStep = 0.25f;

World::World()
{
  bvh = std::make_unique<BVH>();
  //Registering and setting forward euler as a default integrator on world start-up
  IntegratorFactory::registerFunc(Integrators::ForwardEuler, ForwardEuler::integrate);
  IntegratorFactory::setGlobal(Integrators::ForwardEuler);
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
    for (auto i = bodies.begin(); i != bodies.end(); i++)
    {
      //Reset collision flag - set in broadphase
      (*i)->boundingBox->collisionFlag = false;
      IntegratorFactory::getGlobalFunction()(*i, fixedTimestep);
      //Updating bv for broadphase
      (*i)->boundingBox->Update((*i)->getModelMat());
    }

    //Broadphase
    
    //DEBUG
    std::vector<broadColPair> b = BroadphaseBruteForce();    

    //Narrowphase


    //Collision solving
    

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