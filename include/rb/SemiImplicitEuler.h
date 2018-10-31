#ifndef SEMIIMPLICITEULER_H_
#define SEMIIMPLICITEULER_H_

#include "Integrator.h"

#include <memory>

namespace RB
{
  class Body;
  class SemiImplicitEuler
  {
  public:
    static void registerFunc();
    static void unregisterFunc();
    static void integrate(std::shared_ptr<Body> _b, float _dt);
  };
}

INTERNAL_REG_INTEGRATOR(SemiImplicitEuler);

#endif