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

void Render::setWindow(Window &window, SDL_Color background_color)
{
  this->renderer = SDL_CreateRenderer(window.getWindow(), NULL);
  if (!this->renderer)
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());

  if (this->renderer)
    SDL_SetRenderVSync(this->renderer, 1);

  // Immediately set the color of the window to reduce the black flash.
  this->clear(background_color);
  this->draw();
}

void Render::clear(SDL_Color background_color)
{
  SDL_SetRenderDrawColor(this->renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderClear(this->renderer);
}

void Render::draw()
{
  if (!SDL_RenderPresent(renderer))
    printf("SDL_RenderPresent Error: %s\n", SDL_GetError());
}

void Render::rect(SDL_FRect &rect, SDL_Color background_color)
{
  SDL_SetRenderDrawColor(this->renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderFillRect(this->renderer, &rect);
}

SDL_Renderer *Render::getRenderer()
{
  return this->renderer;
}
