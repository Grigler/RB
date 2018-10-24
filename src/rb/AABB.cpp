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

  return false;
}
bool AABB::Check(AABB _l, AABB _r)
{

  return false;
}