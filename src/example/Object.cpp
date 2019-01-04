#include "Object.h"

#include <RB.h>

Object::Object(std::weak_ptr<RB::Body> _body)
{
  AssignBody(_body);
}

Object::~Object()
{
  if (!body.expired()) body.lock()->kill();
}

void Object::AssignBody(std::weak_ptr<RB::Body> _body)
{
  body = _body;
}