#include "Camera.h"

#include "Renderer.h"

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
  //DEBUG
  //printf("Camera: %p\n", singleton);
}

glm::mat4 Camera::getVP()
{
  //Just returning identity matrix if there is no camera
  if (singleton == nullptr) return glm::mat4(1);

  return getV() * getP();
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

  return 
    glm::perspectiveFov(singleton->fov,
      Renderer::getScreenWidth(), Renderer::getScreenHeight(),
      singleton->nearPlane, singleton->farPlane);
}