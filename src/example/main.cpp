#include <iostream>

#include <ngl/NGLInit.h>
#include <sdl/SDL.h>
#include <RB.h>

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Cube.h"
#include "GameClock.h"

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


  //Setting up initial test collision - TEMPORARY
  std::weak_ptr<Sphere> sphereRight = 
    scene.AddObject<Sphere>(glm::vec3(-8.0f, 5.8f, 15.0f), true);
  sphereRight.lock()->colour = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);

  
  std::weak_ptr<Sphere> sphereLeft =
    scene.AddObject<Sphere>(glm::vec3(10.0f, 5.0f, 15.0f), true);
  sphereLeft.lock()->colour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
  /*
  std::weak_ptr<Sphere> sphereLeft1 =
    scene.AddObject<Sphere>(glm::vec3(8.0f, 12.0f, -15.0f), true);
  sphereLeft1.lock()->colour = glm::vec4(0.0f, 1.0f, 1.0f, 0.5f);
  */
  for (size_t i = 0; i < 100; i++)
  {
    std::weak_ptr<Sphere> sphere =
      scene.AddObject<Sphere>(glm::vec3(8.0f, 5.0f*i, 15.0f), true);
    float r = (rand() % 1000) / 1000.0f,
      g = (rand() % 1000) / 1000.0f,
      b = (rand() % 1000) / 1000.0f;
    sphere.lock()->colour = glm::vec4(r, g, b, 0.5f);
  }
  //sphereLeft.lock()->body.lock()->orientation = glm::rotation

  //sphereRight.lock()->body.lock()->applyForceImpulse(2.0f*glm::vec3(9.0f, 2.5f, 1.0f));
  //sphereLeft.lock()->body.lock()->applyForceImpulse(2.0f*glm::vec3(-9.0f, 2.5f, 1.0f));
  
  //sphereRight.lock()->body.lock()->applyTorqueImpulse(glm::vec3(0.0f,0.0f,-90.0f));
  //sphereLeft.lock()->body.lock()->applyTorqueImpulse(glm::vec3(0.0f,0.0f,90.0f));
  
  std::weak_ptr<Cube> cube =
    scene.AddObject<Cube>(glm::vec3(-8.0f, -2.0f, 15.0f), true);
    //scene.AddObject<Cube>(glm::vec3(0.0f, 7.0f, -15.0f), true);
  cube.lock()->colour = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);
  cube.lock()->transform.scale = glm::vec3(2.0f);
  cube.lock()->body.lock()->SetMass(0.0f);

  std::weak_ptr<Cube> cube2 =
    scene.AddObject<Cube>(glm::vec3(8.0f, -2.0f, 15.0f), true);
  //scene.AddObject<Cube>(glm::vec3(0.0f, 7.0f, -15.0f), true);
  cube2.lock()->colour = glm::vec4(0.0f, 0.0f, 1.0f, 0.5f);
  cube2.lock()->transform.scale = glm::vec3(2.0f);
  cube2.lock()->body.lock()->SetMass(0.0f);

  //Used for pausing simulation with spacebar
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
        case SDLK_SPACE: isUpdating = !isUpdating; break;
        case SDLK_BACKSPACE: Renderer::isDrawingDebug = !Renderer::isDrawingDebug; break;
        case SDLK_EQUALS:
          sphereLeft.lock()->colour.a += 0.2f;
          sphereRight.lock()->colour.a += 0.2f;
          cube.lock()->colour.a += 0.2f;
          break;
        case SDLK_MINUS:
          sphereLeft.lock()->colour.a -= 0.2f;
          sphereRight.lock()->colour.a -= 0.2f;
          cube.lock()->colour.a -= 0.2f;
          break;
        }
      }
    }

    //printf("fps: %3f\r", (1.0f/GameClock::dt));

    if(isUpdating) scene.Update();
    else GameClock::UpdateDT(); //should still update dt

    //render here
    Renderer::ClearBuffers();
    //Scene draw
    scene.Draw();
    Renderer::SwapBuffers();
    
    //shitty vsync
    SDL_Delay(8);
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}