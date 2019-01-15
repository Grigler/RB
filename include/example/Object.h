#ifndef OBJECT_H_
#define OBJECT_H_

#include "Transform.h"

#include <memory>

//Forwrad declaration with namespace
namespace RB { class Body;  }

class Object
{
public:
  ~Object();

  void AssignBody(std::weak_ptr<RB::Body> _body);

  virtual void Update() {}
  virtual void Draw() {}
  virtual void onCreation() {}

  std::weak_ptr<RB::Body> body;

  Transform transform;

private:

};

#endif