#ifndef RENDERER_H_
#define RENDERER_H_

#include <sdl/SDL.h>
#include <string>

class Renderer
{
public:
  static void SwapBuffers();

  static void Startup();
  static void ShutDown();

private:
  static SDL_Rect screenRect;
  static SDL_Window *window;
  static SDL_GLContext glContext;

  static void sdlError(const std::string &_msg);
};

#endif