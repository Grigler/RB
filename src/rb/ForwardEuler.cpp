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
  _b->angularVelocity += _b->accumulatedTorque*(glm::inverse(_b->ineritaTensor));
  //TODO - orientation
}