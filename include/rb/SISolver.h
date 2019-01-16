#ifndef SISOLVER_H_
#define SISOLVER_H_

#include "LCPSolver.h"

#include <vector>
#include <memory>

namespace RB
{
  class Constraint;

  class SISolver
  {
  public:
    static void Solve(std::vector<std::shared_ptr<Constraint>> &_constraints);
    //4 by default (number used by PhysX, Bullet and Box2D) TODO - CITE
    static size_t iterations;
  private:

  };
}

INTERNAL_REG_LCPSOLVER(SISolver);

#endif