#include "Sphere.h"

#include "Camera.h"
#include "Scene.h"

#include <RB.h>

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>

Sphere::Sphere()
{
  std::weak_ptr<RB::Body> b = Scene::world->AddBody();
  AssignBody(b);

  //ngl::VAOPrimitives::createSphere("sphereobj", 1.0f, 4);
  ngl::VAOPrimitives::instance()->createSphere("sphere", 2.0f, 4);
}

void Sphere::Update()
{
  //Retrieving body data
  transform.pos = body.lock()->position;
  transform.rot = body.lock()->getRotationEuler();
}

void Sphere::Draw()
{
  //get vao
  ngl::AbstractVAO *vao = ngl::VAOPrimitives::instance()->getVAOFromName("sphere");
  //get and bind shader
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  //bind vao
  vao->bind();
    //use shader prog
      //calc uniforms
      glm::mat4 mv = Camera::getV() * transform.getModelMat();
      glm::mat4 mvp = Camera::getVP() * transform.getModelMat();
      

      //set uniforms
      //shader->setUniform("MV", ngl::mv);
      //shader->setUniform("MVP", mvp);
      //shader->setUniform("normalMatrix",
      //  glm::transpose(glm::inverse(transform.getModelMat())));

      //draw calls
      ngl::VAOPrimitives::instance()->getVAOFromName("sphere")->draw();
    //unbind shader prog
  //unbind vao
  vao->unbind();
}