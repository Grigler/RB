#include "KeyInput.h"

std::vector<SDL_Keycode> KeyInput::keysDown;

bool KeyInput::CheckKey(SDL_Keycode _key)
{
  for(auto k = keysDown.begin(); k < keysDown.end(); k++)
  {
    if((*k) == _key) return true;
  }
  return false;
}

void KeyInput::RegisterKeyUp(SDL_Keycode _key)
{
  for(auto k = keysDown.begin(); k < keysDown.end(); k++)
  {
    if((*k) == _key)
    {
      keysDown.erase(k);
      return;
    }
  }
}

void KeyInput::RegisterKeyDown(SDL_Keycode _key)
{
  //In case it sends multiple down events
  for(auto k = keysDown.begin(); k < keysDown.end(); k++)
  {
      if((*k) == _key) return;
  }
  keysDown.push_back(_key);
}