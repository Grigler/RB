#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>

//Forwrad declaration with namespace
namespace RB { class Body;  }

class Object
{
public:
  Object() {}
  Object(std::weak_ptr<RB::Body> _body);
  
  ~Object();

  void AssignBody(std::weak_ptr<RB::Body> _body);

  virtual void Update() {}
  virtual void Draw() {}

private:
  std::weak_ptr<RB::Body> body;

};

#endif