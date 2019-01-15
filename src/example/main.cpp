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
  Scene scene;
  /*
  for (size_t i = 0; i < 1; i++)
  {
    s.AddObject<Sphere>();
  }
  */

  std::weak_ptr<Camera> camera = scene.AddObject<Camera>();

  std::weak_ptr<Sphere> sphereRight = 
    scene.AddObject<Sphere>(glm::vec3(-10.0f, 0.0f, -20.0f), true);
  sphereRight.lock()->colour = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
  std::weak_ptr<Sphere> sphereLeft =
    scene.AddObject<Sphere>(glm::vec3(10.0f, 0.0f, -20.0f), true);
  sphereLeft.lock()->colour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);

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
        case SDLK_BACKSPACE: Renderer::isDrawingDebug = !Renderer::isDrawingDebug; break;
        case SDLK_EQUALS:
          sphereLeft.lock()->colour.a += 0.2f;
          sphereRight.lock()->colour.a += 0.2f;
          break;
        case SDLK_MINUS:
          sphereLeft.lock()->colour.a -= 0.2f;
          sphereRight.lock()->colour.a -= 0.2f;
          break;
        }
      }
    }

    //Scene Update
    if(isUpdating) scene.Update();

    //render here
    Renderer::ClearBuffers();
    //Scene draw
    scene.Draw();
    Renderer::SwapBuffers();
    
    //shitty vsync
    //SDL_Delay(8);
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}