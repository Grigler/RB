#ifndef GREEDYCOLLIDER_H_
#define GREEDYCOLLIDER_H_

#include <memory>
#include <vector>

#include <glm/vec3.hpp>

namespace RB
{
  class Constraint;
  class Body;

  //Set up with binary numbers for easy bitfield comparison
  enum ColliderType : unsigned char
  {
    Sphere = 0b0001,
    OBB = 0b0010
  };

  enum CollisionType : unsigned char
  {
    SphereSphere = ColliderType::Sphere & ColliderType::Sphere,
    SphereOBB = ColliderType::Sphere & ColliderType::OBB,
    OBBOBB = ColliderType::OBB & ColliderType::OBB
  };

  //Contains member variables for all collider types
  //which data to use is contextual of ColliderType
  //higher memory usage, but less dynamic_casting and such
  class GreedyCollider
  {
  public:
    //Assigns sphere collider with radius of 1.0f by default
    GreedyCollider();
    //Sets flag for sphere and radius
    GreedyCollider(float _radius);
    //Sets flag for OBB and halfextents - body orientation is assumed
    GreedyCollider(glm::vec3 _halfExtents);

    ColliderType type;

    //sphere
    float radius;
    //box - assumed local{0,0,0} center and body orientation
    glm::vec3 halfExtents;
    //plane - assumed local{0,0,0} center and no orientation
    glm::vec3 normal;

    //Returns an empty shared_ptr (use_count==0) if no collision
    static std::shared_ptr<Constraint> SphereSphere(GreedyCollider &_l, GreedyCollider &_r);
    //Returns an empty shared_ptr (use_count==0) if no collision
    //_l or _r can be either ColliderType::Sphere or ColliderType::OBB
    //as long as there is one of each in either paramater
    static std::shared_ptr<Constraint> SphereOBB(GreedyCollider &_l, GreedyCollider &_r);
    //returns empty vector if no collision
    static std::vector<std::shared_ptr<Constraint>> OBBOBB(GreedyCollider &_l, GreedyCollider &_r);


    std::weak_ptr<Body> parent;
  private:
    //Using 2 OBBs and an arbitrary axis - returns interpenetration depth 
    //(negative if there is no penetration)
    static float PenetrationOnAxis(GreedyCollider &_l, GreedyCollider &_r, glm::vec3 _axis);
  };
}

#endif
