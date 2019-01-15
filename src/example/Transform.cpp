#include "Transform.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

const glm::vec3 Transform::forward = glm::vec3(0, 0, 1);
const glm::vec3 Transform::up = glm::vec3(0, 1, 0);
const glm::vec3 Transform::right = glm::vec3(1, 0, 0);

glm::mat4 Transform::getModelMat()
{
  glm::mat4 from = glm::mat4(1);
  glm::mat4 t = glm::translate(from, pos);
  glm::mat4 r = getRotationMat();
  
  //SCALE ASSUMED TO BE 1:1 TO GEOMETRY FOR EXAMPLE
  return t * r;
}

glm::mat4 Transform::getRotationMat()
{
  glm::mat4 from = glm::mat4(1);
  glm::mat4 r = glm::rotate(from, rot.y, glm::vec3(0, 1, 0));
  r = glm::rotate(r, rot.z, glm::vec3(0, 0, 1));
  r = glm::rotate(r, rot.x, glm::vec3(1, 0, 0));
  return r;
}

glm::vec3 Transform::getUp()
{
  return glm::normalize((glm::vec3)(getRotationMat() * glm::vec4(up,1)));
}
glm::vec3 Transform::getForward()
{
  return glm::normalize((glm::vec3)(getRotationMat() * glm::vec4(forward,1)));
}