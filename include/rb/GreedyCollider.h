#ifndef GREEDYCOLLIDER_H_
#define GREEDYCOLLIDER_H_

#include "AbstractCollider.h"

#include <glm/vec3.hpp>

namespace RB
{
  //Contains member variables for all collider types
  //which data to use is contextual of ColliderType
  //higher memory usage, but less dynamic_casting and such
  class GreedyCollider : public AbstractCollider
  {
  public:


    //sphere
    float radius;
    //box - assumed local{0,0,0} center and body orientation
    glm::vec3 halfExtents;
    //plane - assumed local{0,0,0} center and no orientation
    glm::vec3 normal;

  private:


  };
}

#endif