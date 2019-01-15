#ifndef RENDERER_H_
#define RENDERER_H_

#include <sdl/SDL.h>
#include <string>

class Renderer
{
public:
  static void SwapBuffers();
  static void ClearBuffers();

  static void Startup();
  static void ShutDown();

  //Have to deal with conversion of int to float to play nice with glm
  //it's that or dirty casting in some of the renderer functions
  static float getScreenWidth() { return screenRect.w; }
  static float getScreenHeight() { return screenRect.h; }

  static bool isDrawingDebug;

private:
  static SDL_Rect screenRect;
  static SDL_Window *window;
  static SDL_GLContext glContext;

  static void sdlError(const std::string &_msg);
};

#endif