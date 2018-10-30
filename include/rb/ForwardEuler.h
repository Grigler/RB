#ifndef FORWARDEULER_H_
#define FORWARDEULER_H_

#include "Integrator.h"

namespace RB
{
  class ForwardEuler : public Integrator
  {
  public:
    static std::shared_ptr<Integrator> create() ;
    void integrate(std::weak_ptr<Body> _b) override;
  };
}

#endif