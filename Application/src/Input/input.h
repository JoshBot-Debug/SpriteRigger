#pragma once

#include <SDL3/SDL.h>
#include "mouse.h"

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