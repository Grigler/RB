#include "Object.h"

#include <RB.h>

/*
Object::Object(std::weak_ptr<RB::Body> _body, glm::vec3 _pos)
{
  AssignBody(_body);
}
*/
/*
Object::Object(glm::vec3 _pos)
{
  transform.pos = _pos;
}
*/

Object::~Object()
{
  if (!body.expired()) body.lock()->kill();
}

void Object::AssignBody(std::weak_ptr<RB::Body> _body)
{
  body = _body;
}