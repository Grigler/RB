//#define NO_DLL
#include <ngl/NGLInit.h>
#include <sdl/SDL.h>
#include <iostream>

void InitNGL();

SDL_GLContext createOpenGLContext(SDL_Window *window);
void SDLErrorExit(const std::string &_msg);

int main(int argc, char **argv)
{
  NGL_UNUSED(argc);
  NGL_UNUSED(argv);

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDLErrorExit("! Unable to init sdl2: ");
  }

  SDL_Rect rect;
  SDL_GetDisplayBounds(0, &rect);

  SDL_Window *window = SDL_CreateWindow("RB",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    rect.w / 2,
    rect.h / 2,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );

  if (!window)
  {
    SDLErrorExit("! Unable to create SDL window: ");
  }

  SDL_GLContext glContext = createOpenGLContext(window);
  if (!glContext)
  {
    SDLErrorExit("! Unable to create GL context: ");
  }

  SDL_GL_MakeCurrent(window, glContext);
  SDL_GL_SetSwapInterval(1);
  
  //NGL INIT
  InitNGL();

  SDL_GL_SwapWindow(window);

  SDL_Event e;
  bool isRunning = true;
  while (isRunning)
  {
    //Input
    while (SDL_PollEvent(&e))
    {
      switch (e.type)
      {
      case SDL_QUIT: isRunning = false; break;
      case SDL_WINDOWEVENT: break;

      case SDL_KEYDOWN:
        switch(e.key.keysym.sym)
        {
        case SDLK_ESCAPE: isRunning = false;
        case SDLK_SPACE: ngl::NGLMessage::addMessage("This is a test", Colours::RED); break;
        }
      }
    }
    //logic
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //render here
    SDL_GL_SwapWindow(window);
    
    //shitty vsync
    _sleep(16);
  }

  SDL_Quit();

  return EXIT_SUCCESS;
}

void InitNGL()
{
  //NGLWindow
  ngl::NGLInit::instance();
  
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

SDL_GLContext createOpenGLContext(SDL_Window *window)
{
#ifdef __APPLE__
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
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr << _msg << "\n";
  std::cerr << SDL_GetError() << "\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}