#pragma once

#include <SDL3/SDL.h>
#include "Mouse.h"

class Input
{
private:
  Mouse mouse;

public:
  Input() = default;
  ~Input() = default;

  void onEvent(SDL_Event *event);

  Mouse *getMouse();
};