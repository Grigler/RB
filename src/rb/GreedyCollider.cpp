#include "GreedyCollider.h"

#include "Constraint.h"

using namespace RB;

GreedyCollider::GreedyCollider()
{
  this->type = ColliderType::Sphere;
  radius = 1.0f;
}

//TODO
std::shared_ptr<Constraint> GreedyCollider::SphereSphere(GreedyCollider &_l, GreedyCollider &_r)
{
  bool isColliding = true;

  //Check for collision

  if (isColliding)
  {
    //Form constraint
    std::shared_ptr<Constraint> c = std::make_shared<Constraint>();
    
    return c;
  }
  else
  {
    return std::shared_ptr<Constraint>();
  }
}