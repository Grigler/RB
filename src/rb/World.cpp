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
  timeAccumulator += _dt;

  //Consuming frame time with multiple fixed sub-steps
  while (timeAccumulator >= fixedTimestep)
  {
    //Itegration
    for (auto i = bodies.begin(); i != bodies.end(); i++)
    {
      IntegratorFactory::getGlobalFunction()(*i, fixedTimestep);
    }

    //Broadphase


    //Narrowphase


    //Collision solving
    

    //Reducing accumulator by fixed timestep
    timeAccumulator -= fixedTimestep;
  }
}
void World::Kill()
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