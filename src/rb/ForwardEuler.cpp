#include "ForwardEuler.h"

#include "Body.h"

using namespace RB;

std::shared_ptr<Integrator> ForwardEuler::create()
{
  std::shared_ptr<Integrator> ret = std::make_shared<ForwardEuler>();
  return ret;
}
void ForwardEuler::Integrate(std::weak_ptr<Body> _b)
{
  //TODO
}