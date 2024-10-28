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

void Render::setWindow(Window &window, SDL_Color backgroundColor)
{
  this->renderer = SDL_CreateRenderer(window.getWindow(), NULL);
  if (!this->renderer)
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());

  // Immediately set the color of the window to reduce the black flash.
  this->clear(backgroundColor);
  this->draw();
}

void Render::clear(SDL_Color backgroundColor)
{
  SDL_SetRenderDrawColor(this->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
  SDL_RenderClear(this->renderer);
}

void Render::draw()
{
  if (!SDL_RenderPresent(renderer))
    printf("SDL_RenderPresent Error: %s\n", SDL_GetError());
}

void Render::rect(SDL_FRect &rect, SDL_Color backgroundColor)
{
  SDL_SetRenderDrawColor(this->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
  SDL_RenderFillRect(this->renderer, &rect);
}