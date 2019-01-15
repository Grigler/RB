#ifndef BVH_H_
#define BVH_H_

#include <memory>
#include <vector>
#include <list>

#include "AABB.h"

namespace RB
{
  struct BVHNode
  {
    friend class BVH;
  private:
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;
    std::weak_ptr<AABB> bv;
  };

  class BVH
  {
  public:
    BVH();

    //Prunes BVs and then builds tree
    void Rebuild();

    //bool CheckAgainst(std::weak_ptr<AABB> _against);
    std::weak_ptr<AABB> CheckAgainst(std::weak_ptr<AABB> _against);


    void AddAABB(std::weak_ptr<AABB> _aabb);
    void AddAABB(AABB &_aabb);

  private:
    std::shared_ptr<BVHNode> root;
    //Using list instead of vector to avoid issues with resizing
    std::list< std::shared_ptr<AABB> > bvs;

    void RecurseBuild(std::weak_ptr<BVHNode> _curr,
      std::list< std::shared_ptr<AABB> > &_bvs);
  };
}
#endif