#pragma once

#include "imgui.h"

#include "Input/Mouse.h"
#include "ECS/Registry.h"
#include "Viewport/Viewport.h"
#include "Component/Component.h"

#include "System/ColorSystem.h"
#include "System/RenderSystem.h"
#include "System/TransformSystem.h"

#include "Controller/Controller.h"
#include "Render/Camera/OrthographicCamera.h"

class AnimatorViewport : public Viewport
{
private:
  Registry *registry;
  Mouse mouse;
  OrthographicCamera camera;

  ColorSystem *colorSystem;
  RenderSystem *renderSystem;
  TransformSystem *transformSystem;

public:
  AnimatorViewport() = default;
  ~AnimatorViewport() = default;

  void onInitialize(Registry *registry, Controller *controller, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem);
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
  void onResize(float width, float height) override;
};