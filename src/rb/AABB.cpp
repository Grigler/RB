#include <AABB.h>

#include <Body.h>
#include <BVH.h>

using namespace RB;


//Doesn't parent to anything
AABB::AABB(glm::vec3 _min, glm::vec3 _max)
{
  localMin = _min; localMax = _max;
  worldMin = _min; worldMax = _max;
}
//Derives bounds from parent
AABB::AABB(std::weak_ptr<Body> _parent)
{
  parent = _parent.lock();
}

void AABB::Update(glm::mat3 _bodyOrientation)
{
  //TODO
  //Rotate locals by _bodyOrientation and find new min/max
}

//TODO
bool AABB::CheckAgainst(AABB _other)
{
  bool ret = true;

  if (worldMax.z < _other.worldMin.z || worldMin.z > _other.worldMax.z) ret = false;
  if (worldMax.x < _other.worldMin.x || worldMin.x > _other.worldMax.x) ret = false;
  if (worldMax.y < _other.worldMin.y || worldMin.y > _other.worldMax.y) ret = false;


  return ret;
}
bool AABB::Check(AABB _l, AABB _r)
{
  bool ret = true;

  if (_l.worldMax.z < _r.worldMin.z || _l.worldMin.z > _r.worldMax.z) ret = false;
  if (_l.worldMax.x < _r.worldMin.x || _l.worldMin.x > _r.worldMax.x) ret = false;
  if (_l.worldMax.y < _r.worldMin.y || _l.worldMin.y > _r.worldMax.y) ret = false;


  return ret;
}