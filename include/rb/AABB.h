#ifndef AABB_H_
#define AABB_H_

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <memory>

namespace RB
{
  class Body;

  class AABB
  {
    friend class Body;
    friend class BVH;
    friend class World;
  public:
    //Doesn't parent to anything
    AABB(glm::vec3 _min, glm::vec3 _max);
    //Derives bounds from given parent
    AABB(std::weak_ptr<Body> _parent);
    //Assigns parents and takes given bounds
    AABB(std::weak_ptr<Body> _parent, glm::vec3 _min, glm::vec3 _max);

    static bool Check(AABB _l, AABB _r);
    bool CheckAgainst(AABB _other);

    //Updates with a given rotation matrix
    void Update(glm::mat4 _transform);


    glm::vec3 getWorldMin() { return worldMin; }
    glm::vec3 getWorldMax() { return worldMax; }
    glm::vec3 getLocalMin() { return localMin; }
    glm::vec3 getLocalMax() { return localMax; }

    //True if it has been deemed as colliding this update
    bool collisionFlag;
  private:
    //Initialised and never changed - used to orient
    glm::vec3 localMin = glm::vec3(0.0f);
    glm::vec3 localMax = glm::vec3(0.0f);

    glm::vec3 worldMin = glm::vec3(0.0f);
    glm::vec3 worldMax = glm::vec3(0.0f);

    

    std::weak_ptr<Body> parent;
  };
}


#endif