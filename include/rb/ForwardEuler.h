#ifndef FORWARDEULER_H_
#define FORWARDEULER_H_

#include "Integrator.h"

namespace RB
{
  class Body;

  class ForwardEuler : public Integrator
  {
  public:
    static void registerFunc();
    static void unregisterFunc();
    static void integrate(std::shared_ptr<Body> _b, float _dt);
  private:

  };
}

//Must be called at the end of all integrators to register name
INTERNAL_REG_INTEGRATOR(ForwardEuler);

#endif