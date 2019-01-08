#ifndef OBJECT_H_
#define OBJECT_H_

#include "Transform.h"

#include <memory>

//Forwrad declaration with namespace
namespace RB { class Body;  }

class Object
{
public:
  //Does not assign a RB body to the object
  Object() {}
  //Assigns the given body to the object
  Object(std::weak_ptr<RB::Body> _body);
  
  ~Object();

  void AssignBody(std::weak_ptr<RB::Body> _body);

  virtual void Update() {}
  virtual void Draw() {}


protected:
  std::weak_ptr<RB::Body> body;

  Transform transform;

private:

};

#endif