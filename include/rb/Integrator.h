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
  static std::string T = std::string(#T);\
}\

namespace RB
{
  class Body;

  class Integrator
  {
  public:

  };


}

#endif