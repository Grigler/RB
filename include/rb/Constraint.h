#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_


#include <memory>


#include <glm/vec3.hpp>

namespace RB
{
  class Body;

  //Stores information relating to a constraint
  //vectors are all relative to body 'l'
  //(e.g. normal and penetration depth)
  class Constraint
  {
  public:
    glm::vec3 worldPos;
    glm::vec3 normal;
    float penetrationDepth;
    //TODO - RENAME
    //Legacy variable from old code that I've never
    //fully gotten my head around
    float jacDiagABInv;

    //To store impulse force applied across iterations
    float appliedImpulse;

    std::shared_ptr<Body> l;
    std::shared_ptr<Body> r;

    //Provides a value to jacDiagABInv from other member variables
    void CalcJacM();

  private:

  };
}

#endif