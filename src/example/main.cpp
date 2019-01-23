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
  Scene *scene = Scene::instance();

  std::shared_ptr<Camera> camera = scene->AddObject<Camera>().lock();
  camera->transform.pos = { 0.0f, 5.0f, -10.0f };

  std::shared_ptr<Cube> bottomWall = scene->AddObject<Cube>({ 0,0,0 }, true).lock();
  bottomWall->body.lock()->SetMass(0.0f);
  bottomWall->transform.scale = glm::vec3(10.0f, 2.0f, 10.0f);
  bottomWall->ResetColliderToScale();
  bottomWall->colour = { 0.8f,0.4f,0.8f,1.0f };  

  std::shared_ptr<Cube> backWall = scene->AddObject<Cube>({ 0,5.0f,5.0f }, true).lock();
  backWall->body.lock()->SetMass(0.0f);
  backWall->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1,0,0)));
  backWall->transform.scale = glm::vec3(10, 2, 10);
  backWall->ResetColliderToScale();
  backWall->colour = { 0.7f, 0.3f, 0.8f, 1.0f };

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
        case SDLK_a:
          camera->RotateBy(glm::vec3(0, 2, 0));
          break;
        case SDLK_d:
          camera->RotateBy(glm::vec3(0, -2, 0));
          break;
        case SDLK_w:
          camera->RotateBy(glm::vec3(-2, 0, 0));
          break;
        case SDLK_s:
          camera->RotateBy(glm::vec3(2, 0, 0));
          break;
        case SDLK_f:
          camera->ShootCube(20.0f);
          break;
        case SDLK_h:
          camera->ShootSphere(20.0f);
          break;
        }
      }
    }

    //printf("fps: %3f\r", (1.0f/GameClock::dt));

    if(isUpdating) scene->Update();
    else GameClock::UpdateDT(); //should still update dt

    //render here
    Renderer::ClearBuffers();
    //Scene draw
    scene->Draw();
    Renderer::SwapBuffers();
    
    //shitty vsync
    SDL_Delay(8);
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}