#ifndef CAMERA_H_
#define CAMERA_H_

#include "Object.h"

class Camera : public Object
{
public:

  void Update();
  void onCreation();

  static glm::mat4 getVP();
  static glm::mat4 getV();
  static glm::mat4 getP();

  void setFOV(float _fov) { fov = _fov; }
  void setNear(float _n) { nearPlane = _n; }
  void setFar(float _f) { farPlane = _f; }

  //Will rotate the camera by given Euler angle in degrees
  void RotateBy(glm::vec3 _eulerDegrees);
  //Moves the camera by _deltaPosition units in WS axis
  void MoveBy(glm::vec3 _deltaPosition);
  //Moves the camera by _deltaPosition units in LS axis
  void LocalMoveBy(glm::vec3 _deltaPosition);

  //Emits objects with initial force impulse in direction of camera
  //forward vector - optional torque setting is allowed
  void ShootSphere(
    float _fScale, float _mass = 1.0f, glm::vec3 _torque = { 0,0,0 });
  void ShootCube(
    float _fScale, float _mass = 1.0f, glm::vec3 _torque = { 0,0,0 });

private:
  float fov;
  float nearPlane;
  float farPlane;

  static Camera *singleton;
};

#endif