#include "GameClock.h"

float GameClock::dt = 0.0f;
float GameClock::elapsedT = 0.0f;

Uint64 GameClock::tNow = 0;
Uint64 GameClock::tLast = 0;

void GameClock::UpdateDT()
{
  static bool firstRun = true;
  if (firstRun)
  {
    tNow = SDL_GetPerformanceCounter();
    firstRun = false;
  }

  tLast = tNow;
  tNow = SDL_GetPerformanceCounter();
  dt = (((tNow - tLast)*1000.0f) / SDL_GetPerformanceFrequency())*0.001f;
  elapsedT += dt;
}