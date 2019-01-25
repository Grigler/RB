#include "Cube.h"

#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"

#include <RB.h>

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>

std::unique_ptr<ngl::AbstractVAO> Cube::bvVAO;

void Cube::onCreation()
{
  static bool hasInitVAO = false;
  if (!hasInitVAO)
  {
    bvVAO = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_LINES);
    hasInitVAO = true;
  }
  /*
  if (ngl::VAOPrimitives::instance()->getVAOFromName("cube") == nullptr)
  {
    //ngl::VAOPrimitives::instance()->createSphere("sphere", 1.0f, 64);
    
    bvVAO = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_LINES);
  }
  */

  body.lock()->collider = std::make_unique<RB::GreedyCollider>(glm::vec3(1.0f));
  body.lock()->CalcInertiaTensorBox(glm::vec3(1.0f));
  body.lock()->collider->parent = body;
}
void Cube::Update()
{
  //Retrieving body data
  transform.pos = body.lock()->position;
  transform.rot = body.lock()->getRotationEuler();
  body.lock()->SetBVScale(transform.scale);
}
void Cube::Draw()
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

  if (Renderer::isDrawingDebug)
  {
    //glDisable(GL_DEPTH_TEST);
    //generating BV verts - using localSpace as it is transformed by MVP in shader
    glm::vec3 minLS = body.lock()->boundingBox->getLocalMin();
    glm::vec3 extentLS = -minLS + (body.lock()->boundingBox->getLocalMax());

    glm::vec3 ex = glm::vec3(extentLS.x, 0.0f, 0.0f);
    glm::vec3 ey = glm::vec3(0.0f, extentLS.y, 0.0f);
    glm::vec3 ez = glm::vec3(0.0f, 0.0f, extentLS.z);

    //This is disgusting and could be done in a geometry shader -
    //depends where you want to shovel your shit really
    std::array<glm::vec3, 24> lines = {
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
    if (body.lock()->boundingBox->collisionFlag)
    {
      shader->setUniform("colour", 1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
      shader->setUniform("colour", 0.0f, 1.0f, 0.0f, 1.0f);
    }
    //shader->setUniform("colour", colour.r, colour.g, colour.b, 1.0f);

    shader->setUniformMatrix4fv("MVP",
      &mvp[0][0]);

    //draw call
    bvVAO->draw();
    bvVAO->unbind();
    //glEnable(GL_DEPTH_TEST);
  }
}

void Cube::ResetColliderToScale()
{
  //divide by 2.0f because it's half-extents
  glm::vec3 halfExtents = transform.scale / 2.0f;
  body.lock()->collider = std::make_unique<RB::GreedyCollider>(halfExtents);
  body.lock()->CalcInertiaTensorBox(halfExtents);
  body.lock()->collider->parent = body;
}