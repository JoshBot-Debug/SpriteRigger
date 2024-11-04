#pragma once

#include "imgui.h"
#include "application.h"
#include "Viewport/viewport.h"
#include "Entity/components.h"

class AnimatorViewport : public Viewport
{
private:
  FPS fps;

public:
  AnimatorViewport(Application *application) : Viewport(application) {}
  ~AnimatorViewport() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDrawViewport(float deltaTime) override;
  void onCleanUp() override;
};