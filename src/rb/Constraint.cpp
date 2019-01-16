#include "Constraint.h"

#include "Body.h"

using namespace RB;

void Constraint::CalcJacM()
{
  //Interpreted from bullet3 src
  float jmj0 = l->invMass;
  float jmj1 = 0.0f; //init to 0 in case of inf mass
  if (jmj0 != 0.0f)
  {
    glm::vec3 armL = glm::cross(worldPos - l->position, normal);
    jmj1 = glm::dot(l->invWorldInertiaTensor * armL, armL);
  }

  float jmj2 = r->invMass;
  float jmj3 = 0.0f;
  if (jmj0 != 0.0f)
  {
    glm::vec3 armR = glm::cross(worldPos - r->position, -normal);
    jmj3 = glm::dot(r->invWorldInertiaTensor * armR, armR);
  }

  jacDiagABInv = -(1.0f / (jmj0 + jmj1 + jmj2 + jmj3));
}