#ifndef SPHERE_EMITTER_H_
#define SPHERE_EMITTER_H_

#include "Object.h"

#include <glm/vec4.hpp>

class SphereEmitter : public Object
{
public:
  void onCreation();
  void Update();
  void Draw();

  void EmitBall();
private:
  float ballTimer = 0.0f;
  float emitFrequency = 1.0f;
  float initX = 0.0f;
  glm::vec4 colour = {1.0f,0.2f,0.2f,1.0f};
};

#endif