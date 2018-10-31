#ifndef RK4_H_
#define RK4_H_

#include "Integrator.h"

#include <memory>

namespace RB
{
  class Body;
  class RK4
  {
  public:
    static void registerFunc();
    static void unregisterFunc();
    static void integrate(std::shared_ptr<RB::Body> _b, float _dt);
  };
}

INTERNAL_REG_INTEGRATOR(RK4);

#endif