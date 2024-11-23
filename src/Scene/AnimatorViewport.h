#pragma once

#include "imgui.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "ECS/Registry.h"
#include "Viewport/Viewport.h"
#include "Component/Component.h"

#include "Manager/ResourceManager.h"

#include "System/CameraSystem.h"
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
  Keyboard keyboard;
  OrthographicCamera camera;

  CameraSystem cameraSystem;

  ColorSystem *colorSystem;
  RenderSystem *renderSystem;
  TransformSystem *transformSystem;

public:
  AnimatorViewport() = default;
  ~AnimatorViewport() = default;

  void onInitialize(Registry *registry, Controller *controller, ResourceManager *resourceManager, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem);
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
  void onResize(glm::vec2 size) override;
};