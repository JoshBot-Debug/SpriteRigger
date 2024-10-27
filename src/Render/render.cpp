#include <stdio.h>
#include <sstream>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_time.h>

#include "render.h"

Render::~Render()
{
  SDL_DestroyRenderer(this->renderer);
  this->renderer = NULL;
}

void Render::setWindow(Window &window)
{
  this->renderer = SDL_CreateRenderer(window.getWindow(), NULL);
  if (!this->renderer)
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
}

void Render::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
  SDL_RenderClear(this->renderer);
}

void Render::draw()
{
  if (!SDL_RenderPresent(renderer))
    printf("SDL_RenderPresent Error: %s\n", SDL_GetError());
}