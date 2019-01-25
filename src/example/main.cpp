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
#include "KeyInput.h"

#include <chrono>

void InitBoxScene();
void InitPlinko();

int main(int argc, char **argv)
{
  NGL_UNUSED(argc);
  NGL_UNUSED(argv);

  Renderer::Startup();

  //Basic Scene Creation
  Scene *scene = Scene::instance();
  RB::IntegratorFactory::setGlobal(RB::Integrators::SemiImplicitEuler);

  std::shared_ptr<Camera> camera = Scene::instance()->AddObject<Camera>().lock();
  camera->transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);

  InitPlinko();
  camera->transform.pos = glm::vec3(10.0f, 0.0f, -10.0f);
  //InitBoxScene();

  //Used for pausing simulation with spacebar
  bool isUpdating = true;

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


      //Keyboard input
      case SDL_KEYDOWN:
        KeyInput::RegisterKeyDown(e.key.keysym.sym);
        //Putting here to stop accidentally firing off multiple instances
        if(e.key.keysym.sym == SDLK_SPACE) isUpdating = !isUpdating;
        else if(e.key.keysym.sym == SDLK_f) camera->ShootSphere(100.0f,10.0f);
        else if(e.key.keysym.sym == SDLK_g) camera->ShootCube(100.0f,10.0f);
        else if(e.key.keysym.sym == SDLK_BACKSPACE) Renderer::isDrawingDebug = !Renderer::isDrawingDebug;
        else if(e.key.keysym.sym == SDLK_i){ RB::IntegratorFactory::setGlobal(RB::Integrators::ForwardEuler); printf("FE\n");}
        else if(e.key.keysym.sym == SDLK_o) {RB::IntegratorFactory::setGlobal(RB::Integrators::SemiImplicitEuler);printf("SIE\n");}
        break;
      case SDL_KEYUP:
        KeyInput::RegisterKeyUp(e.key.keysym.sym);
        break;
      case SDL_MOUSEMOTION:
        camera->RotateBy(glm::vec2(e.motion.xrel,e.motion.yrel));

        SDL_WarpMouseInWindow(Renderer::getWindow(),
         Renderer::getScreenWidth()/2,
          Renderer::getScreenHeight()/2);
        break;
      }
    }
    //if(KeyInput::CheckKey(SDLK_SPACE)) 
    //printf("fps: %3f\r", (1.0f/GameClock::dt));

    //This is called seperately so camera can move in "freezeframe"
    camera->UpdateMovement();

    if(isUpdating) scene->Update();
    else GameClock::UpdateDT(); //should still update dt
    
    //render here
    Renderer::ClearBuffers();
    //Scene draw
    scene->Draw();
    Renderer::SwapBuffers();
    
    //shitty vsync
    //SDL_Delay(8);

    if(KeyInput::CheckKey(SDLK_ESCAPE)) isRunning = false;
  }

  Renderer::ShutDown();

  return EXIT_SUCCESS;
}

