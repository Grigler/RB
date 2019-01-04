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

  std::weak_ptr<Object> AddObject();
  std::weak_ptr<Object> AddObject(glm::vec3 _pos);

private:
  std::unique_ptr<RB::World> world;
  std::vector<std::shared_ptr<Object>> objects;
};

#endif