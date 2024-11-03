#pragma once

#include "application.h"
#include "MainMenu/mainMenu.h"

class SpriteRigger : public Application
{
private:
  MainMenu mainMenu;
  Viewport viewport;
  Mouse mouse;

public:
  SpriteRigger() = default;
  ~SpriteRigger() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void onDrawViewport(float deltaTime);
};