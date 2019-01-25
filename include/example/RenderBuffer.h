#ifndef RENDER_BUFFER_H_
#define RENDER_BUFFER_H_

#include <memory>

#include <ngl/NGLInit.h>

class RenderBuffer
{
public:
  void Init();
  void Bind();
  void DrawToQuad();
private:

  GLuint frameBufferID;
  GLuint depthBufferID;
  GLuint textureID;
  GLuint quadVAO;

  GLsizei x,y;

  std::unique_ptr<ngl::AbstractVAO> vao;

};

#endif