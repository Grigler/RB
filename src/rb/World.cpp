#include <World.h>

#include <BVH.h>
#include <Body.h>

using namespace RB;

std::unique_ptr<BVH> World::bvh;

World::World()
{
  bvh = std::make_unique<BVH>();
}
World::~World()
{
  bvh.reset();
}

//TODO
void World::Tick()
{
  for (auto i = bodies.begin(); i != bodies.end(); i++)
  {
    //i->integrate
  }

  //Broadphase

  //Narrowphase

  //Collision solving

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