#include "RenderBuffer.h"

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Renderer.h"

void RenderBuffer::Init()
{
  x = Renderer::getScreenWidth();
  y = Renderer::getScreenHeight();

  //Gen and bind framebuffrer
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);


  //glClearColor(75.0f / 255.0f, 0.0f, 130.0f / 255.0f, 1.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  //Gen, bind texutre and define parameters
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  //Gen, binding and defining depthbuffer
  glGenRenderbuffers(1, &depthBufferID);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x, y);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
  
  //Linking texture to framebuffer as GL_COLOR_ATTACHMENT0
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

  //Quad Data
  /*
  std::array<glm::vec3,6> quadVerts=
  {
    glm::vec3(-1.0f, -1.0f, 0.0f),
    glm::vec3(1.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f,  1.0f, 0.0f),

    glm::vec3(-1.0f,  1.0f, 0.0f),
    glm::vec3(1.0f, -1.0f, 0.0f),
    glm::vec3(1.0f,  1.0f, 0.0f)
  };
  std::array<glm::vec2,6> quadUVs =
  {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),

    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f)
  };
  */

  static const GLfloat quadVerts[] =
  {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f
  };
  static const GLfloat quadUVs[] =
  {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,

    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
  };

  //Giving the framebuffer the list of buffers to draw to - only the bound texture
  const GLenum buf[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, buf);

  //Gen VAO
  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);

  //Gen VBO locally - stored in VAO anyway
  GLuint quadVBO;
  glGenBuffers(1, &quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadUVs), quadUVs, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

  /*
  vao = ngl::VAOFactory::createVAO("renderbuffer", GL_TRIANGLES);
  vao->bind();
  vao->setData(ngl::AbstractVAO::VertexData(
      quadVerts.size()*sizeof(glm::vec3),
      quadVerts[0][0],
      GL_STATIC_DRAW));
  vao->setNumIndices(quadVerts.size());
  vao->setVertexAttributePointer(0, 3, GL_FLOAT, 0, 0);

  vao->setData(ngl::AbstractVAO::VertexData(
      quadUVs.size()*sizeof(glm::vec2),
      quadUVs[0][0],
      GL_STATIC_DRAW));
  vao->setNumIndices(quadUVs.size());
  vao->setVertexAttributePointer(1, 2, GL_FLOAT, 0, 0);

  vao->unbind();
  */
}
void RenderBuffer::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, x, y);
}
void RenderBuffer::DrawToQuad()
{
    
  glBindVertexArray(quadVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  //Rendering to the screen now
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, x, y);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glDisable(GL_DEPTH_TEST);
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Gamma"]->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(glGetUniformLocation(shader->getProgramID("Gamma"), "texColour"), 0);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, depthBufferID);
  glUniform1i(glGetUniformLocation(shader->getProgramID("Gamma"), "texDepth"), 1);

  glDrawArrays(GL_TRIANGLES, 0, 6);
    printf("Draw\n");
   /*
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  (*shader)["Gamma"]->use();
  vao->bind();
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(glGetUniformLocation(shader->getProgramID("Gamma"), "texColour"), 0);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, depthBufferID);
  glUniform1i(glGetUniformLocation(shader->getProgramID("Gamma"), "texDepth"), 1);

  vao->draw();
  */
}