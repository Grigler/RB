#ifndef CUBE_H_
#define CUBE_H_

#include "Object.h"

#include <ngl/VAOPrimitives.h>

class Cube : public Object
{
public:

  void onCreation();
  void Update();
  void Draw();

  //Resets the collider to the scale of the box
  //rather than half-extents of 1.0f in all axis
  void ResetColliderToScale();

  glm::vec4 colour;

private:
  static std::unique_ptr<ngl::AbstractVAO> bvVAO;
};

#endif