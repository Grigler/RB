#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>
#include <glm/vec3.hpp>

#define STRINGIFY(A) (#A)

namespace RB { class World; }

class Object;

class Scene
{
public:
  
  Scene();
  ~Scene();

  void Update();
  void Draw();

  std::weak_ptr<Object> AddObject();
  std::weak_ptr<Object> AddObject(glm::vec3 _pos);

  template<class T>
  std::weak_ptr<T> AddObject();

  static std::unique_ptr<RB::World> world;

private:
  std::vector<std::shared_ptr<Object>> objects;
};

template<class T>
std::weak_ptr<T> Scene::AddObject()
{
  static_assert(std::is_base_of<Object, T>(), "T Needs to be derived of Object");
  std::shared_ptr<T> ret = std::make_shared<T>();
  objects.push_back(ret);
  return ret;  
}

#endif