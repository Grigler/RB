#include "Scene.h"

#include <RB.h>
#include <glm/glm.hpp>

#include "GameClock.h"
#include "Object.h"

std::unique_ptr<RB::World> Scene::world;

Scene::Scene()
{
  world = std::make_unique<RB::World>();
}
Scene::~Scene()
{
  world.reset();
}

void Scene::Update()
{
  GameClock::UpdateDT();

  //TODO - look at the order of these two
  //Physics tick (fixed timestep - could be multiple per update)
  
  //TEMPORARY DEBUG COMMENTED OUT
  world->Tick(GameClock::dt);

  //Objects tick - one per frame
  for (auto o = objects.begin(); o != objects.end(); o++)
  {
    (*o)->Update();
  }
}
void Scene::Draw()
{
  for (auto o = objects.begin(); o != objects.end(); o++)
  {
    (*o)->Draw();
  }
}