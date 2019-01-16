#ifndef LCPSOLVER_H_
#define LCPSOLVER_H_

//Creates a nice namespaced cstring we can access
#define INTERNAL_REG_LCPSOLVER(T)\
namespace RB::LCPSolvers\
{\
  static constexpr char *T = #T;\
}\

namespace RB
{

  class LCPSolver
  {
  public:

  private:

  };
}

#endif