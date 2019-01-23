#include <Body.h>

#include <World.h>
#include <BVH.h>

#include <glm/gtx/transform.hpp>

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

void Body::SetMass(float _mass)
{
  if (_mass <= 0.0f)
  {
    mass = 0.0f;
    invMass = 0.0f;
  }
  else
  {
    mass = _mass;
    invMass = 1.0f / mass;
  }
}

glm::mat4 Body::getModelMat()
{
  //Allows a hierarchy to be added easier in future
  glm::mat4 from = glm::mat4(1);
  glm::mat4 t = glm::translate(from, position);
  glm::mat4 r = glm::toMat4(orientation);
  //Assuming now scale
  return t * r * scaleBVMat;
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

void Body::CalcWorldInvInertiaTensor()
{
  //invWorldInertiaTensor = glm::inverse(glm::toMat3(orientation) * inertiaTensor);
  invWorldInertiaTensor =
    glm::toMat3(orientation) * 
    (glm::inverse(inertiaTensor)*glm::transpose(glm::toMat3(orientation)));
}

void Body::CalcInertiaTensorSphere(float _radius)
{
  //clearing inertia tensor
  inertiaTensor = glm::mat3(0.0f);
  //diag value (from https://en.wikipedia.org/wiki/List_of_moments_of_inertia)
  float diag = 0.4f*mass*_radius*_radius;
  for (int i = 0; i < 3; i++)
  {
    inertiaTensor[i][i] = diag;
  }
  CalcWorldInvInertiaTensor();
}
void Body::CalcInertiaTensorBox(glm::vec3 _halfExtents)
{
  //Clearing out inertia tensor
  inertiaTensor = glm::mat3(0.0f);
  //doubling and squaring halfExtents for ease
  for (int i = 0; i < 3; i++)
    _halfExtents[i] = glm::pow(_halfExtents[i] * 2.0f, 2);

  //Caching repeated value
  float massRatio = (1.0f / 12.0f)*mass;
  inertiaTensor[0][0] = massRatio * (_halfExtents.y + _halfExtents.z);
  inertiaTensor[1][1] = massRatio * (_halfExtents.x + _halfExtents.z);
  inertiaTensor[2][2] = massRatio * (_halfExtents.x + _halfExtents.y);
}

void Body::SetBVScale(glm::vec3 _scale)
{
  scaleBVMat = glm::scale(_scale);
}

void Body::kill()
{
  
}