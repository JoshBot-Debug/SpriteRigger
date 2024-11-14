#pragma once

#include "imgui.h"

#include "App.h"
#include "Viewport/Viewport.h"
#include "Component/Component.h"

class SystemManager;

class AnimatorViewport : public Viewport
{
private:
  FPS fps;
  App *app;
  SystemManager *system;

public:
  AnimatorViewport(App *app);
  ~AnimatorViewport();

  void onInitialize() override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
};