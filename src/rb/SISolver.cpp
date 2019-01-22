#include "SISolver.h"

#include "Constraint.h"
#include "World.h" //Included for World::fixedTimestep
#include "Body.h"

using namespace RB;

size_t SISolver::iterations = 4;

//TODO
void SISolver::Solve(std::vector<std::shared_ptr<Constraint>> &_constraints)
{

  for (size_t i = 0; i < iterations; i++)
  {
    for (auto c = _constraints.begin(); c != _constraints.end(); c++) //Haha c++
    {
      std::shared_ptr<Body> l = (*c)->l;
      std::shared_ptr<Body> r = (*c)->r;
    
      //TODO - Would ideally be cached as these do not change
      glm::vec3 armL = (*c)->worldPos - l->position;
      glm::vec3 armR = (*c)->worldPos - r->position;
      glm::vec3 tangentL = glm::cross(armL, (*c)->normal);
      glm::vec3 tangentR = -glm::cross(armR, (*c)->normal);

      //linear
      float relativeVelocity = 
        glm::dot((*c)->normal, l->linearVelocity) +
        glm::dot(-(*c)->normal, r->linearVelocity);
      //angular
      relativeVelocity += 
        glm::dot(tangentL, l->angularVelocity) +
        glm::dot(tangentR, r->angularVelocity);

      //calculating b for linear projection
      float beta = 0.1f; //TODO - move to static var
      float b = (beta / World::getFixedTimestep()) * (*c)->penetrationDepth;

      //calculating lambda
      float lambda = (*c)->jacDiagABInv * (relativeVelocity + b);

      //clamping according to Signorini conditions with accumulated lambda
      lambda = glm::max((*c)->appliedImpulse+lambda,0.0f) - (*c)->appliedImpulse;
      (*c)->appliedImpulse += lambda;
      


      //calculating impulse forces and applying to velocity
      glm::vec3 linImpulseL = lambda * (*c)->normal * l->invMass;
      glm::vec3 linImpulseR = lambda * -(*c)->normal * r->invMass;
      glm::vec3 angImpulseL = glm::vec3(0.0f);
      glm::vec3 angImpulseR = glm::vec3(0.0f);
      if (l->invMass != 0.0f) angImpulseL = lambda * (l->invWorldInertiaTensor * tangentL);
      if (r->invMass != 0.0f) angImpulseR = lambda * (r->invWorldInertiaTensor * tangentR);
      //applying linear
      l->linearVelocity += linImpulseL;
      r->linearVelocity += linImpulseR;
      //applying angular
      l->angularVelocity += angImpulseL;
      r->angularVelocity += angImpulseR;
      
      //TODO - use these functions once they actually add impulse at velocity level
      //l->applyForceImpulseAtLocation((*c)->normal*lambda, (*c)->worldPos - l->position);
      //r->applyForceImpulseAtLocation(-(*c)->normal*lambda, (*c)->worldPos - r->position);
    }
  }
}