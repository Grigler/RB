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
  void setSensitivity(float _s) { mouseSensitivity = _s; }
  //Will rotate the camera by given Euler angle in degrees
  void RotateBy(glm::vec3 _eulerDegrees);
  //Rotates the camera by interpreting relative mouse motion in XY screenspace axis
  //this is scaled by mouseSensitivity
  void RotateBy(glm::vec2 _mouseXY);
  //Moves the camera by _deltaPosition units in WS axis
  //this is scaled by moveSensitivity
  void MoveBy(glm::vec3 _deltaPosition);
  //Moves the camera by _deltaPosition units in LS axis
  //this is scaled by moveSensitivity
  void LocalMoveBy(glm::vec3 _deltaPosition);

  //Emits objects with initial force impulse in direction of camera
  //forward vector - optional torque setting is allowed
  void ShootSphere(
    float _fScale, float _mass = 1.0f, glm::vec3 _torque = { 0,0,0 });
  void ShootCube(
    float _fScale, float _mass = 1.0f, glm::vec3 _torque = { 0,0,0 });

  //Reads KeyInput for relevant input to move camera
  void UpdateMovement();

private:
  float fov;
  float nearPlane;
  float farPlane;
  float mouseSensitivity = 0.05f;
  float moveSensitivity = 0.05f;

  static Camera *singleton;
};

#endif