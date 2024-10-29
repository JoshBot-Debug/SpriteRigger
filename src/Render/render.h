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

  void setWindow(Window &window, SDL_Color backgroundColor = {255, 255, 255, SDL_ALPHA_OPAQUE});
  SDL_Renderer *getRenderer();

  void clear(SDL_Color backgroundColor);
  void draw();
  void rect(SDL_FRect &rect, SDL_Color backgroundColor);
};