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

  //orienting world axis by OBB orientation to find new axis
  glm::mat3 r = obb.parent.lock()->getRotationMatrix3();
  //Orienting left,up,right coordinates by orientation of body
  glm::vec3 relAxis[] = {
    r * glm::vec3(1,0,0),
    r * glm::vec3(0,1,0),
    r * glm::vec3(0,0,1)
  };

  //Starting from centre of OBB and projecting out
  glm::vec3 point = obb.parent.lock()->position;
  for (int i = 0; i < 3; i++)
  {
    //Projecting d onto obb axis for distance
    float dist = glm::dot(d, relAxis[i]);
    point += relAxis[i] * glm::clamp(dist, -obb.halfExtents[i], obb.halfExtents[i]);
  }

  //Testing if it is within Distance2 of r*r
  glm::vec3 v = point - sphere.parent.lock()->position;
  isColliding = (glm::dot(v, v) < sphere.radius*sphere.radius);

  //Forming constraint or return empty shared_ptr<Constraint>
  if (isColliding)
  {
    std::shared_ptr<Constraint> c = std::make_shared<Constraint>();

    c->worldPos = point;
    //Might need to invert
    c->normal = glm::normalize(sphere.parent.lock()->position - point);
    c->penetrationDepth = glm::length(v) - sphere.radius;
    c->l = sphere.parent.lock();
    c->r = obb.parent.lock();


    c->CalcJacM();
    return c;
  }
  else
  {
    return std::shared_ptr<Constraint>();
  }
}
std::vector<std::shared_ptr<Constraint>> GreedyCollider::OBBOBB(GreedyCollider &_l, GreedyCollider &_r)
{
  std::vector<std::shared_ptr<Constraint>> ret;
  //sanity check for collider types
  if (_l.type != ColliderType::OBB || _r.type != ColliderType::OBB)
  {
    //returning empty vector on failure
    return ret;
  }

  //glm::mat3 lMat = _l.parent.lock()->getModelMat();
  //glm::mat3 rMat = _r.parent.lock()->getModelMat();

  //Finding local xyz axis for OBBs
  glm::mat3 rL = _l.parent.lock()->getRotationMatrix3();
  //Orienting left,up,right coordinates by orientation of body
  glm::vec3 relAxisL[] = {
    rL * glm::vec3(1,0,0),
    rL * glm::vec3(0,1,0),
    rL * glm::vec3(0,0,1)
  };
  glm::mat3 rR = _r.parent.lock()->getRotationMatrix3();
  glm::vec3 relAxisR[] = {
    rR * glm::vec3(1,0,0),
    rR * glm::vec3(0,1,0),
    rR * glm::vec3(0,0,1)
  };
  glm::vec3 arm = _r.parent.lock()->position - _l.parent.lock()->position;

  //Performing SAT - mostly lifted from Christer Ericson's "Realtime Collision Detection"
  //I'm not sure anyone has written any original SAT code in the last 5 years if I'm honest
  /*
  glm::mat3 R, absR;
  float ra, rb;
  
  //rotation matrix expressing _r in _l's coordinates
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      R[i][j] = glm::dot(relAxisL[i], relAxisR[j]);
      absR[i][j] = glm::abs(R[i][j]);
    }
  }
  

  glm::vec3 t = _r.parent.lock()->position - _l.parent.lock()->position;
  //reference has axis [0] [1] [2] - assuming this is a typo for now
  t = glm::vec3(glm::dot(t, relAxisL[0]), glm::dot(t, relAxisL[1]), glm::dot(t, relAxisL[2]));
  

  //_l axes
  for (int i = 0; i < 3; i++)
  {
    ra = _l.halfExtents[i];
    rb = glm::dot(_r.halfExtents, absR[i]);
    if (glm::abs(t[i]) > ra + rb) return ret;
  }

  //_r axes
  for (int i = 0; i < 3; i++)
  {
    ra = _r.halfExtents[i];
    rb = glm::dot(_l.halfExtents, absR[i]);
    if (glm::abs(t[0]*R[0][i]+t[1]*R[1][i]*t[2]*R[2][i]) > ra + rb) return ret;
  }
  printf("They are being tested\n");
  
  float bestD = -1.0f;

  //Cross product axes
  ra = _l.halfExtents[1] * absR[2][0] + _l.halfExtents[2] * absR[1][0];
  rb = _r.halfExtents[1] * absR[0][2] + _r.halfExtents[2] * absR[0][1];
  bestD = glm::max(bestD, glm::abs(t[2] * R[1][0] - t[1] * R[2][0]));
  if (glm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return ret;

  ra = _l.halfExtents[1] * absR[2][0] + _l.halfExtents[2] * absR[1][1];
  rb = _r.halfExtents[0] * absR[0][2] + _r.halfExtents[2] * absR[0][0];
  bestD = glm::max(bestD, glm::abs(t[2] * R[1][1] - t[1] * R[2][1]));
  if (glm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return ret;
 
  ra = _l.halfExtents[1] * absR[2][2] + _l.halfExtents[2] * absR[1][2];
  rb = _r.halfExtents[0] * absR[0][1] + _r.halfExtents[1] * absR[0][0];
  bestD = glm::max(bestD, glm::abs(t[2] * R[1][2] - t[1] * R[2][2]));
  if (glm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[2][0] + _l.halfExtents[2] * absR[0][0];
  rb = _r.halfExtents[1] * absR[1][2] + _r.halfExtents[2] * absR[1][1];
  bestD = glm::max(bestD, glm::abs(t[0] * R[2][0] - t[2] * R[0][0]));
  if (glm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[2][1] + _l.halfExtents[2] * absR[0][1];
  rb = _r.halfExtents[0] * absR[1][2] + _r.halfExtents[2] * absR[1][0];
  bestD = glm::max(bestD, glm::abs(t[0] * R[2][1] - t[2] * R[0][1]));
  if (glm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[2][2] + _l.halfExtents[2] * absR[0][2];
  rb = _r.halfExtents[0] * absR[1][1] + _r.halfExtents[1] * absR[1][0];
  bestD = glm::max(bestD, glm::abs(t[0] * R[2][2] - t[2] * R[0][2]));
  if (glm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[1][0] + _l.halfExtents[1] * absR[0][0];
  rb = _r.halfExtents[1] * absR[2][2] + _r.halfExtents[2] * absR[2][1];
  bestD = glm::max(bestD, glm::abs(t[1] * R[0][0] - t[0] * R[1][0]));
  if (glm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[1][1] + _l.halfExtents[1] * absR[0][1];
  rb = _r.halfExtents[0] * absR[2][2] + _r.halfExtents[2] * absR[2][0];
  bestD = glm::max(bestD, glm::abs(t[1] * R[0][1] - t[0] * R[1][1]));
  if (glm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return ret;

  ra = _l.halfExtents[0] * absR[1][2] + _l.halfExtents[1] * absR[0][2];
  rb = _r.halfExtents[0] * absR[2][1] + _r.halfExtents[1] * absR[2][0];
  bestD = glm::max(bestD, glm::abs(t[1] * R[0][2] - t[0] * R[1][2]));
  if (glm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return ret;

  printf("\tThey are Colliding - bestD: %f\n", bestD);
  std::shared_ptr<Constraint> c = std::make_shared<Constraint>();
  
  c->penetrationDepth = bestD; //Not sure
  c->l = _l.parent.lock();
  c->r = _r.parent.lock();
  c->normal;
  c->worldPos;

  ret.push_back(c);
  */

  std::vector<glm::vec3> faceAxis;
  //faceAxis.reserve(6); //6 face axis (sign of face not needed)
  //first 3 axis are from l
  faceAxis.push_back(relAxisL[0]);
  faceAxis.push_back(relAxisL[1]);
  faceAxis.push_back(relAxisL[2]);
  faceAxis.push_back(relAxisR[0]);
  faceAxis.push_back(relAxisR[1]);
  faceAxis.push_back(relAxisR[2]);

  std::vector<glm::vec3> edgeAxis;
  //faceAxis.reserve(9); //9 edge axis
  for (size_t i = 0; i < 3; i++)
  {
    //Cross product normals
    for (size_t j = 0; j < 3; j++)
    {
      //epsilon added to counter parallel axis resulting in zero vector
       edgeAxis.push_back(glm::cross(relAxisL[i], relAxisR[j]) + glm::vec3(glm::epsilon<float>()));
    }
  }

  float bestDistance = std::numeric_limits<float>().max();
  glm::vec3 bestAxis = glm::vec3(0.0f);
  enum CaseFlag
  {
    noCol,lFace,rFace,edge
  };
  CaseFlag caseFlag = CaseFlag::noCol;

  bool isColliding = true;
  for (size_t i = 0; i < faceAxis.size(); i++)
  {
    float projectedAxisProduct = 0.0f;
    glm::vec3 axis = faceAxis.at(i);
    for (int i = 0; i < 3; i++)
    {
      projectedAxisProduct += 
        glm::abs(glm::dot(relAxisL[i] * _l.halfExtents[i],  axis));
      projectedAxisProduct +=
        glm::abs(glm::dot(relAxisR[i] * _r.halfExtents[i], axis));
    }

    if (glm::abs(glm::dot(arm, axis)) > projectedAxisProduct)
    {
      //Separating axis has been found, so they are not colliding
      isColliding = false;
      break;
    }
    else
    {
      float actualInterpenetration =
        projectedAxisProduct - glm::dot(arm, axis);
      if (actualInterpenetration < bestDistance)
      {
        if (actualInterpenetration <= glm::epsilon<float>())
          continue;
        bestDistance = actualInterpenetration;
        bestAxis = axis;
        i <= 2 ? caseFlag = CaseFlag::lFace : caseFlag = CaseFlag::rFace;
      }
    }
  }

  for (size_t i = 0; i < edgeAxis.size(); i++)
  {
    float projectedAxisProduct = 0.0f;
    glm::vec3 axis = edgeAxis.at(i);
    for (int i = 0; i < 3; i++)
    {
      projectedAxisProduct +=
        glm::abs(glm::dot(relAxisL[i] * _l.halfExtents[i], axis));
      projectedAxisProduct +=
        glm::abs(glm::dot(relAxisR[i] * _r.halfExtents[i], axis));
    }
    if (glm::abs(glm::dot(arm, axis)) > projectedAxisProduct)
    {
      //Separating axis has been found, so they are not colliding
      isColliding = false;
      break;
    }
    else
    {
      float actualInterpenetration =
        projectedAxisProduct - glm::dot(arm, axis);
      if (actualInterpenetration < bestDistance)
      {
        if (actualInterpenetration <= glm::epsilon<float>()*5.0f)
          continue;
        bestDistance = actualInterpenetration;
        bestAxis = axis;
        caseFlag = CaseFlag::edge;
      }
    }
  }

  printf("isColliding: %i\n", isColliding);
  //Form constraints from bestAxis as no separating axis found
  if (isColliding)
  {
    printf("\tD: %f\n\tA: %f, %f, %f\n\tF: %i\n", bestDistance,
      bestAxis[0], bestAxis[1], bestAxis[2], caseFlag);
  }

  return ret;
}

float GreedyCollider::PenetrationOnAxis(GreedyCollider &_l, GreedyCollider &_r, glm::vec3 _axis)
{
  glm::mat3 lMat = _l.parent.lock()->getModelMat();
  glm::mat3 rMat = _l.parent.lock()->getModelMat();
  float lProjected =
    _l.halfExtents.x * glm::abs(glm::dot(_axis, lMat[0])) +
    _l.halfExtents.y * glm::abs(glm::dot(_axis, lMat[1])) +
    _l.halfExtents.z * glm::abs(glm::dot(_axis, lMat[2]));
  float rProjected =
    _r.halfExtents.x * glm::abs(glm::dot(_axis, rMat[0])) +
    _r.halfExtents.y * glm::abs(glm::dot(_axis, rMat[1])) +
    _r.halfExtents.z * glm::abs(glm::dot(_axis, rMat[2]));

  glm::vec3 arm = _r.parent.lock()->position - _l.parent.lock()->position;

  
  return 0.0f;
}