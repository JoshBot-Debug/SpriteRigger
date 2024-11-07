#pragma once

#include "imgui.h"

#include "app.h"
#include "Viewport/viewport.h"
#include "Entity/components.h"

class AnimatorViewport : public Viewport
{
private:
  FPS fps;
  App *app;

public:
  AnimatorViewport(App *app) : Viewport(app), app(app) {}
  ~AnimatorViewport() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
  void onCleanUp() override;
};