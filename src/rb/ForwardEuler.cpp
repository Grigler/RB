#include "ForwardEuler.h"

#include "Body.h"

using namespace RB;

void ForwardEuler::registerFunc()
{
  IntegratorFactory::registerFunc("ForwardEuler", integrate);
}
void ForwardEuler::unregisterFunc()
{
  IntegratorFactory::unregisterFunc("ForwardEuler");
}
void ForwardEuler::integrate(std::shared_ptr<RB::Body> _b, float _dt)
{
  
}