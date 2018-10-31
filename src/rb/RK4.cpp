#include "RK4.h"

#include "Body.h"

using namespace RB;

void RK4::registerFunc()
{
  IntegratorFactory::registerFunc(RB::Integrators::RK4, integrate);
}
void RK4::unregisterFunc()
{
  IntegratorFactory::unregisterFunc(RB::Integrators::RK4);
}
void RK4::integrate(std::shared_ptr<RB::Body> _b, float _dt)
{
  //TODO - https://github.com/NCCA/MassSpring/blob/master/src/AbstractRK4Integrator.cpp
}