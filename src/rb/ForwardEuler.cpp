#include "ForwardEuler.h"

#include "Body.h"

using namespace RB;

void ForwardEuler::registerFunc()
{
  IntegratorFactory::registerFunc(RB::Integrators::ForwardEuler, integrate);
}
void ForwardEuler::unregisterFunc()
{
  IntegratorFactory::unregisterFunc(RB::Integrators::ForwardEuler);
}
void ForwardEuler::integrate(std::shared_ptr<RB::Body> _b, float _dt)
{
  //Updating position and orientation from current velocity
  _b->position += _b->linearVelocity*_dt;
  //_b->orientation += glm::quat(_b->orientation * glm::vec4(_b->angularVelocity,1.0f) * _dt);
  _b->orientation = glm::quat(_b->angularVelocity*_dt) * _b->orientation + (_b->orientation*0.5f);  
  _b->orientation = glm::normalize(_b->orientation);

  //Updating worldInvInertiaTensor for the adjustment in orientation
  _b->CalcWorldInvInertiaTensor();

  //Linear
  _b->linearVelocity += _b->accumulatedForce*(1.0f / _b->mass)*_dt;
  _b->linearVelocity += glm::vec3(0.0f, -9.81f, 0.0f) * _dt;
  //Angular
  _b->angularVelocity += _b->accumulatedTorque*_b->invWorldInertiaTensor * _dt;
  
  //reset accumulated forces
  _b->accumulatedForce = glm::vec3(0.0f);
  _b->accumulatedTorque = glm::vec3(0.0f);
}