#ifndef GAMECLOCK_H_
#define GAMECLOCK_H_

#include <sdl/SDL.h>

class GameClock
{
public:

  static float dt;

  static void UpdateDT();

private:
  static Uint64 tNow;
  static Uint64 tLast;
};

#endif