#ifndef AABB_H_
#define AABB_H_

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

namespace RB
{
  class AABB
  {
    friend class Body;
  private:
    //Initialised and never changed - used to orient
    glm::vec3 localMin = glm::vec3(0.0f);
    glm::vec3 localMax = glm::vec3(0.0f);

    glm::vec3 worldMin = glm::vec3(0.0f);
    glm::vec3 worldMax = glm::vec3(0.0f);

    void Update(glm::mat3 _bodyOrientation);

    void CheckAgainst(AABB _other);
    static void Check(AABB _l, AABB _r);
  };
}


#endif