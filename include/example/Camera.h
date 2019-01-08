#ifndef CAMERA_H_
#define CAMERA_H_

#include "Object.h"

class Camera : public Object
{
public:
  Camera();

  void Update();

  static glm::mat4 getVP();
  static glm::mat4 getV();
  static glm::mat4 getP();

  void setFOV(float _fov) { fov = _fov; }
  void setNear(float _n) { nearPlane = _n; }
  void setFar(float _f) { farPlane = _f; }

private:
  float fov;
  float nearPlane;
  float farPlane;

  static Camera *singleton;
};

#endif