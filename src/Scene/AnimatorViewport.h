#pragma once

#include "imgui.h"

#include "Input/Mouse.h"
#include "ECS/Registry.h"
#include "Viewport/Viewport.h"
#include "Component/Component.h"

class SystemManager;

class AnimatorViewport : public Viewport
{
private:
  SystemManager *systemManager;

public:
  AnimatorViewport(SystemManager *systemManager) : systemManager(systemManager) {};
  ~AnimatorViewport() = default;

  void onInitialize() override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
};