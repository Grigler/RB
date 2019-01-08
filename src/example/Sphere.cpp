#include "Sphere.h"

#include "Camera.h"
#include "Scene.h"

#include <RB.h>

Sphere::Sphere()
{
  std::weak_ptr<RB::Body> b = Scene::world->AddBody();
  AssignBody(b);
}

void Sphere::Update()
{
  //Retrieving body data
  transform.pos = body.lock()->position;
  transform.rot = body.lock()->getRotationEuler();
}

void Sphere::Draw()
{
  //bind vao
    //use shader prog
      //calc mvp
      glm::mat4 mvp = Camera::getVP() * transform.getModelMat();
      //set uniforms

      //draw calls
    //unbind shader prog
  //unbind vao
}