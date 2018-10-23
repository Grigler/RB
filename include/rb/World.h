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
    
  private:
    std::list< std::unique_ptr<Body> > bodies;

  };

}

#endif