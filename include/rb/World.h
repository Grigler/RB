#ifndef WORLD_H_
#define WORLD_H_

#include <memory>
#include <list>

namespace RB
{
  class Body;

  class World
  {
  public:
    void Tick();
    void Kill();
    
    std::weak_ptr<Body> AddBody();

  private:
    std::list< std::shared_ptr<Body> > bodies;

  };

}

#endif