#include "SemiImplicitEuler.h"

#include "Body.h"

using namespace RB;

void SemiImplicitEuler::registerFunc()
{
  IntegratorFactory::registerFunc(RB::Integrators::SemiImplicitEuler, integrate);
}
void SemiImplicitEuler::unregisterFunc()
{
  IntegratorFactory::unregisterFunc(RB::Integrators::SemiImplicitEuler);
}
void SemiImplicitEuler::integrate(std::shared_ptr<Body> _b, float _dt)
{
  _b->linearVelocity += _b->accumulatedForce*(1.0f / _b->mass);
  _b->angularVelocity += _b->accumulatedTorque*glm::inverse(_b->inertiaTensor);
  _b->position += _b->linearVelocity*_dt;
  //TODO - orientations
}