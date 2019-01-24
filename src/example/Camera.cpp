#include "Camera.h"

#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Cube.h"
#include "KeyInput.h"

#include <RB.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

Camera *Camera::singleton = nullptr;

void Camera::onCreation()
{
  transform.pos = glm::vec3(0);
  transform.rot = glm::vec3(0);
  //Arbitrarily small/large values to avoid clipping issues for now
  fov = glm::radians(60.0f);
  nearPlane = 0.001f;
  farPlane = 1000000.0f;

  if (singleton == nullptr) singleton = this;
}

void Camera::Update()
{
}

glm::mat4 Camera::getVP()
{
  //Just returning identity matrix if there is no camera
  if (singleton == nullptr) return glm::mat4(1);

  return getP() * getV();
}

glm::mat4 Camera::getV()
{
  if (singleton == nullptr) return glm::mat4(1);

  return
    glm::lookAtRH(singleton->transform.pos,
      singleton->transform.pos + singleton->transform.getForward(),
      singleton->transform.getUp());
}
glm::mat4 Camera::getP()
{
  if (singleton == nullptr) return glm::mat4(1);

  //return 
  //  glm::perspectiveFovRH(singleton->fov,
  //    Renderer::getScreenWidth(), Renderer::getScreenHeight(),
  //    singleton->nearPlane, singleton->farPlane);
  return
    glm::perspectiveRH(singleton->fov,
      Renderer::getScreenWidth() / Renderer::getScreenHeight(),
      singleton->nearPlane, singleton->farPlane);
}

void Camera::RotateBy(glm::vec3 _eulerDegrees)
{
  transform.rot += glm::radians(_eulerDegrees);
}
void Camera::RotateBy(glm::vec2 _mouseXY)
{
  RotateBy(glm::vec3(_mouseXY.y*mouseSensitivity,-_mouseXY.x*mouseSensitivity,0.0f));
}
void Camera::MoveBy(glm::vec3 _deltaPosition)
{
  transform.pos += _deltaPosition * moveSensitivity;
}
void Camera::LocalMoveBy(glm::vec3 _deltaPosition)
{
  //translate _deltaPosition into LS
  glm::vec3 ls = transform.getRotationMat() * glm::vec4(_deltaPosition,1.0f);
  //Apply translation
  transform.pos += ls * moveSensitivity;
}

void Camera::ShootSphere(float _fScale, float _mass, glm::vec3 _torque)
{
  std::shared_ptr<Sphere> sphere =
    Scene::instance()->AddObject<Sphere>(transform.pos, true).lock();
  sphere->colour = glm::vec4(
    (rand() % 1000) / 1000.0f,
    (rand() % 1000) / 1000.0f,
    (rand() % 1000) / 1000.0f,
    //0.5f
    1.0f
  );
  sphere->body.lock()->SetMass(_mass);
  glm::vec4 localAxisForward = transform.getRotationMat() * glm::vec4(0,0,1,1);
  sphere->body.lock()->applyForceImpulse(localAxisForward * _fScale);
  sphere->body.lock()->applyTorqueImpulse(transform.getRotationMat() * glm::vec4(_torque,1));
}
void Camera::ShootCube(float _fScale, float _mass, glm::vec3 _torque)
{
  std::shared_ptr<Cube> cube =
    Scene::instance()->AddObject<Cube>(transform.pos, true).lock();
  cube->colour = glm::vec4(
    (rand() % 1000) / 1000.0f,
    (rand() % 1000) / 1000.0f,
    (rand() % 1000) / 1000.0f,
    //0.5f
    1.0f
  );
  cube->body.lock()->SetMass(_mass);
  glm::vec4 localAxisForward = transform.getRotationMat() * glm::vec4(0, 0, 1, 1);
  cube->body.lock()->applyForceImpulse(localAxisForward * _fScale);
  cube->body.lock()->applyTorqueImpulse(transform.getRotationMat() * glm::vec4(_torque, 1));
}

void Camera::UpdateMovement()
{
  //Accumulating relevant input and moving in local space
  glm::vec3 moveVec = glm::vec3(0);
  if(KeyInput::CheckKey(SDLK_w))
  {
    moveVec += glm::vec3(0,0,1);
  }
  if(KeyInput::CheckKey(SDLK_s))
  {
    moveVec += glm::vec3(0,0,-1);
  }

  if(KeyInput::CheckKey(SDLK_a))
  {
    moveVec += glm::vec3(1,0,0);
  }
  if(KeyInput::CheckKey(SDLK_d))
  {
    moveVec += glm::vec3(-1,0,0);
  }
  //Function accounts for movement scaling
  LocalMoveBy(moveVec);
}