void InitBoxScene()
{
  Scene *scene = Scene::instance();

  std::shared_ptr<Cube> bottomWall = scene->AddObject<Cube>({ 0,0,0 }, true).lock();
  bottomWall->body.lock()->SetMass(0.0f);
  bottomWall->transform.scale = glm::vec3(10.0f, 2.0f, 10.0f);
  bottomWall->ResetColliderToScale();
  bottomWall->colour = { 0.8f,0.4f,0.8f,1.0f };

  std::shared_ptr<Cube> backWall = scene->AddObject<Cube>({ 0,6.0f,6.0f }, true).lock();
  backWall->body.lock()->SetMass(0.0f);
  backWall->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1,0,0)));
  backWall->transform.scale = glm::vec3(10, 2, 10);
  backWall->ResetColliderToScale();
  backWall->colour = { 0.7f, 0.3f, 0.8f, 1.0f };

  std::shared_ptr<Cube> frontWall = scene->AddObject<Cube>({0,6.0f,-6.0f}, true).lock();
  frontWall->body.lock()->SetMass(0.0f);
  frontWall->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1,0,0)));
  frontWall->transform.scale = glm::vec3(10,2,10);
  frontWall->ResetColliderToScale();
  frontWall->colour = { 0.7f, 0.3f, 0.8f, 1.0f };

  std::shared_ptr<Cube> leftWall = scene->AddObject<Cube>({6.0f,6.0f,0.0f}, true).lock();
  leftWall->body.lock()->SetMass(0.0f);
  leftWall->body.lock()->orientation =
    glm::toQuat(
      glm::rotate(
        glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0))
        , glm::radians(90.0f), glm::vec3(1,0,0)
        )
        );
  leftWall->transform.scale = glm::vec3(10,2,10);
  leftWall->ResetColliderToScale();
  leftWall->colour = { 0.6f, 0.2f, 0.7f, 1.0f };

  std::shared_ptr<Cube> rightWall = scene->AddObject<Cube>({-6.0f,6.0f,0.0f}, true).lock();
  rightWall->body.lock()->SetMass(0.0f);
  rightWall->body.lock()->orientation =
    glm::toQuat(
      glm::rotate(
        glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0))
        , glm::radians(90.0f), glm::vec3(1,0,0)
        )
        );
  rightWall->transform.scale = glm::vec3(10,2,10);
  rightWall->ResetColliderToScale();
  rightWall->colour = { 0.6f, 0.2f, 0.7f, 1.0f };
}
void InitPlinko()
{
  constexpr int tiers = 6;
  float offset = 1.0f;
  //Generate box pyramid
  for(int y = -1; y < tiers; y++)
  {
    for(int x = y-5-tiers; x < 4+tiers; x++)
    {
      std::shared_ptr<Cube> c = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
      c->body.lock()->position = glm::vec3(x*5.0f+offset,y*5.0f,20.0f);
      c->transform.scale = glm::vec3(1.5f,1.5f,1.0f);
      //c->body.lock()->orientation =
      //  glm::toQuat(glm::rotate(glm::mat4(1),glm::radians(45.0f),glm::vec3(0,0,1)));
      c->body.lock()->SetMass(0.0f);
      c->ResetColliderToScale();
      c->colour = glm::vec4((rand()%1000)/1000.0f,1.0f,1.0f,1.0f);
    }
    offset += 2.5f;
  }

  std::shared_ptr<Cube> backWall = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
  backWall->body.lock()->position = glm::vec3(-10, 20, 22.5f);
  backWall->transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
  backWall->body.lock()->SetMass(0.0f);
  backWall->ResetColliderToScale();
  backWall->colour = glm::vec4(0.4f,0.6f,0.4f,1.0f);

  std::shared_ptr<Cube> frontWall = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
  frontWall->body.lock()->position = glm::vec3(-10, 20, 17.5f);
  frontWall->transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
  frontWall->body.lock()->SetMass(0.0f);
  frontWall->ResetColliderToScale();
  frontWall->colour = glm::vec4(0.4f,0.6f,0.4f,0.0f);

  std::shared_ptr<Cube> rightWall = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
  rightWall->body.lock()->position = glm::vec3(-10.0f, 20.0f, 20.0f);
  rightWall->body.lock()->SetMass(0.0f);
  rightWall->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0)));
  rightWall->transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
  rightWall->ResetColliderToScale();
  rightWall->colour = glm::vec4(0.3f,0.5f,0.3f,1.0f);
  
  std::shared_ptr<Cube> leftWall = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
  leftWall->body.lock()->position = glm::vec3(25.0f, 20.0f, 20.0f);
  leftWall->body.lock()->SetMass(0.0f);
  leftWall->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0)));
  leftWall->transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
  leftWall->ResetColliderToScale();
  leftWall->colour = glm::vec4(0.3f,0.5f,0.3f,1.0f);

  std::shared_ptr<Cube> floor = Scene::instance()->AddObject<Cube>({0,0,0},true).lock();
  floor->body.lock()->position = glm::vec3(0,-20,20);
  floor->body.lock()->SetMass(0.0f);
  floor->body.lock()->orientation =
    glm::toQuat(glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(1,0,0)));
  floor->transform.scale = glm::vec3(100.0f, 100.0f, 1.0f);
  floor->ResetColliderToScale();
  floor->colour = glm::vec4(0.2f, 0.4f, 0.2f, 1.0f);

  
}