#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"

#include <ngl/VAOPrimitives.h>

class Sphere : public Object
{
public:

  void onCreation();
  void Update();
  void Draw();

  glm::vec4 colour;

private:
  static std::unique_ptr<ngl::AbstractVAO> bvVAO;

};

#endif