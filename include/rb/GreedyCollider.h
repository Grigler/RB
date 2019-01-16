#ifndef GREEDYCOLLIDER_H_
#define GREEDYCOLLIDER_H_

#include <memory>

#include <glm/vec3.hpp>

namespace RB
{
  class Constraint;
  class Body;

  enum ColliderType : unsigned char
  {
    Sphere = 0b0001
  };

  //Contains member variables for all collider types
  //which data to use is contextual of ColliderType
  //higher memory usage, but less dynamic_casting and such
  class GreedyCollider
  {
  public:
    //Assigns sphere collider with radius of 1.0f by default
    GreedyCollider();

    ColliderType type;

    //sphere
    float radius;
    //box - assumed local{0,0,0} center and body orientation
    glm::vec3 halfExtents;
    //plane - assumed local{0,0,0} center and no orientation
    glm::vec3 normal;

    //Returns an empty shared_ptr (use_count==0) if no collision
    static std::shared_ptr<Constraint> SphereSphere(GreedyCollider &_l, GreedyCollider &_r);


    std::weak_ptr<Body> parent;
  private:

  };
}

#endif