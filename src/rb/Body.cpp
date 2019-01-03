#include <Body.h>

#include <World.h>
#include <BVH.h>

using namespace RB;

Body::Body()
{
  boundingBox = std::make_shared<AABB>(selfRef);
  World::bvh.get()->AddAABB(boundingBox);
}
Body::Body(glm::vec3 _position, glm::quat _orientation,
  glm::vec3 _bvMin, glm::vec3 _bvMax)
{
  position = _position;
  orientation = _orientation;

  boundingBox = std::make_shared<AABB>(selfRef, _bvMin, _bvMax);
  World::bvh.get()->AddAABB(boundingBox);
}

void Body::applyForceImpulse(glm::vec3 _force)
{
  accumulatedForce += _force;
}
void Body::applyForceImpulseAtLocation(
  glm::vec3 _force, glm::vec3 _localLocation)
{
  accumulatedForce += _force;

  glm::vec3 com = glm::vec3(0.0f);
  //glm::vec3 arm = com - _localLocation;
  //Location is given as local, so it _is_ the torque lever arm
  accumulatedTorque += glm::cross(_localLocation, _force);
}

void Body::applyTorqueImpulse(glm::vec3 _torque)
{
  accumulatedTorque += _torque;
}