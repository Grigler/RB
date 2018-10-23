#include <World.h>

#include <Body.h>

using namespace RB;

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
  return std::weak_ptr<Body>(bodies.back());
}