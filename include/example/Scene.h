#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include <memory>
#include <glm/vec3.hpp>

#include <ngl/Singleton.h>

namespace RB { class World; }

class Object;

class Scene : public ngl::Singleton<Scene>
{
public:
  
  Scene();
  ~Scene();

  void Update();
  void Draw();
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
  template<class T>
  std::weak_ptr<T> AddObject(glm::vec3 _pos = { 0,0,0 }, bool isRB = false);

  static std::unique_ptr<RB::World> world;

private:
  //Using a list instead of vector as it would cause a segmentation fault when
  //the number of objects exceded 255 (presumably an issue in resizing to a 
  //large contiguous memory block)
  std::list<std::shared_ptr<Object>> objects;
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