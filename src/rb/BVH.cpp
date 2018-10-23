#include "BVH.h"

#include <glm/glm.hpp>

using namespace RB;

BVH::BVH()
{
  root = std::make_unique<BVHNode>();
}

void BVH::Tick()
{
  Prune();
}

void BVH::BuildFrom(std::vector< std::weak_ptr<AABB> > _allBVs)
{
  //Anything is smaller than max and vice-verca - good initial vals
  glm::vec3 allMin = glm::vec3(std::numeric_limits<float>().max());
  glm::vec3 allMax = glm::vec3(std::numeric_limits<float>().min());

  for (auto i = _allBVs.begin(); i != _allBVs.end(); i++)
  {
    allMin = glm::min(allMin, i->lock()->worldMin);
    allMax = glm::max(allMax, i->lock()->worldMax);
  }

}

void BVH::Prune()
{
  for (auto i = bvs.begin(); i != bvs.end(); i++)
  {
    //Single use_count and no isVirtual flag implies that it belonged
    //to body that no longer exists within the system
    if (i->use_count() == 1 && !(*i)->isVirtual)
    {
      bvs.erase(i);
      i--;
    }
  }
}