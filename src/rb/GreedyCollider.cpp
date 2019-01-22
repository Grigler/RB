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
GreedyCollider::GreedyCollider(float _radius)
{
  this->type = ColliderType::Sphere;
  radius = _radius;
}
GreedyCollider::GreedyCollider(glm::vec3 _halfExtents)
{
  this->type = ColliderType::OBB;
  halfExtents = _halfExtents;
}

std::shared_ptr<Constraint> GreedyCollider::SphereSphere(GreedyCollider &_l, GreedyCollider &_r)
{
  //Sanity check for collider types
  if (_l.type != ColliderType::Sphere || _r.type != ColliderType::Sphere)
  {
    return std::shared_ptr<Constraint>();
  }


  bool isColliding = true;

  float radSum = _l.radius + _r.radius;
  float distSqr = glm::distance2(_l.parent.lock()->position, _r.parent.lock()->position);
  isColliding = distSqr < radSum*radSum;

  if (isColliding)
  {
    //Form constraint
    std::shared_ptr<Constraint> c = std::make_shared<Constraint>();
    c->appliedImpulse = 0.0f;
    c->l = _l.parent.lock();
    c->r = _r.parent.lock();
    float dist = glm::sqrt(distSqr);
    glm::vec3 midLine = _l.parent.lock()->position - _r.parent.lock()->position;
    c->normal = midLine / dist;
    c->worldPos = _l.parent.lock()->position + (midLine * 0.5f);
    c->penetrationDepth = glm::dot(glm::vec3(radSum - dist), c->normal);
    c->CalcJacM();
    return c;
  }
  else
  {
    return std::shared_ptr<Constraint>();
  }
}
std::shared_ptr<Constraint> GreedyCollider::SphereOBB(GreedyCollider &_l, GreedyCollider &_r)
{
  GreedyCollider sphere;
  GreedyCollider obb;
  //Sanity check for ColliderTypes
  if (_l.type == ColliderType::Sphere && _r.type == ColliderType::OBB)
  {
    sphere = _l;
    obb = _r;
  }
  else if (_l.type == ColliderType::OBB && _r.type == ColliderType::Sphere)
  {
    sphere = _r;
    obb = _l;
  }
  else
  {
    return std::shared_ptr<Constraint>();
  }

  //Collision check between Sphere and OBB
  bool isColliding = false;

  //Find closest point from sphere center to OBB surface
  glm::vec3 d = sphere.parent.lock()->position - obb.parent.lock()->position;
  glm::vec3 point = obb.parent.lock()->position;

  glm::mat3 r = obb.parent.lock()->getRotationMatrix3();

  //Orienting left,up,right coordinates by orientation of body
  glm::vec3 relAxis[] = {
    r * glm::vec3(1,0,0),
    r * glm::vec3(0,1,0),
    r * glm::vec3(0,0,1)
  };

  for (int i = 0; i < 3; i++)
  {
    //Projecting d onto obb axis for distance
    float dist = glm::dot(d, relAxis[i]);
    point += glm::clamp(dist, -obb.halfExtents[i], obb.halfExtents[i]) * dist;
  }

  //Testing if it is within Distance2 of r*r
  glm::vec3 v = point - sphere.parent.lock()->position;
  isColliding = (glm::dot(v, v) <= sphere.radius*sphere.radius);

  //Forming constraint or return empty shared_ptr<Constraint>
  if (isColliding)
  {
    std::shared_ptr<Constraint> c = std::make_shared<Constraint>();

    c->worldPos = point;
    //Might need to invert
    c->normal = glm::normalize(v);
    c->penetrationDepth = sphere.radius - glm::length(v);
    c->l = obb.parent.lock();
    c->r = sphere.parent.lock();


    c->CalcJacM();
    return c;
  }
  else
  {
    return std::shared_ptr<Constraint>();
  }
}