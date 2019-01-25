#include "SphereEmitter.h"

#include <RB.h>

#include "GameClock.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"
#include "Renderer.h"

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>

void SphereEmitter::onCreation()
{
  //Stored for oscillation base
  initX = transform.pos.x;
  transform.scale = glm::vec3(7.0f, 4.0f, 4.0f);
}

void SphereEmitter::Update()
{
  ballTimer += GameClock::dt;

  transform.pos.x = initX + 8.0f*glm::sin(GameClock::elapsedT*0.75f);

  if(ballTimer > emitFrequency)
  {
      ballTimer -= emitFrequency;
      EmitBall();
  }
}
void SphereEmitter::Draw()
{
  //Early out to avoid blending issues
  if(colour.a == 0.0f) return;

  //get vao
  ngl::AbstractVAO *vao = ngl::VAOPrimitives::instance()->getVAOFromName(ngl::cube);
  //get and bind shader
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Basic"]->use();

  //bind vao
  vao->bind();

  //set uniforms
  glm::mat4 mvp = Camera::getVP() * transform.getModelMat();
  shader->setUniformMatrix4fv("MVP", &mvp[0][0]);

  shader->setUniform("colour", colour.r, colour.g, colour.b, colour.a);

  //draw calls
  vao->draw();
  
  //unbind vao
  vao->unbind();
}

void SphereEmitter::EmitBall()
{
  static glm::vec3 rgb = glm::vec3(0.0f);

  std::shared_ptr<Sphere> s = Scene::instance()->AddObject<Sphere>(transform.pos,true).lock();
  s->body.lock()->SetMass(1.0f);
  s->colour = glm::vec4(rgb,1.0f);
  rgb += glm::vec3(0.01f);
  if(rgb.r >= 1.0f) rgb = glm::vec3(0.0f);
}