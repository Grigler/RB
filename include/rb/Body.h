#ifndef BODY_H_
#define BODY_H_

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/quaternion.hpp>

#include <AABB.h>

namespace RB
{

  class Body
  {
  public:
    Body();
    Body(glm::vec3 _position, glm::quat _orientation);

    glm::vec3 position = glm::vec3(0.0f);
    glm::quat orientation = glm::identity<glm::quat>();

    glm::vec3 linearVelocity = glm::vec3(0.0f);
    float mass = 1.0f;
    glm::vec3 accumulatedForce = glm::vec3(0.0f);

    glm::vec3 angularVelocity = glm::vec3(0.0f);
    glm::mat3 ineritaTensor = glm::identity<glm::mat3>();
    glm::vec3 accumulatedTorque = glm::vec3(0.0f);

    //Helper functions
    glm::vec4 getPosition4() { return glm::vec4(position, 1); }
    glm::mat3 getRotationMatrix3() { return glm::toMat3(orientation); }
    glm::mat4 getRotationMatrix4() { return glm::toMat4(orientation); }

  private:

    std::shared_ptr<AABB> boundingBox;
    std::weak_ptr<Body> selfRef;

  };

}//RB namespace
#endif