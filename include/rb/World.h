#ifndef WORLD_H_
#define WORLD_H_

#include <memory>
#include <list>
#include <vector>

#include "glm/glm.hpp"

namespace RB
{
  class Body;
  class BVH;
  class Constraint;

  //Simple tuple struct for ease
  struct broadColPair
  {
    std::weak_ptr<Body> l, r;
  };

  class World
  {
  public:
    World();
    ~World();

    void Tick(float _dt);
    void KillWorld();
    
    std::weak_ptr<Body> AddBody();
    std::weak_ptr<Body> AddBody(glm::vec3 _p, glm::vec3 _o);

    void KillBody(std::weak_ptr<Body> _body);

    static std::unique_ptr<BVH> bvh;

    static const float getFixedTimestep() { return fixedTimestep; }

  private:
    std::list< std::shared_ptr<Body> > bodies;
    std::vector<std::shared_ptr<Constraint>> constraints;
    
    //Used for debugging and comparison purposes
    std::vector<broadColPair> BroadphaseBruteForce();

    //Static variables for sub-stepping fixed timestep
    static float timeAccumulator;
    static float fixedTimestep;
    static float maxTimeStep;
  };

}

#endif