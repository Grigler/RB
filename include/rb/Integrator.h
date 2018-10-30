#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <memory>

namespace RB
{
  class Body;

  class Integrator
  {
  public:
    virtual std::shared_ptr<Integrator> create() { return nullptr; }
    virtual void integrate(std::weak_ptr<Body> _b) {}
  };
}

#endif