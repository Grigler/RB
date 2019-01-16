#ifndef ABSTRACTCOLLIDER_H_
#define ABSTRACTCOLLIDER_H_

#include <string>

//Purely a virtual class - don't think there is any common ground
//TODO - CHANGE IF NEEDED
namespace RB
{

  enum ColliderType : unsigned char
  {
    Sphere = 0b0001
  };

  class AbstractCollider
  {
  public:
    ColliderType type;
  };

}

#endif