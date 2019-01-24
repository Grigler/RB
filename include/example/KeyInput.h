#ifndef KEY_INPUT_H_
#define KEY_INPUT_H_

#include <vector>

#include <sdl/SDL.h>

class KeyInput
{
public:
  static bool CheckKey(SDL_Keycode _key);

  static void RegisterKeyUp(SDL_Keycode _key);
  static void RegisterKeyDown(SDL_Keycode _key);
private:
  static std::vector<SDL_Keycode> keysDown;
};

#endif