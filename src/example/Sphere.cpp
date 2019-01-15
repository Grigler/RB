#include "Sphere.h"

#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"

#include <RB.h>

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>

std::unique_ptr<ngl::AbstractVAO> Sphere::bvVAO;

void Sphere::onCreation()
{
  //ngl::VAOPrimitives::createSphere("sphereobj", 1.0f, 4);
  if (ngl::VAOPrimitives::instance()->getVAOFromName("sphere") == nullptr)
  {
    ngl::VAOPrimitives::instance()->createSphere("sphere", 1.0f, 64);
    bvVAO = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_LINES);
  }
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
  (*shader)["Basic"]->use();

  //bind vao
  vao->bind();
    //use shader prog
      //calc uniforms
      //glm::mat4 mv = Camera::getV() * transform.getModelMat();
      glm::mat4 mvp = Camera::getVP() * transform.getModelMat();
      

      //set uniforms
      //shader->setUniform("MV", ngl::mv);
      shader->setUniformMatrix4fv("MVP", &mvp[0][0]);
      shader->setUniform("colour", colour.r, colour.g, colour.b, colour.a);

      //shader->setUniform("normalMatrix",
      //  glm::transpose(glm::inverse(transform.getModelMat())));

      //draw calls
      ngl::VAOPrimitives::instance()->getVAOFromName("sphere")->draw();
    //unbind shader prog
  //unbind vao
  vao->unbind();

  if (Renderer::isDrawingDebug)
  {
    glDisable(GL_DEPTH_TEST);
    //generating BV verts
    glm::vec3 minLS = body.lock()->boundingBox->getLocalMin();
    glm::vec3 extentLS = -minLS + (body.lock()->boundingBox->getLocalMax());

    glm::vec3 ex = glm::vec3(extentLS.x, 0.0f, 0.0f);
    glm::vec3 ey = glm::vec3(0.0f, extentLS.y, 0.0f);
    glm::vec3 ez = glm::vec3(0.0f, 0.0f, extentLS.z);
    
    //This is disgusting and could be done in a geometry shader -
    //depends where you want to shovel your shit really
    std::array<glm::vec3,24> lines = {
      minLS, minLS + ex,
      minLS + ex, minLS + ex + ez,
      minLS, minLS + ez,
      minLS + ez, minLS + ez + ex,

      minLS, minLS + ey,
      minLS + ey, minLS + ey + ez,
      minLS + ez, minLS + ey + ez,

      minLS + ex, minLS + ex + ey,
      minLS + ex + ey, minLS + ex + ey + ez,
      minLS + ex + ez, minLS + ex + ey + ez,

      minLS + ey + ez, minLS + ex + ey + ez,
      minLS + ey, minLS + ey + ex
    };

    //set vao to AABB relevent
    (*shader)["Basic"]->use();
    bvVAO->bind();

      bvVAO->setData(
        ngl::AbstractVAO::VertexData(
          lines.size() * sizeof(glm::vec3),
          lines[0].x,
          GL_STREAM_DRAW)
      );
      bvVAO->setNumIndices(lines.size());
      bvVAO->setVertexAttributePointer(0, 3, GL_FLOAT, 0, 0);
      //uniforms
      //shader->setUniform("colour", 1.0f, 0.0f, 1.0f, 1.0f);
      shader->setUniform("colour", colour.r, colour.g, colour.b, 1.0f);
      shader->setUniformMatrix4fv("MVP",
        &mvp[0][0]);

      //draw call
      bvVAO->draw();
    bvVAO->unbind();
    glEnable(GL_DEPTH_TEST);
  }
}