#ifndef WORLD_H_
#define WORLD_H_

#include <memory>
#include <list>

#include "glm/glm.hpp"

namespace RB
{
  class Body;

  class World
  {
  public:
    void Tick();
    void Kill();
    
    std::weak_ptr<Body> AddBody();
    std::weak_ptr<Body> AddBody(glm::vec3 _p, glm::vec3 _o);

  private:
    std::list< std::shared_ptr<Body> > bodies;

  };

}

#endif