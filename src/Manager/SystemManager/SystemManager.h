#pragma once

#include <SDL3/SDL.h>

#include "Viewport/Viewport.h"
#include "ECS/Registry.h"
#include "Input/Mouse.h"
#include "System/RenderSystem.h"
#include "System/TransformSystem.h"
#include "System/ColorSystem.h"
#include "Manager/ShaderManager/ShaderManager.h"

class SystemManager
{
private:
  Registry *registry;
  Mouse *mouse;
  ShaderManager *shaderManager;

  RenderSystem *render;
  TransformSystem *transform;
  ColorSystem *color;

public:
  SystemManager(Registry *registry, Mouse *mouse, ShaderManager *shaderManager);
  ~SystemManager() = default;

  void update(float deltaTime);

  void draw(float deltaTime);
};