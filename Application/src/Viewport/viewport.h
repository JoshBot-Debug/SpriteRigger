#pragma once

#include <functional>

#include "SDL3/SDL.h"

#include "imgui.h"

class Viewport
{
private:
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
  float h, w;
  void free();

public:
  Viewport() = default;
  Viewport(SDL_Renderer *renderer, int w, int h);
  ~Viewport();

  void resize(float w, float h);
  void setRenderer(SDL_Renderer *renderer);

  void draw(const char *title, std::function<void()> callback);
};