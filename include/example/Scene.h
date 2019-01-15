#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>
#include <glm/vec3.hpp>


namespace RB { class World; }

class Object;

class Scene
{
public:
  
  Scene();
  ~Scene();

  void Update();
  void Draw();

  template<class T>
  std::weak_ptr<T> AddObject(glm::vec3 _pos = { 0,0,0 }, bool isRB = false);

  static std::unique_ptr<RB::World> world;

private:
  std::vector<std::shared_ptr<Object>> objects;
};


template<class T>
std::weak_ptr<T> Scene::AddObject(glm::vec3 _pos, bool isRB)
{
  static_assert(std::is_base_of<Object, T>(), "T Need to be derived of Object");
  
  std::shared_ptr<T> ret = std::make_shared<T>();

  //Sanity checked with static_assert on first line
  std::shared_ptr<Object> o = std::dynamic_pointer_cast<Object>(ret);

  if (isRB)
  {    
    o->AssignBody(world->AddBody(_pos, glm::vec3(0)));
  }

  o->transform.pos = _pos;

  objects.push_back(o);

  //Calling onCreation for the object
  ret->onCreation();
  
  return std::weak_ptr<T>(ret);
}

#endif