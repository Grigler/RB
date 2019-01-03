#ifndef WORLD_H_
#define WORLD_H_

#include <memory>
#include <list>

#include "glm/glm.hpp"

namespace RB
{
  class Body;
  class BVH;

  class World
  {
  public:
    World();
    ~World();

    void Tick(float _dt);
    void Kill();
    
    std::weak_ptr<Body> AddBody();
    std::weak_ptr<Body> AddBody(glm::vec3 _p, glm::vec3 _o);

    static std::unique_ptr<BVH> bvh;

  private:
    std::list< std::shared_ptr<Body> > bodies;

    static float timeAccumulator;
    static float fixedTimestep;
  };

}

#endif