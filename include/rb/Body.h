#ifndef BODY_H_
#define BODY_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace RB
{

  class Body
  {
  public:
    Body(glm::vec3 _position, glm::quat _orientation);

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat orientation = glm::identity<glm::quat>();


    //Helper functions
    glm::vec4 getPosition4() { return glm::vec4(position, 1); }
    glm::mat3 getRotationMatrix3() { return glm::toMat3(orientation); }
    glm::mat4 getRotationMatrix4() { return glm::toMat4(orientation); }

  private:

  };

}//RB namespace
#endif