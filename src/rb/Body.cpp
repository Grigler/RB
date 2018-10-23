#include <Body.h>

using namespace RB;

Body::Body()
{
  boundingBox = std::make_shared<AABB>(selfRef);
}
Body::Body(glm::vec3 _position, glm::quat _orientation)
{
  position = _position;
  orientation = _orientation;
}