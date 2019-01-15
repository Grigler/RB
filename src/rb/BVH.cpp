#include "BVH.h"

#include <algorithm>

#include <glm/glm.hpp>

using namespace RB;

BVH::BVH()
{
  root = std::make_unique<BVHNode>();
}

void BVH::Rebuild()
{
  //Init with most opposite values so for easy initial value
  glm::vec3 min = glm::vec3(std::numeric_limits<float>().max());
  glm::vec3 max = glm::vec3(std::numeric_limits<float>().min());

  //Pruning the tree at the same time as finding the extents
  //reduces overhead in iterating through dynamic memory
  for (auto i = bvs.begin(); i != bvs.end(); i++)
  {
    //Removes all virtual AABBs as they will be re-generated
    if (i->use_count() == 1)
    {
      bvs.erase(i);
      i--;
    }
    else
    {
      min = glm::min(min, (*i)->worldMin);
      max = glm::max(max, (*i)->worldMax);
    }
  }

  //Potentially expensive call - necessary without ability to insert nodes
  root.reset();

  //Adding largest BV to bvs
  std::shared_ptr<AABB> lrg = std::make_shared<AABB>(min, max);
  bvs.push_back(lrg);

  //Making new tree
  root = std::make_unique<BVHNode>();
  root->bv = lrg;
  
  //Making copy of list for bvs for building - DIRT AND EXPENSIVE
  std::list < std::shared_ptr<AABB> > tempBVS(bvs);
  RecurseBuild(root, tempBVS);

}

/*
bool BVH::CheckAgainst(std::weak_ptr<AABB> _against)
{
  std::weak_ptr<BVHNode> _curr = root;
  //Making the assumption that the root BV check will always pass
  while (!_curr.expired())
  {
    
    if (_curr.lock()->left.use_count() && 
      _curr.lock()->left->bv.lock()->CheckAgainst(*_against.lock()))
    {
      _curr = _curr.lock()->left;
    }
    else if (_curr.lock()->right.use_count() && 
      _curr.lock()->right->bv.lock()->CheckAgainst(*_against.lock()))
    {
      _curr = _curr.lock()->right;
    }
    else
    {
      return false;
    }
  }
  //_curr expired means that a lowest-level node has passed AABB check
  return true;
}
*/

std::weak_ptr<AABB> BVH::CheckAgainst(std::weak_ptr<AABB> _against)
{
  std::weak_ptr<BVHNode> curr = root;
  std::weak_ptr<BVHNode> prev;
  //Making the assumption that the root BV check will always pass
  while (!curr.expired())
  {

    if (curr.lock()->left.use_count() &&
      curr.lock()->left->bv.lock()->CheckAgainst(*_against.lock()))
    {
      prev = curr;
      curr = curr.lock()->left;
    }
    else if (curr.lock()->right.use_count() &&
      curr.lock()->right->bv.lock()->CheckAgainst(*_against.lock()))
    {
      prev = curr;
      curr = curr.lock()->right;
    }
    else
    {
      //Returning empty weak_ptr (expired() == true)
      return std::weak_ptr<AABB>();
    }
  }
  //curr expired means that a lowest-level node has passed AABB check
  //returning previous _curr
  return curr.lock()->bv;
}

void BVH::AddAABB(std::weak_ptr<AABB> _aabb)
{

}
void BVH::AddAABB(AABB &_aabb)
{

}

void BVH::RecurseBuild(std::weak_ptr<BVHNode> _curr,
  std::list< std::shared_ptr<AABB> > &_bvs)
{
  //Root has bv already - so only do this for other nodes
  if (_curr.lock()->bv.expired())
  {
    //Must be the lowest node
    if (_bvs.size() == 1)
    {
      //Explicit conversion not 100% necessary
      _curr.lock()->bv = std::weak_ptr<AABB>(_bvs.front());
    }
    else
    {
      glm::vec3 min = glm::vec3(std::numeric_limits<float>().max());
      glm::vec3 max = glm::vec3(std::numeric_limits<float>().min());
      for (auto i = _bvs.begin(); i != _bvs.end(); i++)
      {
        min = glm::min(min, (*i)->worldMin);
        max = glm::max(max, (*i)->worldMax);
      }
      _curr.lock()->bv = std::make_shared<AABB>(min, max);
    }
  }
  else
  {
    return;
  }
  //Find largest axis in current BV
  glm::vec3 axisVector = _curr.lock()->bv.lock()->worldMax -
    _curr.lock()->bv.lock()->worldMin;
  //Storing indx of vector to access for longest axis
  int longestAxisIndx = 0;
  if (axisVector.x > axisVector.y && axisVector.x > axisVector.z)
  {
    longestAxisIndx = 0;
  }
  else if (axisVector.y > axisVector.x && axisVector.y > axisVector.z)
  {
    longestAxisIndx = 1;
  }
  else
  {
    longestAxisIndx = 2;
  }

  //Find splitting point along this axis
  float splitVal = axisVector[longestAxisIndx] / 2.0f;

  //Create local list of these bvs and send into left & right
  std::list< std::shared_ptr<AABB> > lList;
  std::list< std::shared_ptr<AABB> > rList;
  for (auto i = _bvs.begin(); i != _bvs.end(); i++)
  {
    if ((*i)->worldMin[longestAxisIndx] < splitVal)
    {
      lList.push_back((*i));
    }
    else
    {
      rList.push_back((*i));
    }
  }

  RecurseBuild(_curr.lock()->left, lList);
  RecurseBuild(_curr.lock()->right, rList);
}
