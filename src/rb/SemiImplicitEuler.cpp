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
  //Linear
  _b->linearVelocity += _b->accumulatedForce*_b->invMass*_dt;
  _b->linearVelocity += glm::vec3(0.0f, -9.81f, 0.0f) * _dt; //No need to include mass as g =(-9.81)*dt
  //Angular
  _b->angularVelocity += _b->accumulatedTorque*_b->invWorldInertiaTensor * _dt;
  
  //reset accumulated forces
  _b->accumulatedForce = glm::vec3(0.0f);
  _b->accumulatedTorque = glm::vec3(0.0f);

  //Updating position and orientation from current velocity
  _b->position += _b->linearVelocity*_dt;
  //_b->orientation += glm::quat(_b->orientation * glm::vec4(_b->angularVelocity,1.0f) * _dt);
  _b->orientation = glm::quat(_b->angularVelocity*_dt) * _b->orientation + (_b->orientation*0.5f);  
  _b->orientation = glm::normalize(_b->orientation);

  //Updating worldInvInertiaTensor for the adjustment in orientation
  _b->CalcWorldInvInertiaTensor();
}