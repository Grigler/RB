#include "World.h"

#include "Body.h"

using namespace RB;

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