#include "GreedyCollider.h"

#include "Constraint.h"
#include "Body.h"

//for distance2 func
#include <glm/gtx/norm.hpp>

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

  float radSum = _l.radius + _r.radius;
  float distSqr = glm::distance2(_l.parent.lock()->position, _r.parent.lock()->position);
  isColliding = distSqr < radSum*radSum;

  //Check for collision

  if (isColliding)
  {
    //Form constraint
    std::shared_ptr<Constraint> c = std::make_shared<Constraint>();
    printf("pass\n");
    return c;
  }
  else
  {
    printf("\tfail\n");
    return std::shared_ptr<Constraint>();
  }
}