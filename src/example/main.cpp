#include <iostream>

#include <ngl/NGLInit.h>
#include <sdl/SDL.h>
#include <RB.h>

#include "Renderer.h"
#include "GameClock.h"

#include <chrono>

void InitNGL();

SDL_GLContext createOpenGLContext(SDL_Window *window);
void SDLErrorExit(const std::string &_msg);

int main(int argc, char **argv)
{
  NGL_UNUSED(argc);
  NGL_UNUSED(argv);

  Renderer::Startup();
  
  //RB INIT
  RB::World world;
  std::vector<std::weak_ptr<RB::Body>> bodies;
  for (size_t i = 0; i < 1; i++)
  {
    bodies.push_back(world.AddBody(glm::vec3(i),glm::vec3(0)));
    bodies.back().lock()->applyForceImpulse(glm::vec3(0.0f, -1.0f, 0.0f));
  }

  SDL_Event e;
  bool isRunning = true;
  while (isRunning)
  {
    //Updating timer
    GameClock::UpdateDT();
    printf("\rFPS: %.2f\tBodies: %zu", 1.0f / GameClock::dt, bodies.size());
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
        case SDLK_ESCAPE: isRunning = false; break;
        case SDLK_SPACE: ngl::NGLMessage::addMessage("This is a test", Colours::RED); break;
        case SDLK_UP: for(int i = 0; i < 25; i++)bodies.push_back(world.AddBody()); break;
        case SDLK_r: world.Kill(); bodies.clear(); break;
        }
      }
    }
    
    //logic
    world.Tick(GameClock::dt);

    //render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    for (size_t i = 0; i < bodies.size(); i++)
    {
      printf("B: %i\n\t%f.0f\n",i,bodies.at(i).lock()->position.y);
    }
    */
    Renderer::SwapBuffers();
    
    //shitty vsync
    //SDL_Delay(8);
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}

void InitNGL()
{
  //NGLWindow
  ngl::NGLInit::instance();
  
  glClearColor(0.33f, 0.33f, 0.33f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

SDL_GLContext createOpenGLContext(SDL_Window *window)
{
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