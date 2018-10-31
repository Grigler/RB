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
  public:
    //Doesn't parent to anything
    AABB(glm::vec3 _min, glm::vec3 _max);
    //Derives bounds from given parent
    AABB(std::weak_ptr<Body> _parent);

    static bool Check(AABB _l, AABB _r);
    bool CheckAgainst(AABB _other);

    void Update(glm::mat3 _bodyOrientation);

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