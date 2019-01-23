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
    float penetrationDepth = 0.0f;
    
    //variable that is equal to 1/jacobian_diagonal
    //as this constraint is only for point constraint
    //(for collisions), this is calculated with CalcJacM
    float jacDiagABInv = 0.0f;

    //To store impulse force applied across iterations
    float appliedImpulse = 0.0f;

    std::shared_ptr<Body> l;
    std::shared_ptr<Body> r;

    //Provides a value to jacDiagABInv through calculating
    //relevant jacobian diagonal for a collision formed
    //point constraint
    void CalcJacM();

  private:

  };
}

#endif