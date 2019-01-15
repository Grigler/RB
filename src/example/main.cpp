#include <iostream>

#include <ngl/NGLInit.h>
#include <sdl/SDL.h>
#include <RB.h>

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"

#include <chrono>

int main(int argc, char **argv)
{
  NGL_UNUSED(argc);
  NGL_UNUSED(argv);

  Renderer::Startup();

  //Basic Scene Creation
  Scene s;
  /*
  for (size_t i = 0; i < 1; i++)
  {
    s.AddObject<Sphere>();
  }
  */

  std::weak_ptr<Camera> camera = s.AddObject<Camera>();

  std::weak_ptr<Sphere> sphereRight = 
    s.AddObject<Sphere>(glm::vec3(-10.0f, 0.0f, -20.0f), true);
  std::weak_ptr<Sphere> sphereLeft =
    s.AddObject<Sphere>(glm::vec3(10.0f, 0.0f, -20.0f), true);
  
  //Setup initial test collision
  sphereRight.lock()->body.lock()->applyForceImpulse(glm::vec3(5.0f, 5.0f, 0.0f));
  sphereLeft.lock()->body.lock()->applyForceImpulse(glm::vec3(-5.0f, 5.0f, 0.0f));

  bool isUpdating = false;

  SDL_Event e;
  bool isRunning = true;
  while (isRunning)
  {
    //Input handling
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
        case SDLK_r: isUpdating = !isUpdating; break;
        }
      }
    }

    //Scene Update
    if(isUpdating) s.Update();

    //render here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //Scene draw
      s.Draw();

    Renderer::SwapBuffers();
    
    //shitty vsync
    //SDL_Delay(8);
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}