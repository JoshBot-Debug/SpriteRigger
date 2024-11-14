#pragma once

#include "imgui.h"

#include "app.h"
#include "Viewport/viewport.h"
#include "Component/components.h"

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