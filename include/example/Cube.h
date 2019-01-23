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

  glm::vec4 colour;

private:
  static std::unique_ptr<ngl::AbstractVAO> bvVAO;
};

#endif