#pragma once

#include "Application/application.h"

class ShadowFrame : public Application
{
private:
public:
  ShadowFrame();
  ~ShadowFrame();

  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
};