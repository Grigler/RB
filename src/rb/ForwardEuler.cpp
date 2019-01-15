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
  _b->position += _b->linearVelocity*_dt;
  _b->linearVelocity += _b->accumulatedForce*(1.0f / _b->mass);
  _b->linearVelocity += glm::vec3(0.0f, -9.81f*_b->mass, 0.0f) * _dt;
  _b->angularVelocity += _b->accumulatedTorque*(glm::inverse(_b->ineritaTensor));
  //TODO - orientation

  //reset accumulated forces
  _b->accumulatedForce = glm::vec3(0.0f);
  _b->accumulatedTorque = glm::vec3(0.0f);
}