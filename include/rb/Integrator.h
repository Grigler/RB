#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include <memory>

#include "IntegratorFactory.h"


/*
  Macro to register the integrator as a static string defined
  within the RB::Integrators namespace - use as if enum of strings
*/
#define INTERNAL_REG_INTEGRATOR(T) \
namespace RB::Integrators\
{\
  static constexpr char *T = #T;\
}\

namespace RB
{
  class Integrator
  {
  public:

  };
}

#endif