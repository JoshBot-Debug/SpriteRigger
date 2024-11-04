#pragma once

#include "application.h"
#include "Scene/HeaderPanelScene/headerPanelScene.h"
#include "Scene/ToolsScene/toolsScene.h"
#include "Scene/AnimatorViewport/animatorViewport.h"

class SpriteRigger : public Application
{
private:
  AnimatorViewport animatorViewport;
  HeaderPanelScene headerPanelScene;
  ToolsScene toolsScene;

public:
  SpriteRigger() : headerPanelScene(this), toolsScene(this), animatorViewport(this) {};
  ~SpriteRigger() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};