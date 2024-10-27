#pragma once

#include <SDL3/SDL.h>
#include "Window/window.h"

class Render
{
private:
  SDL_Renderer *renderer;

public:
  Render() : renderer(NULL) {};
  ~Render();

  void setWindow(Window &window);

  void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
  void draw();
};