#pragma once

#include <SDL3/SDL.h>

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"
#include "System/RenderSystem.h"
#include "System/TransformSystem.h"
#include "System/ColorSystem.h"

class SystemManager
{
private:
  RenderSystem *render;
  TransformSystem *transform;
  ColorSystem *color;

public:
  SystemManager();
  ~SystemManager() = default;

  void update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse);

  void draw(float deltaTime, Registry *registry);
  void input(SDL_Event *event, float deltaTime);
};