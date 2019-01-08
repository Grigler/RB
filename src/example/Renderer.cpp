#include "Renderer.h"

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>

#include <iostream>

SDL_Rect Renderer::screenRect;
SDL_Window *Renderer::window;
SDL_GLContext Renderer::glContext;

void Renderer::SwapBuffers()
{
  SDL_GL_SwapWindow(window);
}

void Renderer::Startup()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    sdlError("! Unable to init SDL2: ");
  }

  SDL_GetDisplayBounds(0, &screenRect);

  window = SDL_CreateWindow("RB",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    screenRect.w / 2,
    screenRect.h / 2,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );

  if (!window)
  {
    sdlError("! Unable to create SDL window: ");
  }

#ifdef __APPLE__
  //If only Apple didn't have horrible business practices
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  glContext = SDL_GL_CreateContext(window);
  if (!glContext)
  {
    sdlError("! Unable to create GL context: ");
  }

  SDL_GL_MakeCurrent(window, glContext);
  SDL_GL_SetSwapInterval(1);

  ngl::NGLInit::instance();
  glClearColor(0.33f, 0.33f, 0.33f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //TODO - REMOVE DEBUG
  //Creating Phong DEBUG SHADER
  //Creating Phong Shader
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->createShaderProgram("Phong");
  
  shader->attachShader("PhongVertex", ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment", ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("PhongVertex", "data/shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment", "data/shaders/PhongFragment.glsl");

  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");

  shader->attachShaderToProgram("Phong", "PhongVertex");
  shader->attachShaderToProgram("Phong", "PhongFragment");

  shader->linkProgramObject("Phong");
  //!DEBUG
}

void Renderer::ShutDown()
{
  SDL_Quit();
}

void Renderer::sdlError(const std::string &_msg)
{
  std::cerr << _msg << "\n";
  std::cerr << SDL_GetError() << "\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}