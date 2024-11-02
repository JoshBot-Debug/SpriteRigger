#pragma once

#include "Application/application.h"
#include "Application/FPS/fps.h"

class ShadowFrame : public Application
{
private:

public:
  ShadowFrame() = default;
  ~ShadowFrame() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};