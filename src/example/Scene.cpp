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
  
  world->Tick(GameClock::dt);

  //Objects tick - once per frame
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