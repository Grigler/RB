#include <Body.h>

using namespace RB;

Body::Body()
{
  boundingBox = std::make_unique<AABB>();
}
Body::Body(glm::vec3 _position, glm::quat _orientation)
{
  position = _position;
  orientation = _orientation;
}